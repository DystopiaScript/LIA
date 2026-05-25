#ifndef ERROR_H
#define ERROR_H

#include <string>

/**
 * @file Error.h
 * @brief Definición de la clase Error que representa un error léxico
 * 
 * Un error léxico contiene:
 * - linea: número de línea donde ocurrió el error
 * - columna: posición de columna donde ocurrió el error
 * - codigoError: código del estado de error (500+)
 * - caracterProblematico: carácter(es) que causaron el error
 * - descripcion: mensaje descriptivo del error
 */

/**
 * @enum CodigoError
 * @brief Códigos de error léxico del lenguaje LIA
 * 
 * Rangos:
 * - 500-599: Estados de error
 * - Cada código representa un tipo específico de error léxico
 */
enum CodigoError {
    ERROR_FLOTANTE_ESPERA_DIGITOS = 500,                 // Valor flotante que espera digitos después del punto
    ERROR_CIENTIFICA_ESPERA_DIGITO_O_SIGNO = 501,        // Para la notacion cientifica, espera digitos, + o -
    ERROR_CIENTIFICA_DESPUES_SIGNO_ESPERA_DIGITOS = 502, // Para la notacion cientifica, despues de + o -, espera digitos
    ERROR_AND_ESPERA_OTRO_AMPERSAND = 503,               // Error lexico, se espera otro &, para AND
    ERROR_OR_ESPERA_OTRO_PIPE = 504,                     // Error, se espera otro |, para OR
    ERROR_CHAR_DOS_COMILLAS_SIMPLES = 505,               // Dos comillas simples, error al declarar char (vacío)
    ERROR_LEXICO_NO_RECONOCE_LENGUAJE = 506,             // Error lexico, no se reconoce el lenguaje
    ERROR_CHAR_ESPERABA_COMILLA_CIERRE = 507,            // Error en declaracion char, se esperaba comilla de cierre '
    ERROR_STRING_SIN_CERRAR = 508                        // Constante de string sin cerrar: falta comilla doble de cierre "
};

/**
 * @class Error
 * @brief Representa un error léxico encontrado durante el análisis
 */
class Error {
public:
    int linea;                          // Línea donde ocurrió el error
    int columna;                        // Columna donde ocurrió el error
    int codigoError;                    // Código del error (ver enum CodigoError)
    std::string caracterProblematico;   // Carácter(es) que causaron el error
    std::string descripcion;            // Descripción del error
    
    /**
     * @brief Constructor por defecto
     */
    Error();
    
    /**
     * @brief Constructor con parámetros
     * @param lin Número de línea
     * @param col Número de columna
     * @param codigo Código del error
     * @param car Carácter problemático
     * 
     * La descripción se genera automáticamente según el código
     */
    Error(int lin, int col, int codigo, const std::string& car);
    
    /**
     * @brief Obtiene la descripción del error según su código
     * @param codigoError Código del error
     * @return Descripción legible del error
     * 
     * TODO: Implementar mapeo de código a descripción
     * 
     * Ejemplos:
     * - 500: "Carácter no reconocido por el lenguaje"
     * - 501: "Número flotante malformado: esperaba dígitos después del punto"
     * - 505: "Constante de carácter sin cerrar"
     */
    static std::string getDescripcionError(int codigoError);
};

#endif // ERROR_H
