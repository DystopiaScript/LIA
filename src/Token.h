#ifndef TOKEN_H
#define TOKEN_H

#include <string>

/**
 * @file Token.h
 * @brief Definición de la clase Token que representa un token reconocido por el lexer
 * 
 * Un token es la unidad léxica básica compuesta por:
 * - lexema: el texto reconocido (ej: "int", "variable", "123")
 * - gramema: la categoría del token (ej: palabra reservada, identificador, constante)
 * - linea: número de línea donde aparece en el código fuente
 * - columna: posición de columna donde inicia el token
 */

/**
 * @enum Gramema
 * @brief Categorías de tokens reconocidos por el lenguaje LIA
 * 
 * Rangos:
 * - 100-131: Estados de aceptación (tokens válidos)
 * - Cada gramema corresponde a un tipo específico de token
 */
enum Gramema {
    // Palabras reservadas e identificadores
    PALABRA_RESERVADA = 100,    // class, if, while, etc.
    IDENTIFICADOR = 101,         // nombres de variables y funciones
    
    // Constantes numéricas
    CONSTANTE_ENTERA = 102,      // 123, 0, 9999
    CONSTANTE_FLOTANTE = 103,    // 3.14, 0.5
    CONSTANTE_CIENTIFICA = 104,  // 1.0E-10, 2.5e+3
    
    // Operadores aritméticos
    OP_SUMA = 105,               // +
    OP_RESTA = 106,              // -
    OP_MULTIPLICACION = 107,     // *
    OP_DIVISION = 108,           // /
    OP_MODULO = 128,             // %
    
    // Operadores de asignación y relacionales
    OP_ASIGNACION = 109,         // =
    OP_IGUALDAD = 110,           // ==
    OP_MENOR = 111,              // <
    OP_MENOR_IGUAL = 112,        // <=
    OP_MAYOR_IGUAL = 113,        // >=
    OP_MAYOR = 114,              // >
    OP_DIFERENTE = 115,          // !=
    
    // Operadores lógicos
    OP_NOT = 116,                // !
    OP_AND = 117,                // &&
    OP_OR = 118,                 // ||
    
    // Delimitadores
    PAREN_ABRE = 119,            // (
    PAREN_CIERRA = 120,          // )
    CORCHETE_ABRE = 121,         // [
    CORCHETE_CIERRA = 122,       // ]
    PUNTO_COMA = 123,            // ;
    COMA = 124,                  // ,
    LLAVE_ABRE = 129,            // {
    LLAVE_CIERRA = 130,          // }
    DOS_PUNTOS = 131,            // :
    
    // Constantes de texto
    CONSTANTE_CARACTER = 125,    // 'a', 'Z'
    CONSTANTE_STRING = 126,      // "texto"
    
    // Comentarios (normalmente no se generan tokens para comentarios)
    COMENTARIO = 127
};

/**
 * @class Token
 * @brief Representa un token reconocido durante el análisis léxico
 */
class Token {
public:
    std::string lexema;    // Texto del token (ej: "int", "variable", "123")
    int gramema;           // Categoría del token (ver enum Gramema)
    int linea;             // Línea donde aparece (inicia en 1)
    int columna;           // Columna donde inicia (inicia en 1)
    
    /**
     * @brief Constructor por defecto
     */
    Token();
    
    /**
     * @brief Constructor con parámetros
     * @param lex Lexema (texto del token)
     * @param gram Gramema (categoría del token)
     * @param lin Número de línea
     * @param col Número de columna
     */
    Token(const std::string& lex, int gram, int lin, int col);
    
    /**
     * @brief Convierte el token a string para visualización
     * @return String con formato "Línea X: lexema [gramema]"
     * 
     * Ejemplo: "Línea 5: int [PALABRA_RESERVADA]"
     */
    std::string toString() const;
    
    /**
     * @brief Obtiene el nombre legible del gramema
     * @return Nombre del gramema (ej: "PALABRA_RESERVADA", "IDENTIFICADOR")
     * 
     * TODO: Implementar mapeo de código de gramema a nombre
     */
    std::string getGramemaName() const;
};

#endif // TOKEN_H
