#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <map>
#include "Token.h"
#include "Error.h"

/**
 * El Lexer implementa un Autómata Finito Determinístico (AFD) que reconoce
 * los elementos léxicos del lenguaje LIA mediante una matriz de transición.
 * 
 * Proceso de análisis:
 * 1. Lee el código fuente carácter por carácter
 * 2. Mapea cada carácter a una columna de la matriz (función relaciona)
 * 3. Consulta la matriz para obtener el siguiente estado
 * 4. Al alcanzar un estado de aceptación (100-199), genera un token
 * 5. Al alcanzar un estado de error (500+), reporta el error y continúa
 */

// Constantes para la matriz de transición
const int NUM_ESTADOS = 20;    // Número de estados del AFD (ajustar según necesidad)
const int NUM_COLUMNAS = 34;   // Número de columnas (conjuntos de caracteres)

/**
 * Cada columna representa un conjunto de caracteres o un carácter específico
 * IMPORTANTE: El orden debe coincidir con la matriz de transición en Lexer.cpp
 * 
 * Orden de la matriz: l, L, d, _, ., E, e, +, -, *, /, %, &, =, !, >, <, ', ", $, [, ], (, ), |, {, }, ,, :, ;, dif, \t, \n, \b
 */
enum ColumnaMatriz {
    COL_l = 0,          // Letras minúsculas a-z (excepto e)
    COL_L = 1,          // Letras mayúsculas A-Z (excepto E)
    COL_d = 2,          // Dígitos 0-9
    COL_GUION = 3,      // Guión bajo _
    COL_PUNTO = 4,      // Punto .
    COL_E_MAY = 5,      // E mayúscula
    COL_e_MIN = 6,      // e minúscula
    COL_SUMA = 7,       // +
    COL_RESTA = 8,      // -
    COL_MULT = 9,       // *
    COL_DIV = 10,       // /
    COL_MOD = 11,       // %
    COL_AMP = 12,       // &
    COL_IGUAL = 13,     // =
    COL_EXCL = 14,      // !
    COL_MAYOR = 15,     // >
    COL_MENOR = 16,     // <
    COL_COMILLA_S = 17, // '
    COL_COMILLA_D = 18, // "
    COL_DOLAR = 19,     // $
    COL_CORCH_A = 20,   // [
    COL_CORCH_C = 21,   // ]
    COL_PAREN_A = 22,   // (
    COL_PAREN_C = 23,   // )
    COL_PIPE = 24,      // |
    COL_LLAVE_A = 25,   // {
    COL_LLAVE_C = 26,   // }
    COL_COMA = 27,      // ,
    COL_DPUNTOS = 28,   // :
    COL_PCOMA = 29,     // ;
    COL_OTRO = 30,      // cualquier otro carácter (dif)
    COL_TAB = 31,       // \t
    COL_NEWLINE = 32,   // \n
    COL_ESPACIO = 33    // espacio (\b en la matriz)
};

/**
 * @class Lexer
 * @brief Motor del analizador léxico que implementa el AFD
 */
class Lexer {
private:
    // DATOS DEL CÓDIGO FUENTE 
    std::string sourceCode;           // Código fuente a analizar
    size_t currentPos;                // Posición actual en el código (índice)
    int currentLine;                  // Línea actual (inicia en 1)
    int currentColumn;                // Columna actual (inicia en 1)
    
    // ESTADO DEL AFD 
    int currentState;                 // Estado actual del autómata
    std::string lexemeBuffer;         // Buffer para acumular el lexema actual
    
    // RESULTADOS 
    std::vector<Token> tokens;        // Lista de tokens reconocidos
    std::vector<Error> errors;        // Lista de errores encontrados
    
    // ========== MATRIZ DE TRANSICIÓN DEL AFD ==========
    /**
     * Matriz de transición del AFD
     * - Filas: Estados del autómata (0 a NUM_ESTADOS-1)
     * - Columnas: Conjuntos de caracteres (ver enum ColumnaMatriz)
     * - Valores: Siguiente estado
     * 
     * Rangos de estados:
     * - 0-99: Estados de transición (intermedios)
     * - 100-199: Estados de aceptación (token reconocido)
     * - 500+: Estados de error
     * 
     * TODO: Llenar esta matriz con las transiciones del AFD de LIA
     * Ver docs/matriz_transicion.md para la plantilla completa
     */
    static const int MATRIZ_TRANSICION[NUM_ESTADOS][NUM_COLUMNAS];
    
    // ========== TABLA DE PALABRAS RESERVADAS ==========
    /**
     * Mapa de palabras reservadas del lenguaje LIA
     * - Key: palabra reservada (string)
     * - Value: gramema (siempre 100 para palabras reservadas)
     * 
     * TODO: Llenar con las 17 palabras reservadas de LIA:
     * class, endclass, int, float, char, string, bool,
     * if, else, do, while, input, output, def, to, break, loop
     */
    static const std::map<std::string, int> PALABRAS_RESERVADAS;
    
public:
    /**
     * @brief Constructor por defecto
     */
    Lexer();
    
