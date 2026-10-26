@echo off
rem Builds goldbox.3dsx. Uses your native devkitPro install (C:\devkitpro) if
rem present, otherwise the devkitpro/devkitarm Docker image.
rem Output: goldbox.3dsx in the repo root. Log: tools\3ds\build-3ds.log
setlocal
cd /d "%~dp0..\.."
del /q tools\3ds\build-3ds.status 2>nul
if exist "C:\devkitpro\msys2\usr\bin\bash.exe" (
  echo Building goldbox.3dsx with devkitPro ^(C:\devkitpro^)...
  set "DEVKITPRO=/opt/devkitpro"
  set "DEVKITARM=/opt/devkitpro/devkitARM"
  "C:\devkitpro\msys2\usr\bin\bash.exe" -lc "cd \"$(cygpath -u '%cd%')\" && make -f Makefile.3ds -j8" > tools\3ds\build-3ds.log 2>&1
) else (
  echo Building goldbox.3dsx in Docker ^(devkitpro/devkitarm^)...
  docker run --rm -v "%cd%":/src -w /src devkitpro/devkitarm make -f Makefile.3ds -j4 > tools\3ds\build-3ds.log 2>&1
)
if errorlevel 1 (
  echo BUILD FAILED - see tools\3ds\build-3ds.log
  echo FAILED > tools\3ds\build-3ds.status
  pause & exit /b 1
)
echo OK > tools\3ds\build-3ds.status
echo Built goldbox.3dsx
endlocal
