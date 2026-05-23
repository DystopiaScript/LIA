#include "Lexer.h"
#include <fstream>
#include <cctype>

// INICIALIZACIÓN DE DATOS ESTÁTICOS

const int Lexer::MATRIZ_TRANSICION[NUM_ESTADOS][NUM_COLUMNAS] = {
// MATRIZ DE TRANSICIONES LIA (20 Estados x 34 Columnas)
// ==============================================================================================================================================
// Col:     0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29   30   31   32   33
// Char:  | l  | L  | d  | _  | .  | E  | e  | +  | -  | *  | /  | %  | &  | =  | !  | >  | <  | '  | "  | $  | [  | ]  | (  | )  | |  | {  | }  | ,  | :  | ;  |dif | \t | \n | \b |
// ==============================================================================================================================================
//      | l  | L  | d  | _  | .  | E  | e  | +  | -  | *  | /  | %  | &  | =  | !  | >  | <  | '  | "  | $  | [  | ]  | (  | )  | |  | {  | }  | ,  | :  | ;  |dif | \t | \n | \b |
/*q0*/  {  1,   2,   3, 506, 506,   2,   1, 105, 106, 107, 108, 128,  13,   9,  12,  11,  10,  15,  17,  19, 121, 122, 119, 120,  14, 129, 130, 124, 131, 123, 506,   0,   0,   0},
/*q1*/  {  1,   2,   2,   2, 100,   2,   1, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},
/*q2*/  {  2,   2,   2,   2, 101,   2,   2, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101},
/*q3*/  {102, 102,   3, 102,   4, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102},
/*q4*/  {500, 500,   5, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500},
/*q5*/  {103, 103,   5, 103, 103,   6,   6, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103},
/*q6*/  {501, 501,   8, 501, 501, 501, 501,   7,   7, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501},
/*q7*/  {502, 502,   8, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502},
/*q8*/  {104, 104,   8, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104},
/*q9*/  {109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 110, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109},
/*q10*/ {111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 112, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111},
/*q11*/ {113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 114, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113},
/*q12*/ {116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 115, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116},
/*q13*/ {503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 117, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503},
/*q14*/ {504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 118, 504, 504, 504, 504, 504, 504, 504, 504, 504},
/*q15*/ { 16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16, 505,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16},
/*q16*/ {507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 125, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507},
/*q17*/ { 17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  18,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17,  17, 508,  17},
/*q18*/ {126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126,  17, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126},
/*q19*/ { 19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19,  19, 127,  19}
};


// La tabla de palabras reservadas  

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
            // ESTADO DE ACEPTACIÓN 
            
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
                addToken(lexemeBuffer, nextState);
            }
            
            // Limpiar y reiniciar
            lexemeBuffer.clear();
            currentState = 0;
            
            // Si no incluimos el carácter, no avanzamos (backtrack implícito)
            // El siguiente ciclo procesará este carácter desde estado 0
            
        } else if (nextState >= 500) {
            // ESTADO DE ERROR
            
            // Reportar el error
            addError(nextState, c);
            
            // Modo pánico: saltar hasta el siguiente delimitador
            skipToNextDelimiter();
            
            // Limpiar y reiniciar
            lexemeBuffer.clear();
            currentState = 0;
            
        } else if (nextState == 0) {
            // WHITESPACE O RETORNO A ESTADO INICIAL 
            
            // Si estamos en estado 0 y vamos a estado 0, es whitespace
            if (currentState == 0) {
                getChar();  // Consumir y descartar
            } else {
                // Transición desde otro estado a 0 (no debería pasar normalmente)
                currentState = 0;
            }
            
        } else {
            // ESTADO DE TRANSICIÓN (1-99) 
            
            // Acumular carácter en el buffer
            lexemeBuffer += c;
            
            // Actualizar estado
            currentState = nextState;
            
            // Avanzar posición
            getChar();
        }
    }
    
    // ========== MANEJO DEL FINAL DEL CÓDIGO FUENTE ==========
    // El AFD no tiene una transición de error para EOF dentro de un string,
    // así que detectamos el estado final manualmente.

    if (currentState == 17) {
        // Estado 17 al EOF: estábamos dentro del string y nunca encontramos
        // la comilla de cierre → Error 508 (string sin cerrar)
        addError(ERROR_STRING_SIN_CERRAR, '"');
        lexemeBuffer.clear();
        currentState = 0;
    } else if (currentState == 18) {
        // Estado 18 al EOF: la última " que leímos sí era de cierre
        // (no había otra " para duplicarla) → string válido (gramema 126)
        addToken(lexemeBuffer, 126);
        lexemeBuffer.clear();
        currentState = 0;
    } else if (!lexemeBuffer.empty() && currentState >= 100 && currentState < 200) {
        // Token pendiente en estado de aceptación (caso defensivo)
        addToken(lexemeBuffer, currentState);
    }
}

