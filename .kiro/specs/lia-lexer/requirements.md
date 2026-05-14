# Requirements Document

## Introduction

Este documento especifica los requisitos para el analizador léxico (lexer) del compilador "csyl" para el lenguaje de programación LIA. El analizador léxico es la primera fase del compilador y es responsable de leer el código fuente desde archivos *.lia, identificar tokens válidos, y reportar errores léxicos. Este proyecto es parte de la materia "Lenguajes y Autómatas I" con fecha de entrega el 25 de mayo de 2026.

El analizador léxico implementa un Autómata Finito Determinístico (AFD) mediante una matriz de transición y reconoce palabras reservadas, identificadores, constantes numéricas, constantes de caracteres, cadenas, operadores, delimitadores y comentarios del lenguaje LIA.

## Glossary

- **Lexer**: El analizador léxico que procesa el código fuente y genera tokens
- **Token**: Unidad léxica compuesta por un lexema (texto) y un gramema (categoría)
- **Lexema**: La secuencia de caracteres que forma un token
- **Gramema**: La categoría o tipo de un token (palabra reservada, identificador, operador, etc.)
- **AFD**: Autómata Finito Determinístico representado por la matriz de transición
- **Transition_Matrix**: Matriz que define las transiciones del AFD entre estados
- **Source_File**: Archivo de entrada con extensión *.lia que contiene código fuente LIA
- **State**: Estado actual del autómata durante el análisis
- **Acceptance_State**: Estado del AFD que indica un token válido reconocido (rango 100-199)
- **Error_State**: Estado del AFD que indica un error léxico (rango 500+)
- **Transition_State**: Estado intermedio del AFD durante el reconocimiento (rango 0-99)
- **Reserved_Word**: Palabra clave del lenguaje LIA (class, if, while, etc.)
- **Identifier**: Nombre de variable o función que cumple las reglas de identificadores
- **Numeric_Constant**: Literal numérico (entero, flotante o notación científica)
- **Character_Constant**: Literal de carácter encerrado en comillas simples
- **String_Constant**: Literal de cadena encerrado en comillas dobles
- **Operator**: Símbolo que representa una operación (+, -, *, ==, &&, etc.)
- **Delimiter**: Símbolo que separa elementos sintácticos (paréntesis, llaves, punto y coma, etc.)
- **Comment**: Texto que inicia con $ y termina en nueva línea
- **Whitespace**: Espacios en blanco, tabulaciones y saltos de línea

## Requirements

### Requirement 1: Leer Archivo Fuente

**User Story:** Como usuario del compilador, quiero que el lexer lea archivos con extensión *.lia, para que pueda analizar código fuente del lenguaje LIA.

#### Acceptance Criteria

1. WHEN el programa recibe un nombre de archivo como argumento, THE Lexer SHALL abrir el archivo para lectura
2. IF el archivo no existe o no se puede abrir, THEN THE Lexer SHALL mostrar un mensaje de error descriptivo y terminar la ejecución
3. IF el archivo no tiene extensión .lia, THEN THE Lexer SHALL mostrar una advertencia pero continuar el análisis
4. THE Lexer SHALL leer el contenido del archivo carácter por carácter

### Requirement 2: Implementar Motor de Análisis

**User Story:** Como desarrollador del compilador, quiero una función principal de análisis que recorra el código fuente, para que el lexer pueda procesar todos los caracteres del archivo.

#### Acceptance Criteria

1. THE Lexer SHALL implementar una función Analiza que procese el código fuente completo
2. WHILE hay caracteres por procesar en el Source_File, THE Lexer SHALL leer el siguiente carácter
3. WHEN se lee un carácter, THE Lexer SHALL mapear el carácter a una columna de la Transition_Matrix usando la función relaciona
4. THE Lexer SHALL mantener el State actual del AFD durante el análisis
5. WHEN se alcanza un Acceptance_State, THE Lexer SHALL invocar la función Token para clasificar el lexema
6. WHEN se alcanza un Error_State, THE Lexer SHALL invocar la función Error para reportar el error léxico
7. THE Lexer SHALL continuar el análisis hasta alcanzar el final del archivo

