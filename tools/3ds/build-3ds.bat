@echo off
rem Builds goldbox.3dsx. Uses your native devkitPro install (C:\devkitpro) if
rem present, otherwise the devkitpro/devkitarm Docker image.
rem Output: goldbox.3dsx in the goldbox-3ds folder. Log: tools\3ds\build-3ds.log
setlocal
cd /d "%~dp0..\.."
set "LOG=%~dp0build-3ds.log"
del /q "%~dp0build-3ds.status" 2>nul
echo build started > "%LOG%"
if exist "C:\devkitpro\msys2\usr\bin\bash.exe" goto native
echo Building goldbox.3dsx in Docker (devkitpro/devkitarm)...
docker run --rm -v "%cd%":/src -w /src devkitpro/devkitarm bash tools/3ds/build-3ds.sh >> "%LOG%" 2>&1
goto check
:native
echo Building goldbox.3dsx with devkitPro (C:\devkitpro)...
set "MSYSTEM=MSYS"
set "CHERE_INVOKING=1"
"C:\devkitpro\msys2\usr\bin\bash.exe" -l "%~dp0build-3ds.sh" >> "%LOG%" 2>&1
:check
if errorlevel 1 (
  echo BUILD FAILED - see tools\3ds\build-3ds.log
  echo FAILED > "%~dp0build-3ds.status"
  pause
  exit /b 1
)
echo OK > "%~dp0build-3ds.status"
echo Built goldbox.3dsx
endlocal
