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

// Obtiene la descripción del error según su código
std::string Error::getDescripcionError(int codigoError) {
    switch (codigoError) {
        case ERROR_FLOTANTE_ESPERA_DIGITOS:
            return "Número flotante malformado: esperaba dígitos después del punto";
        
        case ERROR_CIENTIFICA_ESPERA_DIGITO_O_SIGNO:
            return "Notación científica malformada: esperaba +, - o dígito después de E/e";
        
        case ERROR_CIENTIFICA_DESPUES_SIGNO_ESPERA_DIGITOS:
            return "Notación científica malformada: esperaba dígito en el exponente";
        
        case ERROR_AND_ESPERA_OTRO_AMPERSAND:
            return "Operador AND incompleto: esperaba && en lugar de &";
        
        case ERROR_OR_ESPERA_OTRO_PIPE:
            return "Operador OR incompleto: esperaba || en lugar de |";
        
        case ERROR_CHAR_DOS_COMILLAS_SIMPLES:
            return "Constante de carácter vacía: dos comillas simples consecutivas ''";
        
        case ERROR_LEXICO_NO_RECONOCE_LENGUAJE:
            return "Carácter no reconocido por el lenguaje";
        
        case ERROR_CHAR_ESPERABA_COMILLA_CIERRE:
            return "Constante de carácter sin cerrar: falta comilla simple de cierre '";
        
        case ERROR_STRING_SIN_CERRAR:
            return "Constante de string sin cerrar: falta comilla doble de cierre \"";
        
        default:
            return "Error léxico desconocido (código: " + std::to_string(codigoError) + ")";
    }
}
