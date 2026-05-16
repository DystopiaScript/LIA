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
    
    // Estado 0: Estado inicial - Basado en el diagrama del PDF página 4
    {   1,   1,   3, 500, 500,   1,   1, 105, 106, 107, 108, 128,   9,  11,  12,  13,  14,  18, 119, 120, 121, 122, 129, 130, 123, 124, 131,  15,  17,  19,   0,   0,   0, 500},
    
    // Estado 1: Identificador/palabra reservada (acumula L|l|d)
    // Transiciones: L|l|d → 1 (loop), _ → 2, dif → 100
    {   1,   1,   1,   2, 100,   1,   1, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
    
    // Estado 2: Después de guión bajo en identificador
    // Transiciones: L|l|d → 1, _ → 508 (error: __ consecutivos), dif → 101
    {   1,   1,   1, 508, 101,   1,   1, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101},
    
    // Estado 3: Número entero (acumula dígitos)
    // Transiciones: d → 3 (loop), . → 4, dif → 102 (entero)
    { 102, 102,   3, 102,   4, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102},
    
    // Estado 4: Después del punto decimal (esperando dígitos)
    // Transiciones: d → 5, otro → 501 (error: esperaba dígitos después del punto)
    { 501, 501,   5, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501},
    
    // Estado 5: Número flotante (acumula dígitos después del punto)
    // Transiciones: d → 5 (loop), E|e → 6, dif → 103 (flotante)
    { 103, 103,   5, 103, 103,   6,   6, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103},
    
    // Estado 6: Después de E/e en notación científica
    // Transiciones: +|- → 7, d → 8, otro → 502 (error: esperaba +, - o dígito)
    { 502, 502,   8, 502, 502, 502, 502,   7,   7, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502},
    
    // Estado 7: Después de signo en exponente (esperando dígito)
    // Transiciones: d → 8, otro → 503 (error: esperaba dígito en exponente)
    { 503, 503,   8, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503},
    
    // Estado 8: Exponente de notación científica (acumula dígitos)
    // Transiciones: d → 8 (loop), dif → 104 (notación científica)
    { 104, 104,   8, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104},
    
    // Estado 9: Después de = (puede ser = o ==)
    // Transiciones: = → 110 (==), dif → 109 (=)
    { 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 110, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109},
    
    // Estado 10: Después de / (puede ser división o inicio de comentario - NO USADO en este diseño)
    // El comentario usa $ según el PDF, no //
    { 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500},
    
    // Estado 11: Después de < (puede ser < o <=)
    // Transiciones: = → 112 (<=), dif → 111 (<)
    { 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 112, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111},
    
    // Estado 12: Después de > (puede ser > o >=)
    // Transiciones: = → 113 (>=), dif → 114 (>)
    { 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 113, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114, 114},
    
    // Estado 13: Después de ! (puede ser ! o !=)
    // Transiciones: = → 115 (!=), dif → 116 (!)
    { 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 115, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116},
    
    // Estado 14: Después de & (esperando segundo &)
    // Transiciones: & → 117 (&&), dif → 504 (error: esperaba &&)
    { 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 117, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504},
    
    // Estado 15: Después de comilla simple ' (esperando carácter)
    // Transiciones: cualquier_char → 16, ' → 505 (error: vacío)
    {  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16, 505,  16,  16,  16,  16,  16,  16},
    
    // Estado 16: Carácter dentro de comillas simples (esperando comilla de cierre)
    // Transiciones: ' → 125 (constante carácter), otro → 505 (error: sin cerrar)
    { 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 125, 505, 505, 505, 505, 505, 505},
    
    // Estado 17: Dentro de string " (acumula hasta encontrar ")
    // Transiciones: cualquier_char → 17 (loop), " → 126 (string), \n → 507 (error: sin cerrar)
    {  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17, 126,  17,  17, 507,  17,  17},
    
    // Estado 18: Después de | (esperando segundo |)
    // Transiciones: | → 118 (||), dif → 509 (error: esperaba ||)
    { 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 118, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509, 509},
    
    // Estado 19: Dentro de comentario $ (acumula hasta encontrar \n)
    // Transiciones: cualquier_char → 19 (loop), \n → 127 (comentario completo)
    {  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19, 127,  19,  19}
};

