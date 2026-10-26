@echo off
rem Copies the game data into Azahar's emulated SD card so goldbox.3dsx can be
rem tested in the emulator (File > Load File... > goldbox.3dsx).
setlocal
set "SD=%APPDATA%\Azahar\sdmc"
if not exist "%SD%" (
  echo Azahar SD folder not found at %SD% - start Azahar once first.
  pause & exit /b 1
)
set "DATA=%~dp0..\..\..\data\cotab"
if not exist "%SD%\3ds\goldbox\cotab" mkdir "%SD%\3ds\goldbox\cotab"
copy /y "%DATA%\*.DAX" "%SD%\3ds\goldbox\cotab\" > nul
copy /y "%DATA%\ITEMS" "%SD%\3ds\goldbox\cotab\" > nul
copy /y "%DATA%\GAME.OVR" "%SD%\3ds\goldbox\cotab\" > nul
echo Game data copied to %SD%\3ds\goldbox\cotab