### Requirement 3: Mapear Caracteres a Columnas de Transición

**User Story:** Como desarrollador del compilador, quiero una función que mapee caracteres a columnas de la matriz de transición, para que el AFD pueda determinar las transiciones correctas.

#### Acceptance Criteria

1. THE Lexer SHALL implementar una función relaciona que reciba un carácter y retorne un índice de columna
2. WHEN el carácter es una letra mayúscula (A-Z), THE relaciona SHALL retornar el índice correspondiente al conjunto L
3. WHEN el carácter es una letra minúscula (a-z), THE relaciona SHALL retornar el índice correspondiente al conjunto l
4. WHEN el carácter es un dígito (0-9), THE relaciona SHALL retornar el índice correspondiente al conjunto d
5. WHEN el carácter es un operador o delimitador reconocido, THE relaciona SHALL retornar el índice correspondiente
6. WHEN el carácter es un espacio en blanco (espacio, tabulación, nueva línea), THE relaciona SHALL retornar el índice correspondiente al conjunto Whitespace
7. WHEN el carácter no pertenece a ningún conjunto reconocido, THE relaciona SHALL retornar un índice que indique carácter no reconocido

### Requirement 4: Reconocer Palabras Reservadas

**User Story:** Como usuario del lenguaje LIA, quiero que el lexer reconozca las palabras reservadas del lenguaje, para que pueda usar las construcciones sintácticas definidas.

#### Acceptance Criteria

1. THE Lexer SHALL reconocer las siguientes palabras reservadas: class, endclass, int, float, char, string, bool, if, else, do, while, input, output, def, to, break, loop
2. WHEN se identifica un lexema que coincide con una Reserved_Word, THE Token SHALL clasificarlo con gramema de palabra reservada
3. THE Lexer SHALL distinguir entre palabras reservadas e identificadores mediante comparación exacta
4. THE Lexer SHALL tratar las palabras reservadas como case-sensitive

### Requirement 5: Reconocer Identificadores

**User Story:** Como usuario del lenguaje LIA, quiero que el lexer reconozca identificadores válidos, para que pueda nombrar variables y funciones en mi código.

#### Acceptance Criteria

1. WHEN un lexema inicia con una letra (mayúscula o minúscula), THE Lexer SHALL considerar que puede ser un Identifier
2. THE Lexer SHALL aceptar letras, dígitos y guiones bajos (_) después del primer carácter de un Identifier
3. IF un Identifier contiene dos guiones bajos consecutivos (__), THEN THE Lexer SHALL rechazarlo como inválido
4. IF un Identifier termina con guión bajo (_), THEN THE Lexer SHALL rechazarlo como inválido
5. WHEN se completa el reconocimiento de un Identifier válido, THE Token SHALL clasificarlo con gramema de identificador
6. THE Lexer SHALL distinguir entre identificadores y palabras reservadas

### Requirement 6: Reconocer Constantes Numéricas Enteras

**User Story:** Como usuario del lenguaje LIA, quiero que el lexer reconozca números enteros, para que pueda usar valores numéricos en mi código.

#### Acceptance Criteria

1. WHEN un lexema consiste de uno o más dígitos consecutivos (d+), THE Lexer SHALL reconocerlo como Numeric_Constant de tipo entero
2. THE Lexer SHALL aceptar enteros de cualquier longitud
3. WHEN se completa el reconocimiento de un entero, THE Token SHALL clasificarlo con gramema de constante entera

### Requirement 7: Reconocer Constantes Numéricas Flotantes

**User Story:** Como usuario del lenguaje LIA, quiero que el lexer reconozca números flotantes, para que pueda usar valores decimales en mi código.

#### Acceptance Criteria

