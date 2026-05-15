/**
 * @file Lexer.cpp
 * @brief Implementación del analizador léxico LIA
 * 
 * Este archivo contiene la implementación del motor del lexer.
 * IMPORTANTE: Este es un ESQUELETO - las funciones están vacías con comentarios TODO
 * que explican qué debe implementarse en cada una.
 */

#include "Lexer.h"
#include <fstream>
#include <cctype>

// ========== INICIALIZACIÓN DE DATOS ESTÁTICOS ==========

/**
 * TODO: Llenar la matriz de transición del AFD
 * 
 * Esta matriz define el comportamiento del autómata.
 * Ver docs/matriz_transicion.md para la plantilla completa.
 * Ver docs/AFD_LIA.md para la descripción del autómata.
 * 
 * Ejemplo de cómo llenar una fila:
 * Estado 0 (inicial):
 * {1, 1, 3, 500, 500, 1, 1, 105, 106, 107, 108, 128, 9, 11, 12, 13, 14, 18, 119, 120, 121, 122, 129, 130, 123, 124, 131, 15, 17, 19, 0, 0, 0, 500}
 * 
 * Donde cada posición corresponde a:
 * [0]=L, [1]=l, [2]=d, [3]=_, [4]=., [5]=E, [6]=e, [7]=+, [8]=-, [9]=*, [10]=/,
 * [11]=%, [12]==, [13]=<, [14]=>, [15]=!, [16]=&, [17]=|, [18]=(, [19]=),
 * [20]=[, [21]=], [22]={, [23]=}, [24]=;, [25]=,, [26]=:, [27]=', [28]=",
 * [29]=$, [30]=\t, [31]=\n, [32]=espacio, [33]=otro
 */
