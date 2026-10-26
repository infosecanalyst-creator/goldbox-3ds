@echo off
rem Sends goldbox.3dsx straight to the Homebrew Launcher's netloader (press Y on
rem the 3DS first). Usage: 3dslink.bat [3ds-ip]
setlocal
cd /d "%~dp0..\.."
set "IP=%~1"
if "%IP%"=="" set "IP=3DS-IP"
docker run --rm -v "%cd%":/src -w /src devkitpro/devkitarm /opt/devkitpro/tools/bin/3dslink -a %IP% goldbox.3dsx
