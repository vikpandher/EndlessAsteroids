@echo off

REM This bat file will be used to compile the code. It's how Casey Muratori built
REM his stuff in his handmade hero and it works for me.

REM Remember to run vcvarsall.bat
REM You also need to use "call" for some reason.
REM call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat"

REM Need to make sure this directory exists
mkdir build

REM Go into the build directory
pushd build

REM The "-Zi" adds in debug info
REM Link to user32.lib for ui stuff like the MessageBox
REM Lint to gdi32.lib for the PatBlt function
cl  -FeendlessAsteroids.exe ^
..\code\Win32Main.cpp ^
..\code\Game.cpp ^
..\code\MathUtilities.cpp  ^
..\code\ColorUtilities.cpp ^
..\code\String.cpp ^
..\code\ScreenBuffer.cpp ^
..\code\Point.cpp ^
..\code\Vector.cpp ^
..\code\Matrix.cpp ^
..\code\Quaternion.cpp ^
..\code\GraphicsUtilities.cpp ^
..\code\MenuUtilities.cpp ^
..\code\GameUtilities.cpp ^
user32.lib ^
gdi32.lib

REM ..\code\Game.cpp ^
REM ..\code\GameUtilities.cpp ^

REM Go back to the directory containing this file
popd