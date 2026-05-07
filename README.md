<div align="center">
    <img src="assets/cat.svg" alt="icon" height=300 />
</div>

# Lolcat++

[![Blazingly fast](https://www.blazingly.fast/api/badge.svg?repo=lolcatpp%2Flolcatpp)]()
[![GitHub License](https://img.shields.io/github/license/lolcatpp/lolcatpp?logo=opensourceinitiative&logoColor=white)](/LICENSE)
[![GitHub top language](https://img.shields.io/github/languages/top/lolcatpp/lolcatpp?logo=cplusplus)]()
[![Build Status](https://img.shields.io/github/actions/workflow/status/lolcatpp/lolcatpp/release.yml?logo=CMake)](https://github.com/lolcatpp/lolcatpp/actions)
[![GitHub Release](https://img.shields.io/github/v/release/lolcatpp/lolcatpp?logo=github&logoColor=white)](https://github.com/lolcatpp/lolcatpp/releases/latest)
[![AUR Version](https://img.shields.io/aur/version/lolcat%2B%2B?logo=archlinux&logoColor=white)](https://aur.archlinux.org/packages/lolcat++)

A rewrite of the popular utility [_LOLCAT_](https://github.com/busyloop/lolcat) in C++.
The rationale is that lolcat is often used in init scripts and such.
It can at times be rather time-consuming to run it, since it's written in Ruby.
Now it's -- naturally -- _BLAZINGLY FAST_ in C++.
It's also cross-platform.

## Example usage

**Help:**

<div align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="assets/help-dark.jpg">
    <source media="(prefers-color-scheme: light)" srcset="assets/help-light.jpg">
    <img alt="Example usage" src="assets/help-light.jpg" width="900" style="max-width: 100%;" >
  </picture>
</div>

**Example usage:** `fortune` + `cowsay` + `lolcat`

<div align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="assets/example-dark.jpg">
    <source media="(prefers-color-scheme: light)" srcset="assets/example-light.jpg">
    <img alt="Example usage" src="assets/example-light.jpg" width="900" style="max-width: 100%;">
  </picture>
</div>

## Getting it

Just head over to the [releases](https://github.com/lolcatpp/lolcatpp/releases) and download it.
Here's some copy-paste scripts for your platform:

### Arch Linux: AUR

Use your prefered AUR helper and choose whether to compile from source, or to just grab the
binary that the CI produces. This aur-package is updated automatically in the CI/CD.

**From source:**

```bash
paru -S lolcat++
```

**Binary release:**

```bash
paru -S lolcat++-bin
```

### Debian / Ubuntu / Linux Mint

| Family | Versions                          | Architectures |
| ------ | --------------------------------- | ------------- |
| Debian | 13 (Trixie)                       | amd64, arm64  |
| Ubuntu | 24.04 LTS (Noble), 25.04 (Plucky) | amd64, arm64  |
| Mint   | LMDE 7 / 22 / 22.1 / 22.2 / 22.3  | amd64, arm64  |

Use the apt repo over on [lolcatpp/apt](https://github.com/lolcatpp/apt).

```bash
sudo install -d /etc/apt/keyrings
. /etc/os-release && \
    CODENAME="${UBUNTU_CODENAME:-$VERSION_CODENAME}" && \
    echo "deb [arch=amd64 signed-by=/etc/apt/keyrings/lolcatpp.asc] https://lolcatpp.github.io/apt $CODENAME main" \
    | sudo tee /etc/apt/sources.list.d/lolcatpp.list

curl -fsSL https://lolcatpp.github.io/apt/pubkey.gpg | sudo tee /etc/apt/keyrings/lolcatpp.asc > /dev/null

sudo apt update
sudo apt install lolcat++
```

### Fedora / RHEL / Rocky / Alma

| Family              | Versions | Architectures   |
| ------------------- | -------- | --------------- |
| Fedora              | 43, 44   | x86_64, aarch64 |
| RHEL / Rocky / Alma | 9, 10    | x86_64, aarch64 |

Use the dnf compatible repo over on [lolcatpp/rpm](https://github.com/lolcatpp/rpm).

```bash
. /etc/os-release
case "$ID" in
  fedora)               family="fedora-${VERSION_ID}" ;;
  rhel|rocky|almalinux) family="rhel-${VERSION_ID%%.*}" ;;
  *) echo "unsupported distro: $ID"; exit 1 ;;
esac
sudo curl -fsSLo /etc/yum.repos.d/lolcatpp.repo "https://lolcatpp.github.io/rpm/${family}/lolcatpp.repo"
sudo dnf install -y lolcat++
```

### openSUSE Leap

**Supported:** openSUSE Leap 16.0 -- x86_64 & aarch64

Use the zypper compatible repo over on [lolcatpp/rpm](https://github.com/lolcatpp/rpm).

```bash
. /etc/os-release
sudo zypper addrepo --gpgcheck \
  "https://lolcatpp.github.io/rpm/opensuse-leap-${VERSION_ID}/lolcatpp.repo"
sudo rpm --import https://lolcatpp.github.io/rpm/pubkey.gpg
sudo zypper install lolcat++
```

### Nix / NixOS

You can run it directly with Nix flakes:

```bash
nix run github:lolcatpp/lolcatpp
```

To install it into your profile:

```bash
nix profile install github:lolcatpp/lolcatpp
```

If you're working from a local checkout:

```bash
nix build
nix develop
```

To install it system-wide on NixOS from your own flake, add this input:

```nix
{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    "lolcat++" = {
      url = "github:lolcatpp/lolcatpp";
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };
}
```

Then add the package to `environment.systemPackages` in one of your NixOS modules:

```nix
{ inputs, pkgs, ... }:
{
  environment.systemPackages = [
    inputs."lolcat++".packages.${pkgs.system}.default
  ];
}
```

If your system flake does not override it with `follows`, `lolcat++` defaults to `nixos-unstable`.

### macOS: Homebrew

There's a homebrew tap setup under [lolcatpp/homebrew-tap](https://github.com/lolcatpp/homebrew-tap).
See the repository for more detailed instructions.
It's updated automatically in the CI/CD, which means that you're getting the latest updates.
It downloads the prebuilt binary found under the releases if you're on _arm64_ or compiles from source
if your're on _x86_64_.

```bash
brew tap lolcatpp/tap
brew install lolcatpp
```

### Linux & macOS: from releases

If none of the options above suit you needs: grab the latest release from the
[releases](https://github.com/lolcatpp/apt/releases) with the following command.

```bash
curl -sSL "https://raw.githubusercontent.com/lolcatpp/lolcatpp/master/scripts/install.sh" | bash
```

### Windows: from releases (Administrator PowerShell)

Grab the latest release from the [releases](https://github.com/lolcatpp/apt/releases),
and update the `$PATH` variable, with the following command.

```powershell
iwr -useb "https://raw.githubusercontent.com/lolcatpp/lolcatpp/master/scripts/install.ps1" | iex
```

### Building

You'll need _cmake_, boost and a C++ 20 compatible compiler.
If your compiler supports C++ 20, but doesn't support `<format>`, then you'll also need
_libfmt_ (it's used as a polyfill).
Thereafter, just run

```bash
cmake -S . -B build && cmake --build build --parallel
```

You'll now have the executable in `build/lolcat`

If you'd like to install the program, just run

```bash
cmake --install build
```

## Acknowledgement

Both the project and the codebase are heavily inspired by the [_LOLCAT_](https://github.com/busyloop/lolcat) project.
