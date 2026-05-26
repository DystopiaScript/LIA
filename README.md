# Analizador Léxico LIA

Proyecto académico de la materia **Lenguajes y Autómatas I**.

## Autores

- Jasher Abraham Carrazco Vaquera
- Juan Pablo Casio Martos

## Descripción

Este proyecto implementa un analizador léxico para el lenguaje LIA con interfaz gráfica en Qt Widgets y construcción mediante CMake. El programa permite cargar archivos `.lia`, analizarlos y mostrar tokens y errores en pantalla.

## Estado actual

El proyecto cuenta con:

- Interfaz gráfica principal en Qt.
- Analizador léxico base en `src/`.
- Variante adicional basada en punteros en `punteros/`.
- Script por lotes para compilar y ejecutar en Windows.

## Estructura actual del proyecto

```text
LIA/
├── CMakeLists.txt
├── compilar_y_ejecutar.bat
├── README.md
├── ejemplos/
│   ├── ejemplo_con_errores.lia
│   ├── ejemplo1.lia
│   └── ejemplo2.lia
├── punteros/
│   ├── CMakeLists.txt
│   ├── compilar_y_ejecutar_punteros.bat
│   ├── ErrorPunteros.cpp
│   ├── ErrorPunteros.h
│   ├── LexerPunteros.cpp
│   ├── LexerPunteros.h
│   ├── main_punteros.cpp
│   ├── MainWindowPunteros.cpp
│   ├── MainWindowPunteros.h
│   ├── TokenPunteros.cpp
│   └── TokenPunteros.h
└── src/
    ├── Error.cpp
    ├── Error.h
    ├── Lexer.cpp
    ├── Lexer.h
    ├── main.cpp
    ├── MainWindow.cpp
    ├── MainWindow.h
    ├── Token.cpp
    └── Token.h
```

Nota: la carpeta `build/` se genera al compilar y puede eliminarse y recrearse sin afectar el código fuente.

## Compilación

### Opción 1: script por lotes en Windows

Desde la raíz del proyecto ejecuta:

```bat
compilar_y_ejecutar.bat
```

Este script está ajustado a mi entorno local de Windows y a las rutas de Qt y MinGW que tengo instaladas. Sirve como atajo personal, pero no es la forma más portable de compilar el proyecto.

El script:

- limpia la carpeta `build/` existente,
- regenera el proyecto con CMake,
- compila el ejecutable,
- y lo ejecuta si la compilación fue exitosa.

Si otra persona quiere compilar el proyecto, lo más recomendable es usar CMake manualmente o abrirlo con Qt Creator.

### Cómo adaptar el `.bat` a otra computadora

Si alguien quiere usar ese mismo script en su equipo, debe modificar estas partes:

1. La ruta de Qt dentro de la línea `set PATH=...`.
    - Sustituye `C:\Qt\6.11.1\mingw_64\bin` por la ruta real donde esté instalado Qt en su máquina.
2. La ruta de MinGW dentro de la misma línea `set PATH=...`.
    - Sustituye `C:\Users\jashe\Downloads\w64devkit\bin` por la carpeta real del compilador.
3. El valor de `CMAKE_PREFIX_PATH` en la línea de CMake.
    - Debe apuntar al directorio raíz de Qt, por ejemplo `C:/Qt/6.11.1/mingw_64`.
4. El nombre del ejecutable al final del script.
    - Si cambia el proyecto, también puede cambiar el nombre del `.exe` generado en `build\AnalizadorLexicoLIA.exe`.

Si la persona no usa exactamente las mismas rutas, lo más seguro es editar el `.bat` con sus propias rutas o usar directamente los comandos de CMake de la opción manual.

### Opción 2: CMake manual

Si prefieres compilar manualmente:

```bash
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH=C:/Qt/6.11.1/mingw_64
cmake --build build
```

### Opción 3: Qt Creator

1. Abre Qt Creator.
2. Abre el archivo `CMakeLists.txt`.
3. Configura el kit de Qt que uses.
4. Compila y ejecuta desde el IDE.

## Ejecución

Si compilaste con el script o con CMake, el ejecutable queda en `build/AnalizadorLexicoLIA.exe`.

## Archivos principales

- `src/Lexer.cpp`: lógica del analizador léxico.
- `src/MainWindow.cpp`: interfaz principal.
- `src/main.cpp`: punto de entrada de la aplicación.
- `punteros/LexerPunteros.cpp`: variante del lexer basada en punteros.
- `compilar_y_ejecutar.bat`: atajo local para compilar y ejecutar en mi entorno de Windows.

## Ejemplos

La carpeta `ejemplos/` contiene archivos de prueba para validar el analizador.

## Licencia

Proyecto académico.

