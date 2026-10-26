@echo off
rem Builds the harness-patched coab (coab-harness\) and runs every key script in
rem harness\scripts\*.txt against the GOG data in data\cotab, writing
rem harness\traces\coab-<script>-<seed>.txt.  Seeds come from harness\seeds.txt
rem (one per line). Log: harness\coab-harness-build.log
setlocal enabledelayedexpansion
cd /d "%~dp0"

set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe`) do set "MSBUILD=%%i"
if not defined MSBUILD (
  echo Could not find MSBuild.exe
  pause & exit /b 1
)
set "REFROOT=%~dp0tools\refasm-net452\build"

echo Building harness-patched coab...
"%MSBUILD%" "coab-harness\coab.sln" /p:Configuration=Debug /p:Platform=x86 "/p:TargetFrameworkRootPath=%REFROOT%" /m /nologo /v:minimal > harness\coab-harness-build.log 2>&1
if errorlevel 1 (
  echo BUILD FAILED - see harness\coab-harness-build.log
  pause & exit /b 1
)
set "EXE=%~dp0coab-harness\Main\bin\x86\Debug\Main.exe"

if not exist harness\traces mkdir harness\traces
pushd "%~dp0data\cotab"
for %%S in ("%~dp0harness\scripts\*.txt") do (
  for /f "usebackq tokens=*" %%D in ("%~dp0harness\seeds.txt") do (
    echo Running %%~nS seed %%D ...
    "%EXE%" --harness "%%~fS" --trace "%~dp0harness\traces\coab-%%~nS-%%D.txt" --seed %%D > nul 2>&1
  )
)
popd
echo.
echo Done. Traces are in harness\traces
echo DONE > "%~dp0harness\coab-harness-done.txt"
endlocal