const int Lexer::MATRIZ_TRANSICION[NUM_ESTADOS][NUM_COLUMNAS] = {
    // Índices de columnas:
    //  0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29   30   31   32   33
    //  L    l    d    _    .    E    e    +    -    *    /    %    =    <    >    !    &    |    (    )    [    ]    {    }    ;    ,    :    '    "    $   \t   \n   sp  otro
    
    // Estado 0: Estado inicial
    {   1,   1,   3, 500, 500,   1,   1, 105, 106, 107, 108, 128,   9,  11,  12,  13,  14,  18, 119, 120, 121, 122, 129, 130, 123, 124, 131,  15,  17,  19,   0,   0,   0, 500},
    
    // Estado 1: Identificador/palabra reservada (acumula letras y dígitos)
    {   1,   1,   1,   2, 100,   1,   1, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
    
    // Estado 2: Después de guión bajo en identificador
    {   1,   1,   1, 508, 101,   1,   1, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101},
    
    // Estado 3: Número entero (acumula dígitos)
    { 102, 102,   3, 102,   4, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102},
    
    // Estado 4: Después del punto decimal (esperando dígitos)
    { 501, 501,   5, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501},
    
    // Estado 5: Número flotante (acumula dígitos después del punto)
    { 103, 103,   5, 103, 103,   6,   6, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103},
    
    // Estado 6: Después de E/e en notación científica (esperando signo o dígito)
    { 502, 502,   8, 502, 502, 502, 502,   7,   7, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502},
    
    // Estado 7: Después de signo en exponente (esperando dígito)
    { 503, 503,   8, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503},
    
    // Estado 8: Exponente de notación científica (acumula dígitos)
    { 104, 104,   8, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104},
    
    // Estado 9: Después de = (puede ser = o ==)
    { 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 110, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109},
    
    // Estado 10: (No usado - reservado para expansión futura)
    { 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500},
    
    // Estado 11: Después de < (puede ser < o <=)
    { 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 112, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111},
    
    // Estado 12: Después de > (puede ser > o >=)
    { 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 113, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114},
    
    // Estado 13: Después de ! (puede ser ! o !=)
    { 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 115, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116},
    
    // Estado 14: Después de & (esperando segundo &)
    { 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 117, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504},
    
    // Estado 15: Después de comilla simple (esperando carácter)
    {  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16, 505,  16,  16,  16,  16,  16,  16},
    
    // Estado 16: Carácter dentro de comillas simples (esperando comilla de cierre)
    { 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 125, 505, 505, 505, 505, 505, 505},
    
    // Estado 17: Dentro de string (acumula hasta encontrar ")
    {  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17, 126,  17,  17, 507,  17,  17},
    
    // Estado 18: Después de | (esperando segundo |)
    { 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 118, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509},
    
    // Estado 19: Dentro de comentario (acumula hasta encontrar \n)
    {  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19, 127,  19,  19}
};

/**
 * TODO: Llenar la tabla de palabras reservadas
 * 
 * Agregar las 17 palabras reservadas del lenguaje LIA:
 * class, endclass, int, float, char, string, bool,
 * if, else, do, while, input, output, def, to, break, loop
 * 
 * Ejemplo:
 * const std::map<std::string, int> Lexer::PALABRAS_RESERVADAS = {
 *     {"class", 100},
 *     {"endclass", 100},
 *     {"int", 100},
 *     // ... resto de palabras
 * };
 */
const std::map<std::string, int> Lexer::PALABRAS_RESERVADAS = {
    {"class", 100},
    {"endclass", 100},
    {"int", 100},
    {"float", 100},
    {"char", 100},
    {"string", 100},
    {"bool", 100},
    {"if", 100},
    {"else", 100},
    {"do", 100},
    {"while", 100},
    {"input", 100},
    {"output", 100},
    {"def", 100},
    {"to", 100},
    {"break", 100},
    {"loop", 100}
};


// ========== CONSTRUCTORES Y DESTRUCTOR ==========

Lexer::Lexer() 
    : sourceCode(""), currentPos(0), currentLine(1), currentColumn(1), 
      currentState(0), lexemeBuffer("") {
    // Constructor por defecto
    // TODO: Inicializar cualquier otro miembro si es necesario
}

Lexer::Lexer(const std::string& source) 
    : sourceCode(source), currentPos(0), currentLine(1), currentColumn(1),
      currentState(0), lexemeBuffer("") {
    // Constructor con código fuente
    // TODO: Inicializar cualquier otro miembro si es necesario
}

Lexer::~Lexer() {
    // Destructor
    // TODO: Liberar recursos si es necesario (probablemente no hay nada que hacer)
}

// ========== FUNCIÓN PRINCIPAL DE ANÁLISIS ==========

void Lexer::Analiza() {
    /**
     * TODO: Implementar el algoritmo principal del análisis léxico
     * 
     * Algoritmo:
     * 1. Reiniciar estado: currentState = 0, currentPos = 0, limpiar tokens y errores
     * 2. Mientras currentPos < sourceCode.length():
     *    a. Obtener carácter actual: char c = sourceCode[currentPos]
     *    b. Mapear a columna: int col = relaciona(c)
     *    c. Obtener siguiente estado: int nextState = MATRIZ_TRANSICION[currentState][col]
     *    d. Analizar el siguiente estado:
     *       - Si nextState >= 100 && nextState < 200: Estado de aceptación
     *         * Llamar Token(lexemeBuffer, nextState)
     *         * Limpiar lexemeBuffer
     *         * Reiniciar currentState = 0
     *         * Considerar si necesita backtrack (ungetChar)
     *       - Si nextState >= 500: Estado de error
     *         * Llamar Error(nextState, c)
     *         * Ejecutar modo pánico: skipToNextDelimiter()
     *         * Limpiar lexemeBuffer
     *         * Reiniciar currentState = 0
     *       - Si nextState == 0: Whitespace o comentario
     *         * No acumular en lexemeBuffer
     *         * Actualizar posición y línea/columna
     *       - Si 0 < nextState < 100: Estado de transición
     *         * Acumular carácter: lexemeBuffer += c
     *         * Actualizar currentState = nextState
     *         * Avanzar posición
     *    e. Actualizar currentLine si c == '\n'
     * 3. Al final, verificar si quedó un token pendiente en lexemeBuffer
     * 
     * Ejemplo de estructura:
     * 
     * reset();  // Limpiar estado anterior
     * 
     * while (currentPos < sourceCode.length()) {
     *     char c = getChar();  // Obtiene y avanza
     *     int col = relaciona(c);
     *     int nextState = MATRIZ_TRANSICION[currentState][col];
     *     
     *     if (nextState >= 100 && nextState < 200) {
     *         // Token reconocido
     *         Token(lexemeBuffer, nextState);
     *         lexemeBuffer.clear();
     *         currentState = 0;
     *     } else if (nextState >= 500) {
     *         // Error
     *         Error(nextState, c);
     *         skipToNextDelimiter();
     *         lexemeBuffer.clear();
     *         currentState = 0;
     *     } else if (nextState == 0) {
     *         // Whitespace - no acumular
     *         // Ya avanzamos con getChar()
     *     } else {
     *         // Estado de transición - acumular
     *         lexemeBuffer += c;
     *         currentState = nextState;
     *     }
     * }
     */
    
    // TODO: Implementar aquí
}

// ========== FUNCIONES AUXILIARES DEL ANÁLISIS ==========

int Lexer::relaciona(char c) {
    // Letras mayúsculas (A-Z)
    if (c >= 'A' && c <= 'Z') {
        if (c == 'E') {
            return COL_E_MAY;  // 5 - E mayúscula (para notación científica)
        }
        return COL_L;  // 0 - Letras mayúsculas
    }
    
    // Letras minúsculas (a-z)
    if (c >= 'a' && c <= 'z') {
        if (c == 'e') {
            return COL_e_MIN;  // 6 - e minúscula (para notación científica)
        }
        return COL_l;  // 1 - Letras minúsculas
    }
    
    // Dígitos (0-9)
    if (c >= '0' && c <= '9') {
        return COL_d;  // 2 - Dígitos
    }
    
    // Caracteres especiales individuales
    switch (c) {
        case '_':  return COL_GUION;      // 3
        case '.':  return COL_PUNTO;      // 4
        case '+':  return COL_SUMA;       // 7
        case '-':  return COL_RESTA;      // 8
        case '*':  return COL_MULT;       // 9
        case '/':  return COL_DIV;        // 10
        case '%':  return COL_MOD;        // 11
        case '=':  return COL_IGUAL;      // 12
        case '<':  return COL_MENOR;      // 13
        case '>':  return COL_MAYOR;      // 14
        case '!':  return COL_EXCL;       // 15
        case '&':  return COL_AMP;        // 16
        case '|':  return COL_PIPE;       // 17
        case '(':  return COL_PAREN_A;    // 18
        case ')':  return COL_PAREN_C;    // 19
        case '[':  return COL_CORCH_A;    // 20
        case ']':  return COL_CORCH_C;    // 21
        case '{':  return COL_LLAVE_A;    // 22
        case '}':  return COL_LLAVE_C;    // 23
        case ';':  return COL_PCOMA;      // 24
        case ',':  return COL_COMA;       // 25
        case ':':  return COL_DPUNTOS;    // 26
        case '\'': return COL_COMILLA_S;  // 27
        case '"':  return COL_COMILLA_D;  // 28
        case '$':  return COL_DOLAR;      // 29
        case '\t': return COL_TAB;        // 30
        case '\n': return COL_NEWLINE;    // 31
        case ' ':  return COL_ESPACIO;    // 32
        
        // Cualquier otro carácter no reconocido
        default:   return COL_OTRO;       // 33
    }
}

void Lexer::Token(const std::string& lexema, int estadoAceptacion) {
    /**
     * TODO: Implementar la clasificación y creación de tokens
     * 
     * Pasos:
     * 1. Determinar el gramema según el estado de aceptación
     * 2. Caso especial: estado 100 puede ser palabra reservada o identificador
     *    - Verificar si está en PALABRAS_RESERVADAS
     *    - Si está: gramema = 100 (palabra reservada)
     *    - Si no está: verificar que sea identificador válido (isValidIdentifier)
     *      * Si es válido: gramema = 101 (identificador)
     *      * Si no es válido: reportar error y retornar
     * 3. Crear objeto Token con: lexema, gramema, currentLine, currentColumn
     * 4. Agregar a la lista: tokens.push_back(token)
     * 
     * Ejemplo:
     * 
     * int gramema = estadoAceptacion;
     * 
     * if (estadoAceptacion == 100) {
     *     if (isReservedWord(lexema)) {
     *         gramema = 100;  // Palabra reservada
     *     } else {
     *         if (!isValidIdentifier(lexema)) {
     *             Error(508, lexema[lexema.length()-1]);
     *             return;
     *         }
     *         gramema = 101;  // Identificador
     *     }
     * }
     * 
     * Token token(lexema, gramema, currentLine, currentColumn - lexema.length());
     * tokens.push_back(token);
     */
    
    // TODO: Implementar aquí
}

void Lexer::Error(int estadoError, char caracterProblematico) {
    /**
     * TODO: Implementar el reporte de errores
     * 
     * Pasos:
     * 1. Crear objeto Error con:
     *    - currentLine
     *    - currentColumn
     *    - estadoError
     *    - string(1, caracterProblematico) o lexemeBuffer si es más apropiado
     * 2. Agregar a la lista: errors.push_back(error)
     * 3. Ejecutar modo pánico: skipToNextDelimiter()
     * 4. Reiniciar estado: currentState = 0, lexemeBuffer.clear()
     * 
     * Ejemplo:
     * 
     * Error error(currentLine, currentColumn, estadoError, std::string(1, caracterProblematico));
     * errors.push_back(error);
     */
    
    // TODO: Implementar aquí
}

// ========== FUNCIONES DE NAVEGACIÓN DEL CÓDIGO ==========

char Lexer::peekChar() {
    /**
     * TODO: Implementar peek (ver sin consumir)
     * 
     * Retorna el siguiente carácter sin avanzar currentPos
     * Si currentPos >= sourceCode.length(), retornar '\0'
     * 
     * Ejemplo:
     * if (currentPos < sourceCode.length()) {
     *     return sourceCode[currentPos];
     * }
     * return '\0';
     */
    
    // TODO: Implementar aquí
    return '\0';
}

char Lexer::getChar() {
    /**
     * TODO: Implementar get (obtener y consumir)
     * 
     * Pasos:
     * 1. Verificar que currentPos < sourceCode.length()
     * 2. Obtener carácter: char c = sourceCode[currentPos]
     * 3. Avanzar posición: currentPos++
     * 4. Actualizar columna: currentColumn++
     * 5. Si c == '\n': currentLine++, currentColumn = 1
     * 6. Retornar c
     * 
     * Ejemplo:
     * if (currentPos >= sourceCode.length()) {
     *     return '\0';
     * }
     * char c = sourceCode[currentPos++];
     * if (c == '\n') {
     *     currentLine++;
     *     currentColumn = 1;
     * } else {
     *     currentColumn++;
     * }
     * return c;
     */
    
    // TODO: Implementar aquí
    return '\0';
}

void Lexer::ungetChar() {
    /**
     * TODO: Implementar backtrack (retroceder)
     * 
     * Usado cuando el último carácter leído no pertenece al token actual
     * 
     * Pasos:
     * 1. Verificar que currentPos > 0
     * 2. Retroceder: currentPos--
     * 3. Actualizar columna: currentColumn--
     * 4. Si el carácter retrocedido es '\n', ajustar currentLine
     *    (esto es complicado, puede requerir buscar hacia atrás)
     * 
     * Nota: Para simplificar, puede no ajustar currentLine si es complejo
     * 
     * Ejemplo:
     * if (currentPos > 0) {
     *     currentPos--;
     *     currentColumn--;
     * }
     */
    
    // TODO: Implementar aquí
}

// ========== FUNCIONES DE VALIDACIÓN ==========

bool Lexer::isReservedWord(const std::string& lexema) {
    /**
     * TODO: Implementar verificación de palabra reservada
     * 
     * Buscar lexema en PALABRAS_RESERVADAS
     * 
     * Ejemplo:
     * return PALABRAS_RESERVADAS.find(lexema) != PALABRAS_RESERVADAS.end();
     */
    
    // TODO: Implementar aquí
    return false;
}

bool Lexer::isValidIdentifier(const std::string& lexema) {
    /**
     * TODO: Implementar validación de identificador
     * 
     * Reglas:
     * 1. No puede terminar con '_'
     * 2. No puede tener '__' (dos guiones bajos consecutivos)
     * 
     * Ejemplo:
     * // Verificar que no termina con _
     * if (lexema.empty() || lexema[lexema.length() - 1] == '_') {
     *     return false;
     * }
     * 
     * // Verificar que no tiene __ consecutivos
     * for (size_t i = 0; i < lexema.length() - 1; i++) {
     *     if (lexema[i] == '_' && lexema[i+1] == '_') {
     *         return false;
     *     }
     * }
     * 
     * return true;
     */
    
    // TODO: Implementar aquí
    return true;
}

// ========== FUNCIONES DE RECUPERACIÓN DE ERRORES ==========

void Lexer::skipToNextDelimiter() {
    /**
     * TODO: Implementar modo pánico
     * 
     * Saltar caracteres hasta encontrar un delimitador o whitespace
     * Delimitadores: ; , ( ) [ ] { } espacio \t \n
     * 
     * Ejemplo:
     * while (currentPos < sourceCode.length()) {
     *     char c = peekChar();
     *     if (c == ';' || c == ',' || c == '(' || c == ')' ||
     *         c == '[' || c == ']' || c == '{' || c == '}' ||
     *         c == ' ' || c == '\t' || c == '\n') {
     *         break;
     *     }
     *     getChar();  // Consumir y descartar
     * }
     */
    
    // TODO: Implementar aquí
}

void Lexer::skipWhitespace() {
    /**
     * TODO: Implementar salto de espacios en blanco
     * 
     * Consumir espacios, tabulaciones y nuevas líneas
     * 
     * Ejemplo:
     * while (currentPos < sourceCode.length()) {
     *     char c = peekChar();
     *     if (c != ' ' && c != '\t' && c != '\n') {
     *         break;
     *     }
     *     getChar();  // Consumir
     * }
     */
    
    // TODO: Implementar aquí
}

void Lexer::skipComment() {
    /**
     * TODO: Implementar salto de comentario
     * 
     * Consumir desde $ hasta \n (inclusive)
     * 
     * Ejemplo:
     * // Asumiendo que ya se leyó el $
     * while (currentPos < sourceCode.length()) {
     *     char c = getChar();
     *     if (c == '\n') {
     *         break;
     *     }
     * }
     */
    
    // TODO: Implementar aquí
}

// ========== FUNCIONES DE ACCESO A RESULTADOS ==========

const std::vector<Token>& Lexer::getTokens() const {
    return tokens;
}

const std::vector<Error>& Lexer::getErrors() const {
    return errors;
}

// ========== FUNCIONES DE CONTROL ==========

void Lexer::setSourceCode(const std::string& source) {
    sourceCode = source;
    reset();
}

void Lexer::reset() {
    /**
     * TODO: Implementar reinicio del lexer
     * 
     * Limpiar todo el estado para un nuevo análisis:
     * - currentPos = 0
     * - currentLine = 1
     * - currentColumn = 1
     * - currentState = 0
     * - lexemeBuffer.clear()
     * - tokens.clear()
     * - errors.clear()
     */
    
    currentPos = 0;
    currentLine = 1;
    currentColumn = 1;
    currentState = 0;
    lexemeBuffer.clear();
    tokens.clear();
    errors.clear();
}

bool Lexer::loadFromFile(const std::string& filename) {
    /**
     * TODO: Implementar carga desde archivo
     * 
     * Pasos:
     * 1. Abrir archivo con std::ifstream
     * 2. Verificar que se abrió correctamente
     * 3. Leer todo el contenido a sourceCode
     * 4. Cerrar archivo
     * 5. Llamar reset()
     * 6. Retornar true si éxito, false si error
     * 
     * Ejemplo:
     * std::ifstream file(filename);
     * if (!file.is_open()) {
     *     return false;
     * }
     * 
     * std::string content((std::istreambuf_iterator<char>(file)),
     *                      std::istreambuf_iterator<char>());
     * file.close();
     * 
     * sourceCode = content;
     * reset();
     * return true;
     */
    
    // TODO: Implementar aquí
    return false;
}