1. WHEN un lexema sigue el patrón d+.d+ (dígitos, punto, dígitos), THE Lexer SHALL reconocerlo como Numeric_Constant de tipo flotante
2. THE Lexer SHALL requerir al menos un dígito antes y después del punto decimal
3. WHEN se completa el reconocimiento de un flotante, THE Token SHALL clasificarlo con gramema de constante flotante

### Requirement 8: Reconocer Constantes en Notación Científica

**User Story:** Como usuario del lenguaje LIA, quiero que el lexer reconozca números en notación científica, para que pueda representar valores muy grandes o muy pequeños.

#### Acceptance Criteria

1. WHEN un lexema sigue el patrón d+.d+(E|e)[+|-]d+ (flotante seguido de E o e, signo opcional, y exponente), THE Lexer SHALL reconocerlo como Numeric_Constant en notación científica
2. THE Lexer SHALL aceptar tanto 'E' mayúscula como 'e' minúscula
3. THE Lexer SHALL aceptar signo positivo (+) o negativo (-) opcional después de E/e
4. THE Lexer SHALL requerir al menos un dígito en el exponente
5. WHEN se completa el reconocimiento de notación científica, THE Token SHALL clasificarlo con gramema de constante científica

### Requirement 9: Reconocer Constantes de Carácter

**User Story:** Como usuario del lenguaje LIA, quiero que el lexer reconozca constantes de carácter, para que pueda usar valores de tipo char en mi código.

#### Acceptance Criteria

1. WHEN un lexema sigue el patrón 'c' (comilla simple, carácter, comilla simple), THE Lexer SHALL reconocerlo como Character_Constant
2. THE Lexer SHALL aceptar cualquier carácter imprimible entre las comillas simples
3. IF las comillas simples no están balanceadas o falta el carácter, THEN THE Lexer SHALL generar un error léxico
4. WHEN se completa el reconocimiento de una constante de carácter, THE Token SHALL clasificarlo con gramema de constante carácter

### Requirement 10: Reconocer Constantes de Cadena

**User Story:** Como usuario del lenguaje LIA, quiero que el lexer reconozca cadenas de texto, para que pueda usar valores de tipo string en mi código.

#### Acceptance Criteria

