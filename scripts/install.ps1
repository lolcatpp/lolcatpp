$ErrorActionPreference = 'Stop'

$existingLolcat = Get-Command lolcat -ErrorAction SilentlyContinue
if ($existingLolcat) {
    $versionOutput = & lolcat --version 2>&1 | Out-String
    if ($versionOutput -like "*lolcat++*") {
        Write-Host ">> Found existing lolcat++ installation. Upgrading..."
    } else {
        Write-Host ">> Found another lolcat installation."
        Write-Host ">> It will be replaced by lolcat++."
        $confirmation = Read-Host ">> Do you want to continue? [y/N]"
        if ($confirmation -notmatch '[Yy]') {
            Write-Host "Installation cancelled."
            exit
        }
    }
}

$assetName = "lolcat-windows-amd64.exe"
$url = "https://github.com/lolcatpp/lolcatpp/releases/latest/download/$assetName"
$tempFile = Join-Path $env:TEMP "lolcat.exe"

Write-Host ">> Downloading lolcat++ ($assetName)..."
$httpCode = 0
try {
    $response = Invoke-WebRequest -Uri $url -OutFile $tempFile -PassThru
    $httpCode = [int]$response.StatusCode
} catch {
    if ($_.Exception.Response) {
        $httpCode = [int]$_.Exception.Response.StatusCode
    }
}

$magic = "<file too short>"
if (Test-Path $tempFile) {
    $stream = [System.IO.File]::OpenRead($tempFile)
    try {
        $bytes = New-Object byte[] 2
        $read = $stream.Read($bytes, 0, 2)
        if ($read -eq 2) {
            $magic = ('{0:x2}{1:x2}' -f $bytes[0], $bytes[1])
        }
    } finally {
        $stream.Close()
    }
}

if ($httpCode -ne 200 -or $magic -ne "4d5a") {
    Write-Host ">> Error: Download failed (HTTP $httpCode)." -ForegroundColor Red
    if ($magic -ne "4d5a") {
        Write-Host ">> Expected Windows executable magic bytes 4d5a (MZ) but got '$magic'." -ForegroundColor Red
    }
    Write-Host ">> GitHub may be temporarily unavailable, rate-limiting your IP, or have hit a per-repo bandwidth cap." -ForegroundColor Red
    Write-Host ">> Please wait a few minutes and try again. If it keeps failing, download the binary manually from:" -ForegroundColor Red
    Write-Host ">>   https://github.com/lolcatpp/lolcatpp/releases/latest" -ForegroundColor Red
    if (Test-Path $tempFile) { Remove-Item -Path $tempFile -Force }
    exit 1
}

$installDir = "C:\Program Files\lolcat++"
Write-Host ">> Installing to $installDir..."
if (-not (Test-Path $installDir)) {
    New-Item -ItemType Directory -Force -Path $installDir | Out-Null
}
Move-Item -Force $tempFile (Join-Path $installDir "lolcat.exe")

$currentPath = [System.Environment]::GetEnvironmentVariable("Path", "Machine")
if ($currentPath -notlike "*$installDir*") {
    Write-Host ">> Adding $installDir to Machine PATH..."
    [System.Environment]::SetEnvironmentVariable("Path", $currentPath + ";$installDir", "Machine")
    Write-Host "Installation complete! Please restart your terminal."
} else {
    Write-Host "Installation complete!"
}