    /**
     * @brief Constructor con código fuente
     * @param source Código fuente a analizar
     */
    Lexer(const std::string& source);
    
    /**
     * @brief Destructor
     */
    ~Lexer();
    
    // ========== FUNCIÓN PRINCIPAL DE ANÁLISIS ==========
    /**
     * @brief Función principal que ejecuta el análisis léxico
     * 
     * Algoritmo:
     * 1. Inicializar estado en 0
     * 2. Mientras haya caracteres:
     *    a. Leer carácter
     *    b. Mapear a columna (relaciona)
     *    c. Consultar matriz para obtener siguiente estado
     *    d. Si estado de aceptación (100-199): generar token
     *    e. Si estado de error (500+): reportar error y recuperar
     *    f. Si estado de transición (0-99): continuar acumulando
     * 3. Procesar último token si quedó pendiente
     * 
     * TODO: Implementar el algoritmo completo
     */
    void Analiza();
    
    // ========== FUNCIONES AUXILIARES DEL ANÁLISIS ==========
    /**
     * @brief Mapea un carácter a su columna en la matriz de transición
     * @param c Carácter a mapear
     * @return Índice de columna (0-33)
     * 
     * Mapeo:
     * - A-Z (excepto E) → COL_L (0)
     * - E → COL_E_MAY (5)
     * - a-z (excepto e) → COL_l (1)
     * - e → COL_e_MIN (6)
     * - 0-9 → COL_d (2)
     * - Operadores y delimitadores → columnas específicas
     * - Otros → COL_OTRO (33)
     * 
     * TODO: Implementar el mapeo completo
     */
    int relaciona(char c);
    
    /**
     * @brief Clasifica y genera un token reconocido
     * @param lexema Texto del token
     * @param estadoAceptacion Estado de aceptación alcanzado (100-199)
     * 
     * Casos especiales:
     * - Estado 100: puede ser palabra reservada o identificador
     *   → Verificar en PALABRAS_RESERVADAS
     *   → Si no está, verificar que sea identificador válido
     * - Otros estados: mapeo directo a gramema
     * 
     * TODO: Implementar clasificación y validación
     */
    void addToken(const std::string& lexema, int estadoAceptacion);
    
    /**
     * @brief Reporta un error léxico
     * @param estadoError Estado de error alcanzado (500+)
     * @param caracterProblematico Carácter que causó el error
     * 
     * Acciones:
     * 1. Crear objeto Error con información del contexto
     * 2. Agregar a la lista de errores
     * 3. Ejecutar modo pánico (skipToNextDelimiter)
     * 4. Reiniciar estado del AFD a 0
     * 
     * TODO: Implementar reporte y recuperación
     */
    void addError(int estadoError, char caracterProblematico);
    
    // ========== FUNCIONES DE NAVEGACIÓN DEL CÓDIGO ==========
    /**
     * @brief Obtiene el siguiente carácter sin consumirlo
     * @return Siguiente carácter o '\0' si llegó al final
     */
    char peekChar();
    
    /**
     * @brief Obtiene y consume el siguiente carácter
     * @return Carácter leído o '\0' si llegó al final
     * 
     * Actualiza currentPos, currentLine y currentColumn
     */
    char getChar();
    
    // ========== FUNCIONES DE VALIDACIÓN ==========
    /**
     * @brief Verifica si un lexema es palabra reservada
     * @param lexema Texto a verificar
     * @return true si es palabra reservada, false si no
     */
    bool isReservedWord(const std::string& lexema);
    
    // ========== FUNCIONES DE RECUPERACIÓN DE ERRORES ==========
    /**
     * @brief Modo pánico: salta hasta el siguiente delimitador
     * 
     * Usado para recuperarse de errores léxicos
     * Salta caracteres hasta encontrar:
     * - Delimitador: ; , ( ) [ ] { }
     * - Espacio en blanco: espacio, \t, \n
     */
    void skipToNextDelimiter();
    
    // ========== FUNCIONES DE ACCESO A RESULTADOS ==========
    /**
     * @brief Obtiene la lista de tokens reconocidos
     * @return Vector de tokens
     */
    const std::vector<Token>& getTokens() const;
    
    /**
     * @brief Obtiene la lista de errores encontrados
     * @return Vector de errores
     */
    const std::vector<Error>& getErrors() const;
    
    // ========== FUNCIONES DE CONTROL ==========
    /**
     * @brief Establece el código fuente a analizar
     * @param source Código fuente
     */
    void setSourceCode(const std::string& source);
    
    /**
     * @brief Reinicia el lexer para un nuevo análisis
     * 
     * Limpia tokens, errores y reinicia posición
     */
    void reset();
    
};

#endif // LEXER_H
