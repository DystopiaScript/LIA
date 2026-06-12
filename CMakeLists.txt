cmake_minimum_required(VERSION 3.16)

project(AnalizadorLexicoLIA VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Buscar los componentes necesarios de Qt6 (Widgets es el común para interfaces)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Habilitar la compilación automática de interfaces de Qt (MOC, RCC, UIC)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Recoger de forma automática todos los archivos .cpp de tu carpeta src
# NOTA: GLOB solo se evalúa al CONFIGURAR CMake. Si agregas archivos nuevos
# a src/ y el linker marca "undefined reference", re-ejecuta CMake:
# Qt Creator: Build > Run CMake   (o modifica este archivo para forzarlo)
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")

# Definir el ejecutable del proyecto
add_executable(AnalizadorLexicoLIA ${SOURCES} ${HEADERS})

# Enlazar las librerías de Qt6 al ejecutable
target_link_libraries(AnalizadorLexicoLIA PRIVATE Qt6::Widgets)