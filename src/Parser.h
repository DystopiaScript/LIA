#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <stack>
#include <map>
#include "Token.h"
#include "ErrorSintactico.h"

/**
 * @file Parser.h
 * @brief Analizador Sintáctico Predictivo No Recursivo (LL(1)) del lenguaje LIA
 *
 * El Parser implementa el MÉTODO PREDICTIVO usando:
 *  1. Una PILA de enteros (códigos de símbolos)
 *  2. La MATRIZ PREDICTIVA M[X,a]: decide QUÉ producción aplicar
 *  3. La MATRIZ DE PRODUCCIONES: dice QUÉ CONTIENE cada producción
 *
 * Proceso de análisis (driver):
 *  1. La pila inicia con: $ PROGRAM   (PROGRAM en el tope)
 *  2. En cada paso se mira el tope X y el token actual a:
 *     - X terminal y X == a == $   -> ACEPTAR (sintaxis correcta)
 *     - X terminal y X == a        -> match: sacar X, pedir siguiente token
 *     - X terminal y X != a        -> ERROR 601 (se esperaba X)
 *     - X no terminal              -> consultar M[X,a]:
 *         * produccion n  -> sacar X, insertar lado derecho de n EN ORDEN INVERSO
 *         * n es epsilon  -> solo sacar X
 *         * -1            -> ERROR 600 (token inesperado)
 *
 * CONVENCIÓN DE CÓDIGOS (la misma de la Matriz de Producciones en Excel):
 *  - 1..29   : NO TERMINALES (índice de fila en la matriz predictiva = código - 1)
 *  - 101..131: TERMINALES = gramemas del lexer (id=101, +=105, ;=123, etc.)
 *  - 200..215: PALABRAS RESERVADAS individuales (el lexer las entrega todas con
 *              gramema 100; el parser las distingue por su LEXEMA)
 *  - 999     : $ (fin de entrada)
 *  - -1      : épsilon / celda vacía (error)
 *
 *  Regla del driver:  codigo < 100  => no terminal
 *                     codigo >= 100 => terminal
 */

// ========== CONSTANTES DE DIMENSIONES ==========
const int NUM_NO_TERMINALES = 29;   // Filas de la matriz predictiva
const int NUM_TERMINALES    = 45;   // Columnas de la matriz predictiva (44 terminales + $)
const int NUM_PRODUCCIONES  = 66;   // Producciones de la gramática LIA
const int MAX_LADO_DERECHO  = 8;    // La producción más larga tiene 8 símbolos (1 y 21)

// ========== CÓDIGOS DE NO TERMINALES (1-29) ==========
/**
 * El código de cada no terminal es su número de fila en la matriz predictiva.
 * Para indexar la matriz: MATRIZ_PREDICTIVA[codigo - 1][columna]
 */
enum NoTerminal {
    NT_PROGRAM   = 1,
    NT_DECLARA   = 2,
    NT_LID       = 3,
    NT_TIPO      = 4,
    NT_ESTATUTOS = 5,
    NT_EST_ASIG  = 6,
    NT_EST_IF    = 7,
    NT_SINO      = 8,
    NT_EST_WHILE = 9,
    NT_EST_DO    = 10,
    NT_EST_LOOP  = 11,
    NT_EST_BREAK = 12,
    NT_EST_WRITE = 13,
    NT_LEXPR     = 14,
    NT_EST_READ  = 15,
    NT_LIDR      = 16,
    NT_EXPR      = 17,
    NT_EXPR_P    = 18,   // EXPR'  (P = prima)
    NT_EXPR2     = 19,
    NT_EXPR2_P   = 20,   // EXPR2'
    NT_EXPR3     = 21,
    NT_EXPR4     = 22,
    NT_EXPR4_P   = 23,   // EXPR4'
    NT_OPREL     = 24,
    NT_EXPR5     = 25,
    NT_EXPR5_P   = 26,   // EXPR5'
    NT_TERM      = 27,
    NT_TERM_P    = 28,   // TERM'
    NT_FACT      = 29
};