/**
 * TODO: Llenar la tabla de palabras reservadas
 * 
 * Agregar las 17 palabras reservadas del lenguaje LIA:
 * class, endclass, int, float, char, string, bool,
 * if, else, do, while, input, output, def, to, break, loop
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
    // Reiniciar estado
    reset();
    
    // Procesar todo el código fuente
    while (currentPos < sourceCode.length()) {
        char c = peekChar();
        
        // Obtener columna de la matriz de transición
        int col = relaciona(c);
        
        // Obtener siguiente estado
        int nextState = MATRIZ_TRANSICION[currentState][col];
        
        // Analizar el siguiente estado
        if (nextState >= 100 && nextState < 200) {
            // ===== ESTADO DE ACEPTACIÓN =====
            
            // Algunos tokens necesitan incluir el carácter actual
            bool incluirCaracter = false;
            
            // Tokens de un solo carácter que se aceptan inmediatamente
            if (nextState == 105 || nextState == 106 || nextState == 107 || 
                nextState == 108 || nextState == 128 || nextState == 119 || 
                nextState == 120 || nextState == 121 || nextState == 122 || 
                nextState == 123 || nextState == 124 || nextState == 131 ||
                nextState == 129 || nextState == 130) {
                incluirCaracter = true;
            }
            
            // Operadores de dos caracteres
            if (nextState == 110 || nextState == 112 || nextState == 113 || 
                nextState == 115 || nextState == 117 || nextState == 118) {
                incluirCaracter = true;
            }
            
            // Constantes de carácter y string
            if (nextState == 125 || nextState == 126) {
                incluirCaracter = true;
            }
            
            // Comentarios
            if (nextState == 127) {
                // Los comentarios no generan tokens, solo limpiar
                lexemeBuffer.clear();
                currentState = 0;
                getChar();  // Consumir el \n
                continue;
            }
            
            // Incluir carácter si es necesario
            if (incluirCaracter) {
                lexemeBuffer += c;
                getChar();  // Consumir el carácter
            }
            
            // Crear el token
            if (!lexemeBuffer.empty()) {
                Token(lexemeBuffer, nextState);
            }
            
            // Limpiar y reiniciar
            lexemeBuffer.clear();
            currentState = 0;
            
            // Si no incluimos el carácter, no avanzamos (backtrack implícito)
            // El siguiente ciclo procesará este carácter desde estado 0
            
        } else if (nextState >= 500) {
            // ===== ESTADO DE ERROR =====
            
            // Reportar el error
            Error(nextState, c);
            
            // Modo pánico: saltar hasta el siguiente delimitador
            skipToNextDelimiter();
            
            // Limpiar y reiniciar
            lexemeBuffer.clear();
            currentState = 0;
            
        } else if (nextState == 0) {
            // ===== WHITESPACE O RETORNO A ESTADO INICIAL =====
            
            // Si estamos en estado 0 y vamos a estado 0, es whitespace
            if (currentState == 0) {
                getChar();  // Consumir y descartar
            } else {
                // Transición desde otro estado a 0 (no debería pasar normalmente)
                currentState = 0;
            }
            
        } else {
            // ===== ESTADO DE TRANSICIÓN (1-99) =====
            
            // Acumular carácter en el buffer
            lexemeBuffer += c;
            
            // Actualizar estado
            currentState = nextState;
            
            // Avanzar posición
            getChar();
        }
    }
    
    // Al final, verificar si quedó un token pendiente
    if (!lexemeBuffer.empty() && currentState >= 100 && currentState < 200) {
        Token(lexemeBuffer, currentState);
    }
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
    int gramema = estadoAceptacion;
    
    // Caso especial: estado 100 puede ser palabra reservada o identificador
    if (estadoAceptacion == 100) {
        if (isReservedWord(lexema)) {
            gramema = 100;  // Palabra reservada
        } else {
            if (!isValidIdentifier(lexema)) {
                Error(508, lexema.empty() ? '\0' : lexema[lexema.length()-1]);
                return;
            }
            gramema = 101;  // Identificador
        }
    }
    
    // Crear el token con la posición donde comenzó (aproximada)
    ::Token token(lexema, gramema, currentLine, currentColumn - lexema.length());
    tokens.push_back(token);
}

void Lexer::Error(int estadoError, char caracterProblematico) {
    ::Error error(currentLine, currentColumn, estadoError, std::string(1, caracterProblematico));
    errors.push_back(error);
}

// ========== FUNCIONES DE NAVEGACIÓN DEL CÓDIGO ==========

char Lexer::peekChar() {
    // Verificar que no hayamos llegado al final del código
    if (currentPos < sourceCode.length()) {
        return sourceCode[currentPos];  // Retornar sin avanzar
    }
    return '\0';  // Fin del archivo
}

char Lexer::getChar() {
    // Verificar que no hayamos llegado al final
    if (currentPos >= sourceCode.length()) {
        return '\0';  // Fin del archivo
    }
    
    // Obtener el carácter actual
    char c = sourceCode[currentPos];
    
    // Avanzar la posición
    currentPos++;
    
    // Actualizar línea y columna
    if (c == '\n') {
        currentLine++;      // Nueva línea
        currentColumn = 1;  // Reiniciar columna
    } else {
        currentColumn++;    // Avanzar columna
    }
    
    return c;
}

void Lexer::ungetChar() {
    // Verificar que podemos retroceder
    if (currentPos > 0) {
        currentPos--;       // Retroceder posición
        currentColumn--;    // Retroceder columna
        
        // Nota: No ajustamos currentLine porque es complejo
        // y raramente retrocedemos a través de un \n
        // Si fuera necesario, tendríamos que buscar hacia atrás
    }
}

// ========== FUNCIONES DE VALIDACIÓN ==========

bool Lexer::isReservedWord(const std::string& lexema) {
    return PALABRAS_RESERVADAS.find(lexema) != PALABRAS_RESERVADAS.end();
}

bool Lexer::isValidIdentifier(const std::string& lexema) {
    // Verificar que no está vacío
    if (lexema.empty()) {
        return false;
    }
    
    // Verificar que no termina con _
    if (lexema[lexema.length() - 1] == '_') {
        return false;
    }
    
    // Verificar que no tiene __ consecutivos
    for (size_t i = 0; i < lexema.length() - 1; i++) {
        if (lexema[i] == '_' && lexema[i+1] == '_') {
            return false;
        }
    }
    
    return true;
}

// ========== FUNCIONES DE RECUPERACIÓN DE ERRORES ==========

void Lexer::skipToNextDelimiter() {
    while (currentPos < sourceCode.length()) {
        char c = peekChar();
        if (c == ';' || c == ',' || c == '(' || c == ')' ||
            c == '[' || c == ']' || c == '{' || c == '}' ||
            c == ' ' || c == '\t' || c == '\n') {
            break;
        }
        getChar();  // Consumir y descartar
    }
}

void Lexer::skipWhitespace() {
    while (currentPos < sourceCode.length()) {
        char c = peekChar();
        if (c != ' ' && c != '\t' && c != '\n') {
            break;
        }
        getChar();  // Consumir
    }
}

void Lexer::skipComment() {
    // Asumiendo que ya se leyó el $
    while (currentPos < sourceCode.length()) {
        char c = getChar();
        if (c == '\n') {
            break;
        }
    }
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
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    std::string content((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());
    file.close();
    
    sourceCode = content;
    reset();
    return true;
}
