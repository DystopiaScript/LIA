#ifndef ERRORSINTACTICO_H
#define ERRORSINTACTICO_H

#include <string>

/**
 * @file ErrorSintactico.h
 * @brief Definición de la clase ErrorSintactico que representa un error de sintaxis
 *
 * Un error sintáctico contiene:
 * - linea, columna: ubicación del token donde se detectó el error
 * - codigoError: código del tipo de error (600+)
 * - esperado: descripción de lo que el parser esperaba encontrar
 * - encontrado: el lexema/token que realmente llegó
 * - descripcion: mensaje descriptivo del error
 *
 * Rangos de códigos de error del proyecto LIA:
 * - 500-508: errores LÉXICOS (clase Error)
 * - 600-603: errores SINTÁCTICOS (esta clase)
 */

/**
 * @enum CodigoErrorSintactico
 * @brief Códigos de error sintáctico del lenguaje LIA
 */
enum CodigoErrorSintactico {
    ERROR_SIN_TOKEN_INESPERADO = 600,   // Celda -1 en la matriz predictiva: el token no puede iniciar lo que se esperaba
    ERROR_SIN_TERMINAL_NO_COINCIDE = 601, // El terminal del tope de la pila no coincide con el token actual
    ERROR_SIN_FIN_INESPERADO = 602,     // Se acabaron los tokens pero la pila aún esperaba más simbolos
    ERROR_SIN_TOKENS_SOBRANTES = 603    // La pila se vació pero quedaron tokens en la entrada
};

/**
 * @class ErrorSintactico
 * @brief Representa un error sintáctico encontrado durante el análisis predictivo
 */
class ErrorSintactico {
public:
    int linea;                  // Línea del token donde ocurrió el error
    int columna;                // Columna del token donde ocurrió el error
    int codigoError;            // Código del error (ver enum CodigoErrorSintactico)
    std::string esperado;       // Lo que el parser esperaba (ej: "int, float, char, bool, string")
    std::string encontrado;     // Lo que llegó en la entrada (ej: ";")
    std::string descripcion;    // Mensaje descriptivo generado según el código

    /**
     * @brief Constructor por defecto
     */
    ErrorSintactico();

    /**
     * @brief Constructor con parámetros
     * @param lin Número de línea del token problemático
     * @param col Número de columna del token problemático
     * @param codigo Código del error (600-603)
     * @param esp Lo que se esperaba encontrar
     * @param enc Lo que se encontró en la entrada
     *
     * La descripción se genera automáticamente según el código
     */
    ErrorSintactico(int lin, int col, int codigo,
                    const std::string& esp, const std::string& enc);

    /**
     * @brief Convierte el error a string para visualización
     * @return String con formato:
     * "Error sintáctico en línea X, columna Y:
     *     Descripción
     *     Se esperaba: ...
     *     Se encontró: '...'"
     */
    std::string toString() const;

    /**
     * @brief Obtiene la descripción del error según su código
     * @param codigoError Código del error
     * @return Descripción legible del error
     */
    static std::string getDescripcionError(int codigoError);
};

#endif // ERRORSINTACTICO_H
