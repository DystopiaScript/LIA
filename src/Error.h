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
    ERROR_GENERAL = 500,              // Carácter no reconocido por el lenguaje
    ERROR_FLOTANTE = 501,             // Punto decimal sin dígitos después
    ERROR_CIENTIFICA_SIGNO = 502,     // E/e no seguido de +, - o dígito
    ERROR_CIENTIFICA_EXPONENTE = 503, // Signo sin dígitos en exponente
    ERROR_AND = 504,                  // Un solo & (debería ser &&)
    ERROR_CARACTER = 505,             // Comilla simple sin cerrar
    ERROR_WHITESPACE = 506,           // Carácter de control inválido
    ERROR_STRING = 507,               // Comilla doble sin cerrar
    ERROR_IDENTIFICADOR_GUION = 508,  // __ consecutivos o termina en _
    ERROR_OR = 509                    // Un solo | (debería ser ||)
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
     * @brief Convierte el error a string para visualización
     * @return String con formato de error completo
     * 
     * Formato sugerido:
     * "Error en línea X, columna Y:
     *    Descripción del error
     *    Carácter problemático: 'char'"
     */
    std::string toString() const;
    
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
