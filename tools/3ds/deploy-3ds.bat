@echo off
rem Installs goldbox-3ds on the 3DS over Wi-Fi (ftpd must be running on the 3DS).
rem  - goldbox.3dsx            -> sdmc:/3ds/goldbox/goldbox.3dsx
rem  - game data (first time)  -> sdmc:/3ds/goldbox/cotab/
rem Usage: deploy-3ds.bat [3ds-ip] [ftp-port]
setlocal enabledelayedexpansion
cd /d "%~dp0..\.."
set "IP=%~1"
if "%IP%"=="" set "IP=3DS-IP"
set "PORT=%~2"
if "%PORT%"=="" set "PORT=5000"
set "DATA=%~dp0..\..\..\data\cotab"

echo Uploading goldbox.3dsx to %IP%:%PORT% ...
curl -s --ftp-create-dirs -T goldbox.3dsx "ftp://%IP%:%PORT%/3ds/goldbox/goldbox.3dsx" || goto :ftpfail

curl -s "ftp://%IP%:%PORT%/3ds/goldbox/cotab/" --list-only > "%TEMP%\gb3ds_list.txt" 2>nul
findstr /i "GEO2.DAX" "%TEMP%\gb3ds_list.txt" > nul
if errorlevel 1 (
  echo Game data not on the 3DS yet - uploading from %DATA% ...
  for %%F in ("%DATA%\*.DAX" "%DATA%\ITEMS" "%DATA%\GAME.OVR" "%DATA%\CURSE.CFG") do (
    if exist "%%~fF" (
      echo   %%~nxF
      curl -s --ftp-create-dirs -T "%%~fF" "ftp://%IP%:%PORT%/3ds/goldbox/cotab/%%~nxF" || goto :ftpfail
    )
  )
)
echo Done. Launch "Curse of the Azure Bonds" from the Homebrew Launcher.
exit /b 0

:ftpfail
echo FTP upload failed - is ftpd running on the 3DS at %IP%:%PORT%?
pause
exit /b 1
