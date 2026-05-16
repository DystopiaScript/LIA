# Analizador Léxico LIA

Proyecto académico para la materia **Lenguajes y Autómatas I**  
Compilador "csyl" para el lenguaje de programación LIA

## 📋 Descripción

Este proyecto implementa un analizador léxico (lexer) para el lenguaje LIA mediante un Autómata Finito Determinístico (AFD). El analizador reconoce tokens del lenguaje incluyendo palabras reservadas, identificadores, constantes numéricas, operadores, delimitadores y comentarios.

**Características principales:**
- ✅ Implementación manual del AFD (sin flex/lex/ANTLR)
- ✅ Interfaz gráfica con Qt Widgets
- ✅ Manejo de errores con modo pánico
- ✅ Soporte para archivos .lia
- ✅ Visualización de tokens, sintaxis y errores

## 🗂️ Estructura del Proyecto

```
LIA/
├── .git/                          # Control de versiones Git
├── .kiro/                         # Especificaciones del proyecto
│   └── specs/
│       └── lia-lexer/
│           ├── requirements.md    # Requisitos del proyecto (32 requisitos)
│           ├── design.md          # Diseño técnico detallado
│           └── .config.kiro       # Configuración de Kiro
├── docs/                          # Documentación técnica
│   ├── AFD_LIA.md                # Descripción del autómata (estados, transiciones)
│   ├── matriz_transicion.md      # Plantilla de la matriz de transición
│   └── mejoras_futuras.md        # Características opcionales para el futuro
├── src/                           # Código fuente
│   ├── Token.h                   # Definición de la clase Token
│   ├── Token.cpp                 # Implementación de Token
│   ├── Error.h                   # Definición de la clase Error
│   ├── Error.cpp                 # Implementación de Error
│   ├── Lexer.h                   # Definición del motor del lexer
│   ├── Lexer.cpp                 # Implementación del lexer (ESQUELETO)
│   ├── MainWindow.h              # Definición de la ventana principal
│   ├── MainWindow.cpp            # Implementación de la GUI (ESQUELETO)
│   └── main.cpp                  # Punto de entrada de la aplicación
├── LIA.pro                        # Archivo de proyecto Qt (qmake)
├── CMakeLists.txt                 # Archivo de proyecto CMake (alternativo)
└── README.md                      # Este archivo

```

## 🏗️ Arquitectura

El proyecto sigue una arquitectura de **tres capas**:

### 1. Capa de Presentación (GUI)
- **MainWindow**: Ventana principal con Qt Widgets
- **Paneles**: Editor de código, visualización de tokens, sintaxis y errores
- **Botones**: Abrir, Guardar, Limpiar, Analizar, Salir

### 2. Capa de Lógica de Negocio (Backend)
- **Lexer**: Motor del analizador léxico
- **Funciones principales**:
  - `Analiza()`: Ejecuta el análisis léxico completo
  - `relaciona()`: Mapea caracteres a columnas de la matriz
  - `Token()`: Clasifica y crea tokens reconocidos
  - `Error()`: Reporta errores léxicos

### 3. Capa de Datos
- **Matriz de Transición**: AFD de 20 estados × 34 columnas
- **Tabla de Palabras Reservadas**: 17 palabras del lenguaje LIA
- **Estructuras de Datos**: Vectores de tokens y errores

## 🔧 Componentes Principales

### Token (Token.h / Token.cpp)
Representa un token reconocido con:
- `lexema`: Texto del token
- `gramema`: Tipo/categoría (100-131)
- `linea`, `columna`: Ubicación en el código fuente

**32 tipos de tokens (gramemas):**
- 100: Palabra reservada
- 101: Identificador
- 102-104: Constantes numéricas (entera, flotante, científica)
- 105-118: Operadores (aritméticos, relacionales, lógicos)
- 119-131: Delimitadores y otros

