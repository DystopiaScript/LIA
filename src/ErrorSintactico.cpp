#include "ErrorSintactico.h"

/**
 * @file ErrorSintactico.cpp
 * @brief Implementación de la clase ErrorSintactico
 */

// Constructor por defecto
ErrorSintactico::ErrorSintactico()
    : linea(0), columna(0), codigoError(0), esperado(""), encontrado(""), descripcion("") {
    // Inicializa un error vacío
}

// Constructor con parámetros
ErrorSintactico::ErrorSintactico(int lin, int col, int codigo,
                                 const std::string& esp, const std::string& enc)
    : linea(lin), columna(col), codigoError(codigo), esperado(esp), encontrado(enc) {
    // Genera la descripción automáticamente según el código
    descripcion = getDescripcionError(codigo);
}

// Convierte el error a string para visualización
std::string ErrorSintactico::toString() const {
    std::string resultado = "Error sintáctico en línea " + std::to_string(linea) +
                            ", columna " + std::to_string(columna) + ":\n";
    resultado += "   " + descripcion + "\n";
    if (!esperado.empty()) {
        resultado += "   Se esperaba: " + esperado + "\n";
    }
    if (!encontrado.empty()) {
        resultado += "   Se encontró: '" + encontrado + "'";
    }
    return resultado;
}

// Obtiene la descripción del error según su código
std::string ErrorSintactico::getDescripcionError(int codigoError) {
    switch (codigoError) {
        case ERROR_SIN_TOKEN_INESPERADO:
            return "Token inesperado: no puede iniciar la construcción esperada";

        case ERROR_SIN_TERMINAL_NO_COINCIDE:
            return "El símbolo esperado no coincide con el token de la entrada";

        case ERROR_SIN_FIN_INESPERADO:
            return "Fin de archivo inesperado: el programa está incompleto";

        case ERROR_SIN_TOKENS_SOBRANTES:
            return "El programa terminó pero hay tokens de más en la entrada";

        default:
            return "Error sintáctico desconocido (código: " + std::to_string(codigoError) + ")";
    }
}