// FUNCIONES AUXILIARES DEL ANÁLISIS

int Lexer::relaciona(char c) {
    // Letras minúsculas (a-z) - COL 0
    if (c >= 'a' && c <= 'z') {
        if (c == 'e') {
            return COL_e_MIN;  // 6 - e minúscula (para notación científica)
        }
        return COL_l;  // 0 - Letras minúsculas
    }
    
    // Letras mayúsculas (A-Z) - COL 1
    if (c >= 'A' && c <= 'Z') {
        if (c == 'E') {
            return COL_E_MAY;  // 5 - E mayúscula (para notación científica)
        }
        return COL_L;  // 1 - Letras mayúsculas
    }
    
    // Dígitos (0-9) - COL 2
    if (c >= '0' && c <= '9') {
        return COL_d;  // 2 - Dígitos
    }
    
    // Caracteres especiales individuales
    // Orden según la matriz: l, L, d, _, ., E, e, +, -, *, /, %, &, =, !, >, <, ', ", $, [, ], (, ), |, {, }, ,, :, ;, dif, \t, \n, \b
    switch (c) {
        case '_':  return COL_GUION;      // 3
        case '.':  return COL_PUNTO;      // 4
        case '+':  return COL_SUMA;       // 7
        case '-':  return COL_RESTA;      // 8
        case '*':  return COL_MULT;       // 9
        case '/':  return COL_DIV;        // 10
        case '%':  return COL_MOD;        // 11
        case '&':  return COL_AMP;        // 12
        case '=':  return COL_IGUAL;      // 13
        case '!':  return COL_EXCL;       // 14
        case '>':  return COL_MAYOR;      // 15
        case '<':  return COL_MENOR;      // 16
        case '\'': return COL_COMILLA_S;  // 17
        case '"':  return COL_COMILLA_D;  // 18
        case '$':  return COL_DOLAR;      // 19
        case '[':  return COL_CORCH_A;    // 20
        case ']':  return COL_CORCH_C;    // 21
        case '(':  return COL_PAREN_A;    // 22
        case ')':  return COL_PAREN_C;    // 23
        case '|':  return COL_PIPE;       // 24
        case '{':  return COL_LLAVE_A;    // 25
        case '}':  return COL_LLAVE_C;    // 26
        case ',':  return COL_COMA;       // 27
        case ':':  return COL_DPUNTOS;    // 28
        case ';':  return COL_PCOMA;      // 29
        case '\t': return COL_TAB;        // 31
        case '\n': return COL_NEWLINE;    // 32
        case ' ':  return COL_ESPACIO;    // 33
        
        // Cualquier otro carácter no reconocido
        default:   return COL_OTRO;       // 30
    }
}

void Lexer::addToken(const std::string& lexema, int estadoAceptacion) {
    int gramema = estadoAceptacion;
    
    // Caso especial: estado 100 puede ser palabra reservada o identificador
    // Nota: el AFD acepta cualquier secuencia de l|L|d|_ como identificador,
    // por lo que no se valida formato adicional aquí (variable_ y mi__variable son válidos).
    if (estadoAceptacion == 100) {
        if (isReservedWord(lexema)) {
            gramema = 100;  // Palabra reservada
        } else {
            gramema = 101;  // Identificador
        }
    }
    
    // Crear el token con la posición donde comenzó (aproximada)
    ::Token token(lexema, gramema, currentLine, currentColumn - lexema.length());
    tokens.push_back(token);
}

void Lexer::addError(int estadoError, char caracterProblematico) {
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
    // El AFD ya garantiza la forma del identificador (L|l|d|_ en bucle).
    // Solo verificamos que no esté vacío.
    return !lexema.empty();
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