### Error (Error.h / Error.cpp)
Representa un error léxico con:
- `linea`, `columna`: Ubicación del error
- `codigoError`: Código del estado de error (500+)
- `caracterProblematico`: Carácter que causó el error
- `descripcion`: Mensaje descriptivo

**10 códigos de error:**
- 500: Carácter no reconocido
- 501-503: Errores en constantes numéricas
- 504-509: Errores en operadores y delimitadores

### Lexer (Lexer.h / Lexer.cpp)
Motor del analizador léxico que implementa el AFD:
- **Matriz de Transición**: Define el comportamiento del autómata
- **Tabla de Palabras Reservadas**: 17 palabras del lenguaje LIA
- **Algoritmo de Análisis**: Recorre el código carácter por carácter

### MainWindow (MainWindow.h / MainWindow.cpp)
Interfaz gráfica de usuario:
- **Panel Izquierdo**: Editor de código + 5 botones
- **Panel Derecho**: 3 secciones (Tokens, Sintaxis, Errores)

## 📝 Estado Actual: ESQUELETO

**IMPORTANTE:** El código actual es un **ESQUELETO** con estructura completa pero sin implementación.

### ✅ Completado:
- [x] Estructura de clases y funciones
- [x] Declaraciones de todos los métodos
- [x] Comentarios TODO detallados explicando qué implementar
- [x] Documentación completa en headers
- [x] Enumeraciones de gramemas y códigos de error
- [x] Archivos de proyecto (Qt .pro y CMake)

### ⏳ Pendiente de Implementar:
- [ ] Llenar la matriz de transición del AFD (Lexer.cpp)
- [ ] Llenar la tabla de palabras reservadas (Lexer.cpp)
- [ ] Implementar función `Analiza()` (Lexer.cpp)
- [ ] Implementar función `relaciona()` (Lexer.cpp)
- [ ] Implementar función `Token()` (Lexer.cpp)
- [ ] Implementar función `Error()` (Lexer.cpp)
- [ ] Implementar todas las funciones de MainWindow (MainWindow.cpp)
- [ ] Implementar función `main()` (main.cpp)

## 🚀 Cómo Compilar (Cuando esté implementado)

### Opción 1: Qt Creator (Recomendado)
1. Abrir Qt Creator
2. Abrir el archivo `LIA.pro`
3. Configurar el kit de compilación
4. Presionar "Build" (Ctrl+B)
5. Ejecutar (Ctrl+R)

### Opción 2: Línea de Comandos con qmake
```bash
qmake LIA.pro
make
./bin/LIA
```

### Opción 3: CMake
```bash
mkdir build
cd build
cmake ..
cmake --build .
./LIA
```

## 📚 Documentación Adicional

- **requirements.md**: 32 requisitos funcionales y no funcionales
- **design.md**: Diseño técnico completo con diagramas y algoritmos
- **AFD_LIA.md**: Descripción detallada del autómata finito determinístico
- **matriz_transicion.md**: Plantilla de la matriz de transición (20×34)
- **mejoras_futuras.md**: Características opcionales (numeración de líneas, syntax highlighting, etc.)

## 🎯 Próximos Pasos

1. **Implementar la matriz de transición** usando `docs/AFD_LIA.md` como referencia
2. **Llenar la tabla de palabras reservadas** (17 palabras)
3. **Implementar el algoritmo `Analiza()`** siguiendo los comentarios TODO
4. **Implementar las funciones auxiliares** del lexer
5. **Implementar la interfaz gráfica** en MainWindow
6. **Probar con archivos .lia** de ejemplo
7. **Ajustar y depurar** según sea necesario

## 👥 Equipo

Proyecto en parejas para la materia Lenguajes y Autómatas I

## 📅 Fecha de Entrega

25 de Mayo, 2026

## 📄 Licencia

Proyecto académico - Universidad

---

**Nota:** Este README será actualizado conforme avance la implementación del proyecto