// ========== CÓDIGOS DE PALABRAS RESERVADAS (serie 200) ==========
/**
 * El lexer entrega TODAS las palabras reservadas con gramema 100.
 * El parser necesita distinguirlas (if != while != int ...), por lo que
 * les asigna estos códigos individuales mapeando por LEXEMA
 * (ver Parser::codigoDelToken y el mapa RESERVADA_A_CODIGO).
 */
enum PalabraReservada {
    PR_CLASS   = 200,
    PR_DEF     = 201,
    PR_INT     = 202,
    PR_FLOAT   = 203,
    PR_CHAR    = 204,
    PR_BOOL    = 205,
    PR_STRING  = 206,
    PR_IF      = 207,
    PR_ELSE    = 208,
    PR_WHILE   = 209,
    PR_DO      = 210,
    PR_DOWHILE = 211,
    PR_LOOP    = 212,
    PR_BREAK   = 213,
    PR_INPUT   = 214,
    PR_OUTPUT  = 215
};

// Código del fin de entrada (token virtual que se procesa al agotar los tokens)
const int FIN_ENTRADA = 999;

/**
 * @class Parser
 * @brief Motor del analizador sintáctico predictivo no recursivo
 *
 * Uso típico (desde MainWindow::onAnalizar, después del léxico):
 *
 *   Parser parser;
 *   parser.setTokens(lexer->getTokens());
 *   bool ok = parser.Analiza();
 *   // mostrar parser.getErrores() y parser.getTraza() en los paneles
 */
class Parser {
private:
    // ========== ENTRADA ==========
    std::vector<Token> tokens;        // Tokens generados por el Lexer
    size_t posActual;                 // Índice del token actual en 'tokens'

    // ========== ESTADO DEL DRIVER ==========
    std::stack<int> pila;             // Pila de códigos de símbolos

    // ========== RESULTADOS ==========
    std::vector<ErrorSintactico> errores;  // Errores sintácticos encontrados
    std::vector<std::string> traza;        // Traza de ejecución paso a paso
                                           // (estilo "Pila de Ejecución" vista en clase)
    bool sintaxisCorrecta;                 // Resultado final del análisis

    // ========== MATRIZ PREDICTIVA M[X, a] ==========
    /**
     * 29 filas (no terminales) x 45 columnas (terminales + $).
     * Valor = número de producción a aplicar (1-66), o -1 = error.
     *
     * Indexado: MATRIZ_PREDICTIVA[codigoNoTerminal - 1][columnaDe(codigoTerminal)]
     *
     * Orden de columnas (0-44):
     *  id cteentera ctereal ctenotacion ctecaracter ctestring class def int
     *  float char bool string if else while do dowhile loop break input output
     *  ( ) { } : ; , = ! || && == != < <= > >= + - * / % $
     */
    static const int MATRIZ_PREDICTIVA[NUM_NO_TERMINALES][NUM_TERMINALES];

    // ========== MATRIZ DE PRODUCCIONES ==========
    /**
     * 66 filas (una por producción) x 9 columnas:
     *  - Columna 0: LONGITUD del lado derecho (0 = producción épsilon)
     *  - Columnas 1..8: los códigos de los símbolos del lado derecho, en orden
     *
     * Indexado: PRODUCCIONES[numeroProduccion - 1]
     *
     * El driver los inserta a la pila EN ORDEN INVERSO (del último al primero)
     * para que el primer símbolo quede en el tope.
     */
    static const int PRODUCCIONES[NUM_PRODUCCIONES][MAX_LADO_DERECHO + 1];

    // ========== TABLA DE PALABRAS RESERVADAS DEL PARSER ==========
    /**
     * Mapa lexema -> código serie 200.
     * Se usa cuando el token tiene gramema 100 (palabra reservada genérica).
     * Nota: "endclass" es reservada del lexer pero NO aparece en la gramática.
     */
    static const std::map<std::string, int> RESERVADA_A_CODIGO;

public:
    /**
     * @brief Constructor por defecto
     */
    Parser();

    /**
     * @brief Destructor
     */
    ~Parser();

