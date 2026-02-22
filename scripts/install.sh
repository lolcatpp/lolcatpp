#!/bin/bash
set -e

OS="$(uname -s)"
ARCH="$(uname -m)"

case "$OS" in
Linux)
    if [ "$ARCH" = "x86_64" ]; then
        ASSET_NAME="lolcat-linux-amd64"
        EXPECTED_MAGIC="7f454c46"
    else
        echo "Unsupported architecture: $ARCH"
        exit 1
    fi
    ;;
Darwin)
    if [ "$ARCH" = "arm64" ]; then
        ASSET_NAME="lolcat-macos-arm64"
        EXPECTED_MAGIC="cffaedfe"
    else
        echo "For macOS x86_64, please use Homebrew or build from source."
        exit 1
    fi
    ;;
*)
    echo "Unsupported OS: $OS"
    exit 1
    ;;
esac

if command -v lolcat >/dev/null 2>&1; then
    VERSION_OUTPUT=$(lolcat --version 2>&1 || true)
    if echo "$VERSION_OUTPUT" | grep -q "lolcat++"; then
        echo ">> Found existing lolcat++ installation. Upgrading..."
    else
        echo ">> Found another lolcat installation (likely the Ruby version)."
        echo ">> It will be replaced by lolcat++ in /usr/local/bin/lolcat."
        echo ">> This might cause weird stuff, if it's installed as a system package."
        read -p ">> Do you want to continue? [y/N] " -n 1 -r </dev/tty
        echo
        if [[ ! $REPLY =~ ^[Yy]$ ]]; then
            echo "Installation cancelled."
            exit 1
        fi
    fi
fi

echo ">> Downloading lolcat++ ($ASSET_NAME)..."
TEMP_FILE=$(mktemp)
HTTP_CODE=$(curl --show-error --location --write-out '%{http_code}' \
    "https://github.com/lolcatpp/lolcatpp/releases/latest/download/$ASSET_NAME" \
    -o "$TEMP_FILE")

ACTUAL_MAGIC=$(head -c 4 "$TEMP_FILE" | od -An -tx1 | tr -d ' \n')

if [ "$HTTP_CODE" != "200" ] || [ "$ACTUAL_MAGIC" != "$EXPECTED_MAGIC" ]; then
    echo ">> Error: Download failed (HTTP $HTTP_CODE)."
    if [ "$ACTUAL_MAGIC" != "$EXPECTED_MAGIC" ]; then
        echo ">> Expected binary magic bytes $EXPECTED_MAGIC but got $ACTUAL_MAGIC."
    fi
    echo ">> GitHub may be temporarily unavailable, rate-limiting your IP, or have hit a per-repo bandwidth cap."
    echo ">> Please wait a few minutes and try again. If it keeps failing, download the binary manually from:"
    echo ">>   https://github.com/lolcatpp/lolcatpp/releases/latest"
    rm "$TEMP_FILE"
    exit 1
fi

chmod +x "$TEMP_FILE"

echo ">> Installing to /usr/local/bin/lolcat..."
sudo mv -v "$TEMP_FILE" /usr/local/bin/lolcat

echo ">> Done! Try running 'lolcat --help'"
