@echo off
set PATH=C:\Qt\6.11.1\mingw_64\bin;C:\Users\jashe\Downloads\w64devkit\bin;%PATH%

echo Limpiando build anterior...
if exist build rmdir /s /q build

echo Generando archivos de compilacion...
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH=C:/Qt/6.11.1/mingw_64

echo Compilando desde cero...
cmake --build build

:: Validar si el compilador tuvo exito antes de intentar ejecutar
if %errorlevel% equ 0 (
    echo.
    echo ====================================
    echo Compilacion exitosa. Ejecutando...
    echo ====================================
    echo.
    
    :: Ejecutar directamente en la misma consola para ver errores de salida o crashes
    build\AnalizadorLexicoLIA.exe
    
) else (
    echo.
    echo [ERROR] Hubo fallas en la compilacion. Revisa los errores de arriba.
    echo.
    pause
)