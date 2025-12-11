@echo off
echo ---------------------------------------
echo Compiling IV.c...
echo ---------------------------------------

:: The Compile Command (Direct Link Method)
gcc IV.c -o main.exe -I "C:\SDL\SDL3-3.2.28\i686-w64-mingw32\include" "C:\SDL\SDL3-3.2.28\i686-w64-mingw32\lib\libSDL3.dll.a"

:: Check if GCC found errors
if %errorlevel% neq 0 (
    echo.
    echo !!! COMPILATION FAILED !!!
    echo.
    pause
    exit /b
)

echo.
echo *** SUCCESS! Running Game... ***
echo.

:: Run the game
main.exe

echo.
echo Game Closed.
pause