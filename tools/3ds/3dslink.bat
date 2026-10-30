@echo off
rem Sends goldbox.3dsx straight to the Homebrew Launcher's netloader (press Y on
rem the 3DS first). Usage: 3dslink.bat <3ds-ip>  (or set GOLDBOX_3DS_IP)
setlocal
cd /d "%~dp0..\.."
set "IP=%~1"
if "%IP%"=="" set "IP=%GOLDBOX_3DS_IP%"
if "%IP%"=="" (echo Usage: 3dslink.bat ^<3ds-ip^>  ^(or set GOLDBOX_3DS_IP^) & exit /b 1)
docker run --rm -v "%cd%":/src -w /src devkitpro/devkitarm /opt/devkitpro/tools/bin/3dslink -a %IP% goldbox.3dsx
