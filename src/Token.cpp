#include "Token.h"

/**
 * @file Token.cpp
 * @brief Implementación de la clase Token
 */

// Constructor por defecto
Token::Token() 
    : lexema(""), gramema(0), linea(0), columna(0) {
    // Inicializa un token vacío
}

// Constructor con parámetros
Token::Token(const std::string& lex, int gram, int lin, int col)
    : lexema(lex), gramema(gram), linea(lin), columna(col) {
    // Inicializa el token con los valores proporcionados
}

// Obtiene el nombre legible del gramema
std::string Token::getGramemaName() const {
    switch (gramema) {
        case PALABRA_RESERVADA: return "PALABRA_RESERVADA";
        case IDENTIFICADOR: return "IDENTIFICADOR";
        case CONSTANTE_ENTERA: return "CONSTANTE_ENTERA";
        case CONSTANTE_FLOTANTE: return "CONSTANTE_FLOTANTE";
        case CONSTANTE_CIENTIFICA: return "CONSTANTE_CIENTIFICA";
        case OP_SUMA: return "OP_SUMA";
        case OP_RESTA: return "OP_RESTA";
        case OP_MULTIPLICACION: return "OP_MULTIPLICACION";
        case OP_DIVISION: return "OP_DIVISION";
        case OP_ASIGNACION: return "OP_ASIGNACION";
        case OP_IGUALDAD: return "OP_IGUALDAD";
        case OP_MENOR: return "OP_MENOR";
        case OP_MENOR_IGUAL: return "OP_MENOR_IGUAL";
        case OP_MAYOR_IGUAL: return "OP_MAYOR_IGUAL";
        case OP_MAYOR: return "OP_MAYOR";
        case OP_DIFERENTE: return "OP_DIFERENTE";
        case OP_NOT: return "OP_NOT";
        case OP_AND: return "OP_AND";
        case OP_OR: return "OP_OR";
        case PAREN_ABRE: return "PAREN_ABRE";
        case PAREN_CIERRA: return "PAREN_CIERRA";
        case CORCHETE_ABRE: return "CORCHETE_ABRE";
        case CORCHETE_CIERRA: return "CORCHETE_CIERRA";
        case PUNTO_COMA: return "PUNTO_COMA";
        case COMA: return "COMA";
        case CONSTANTE_CARACTER: return "CONSTANTE_CARACTER";
        case CONSTANTE_STRING: return "CONSTANTE_STRING";
        case COMENTARIO: return "COMENTARIO";
        case OP_MODULO: return "OP_MODULO";
        case LLAVE_ABRE: return "LLAVE_ABRE";
        case LLAVE_CIERRA: return "LLAVE_CIERRA";
        case DOS_PUNTOS: return "DOS_PUNTOS";
        default: return "DESCONOCIDO";
    }
}