    // ========== FUNCIÓN PRINCIPAL DE ANÁLISIS ==========
    /**
     * @brief Ejecuta el análisis sintáctico predictivo completo (el DRIVER)
     * @return true si la sintaxis es correcta, false si hubo errores
     *
     * Algoritmo (ver documentación del archivo):
     *  1. reset() e inicializar pila con FIN_ENTRADA y NT_PROGRAM
     *  2. Repetir hasta aceptar o agotar:
     *     comparar tope de pila vs token actual según los 4 casos del driver
     *  3. Registrar cada paso en 'traza' y los errores en 'errores'
     */
    bool Analiza();

    // ========== FUNCIONES DE CONTROL ==========
    /**
     * @brief Establece la lista de tokens a analizar (salida del Lexer)
     * @param listaTokens Vector de tokens del análisis léxico
     */
    void setTokens(const std::vector<Token>& listaTokens);

    /**
     * @brief Reinicia el parser para un nuevo análisis
     * (vacía pila, errores, traza y regresa al primer token)
     */
    void reset();

    // ========== FUNCIONES DE ACCESO A RESULTADOS ==========
    /**
     * @brief Obtiene la lista de errores sintácticos encontrados
     */
    const std::vector<ErrorSintactico>& getErrores() const;

    /**
     * @brief Obtiene la traza de ejecución (pila | entrada | acción por paso)
     */
    const std::vector<std::string>& getTraza() const;

    /**
     * @brief Indica si el último análisis fue sintácticamente correcto
     */
    bool esCorrecto() const;

private:
    // ========== FUNCIONES AUXILIARES (el "puente" lexer-parser) ==========
    /**
     * @brief Traduce un Token del lexer al código de terminal del parser
     * @param t Token a traducir
     * @return Código del terminal:
     *  - gramema 100 (reservada)  -> buscar lexema en RESERVADA_A_CODIGO (200-215)
     *  - gramema 101 (identificador) y demás -> el gramema tal cual
     */
    int codigoDelToken(const Token& t) const;

    /**
     * @brief Traduce un código de terminal a su columna en la matriz predictiva
     * @param codigoTerminal Código del terminal (101-131, 200-215, 999)
     * @return Índice de columna 0-44, o -1 si el código no es un terminal válido
     */
    int columnaDe(int codigoTerminal) const;

    /**
     * @brief Obtiene el código del token actual (o FIN_ENTRADA si se agotaron)
     */
    int codigoTokenActual() const;

    /**
     * @brief Obtiene el lexema del token actual (o "$" si se agotaron)
     */
    std::string lexemaActual() const;

    /**
     * @brief Línea del token actual (o la del último token si se agotaron)
     */
    int lineaActual() const;

    /**
     * @brief Columna del token actual (o la del último token si se agotaron)
     */
    int columnaActual() const;

    /**
     * @brief Nombre legible de un símbolo por su código (para traza y errores)
     * Ej: 1 -> "PROGRAM", 101 -> "id", 207 -> "if", 999 -> "$"
     */
    std::string nombreSimbolo(int codigo) const;

    /**
     * @brief Construye la lista de terminales esperados para un no terminal
     * @param codigoNoTerminal Código del no terminal (1-29)
     * @return String tipo "int, float, char, bool, string"
     *
     * Recorre la fila del no terminal en la matriz predictiva y junta los
     * nombres de las columnas cuya celda != -1. Así los mensajes de error
     * salen DIRECTO de la matriz (los FIRST de la fila).
     */
    std::string esperadosDe(int codigoNoTerminal) const;

    /**
     * @brief Registra un error sintáctico en la lista
     */
    void reportarError(int codigo, const std::string& esperado, const std::string& encontrado);

    /**
     * @brief Registra un paso del driver en la traza
     * @param accion Descripción de la acción realizada
     */
    void registrarPaso(const std::string& accion);

    /**
     * @brief Convierte el contenido actual de la pila a string (para la traza)
     * Formato: tope a la izquierda, ej: "EXPR ; DECLARA ESTATUTOS } $"
     */
    std::string pilaToString() const;
};

#endif // PARSER_H