1. WHEN un lexema inicia con comilla doble ("), THE Lexer SHALL reconocer el inicio de una String_Constant
2. THE Lexer SHALL aceptar cualquier secuencia de caracteres hasta encontrar la comilla doble de cierre
3. IF las comillas dobles no están balanceadas, THEN THE Lexer SHALL generar un error léxico
4. WHEN se completa el reconocimiento de una cadena, THE Token SHALL clasificarlo con gramema de constante string

### Requirement 11: Reconocer Operadores Aritméticos

**User Story:** Como usuario del lenguaje LIA, quiero que el lexer reconozca operadores aritméticos, para que pueda realizar operaciones matemáticas en mi código.

#### Acceptance Criteria

1. THE Lexer SHALL reconocer los siguientes operadores aritméticos: +, -, *, /, %
2. WHEN se identifica un Operator aritmético, THE Token SHALL clasificarlo con gramema de operador aritmético
3. THE Lexer SHALL reconocer cada operador como un token individual

### Requirement 12: Reconocer Operadores Relacionales

**User Story:** Como usuario del lenguaje LIA, quiero que el lexer reconozca operadores relacionales, para que pueda realizar comparaciones en mi código.

#### Acceptance Criteria

1. THE Lexer SHALL reconocer los siguientes operadores relacionales: ==, !=, <, <=, >, >=
2. THE Lexer SHALL distinguir entre operadores de un carácter (<, >) y dos caracteres (==, !=, <=, >=)
3. THE Lexer SHALL distinguir entre el operador de asignación (=) y el operador de igualdad (==)
4. WHEN se identifica un Operator relacional, THE Token SHALL clasificarlo con gramema de operador relacional

### Requirement 13: Reconocer Operadores Lógicos

**User Story:** Como usuario del lenguaje LIA, quiero que el lexer reconozca operadores lógicos, para que pueda realizar operaciones booleanas en mi código.

#### Acceptance Criteria

1. THE Lexer SHALL reconocer los siguientes operadores lógicos: &&, ||, !
2. THE Lexer SHALL distinguir entre operadores de un carácter (!) y dos caracteres (&&, ||)
3. WHEN se identifica un Operator lógico, THE Token SHALL clasificarlo con gramema de operador lógico

### Requirement 14: Reconocer Delimitadores

**User Story:** Como usuario del lenguaje LIA, quiero que el lexer reconozca delimitadores, para que pueda estructurar correctamente mi código.

#### Acceptance Criteria

1. THE Lexer SHALL reconocer los siguientes delimitadores: (, ), [, ], {, }, ;, ,, :
2. WHEN se identifica un Delimiter, THE Token SHALL clasificarlo con gramema de delimitador
3. THE Lexer SHALL reconocer cada delimitador como un token individual
4. WHEN se alcanza un estado de delimitador (rango 100-199), THE Lexer SHALL clasificarlo inmediatamente sin acumular más caracteres

### Requirement 15: Reconocer Comentarios

**User Story:** Como usuario del lenguaje LIA, quiero que el lexer reconozca y descarte comentarios, para que pueda documentar mi código.

#### Acceptance Criteria

1. WHEN se encuentra el carácter $, THE Lexer SHALL reconocer el inicio de un Comment
2. THE Lexer SHALL descartar todos los caracteres desde $ hasta el final de la línea (carácter de nueva línea)
3. THE Lexer SHALL NO generar un token para los comentarios
4. WHEN se completa un comentario, THE Lexer SHALL continuar el análisis en la siguiente línea

### Requirement 16: Manejar Espacios en Blanco

**User Story:** Como usuario del lenguaje LIA, quiero que el lexer ignore espacios en blanco, para que pueda formatear mi código libremente.

#### Acceptance Criteria

1. THE Lexer SHALL reconocer espacios en blanco (espacio, tabulación, nueva línea) como separadores de tokens
2. THE Lexer SHALL descartar los espacios en blanco y NO generar tokens para ellos
3. WHEN se encuentran múltiples espacios en blanco consecutivos, THE Lexer SHALL descartarlos todos
4. THE Lexer SHALL usar espacios en blanco para delimitar tokens cuando sea necesario

### Requirement 17: Clasificar Tokens

**User Story:** Como desarrollador del compilador, quiero una función que clasifique tokens reconocidos, para que el parser pueda procesar la salida del lexer.

#### Acceptance Criteria

1. THE Lexer SHALL implementar una función Token que reciba un lexema y un estado de aceptación
2. WHEN se invoca Token, THE Token SHALL determinar el gramema correspondiente al lexema
3. WHEN el lexema es una Reserved_Word, THE Token SHALL clasificarlo con el gramema específico de esa palabra reservada
4. WHEN el lexema es un Identifier, THE Token SHALL verificar que cumple las reglas de identificadores válidos
5. THE Token SHALL generar una salida que incluya el lexema y su gramema
6. THE Token SHALL retornar el control a la función Analiza para continuar el análisis

### Requirement 18: Reportar Errores Léxicos

**User Story:** Como usuario del compilador, quiero que el lexer reporte errores léxicos con mensajes descriptivos, para que pueda corregir mi código fuente.

#### Acceptance Criteria

1. THE Lexer SHALL implementar una función Error que reciba un estado de error y el contexto del error
2. WHEN se alcanza un Error_State (rango 500+), THE Error SHALL generar un mensaje descriptivo del error
3. THE Error SHALL incluir en el mensaje el número de línea y columna donde ocurrió el error
4. THE Error SHALL incluir el carácter o secuencia de caracteres que causó el error
5. THE Error SHALL incluir una descripción del tipo de error (identificador inválido, constante mal formada, carácter no reconocido, etc.)
6. WHEN se reporta un error, THE Lexer SHALL continuar el análisis para detectar errores adicionales (modo pánico)

### Requirement 19: Implementar Matriz de Transición del AFD

**User Story:** Como desarrollador del compilador, quiero una matriz de transición que represente el AFD, para que el lexer pueda reconocer todos los tokens del lenguaje LIA.

#### Acceptance Criteria

1. THE Lexer SHALL definir una Transition_Matrix bidimensional donde las filas representan estados y las columnas representan clases de caracteres
2. THE Transition_Matrix SHALL usar el rango 0-99 para estados de transición (Transition_State)
3. THE Transition_Matrix SHALL usar el rango 100-199 para estados de aceptación (Acceptance_State)
4. THE Transition_Matrix SHALL usar el rango 500+ para estados de error (Error_State)
5. WHEN el State actual y la columna del carácter se consultan en la Transition_Matrix, THE Lexer SHALL obtener el siguiente estado
6. THE Transition_Matrix SHALL ser extensible para agregar nuevos tokens en el futuro

### Requirement 20: Generar Salida de Tokens

**User Story:** Como usuario del compilador, quiero que el lexer genere una lista de tokens reconocidos, para que pueda verificar el análisis léxico y alimentar la siguiente fase del compilador.

#### Acceptance Criteria

1. THE Lexer SHALL generar una salida que liste todos los tokens reconocidos en orden de aparición
2. WHEN se reconoce un token, THE Lexer SHALL mostrar el lexema y su gramema
3. THE Lexer SHALL incluir el número de línea donde aparece cada token
4. WHEN se completa el análisis del archivo, THE Lexer SHALL mostrar un resumen con el total de tokens reconocidos y errores encontrados

### Requirement 21: Implementar Función Principal

**User Story:** Como usuario del compilador, quiero un punto de entrada que inicialice el lexer y procese argumentos de línea de comandos, para que pueda ejecutar el analizador léxico fácilmente.

#### Acceptance Criteria

1. THE Lexer SHALL implementar una función main que sea el punto de entrada del programa
2. WHEN se ejecuta el programa sin argumentos, THE main SHALL mostrar un mensaje de uso y terminar
3. WHEN se ejecuta el programa con un nombre de archivo, THE main SHALL invocar la función Analiza con ese archivo
4. THE main SHALL manejar excepciones y errores de sistema de manera apropiada
5. WHEN el análisis se completa exitosamente, THE main SHALL retornar código de salida 0
6. WHEN ocurren errores durante el análisis, THE main SHALL retornar un código de salida diferente de 0

### Requirement 22: Implementación Manual del Analizador Léxico

**User Story:** Como desarrollador del proyecto, quiero que el analizador léxico esté implementado manualmente sin usar herramientas generadoras automáticas, para que cumpla con los requisitos académicos del curso y demuestre comprensión de la teoría de autómatas.

#### Acceptance Criteria

1. THE Lexer SHALL estar implementado en C++ estándar (C++11 o superior)
2. THE Lexer SHALL utilizar librerías estándar de C++ (iostream, fstream, string, vector, map, etc.)
3. THE Lexer SHALL NO utilizar herramientas generadoras de lexers (flex, lex, ANTLR, yacc, bison, etc.)
4. THE Lexer SHALL implementar manualmente la matriz de transición del AFD
5. THE Lexer SHALL implementar manualmente todas las funciones de análisis (Analiza, relaciona, Token, Error)
6. THE Lexer SHALL compilar con compiladores estándar (g++, clang++, MSVC) sin dependencias externas

### Requirement 23: Documentación del Autómata Finito Determinístico

**User Story:** Como desarrollador del compilador, quiero tener una representación clara del AFD del lenguaje LIA, para que pueda implementar correctamente la matriz de transición.

#### Acceptance Criteria

1. THE Lexer SHALL incluir documentación del AFD en formato de comentarios en el código fuente
2. THE Lexer SHALL documentar los estados del autómata (0-99 transición, 100-199 aceptación, 500+ error)
3. THE Lexer SHALL documentar las columnas de la matriz de transición y su mapeo a conjuntos de caracteres
4. THE Lexer SHALL incluir ejemplos de transiciones para cada tipo de token reconocido
5. THE Lexer SHALL mantener consistencia entre el AFD documentado y la implementación de la matriz de transición
6. WHERE se define la Transition_Matrix, THE Lexer SHALL incluir comentarios explicando la estructura del AFD

### Requirement 24: Interfaz Gráfica de Usuario

**User Story:** Como usuario del compilador, quiero una interfaz gráfica intuitiva para cargar archivos, analizar código y visualizar resultados, para que pueda usar el analizador léxico de manera interactiva.

#### Acceptance Criteria

1. THE Lexer SHALL proporcionar una interfaz gráfica de usuario (GUI) con layout de dos paneles
2. THE GUI SHALL incluir un panel principal izquierdo con área de texto para mostrar el código fuente
3. THE GUI SHALL incluir un panel derecho dividido en tres secciones: Tokens, Sintaxis y Errores
4. THE GUI SHALL implementar un botón "Abrir" para cargar archivos *.lia desde el sistema de archivos
5. THE GUI SHALL implementar un botón "Guardar" para guardar el código fuente editado
6. THE GUI SHALL implementar un botón "Limpiar" para borrar el contenido del área de texto y los resultados
7. THE GUI SHALL implementar un botón "Analizar" para ejecutar el análisis léxico del código mostrado
8. THE GUI SHALL implementar un botón "Salir" para cerrar la aplicación
9. WHEN se presiona "Analizar", THE GUI SHALL mostrar los tokens reconocidos en el panel "Token"
10. WHEN se presiona "Analizar", THE GUI SHALL mostrar información sintáctica en el panel "Sintaxis"
11. WHEN se presiona "Analizar", THE GUI SHALL mostrar errores léxicos en el panel "Errores"
12. THE GUI SHALL usar una paleta de colores consistente y profesional

### Requirement 25: Panel de Tokens

**User Story:** Como usuario del compilador, quiero ver una lista de todos los tokens reconocidos, para que pueda verificar que el análisis léxico es correcto.

#### Acceptance Criteria

1. THE GUI SHALL mostrar cada token reconocido en el panel "Token"
2. WHEN se reconoce un token, THE GUI SHALL mostrar el lexema y su gramema
3. THE GUI SHALL mostrar el número de línea donde aparece cada token
4. THE GUI SHALL actualizar el panel "Token" cada vez que se ejecuta el análisis
5. THE GUI SHALL limpiar el panel "Token" cuando se presiona el botón "Limpiar"
6. THE GUI SHALL permitir scroll si la lista de tokens excede el tamaño del panel

### Requirement 26: Panel de Sintaxis

**User Story:** Como usuario del compilador, quiero ver información sobre la estructura sintáctica del código, para que pueda entender la organización del programa.

#### Acceptance Criteria

1. THE GUI SHALL mostrar información sintáctica en el panel "Sintaxis"
2. THE GUI SHALL mostrar estadísticas del análisis (total de tokens, líneas procesadas, etc.)
3. THE GUI SHALL actualizar el panel "Sintaxis" cada vez que se ejecuta el análisis
4. THE GUI SHALL limpiar el panel "Sintaxis" cuando se presiona el botón "Limpiar"
5. THE GUI SHALL permitir scroll si la información excede el tamaño del panel

### Requirement 27: Panel de Errores

**User Story:** Como usuario del compilador, quiero ver una lista clara de todos los errores léxicos encontrados, para que pueda corregir mi código fuente.

#### Acceptance Criteria

1. THE GUI SHALL mostrar cada error léxico en el panel "Errores"
2. WHEN se encuentra un error, THE GUI SHALL mostrar el número de línea y columna
3. WHEN se encuentra un error, THE GUI SHALL mostrar el carácter o secuencia que causó el error
4. WHEN se encuentra un error, THE GUI SHALL mostrar una descripción del tipo de error
5. THE GUI SHALL actualizar el panel "Errores" cada vez que se ejecuta el análisis
6. THE GUI SHALL limpiar el panel "Errores" cuando se presiona el botón "Limpiar"
7. IF no hay errores, THEN THE GUI SHALL mostrar un mensaje indicando que el análisis fue exitoso
8. THE GUI SHALL permitir scroll si la lista de errores excede el tamaño del panel

### Requirement 28: Funcionalidad de Abrir Archivo

**User Story:** Como usuario del compilador, quiero abrir archivos *.lia desde mi sistema de archivos, para que pueda analizar código existente.

#### Acceptance Criteria

1. WHEN se presiona el botón "Abrir", THE GUI SHALL mostrar un diálogo de selección de archivos
2. THE GUI SHALL filtrar archivos con extensión *.lia en el diálogo
3. WHEN se selecciona un archivo, THE GUI SHALL cargar su contenido en el área de texto principal
4. IF el archivo no se puede abrir, THEN THE GUI SHALL mostrar un mensaje de error
5. THE GUI SHALL actualizar el título de la ventana con el nombre del archivo abierto

### Requirement 29: Funcionalidad de Guardar Archivo

**User Story:** Como usuario del compilador, quiero guardar el código editado en un archivo, para que pueda preservar mis cambios.

#### Acceptance Criteria

1. WHEN se presiona el botón "Guardar", THE GUI SHALL mostrar un diálogo para guardar archivo
2. THE GUI SHALL sugerir extensión *.lia por defecto
3. WHEN se confirma el guardado, THE GUI SHALL escribir el contenido del área de texto al archivo
4. IF el archivo no se puede guardar, THEN THE GUI SHALL mostrar un mensaje de error
5. THE GUI SHALL actualizar el título de la ventana con el nombre del archivo guardado

### Requirement 30: Funcionalidad de Limpiar

**User Story:** Como usuario del compilador, quiero limpiar el área de trabajo, para que pueda empezar un nuevo análisis desde cero.

#### Acceptance Criteria

1. WHEN se presiona el botón "Limpiar", THE GUI SHALL borrar el contenido del área de texto principal
2. WHEN se presiona el botón "Limpiar", THE GUI SHALL limpiar el panel "Token"
3. WHEN se presiona el botón "Limpiar", THE GUI SHALL limpiar el panel "Sintaxis"
4. WHEN se presiona el botón "Limpiar", THE GUI SHALL limpiar el panel "Errores"
5. THE GUI SHALL restablecer el título de la ventana al valor por defecto

### Requirement 31: Funcionalidad de Analizar

**User Story:** Como usuario del compilador, quiero ejecutar el análisis léxico del código mostrado, para que pueda ver los tokens y errores encontrados.

#### Acceptance Criteria

1. WHEN se presiona el botón "Analizar", THE GUI SHALL invocar la función Analiza con el contenido del área de texto
2. WHEN el análisis está en progreso, THE GUI SHALL mostrar un indicador de progreso o deshabilitar el botón
3. WHEN el análisis se completa, THE GUI SHALL actualizar los paneles Token, Sintaxis y Errores
4. IF el área de texto está vacía, THEN THE GUI SHALL mostrar un mensaje indicando que no hay código para analizar
5. THE GUI SHALL permitir ejecutar múltiples análisis sin reiniciar la aplicación

### Requirement 32: Implementación con Qt Framework

**User Story:** Como desarrollador del proyecto, quiero usar Qt Widgets para implementar la interfaz gráfica, para que tenga una GUI profesional y multiplataforma.

#### Acceptance Criteria

1. THE GUI SHALL estar implementada usando Qt Widgets (Qt 5 o Qt 6)
2. THE GUI SHALL usar QTextEdit para el área de código y los paneles de salida
3. THE GUI SHALL usar QPushButton para los botones de control
4. THE GUI SHALL usar QHBoxLayout y QVBoxLayout para organizar los componentes
5. THE GUI SHALL compilar con Qt Creator o qmake
6. THE GUI SHALL ser funcional en Windows (plataforma objetivo principal)
