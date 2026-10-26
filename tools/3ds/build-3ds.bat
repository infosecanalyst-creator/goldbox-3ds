@echo off
rem Builds goldbox.3dsx with the devkitpro/devkitarm Docker image.
rem Needs Docker Desktop running. Output: ..\..\goldbox.3dsx, log: build-3ds.log
setlocal
cd /d "%~dp0..\.."
echo Building goldbox.3dsx in Docker (devkitpro/devkitarm)...
docker run --rm -v "%cd%":/src -w /src devkitpro/devkitarm make -f Makefile.3ds -j4 > tools\3ds\build-3ds.log 2>&1
if errorlevel 1 (
  echo BUILD FAILED - see tools\3ds\build-3ds.log
  echo FAILED > tools\3ds\build-3ds.status
  pause & exit /b 1
)
echo OK > tools\3ds\build-3ds.status
echo Built goldbox.3dsx
endlocal
