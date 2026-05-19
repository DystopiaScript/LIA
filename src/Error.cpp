#include "Error.h"

/**
 * @file Error.cpp
 * @brief Implementación de la clase Error
 */

// Constructor por defecto
Error::Error()
    : linea(0), columna(0), codigoError(0), caracterProblematico(""), descripcion("") {
    // Inicializa un error vacío
}

// Constructor con parámetros
Error::Error(int lin, int col, int codigo, const std::string& car)
    : linea(lin), columna(col), codigoError(codigo), caracterProblematico(car) {
    // Genera la descripción automáticamente según el código
    descripcion = getDescripcionError(codigo);
}

// Convierte el error a string para visualización
std::string Error::toString() const {
    std::string resultado = "Error en línea " + std::to_string(linea) + 
                        ", columna " + std::to_string(columna) + ":\n";
    resultado += "   " + descripcion + "\n";
    if (!caracterProblematico.empty()) {
        resultado += "   Carácter problemático: '" + caracterProblematico + "'";
    }
    return resultado;
}

// Obtiene la descripción del error según su código
std::string Error::getDescripcionError(int codigoError) {
    switch (codigoError) {
        case ERROR_GENERAL:
            return "Carácter no reconocido por el lenguaje";
        case ERROR_FLOTANTE:
            return "Número flotante malformado: esperaba dígitos después del punto";
        case ERROR_CIENTIFICA_SIGNO:
            return "Notación científica malformada: esperaba +, - o dígito después de E/e";
        case ERROR_CIENTIFICA_EXPONENTE:
            return "Notación científica malformada: esperaba dígito en el exponente";
        case ERROR_AND:
            return "Operador AND incompleto: esperaba && en lugar de &";
        case ERROR_CARACTER:
            return "Constante de carácter sin cerrar: falta comilla simple de cierre";
        case ERROR_WHITESPACE:
            return "Carácter de control inválido";
        case ERROR_STRING:
            return "Constante string sin cerrar: falta comilla doble de cierre";
        case ERROR_IDENTIFICADOR_GUION:
            return "Identificador inválido: no puede tener __ consecutivos ni terminar en _";
        case ERROR_OR:
            return "Operador OR incompleto: esperaba || en lugar de |";
        default:
            return "Error léxico desconocido";
    }
}
