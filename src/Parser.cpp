#include "Parser.h"

/**
 * @file Parser.cpp
 * @brief Implementación del Analizador Sintáctico Predictivo de LIA
 *
 * DATOS (ya verificados contra el diseño en Excel):
 *  - MATRIZ_PREDICTIVA  29x45
 *  - PRODUCCIONES       66x9 (longitud + hasta 8 símbolos)
 *  - RESERVADA_A_CODIGO (16 palabras reservadas de la gramática)
 *
 * El driver (función Analiza) implementa fielmente el algoritmo de manejo
 * de la matriz predictiva visto en clase (los 4 casos terminal/no terminal).
 */

// ============================================================================
// MATRIZ PREDICTIVA M[X, a]   (29 no terminales x 45 terminales)
// Valor = número de producción (1-66) | -1 = error sintáctico
// ============================================================================
// Columnas:
//        0   id          9  float       18  loop        27  ;        36  <=
//        1   cteentera   10 char        19  break       28  ,        37  >
//        2   ctereal     11 bool        20  input       29  =        38  >=
//        3   ctenotacion 12 string      21  output      30  !        39  +
//        4   ctecaracter 13 if          22  (           31  ||       40  -
//        5   ctestring   14 else        23  )           32  &&       41  *
//        6   class       15 while       24  {           33  ==       42  /
//        7   def         16 do          25  }           34  !=       43  %
//        8   int         17 dowhile     26  :           35  <        44  $
// ============================================================================
const int Parser::MATRIZ_PREDICTIVA[NUM_NO_TERMINALES][NUM_TERMINALES] = {
//                id  cte ctr ctn ctc cts cls def int flt chr bol str  if els whl  do dwh lop brk inp out   (   )   {   }   :   ;   ,   =   !  ||  &&  ==  !=   <  <=   >  >=   +   -   *   /   %   $
/* 1 PROGRAM  */ { -1, -1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 2 DECLARA  */ {  3, -1, -1, -1, -1, -1, -1,  2, -1, -1, -1, -1, -1,  3, -1,  3,  3, -1,  3,  3,  3,  3, -1, -1, -1,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 3 LID      */ { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  5, -1,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 4 TIPO     */ { -1, -1, -1, -1, -1, -1, -1, -1,  6,  7,  8,  9, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 5 ESTATUTOS*/ { 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 12, 19, 13, 14, 19, 17, 18, 16, 15, -1, -1, -1, 19, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 6 EST_ASIG */ { 20, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 7 EST_IF   */ { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 21, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 8 SINO     */ { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 22, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 23, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 9 EST_WHILE*/ { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 24, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/*10 EST_DO   */ { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 25, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/*11 EST_LOOP */ { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 26, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/*12 EST_BREAK*/ { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 27, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/*13 EST_WRITE*/ { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 28, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/*14 LEXPR    */ { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 30, -1, -1, -1, -1, 29, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/*15 EST_READ */ { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 31, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/*16 LIDR     */ { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 33, -1, -1, -1, -1, 32, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/*17 EXPR     */ { 34, 34, 34, 34, 34, 34, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 34, -1, -1, -1, -1, -1, -1, -1, 34, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/*18 EXPR'    */ { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 36, -1, -1, -1, 36, 36, -1, -1, 35, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/*19 EXPR2    */ { 37, 37, 37, 37, 37, 37, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 37, -1, -1, -1, -1, -1, -1, -1, 37, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/*20 EXPR2'   */ { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 39, -1, -1, -1, 39, 39, -1, -1, 39, 38, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/*21 EXPR3    */ { 41, 41, 41, 41, 41, 41, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 41, -1, -1, -1, -1, -1, -1, -1, 40, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/*22 EXPR4    */ { 42, 42, 42, 42, 42, 42, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 42, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/*23 EXPR4'   */ { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 44, -1, -1, -1, 44, 44, -1, -1, 44, 44, 43, 43, 43, 43, 43, 43, -1, -1, -1, -1, -1, -1},
/*24 OPREL    */ { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 45, 46, 47, 48, 49, 50, -1, -1, -1, -1, -1, -1},
/*25 EXPR5    */ { 51, 51, 51, 51, 51, 51, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 51, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/*26 EXPR5'   */ { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 54, -1, -1, -1, 54, 54, -1, -1, 54, 54, 54, 54, 54, 54, 54, 54, 52, 53, -1, -1, -1, -1},
/*27 TERM     */ { 55, 55, 55, 55, 55, 55, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 55, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
/*28 TERM'    */ { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 59, -1, -1, -1, 59, 59, -1, -1, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 56, 57, 58, -1},
/*29 FACT     */ { 60, 61, 62, 63, 64, 65, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 66, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
};

// ============================================================================
// MATRIZ DE PRODUCCIONES   (66 producciones x [longitud + 8 símbolos])
// Columna 0 = longitud del lado derecho (0 = épsilon)
// Columnas 1..8 = códigos de los símbolos EN ORDEN DE LECTURA
// El driver los inserta a la pila EN ORDEN INVERSO (del último al primero)
// ============================================================================
const int Parser::PRODUCCIONES[NUM_PRODUCCIONES][MAX_LADO_DERECHO + 1] = {
/* 1 PROGRAM -> class ( id ) { DECLARA ESTATUTOS } */ {8, 200,119,101,120,129,  2,  5,130},
/* 2 DECLARA -> def id LID : TIPO ; DECLARA        */ {7, 201,101,  3,131,  4,123,  2,  0},
/* 3 DECLARA -> epsilon                            */ {0,  -1,  0,  0,  0,  0,  0,  0,  0},
/* 4 LID -> , id LID                               */ {3, 124,101,  3,  0,  0,  0,  0,  0},
/* 5 LID -> epsilon                                */ {0,  -1,  0,  0,  0,  0,  0,  0,  0},
/* 6 TIPO -> int                                   */ {1, 202,  0,  0,  0,  0,  0,  0,  0},
/* 7 TIPO -> float                                 */ {1, 203,  0,  0,  0,  0,  0,  0,  0},
/* 8 TIPO -> char                                  */ {1, 204,  0,  0,  0,  0,  0,  0,  0},
/* 9 TIPO -> bool                                  */ {1, 205,  0,  0,  0,  0,  0,  0,  0},
/*10 TIPO -> string                                */ {1, 206,  0,  0,  0,  0,  0,  0,  0},
/*11 ESTATUTOS -> EST_ASIG ESTATUTOS               */ {2,   6,  5,  0,  0,  0,  0,  0,  0},
/*12 ESTATUTOS -> EST_IF ESTATUTOS                 */ {2,   7,  5,  0,  0,  0,  0,  0,  0},
/*13 ESTATUTOS -> EST_WHILE ESTATUTOS              */ {2,   9,  5,  0,  0,  0,  0,  0,  0},
/*14 ESTATUTOS -> EST_DO ESTATUTOS                 */ {2,  10,  5,  0,  0,  0,  0,  0,  0},
/*15 ESTATUTOS -> EST_READ ESTATUTOS               */ {2,  15,  5,  0,  0,  0,  0,  0,  0},
/*16 ESTATUTOS -> EST_WRITE ESTATUTOS              */ {2,  13,  5,  0,  0,  0,  0,  0,  0},
/*17 ESTATUTOS -> EST_LOOP ESTATUTOS               */ {2,  11,  5,  0,  0,  0,  0,  0,  0},
/*18 ESTATUTOS -> EST_BREAK ESTATUTOS              */ {2,  12,  5,  0,  0,  0,  0,  0,  0},
/*19 ESTATUTOS -> epsilon                          */ {0,  -1,  0,  0,  0,  0,  0,  0,  0},
/*20 EST_ASIG -> id = EXPR ;                       */ {4, 101,109, 17,123,  0,  0,  0,  0},
/*21 EST_IF -> if ( EXPR ) { ESTATUTOS SINO }      */ {8, 207,119, 17,120,129,  5,  8,130},
/*22 SINO -> else ESTATUTOS                        */ {2, 208,  5,  0,  0,  0,  0,  0,  0},
/*23 SINO -> epsilon                               */ {0,  -1,  0,  0,  0,  0,  0,  0,  0},
/*24 EST_WHILE -> while ( EXPR ) { ESTATUTOS }     */ {7, 209,119, 17,120,129,  5,130,  0},
/*25 EST_DO -> do ESTATUTOS dowhile ( EXPR ) ;     */ {7, 210,  5,211,119, 17,120,123,  0},
/*26 EST_LOOP -> loop { ESTATUTOS }                */ {4, 212,129,  5,130,  0,  0,  0,  0},
/*27 EST_BREAK -> break ;                          */ {2, 213,123,  0,  0,  0,  0,  0,  0},
/*28 EST_WRITE -> input ( EXPR LEXPR ) ;           */ {6, 214,119, 17, 14,120,123,  0,  0},
/*29 LEXPR -> , EXPR LEXPR                         */ {3, 124, 17, 14,  0,  0,  0,  0,  0},
/*30 LEXPR -> epsilon                              */ {0,  -1,  0,  0,  0,  0,  0,  0,  0},
/*31 EST_READ -> output ( id LIDR ) ;              */ {6, 215,119,101, 16,120,123,  0,  0},
/*32 LIDR -> , id LIDR                             */ {3, 124,101, 16,  0,  0,  0,  0,  0},
/*33 LIDR -> epsilon                               */ {0,  -1,  0,  0,  0,  0,  0,  0,  0},
/*34 EXPR -> EXPR2 EXPR'                           */ {2,  19, 18,  0,  0,  0,  0,  0,  0},
/*35 EXPR' -> || EXPR                              */ {2, 118, 17,  0,  0,  0,  0,  0,  0},
/*36 EXPR' -> epsilon                              */ {0,  -1,  0,  0,  0,  0,  0,  0,  0},
/*37 EXPR2 -> EXPR3 EXPR2'                         */ {2,  21, 20,  0,  0,  0,  0,  0,  0},
/*38 EXPR2' -> && EXPR2                            */ {2, 117, 19,  0,  0,  0,  0,  0,  0},
/*39 EXPR2' -> epsilon                             */ {0,  -1,  0,  0,  0,  0,  0,  0,  0},
/*40 EXPR3 -> ! EXPR4                              */ {2, 116, 22,  0,  0,  0,  0,  0,  0},
/*41 EXPR3 -> EXPR4                                */ {1,  22,  0,  0,  0,  0,  0,  0,  0},
/*42 EXPR4 -> EXPR5 EXPR4'                         */ {2,  25, 23,  0,  0,  0,  0,  0,  0},
/*43 EXPR4' -> OPREL EXPR5                         */ {2,  24, 25,  0,  0,  0,  0,  0,  0},
/*44 EXPR4' -> epsilon                             */ {0,  -1,  0,  0,  0,  0,  0,  0,  0},
/*45 OPREL -> ==                                   */ {1, 110,  0,  0,  0,  0,  0,  0,  0},
/*46 OPREL -> !=                                   */ {1, 115,  0,  0,  0,  0,  0,  0,  0},
/*47 OPREL -> <                                    */ {1, 111,  0,  0,  0,  0,  0,  0,  0},
/*48 OPREL -> <=                                   */ {1, 112,  0,  0,  0,  0,  0,  0,  0},
/*49 OPREL -> >                                    */ {1, 113,  0,  0,  0,  0,  0,  0,  0},
/*50 OPREL -> >=                                   */ {1, 114,  0,  0,  0,  0,  0,  0,  0},
/*51 EXPR5 -> TERM EXPR5'                          */ {2,  27, 26,  0,  0,  0,  0,  0,  0},
/*52 EXPR5' -> + EXPR5                             */ {2, 105, 25,  0,  0,  0,  0,  0,  0},
/*53 EXPR5' -> - EXPR5                             */ {2, 106, 25,  0,  0,  0,  0,  0,  0},
/*54 EXPR5' -> epsilon                             */ {0,  -1,  0,  0,  0,  0,  0,  0,  0},
/*55 TERM -> FACT TERM'                            */ {2,  29, 28,  0,  0,  0,  0,  0,  0},
/*56 TERM' -> * TERM                               */ {2, 107, 27,  0,  0,  0,  0,  0,  0},
/*57 TERM' -> / TERM                               */ {2, 108, 27,  0,  0,  0,  0,  0,  0},
/*58 TERM' -> % TERM                               */ {2, 128, 27,  0,  0,  0,  0,  0,  0},
/*59 TERM' -> epsilon                              */ {0,  -1,  0,  0,  0,  0,  0,  0,  0},
/*60 FACT -> id                                    */ {1, 101,  0,  0,  0,  0,  0,  0,  0},
/*61 FACT -> cteentera                             */ {1, 102,  0,  0,  0,  0,  0,  0,  0},
/*62 FACT -> ctereal                               */ {1, 103,  0,  0,  0,  0,  0,  0,  0},
/*63 FACT -> ctenotacion                           */ {1, 104,  0,  0,  0,  0,  0,  0,  0},
/*64 FACT -> ctecaracter                           */ {1, 125,  0,  0,  0,  0,  0,  0,  0},
/*65 FACT -> ctestring                             */ {1, 126,  0,  0,  0,  0,  0,  0,  0},
/*66 FACT -> ( EXPR )                              */ {3, 119, 17,120,  0,  0,  0,  0,  0}
};

// ============================================================================
// TABLA DE PALABRAS RESERVADAS DEL PARSER (lexema -> código serie 200)
// ============================================================================
const std::map<std::string, int> Parser::RESERVADA_A_CODIGO = {
    {"class",   PR_CLASS},
    {"def",     PR_DEF},
    {"int",     PR_INT},
    {"float",   PR_FLOAT},
    {"char",    PR_CHAR},
    {"bool",    PR_BOOL},
    {"string",  PR_STRING},
    {"if",      PR_IF},
    {"else",    PR_ELSE},
    {"while",   PR_WHILE},
    {"do",      PR_DO},
    {"dowhile", PR_DOWHILE},
    {"loop",    PR_LOOP},
    {"break",   PR_BREAK},
    {"input",   PR_INPUT},
    {"output",  PR_OUTPUT}
    // Nota: "endclass" es reservada del lexer pero NO forma parte de la
    // gramática (el programa cierra con '}'), por eso no aparece aquí.
};

// ========== CONSTRUCTOR Y DESTRUCTOR ==========

Parser::Parser()
    : posActual(0), sintaxisCorrecta(false) {
    // Constructor por defecto
}

Parser::~Parser() {
    // Destructor (no hay recursos dinámicos que liberar)
}

// ========== FUNCIÓN PRINCIPAL DE ANÁLISIS (EL DRIVER) ==========

bool Parser::Analiza() {
    // Reiniciar estado
    reset();

    // 1. INICIALIZAR LA PILA: $ en el fondo, PROGRAM en el tope
    pila.push(FIN_ENTRADA);
    pila.push(NT_PROGRAM);

    // 2. BUCLE PRINCIPAL DEL DRIVER
    // Cada paso de la traza se registra ANTES de modificar la pila, para que
    // cada renglón muestre: pila actual | token actual | acción que se ejecuta
    // (mismo formato que la "Pila de Ejecución" vista en clase)
    while (!pila.empty()) {
        int X = pila.top();             // símbolo del tope de la pila
        int a = codigoTokenActual();    // código del token actual (o $)

        if (X >= 100) {
            // ================= X ES TERMINAL =================
            if (X == a && a == FIN_ENTRADA) {
                // CASO 1: tope == token == $  -> ACEPTAR
                registrarPaso("$ = $. La cadena de entrada es SINTACTICAMENTE CORRECTA");
                sintaxisCorrecta = true;
                pila.pop();
                break;

            } else if (X == a) {
                // CASO 2: MATCH -> sacar de la pila y pedir siguiente token al lexer
                registrarPaso(nombreSimbolo(X) + " = '" + lexemaActual() +
                              "'. Match: sacar de la pila y pedir nuevo token");
                pila.pop();
                posActual++;

            } else {
                // CASO 3: el terminal esperado NO coincide con el token
                if (a == FIN_ENTRADA) {
                    // La entrada se agotó pero la pila esperaba más símbolos
                    registrarPaso("ERROR: fin de archivo, se esperaba '" +
                                  nombreSimbolo(X) + "'");
                    reportarError(ERROR_SIN_FIN_INESPERADO,
                                  nombreSimbolo(X), "$ (fin de archivo)");
                } else {
                    registrarPaso("ERROR: se esperaba '" + nombreSimbolo(X) +
                                  "' y se encontro '" + lexemaActual() + "'");
                    reportarError(ERROR_SIN_TERMINAL_NO_COINCIDE,
                                  nombreSimbolo(X), lexemaActual());
                }
                break;  // detener el análisis en el primer error
            }

        } else {
            // ================= X ES NO TERMINAL =================
            int col = columnaDe(a);
            int n = (col != -1) ? MATRIZ_PREDICTIVA[X - 1][col] : -1;

            if (n != -1) {
                // Aplicar la producción n: sacar X e insertar su lado
                // derecho EN ORDEN INVERSO (el primer símbolo queda al tope)
                int longitud = PRODUCCIONES[n - 1][0];

                if (longitud == 0) {
                    // Producción épsilon: solo se elimina X de la pila
                    registrarPaso("M[" + nombreSimbolo(X) + ", " + nombreSimbolo(a) +
                                  "] = " + std::to_string(n) +
                                  ". Deriva a epsilon: solo se elimina " + nombreSimbolo(X));
                    pila.pop();
                } else {
                    registrarPaso("M[" + nombreSimbolo(X) + ", " + nombreSimbolo(a) +
                                  "] = " + std::to_string(n) + ". Sacar " + nombreSimbolo(X) +
                                  " e insertar su derivacion (invertida)");
                    pila.pop();
                    for (int i = longitud; i >= 1; i--) {
                        pila.push(PRODUCCIONES[n - 1][i]);
                    }
                }

            } else {
                // Celda vacía (-1): el token no puede iniciar la construcción X
                registrarPaso("ERROR: M[" + nombreSimbolo(X) + ", " + nombreSimbolo(a) +
                              "] = -1. Token inesperado '" + lexemaActual() + "'");
                reportarError(ERROR_SIN_TOKEN_INESPERADO,
                              esperadosDe(X), lexemaActual());
                break;  // detener el análisis en el primer error
            }
        }
    }

    // 3. CONDICIÓN ANÓMALA: la pila se vació/aceptó pero sobran tokens
    if (sintaxisCorrecta && posActual < tokens.size()) {
        sintaxisCorrecta = false;
        registrarPaso("ERROR: el programa termino pero hay tokens de mas en la entrada");
        reportarError(ERROR_SIN_TOKENS_SOBRANTES,
                      "$ (fin de archivo)", lexemaActual());
    }

    return sintaxisCorrecta;
}

// ========== FUNCIONES DE CONTROL ==========

void Parser::setTokens(const std::vector<Token>& listaTokens) {
    tokens = listaTokens;
    reset();
}

void Parser::reset() {
    posActual = 0;
    sintaxisCorrecta = false;
    errores.clear();
    traza.clear();
    // Vaciar la pila (std::stack no tiene clear())
    while (!pila.empty()) {
        pila.pop();
    }
}

// ========== FUNCIONES DE ACCESO A RESULTADOS ==========

const std::vector<ErrorSintactico>& Parser::getErrores() const {
    return errores;
}

const std::vector<std::string>& Parser::getTraza() const {
    return traza;
}

bool Parser::esCorrecto() const {
    return sintaxisCorrecta;
}

// ========== FUNCIONES AUXILIARES (PUENTE LEXER-PARSER) ==========

int Parser::codigoDelToken(const Token& t) const {
    // Las palabras reservadas llegan del lexer con gramema 100:
    // se distinguen por su lexema usando la tabla serie 200
    if (t.gramema == 100) {
        std::map<std::string, int>::const_iterator it = RESERVADA_A_CODIGO.find(t.lexema);
        if (it != RESERVADA_A_CODIGO.end()) {
            return it->second;
        }
        // Reservada que no usa la gramática (ej: endclass):
        // se regresa el gramema original y caerá en celda -1 (error sintáctico)
        return t.gramema;
    }
    // Para el resto de tokens el gramema del lexer ES el código del terminal
    return t.gramema;
}

int Parser::columnaDe(int codigoTerminal) const {
    switch (codigoTerminal) {
        // --- identificador y constantes ---
        case 101: return 0;   // id
        case 102: return 1;   // cteentera
        case 103: return 2;   // ctereal
        case 104: return 3;   // ctenotacion
        case 125: return 4;   // ctecaracter
        case 126: return 5;   // ctestring
        // --- palabras reservadas (serie 200) ---
        case PR_CLASS:   return 6;
        case PR_DEF:     return 7;
        case PR_INT:     return 8;
        case PR_FLOAT:   return 9;
        case PR_CHAR:    return 10;
        case PR_BOOL:    return 11;
        case PR_STRING:  return 12;
        case PR_IF:      return 13;
        case PR_ELSE:    return 14;
        case PR_WHILE:   return 15;
        case PR_DO:      return 16;
        case PR_DOWHILE: return 17;
        case PR_LOOP:    return 18;
        case PR_BREAK:   return 19;
        case PR_INPUT:   return 20;
        case PR_OUTPUT:  return 21;
        // --- delimitadores ---
        case 119: return 22;  // (
        case 120: return 23;  // )
        case 129: return 24;  // {
        case 130: return 25;  // }
        case 131: return 26;  // :
        case 123: return 27;  // ;
        case 124: return 28;  // ,
        // --- operadores ---
        case 109: return 29;  // =  (asignación)
        case 116: return 30;  // !
        case 118: return 31;  // ||
        case 117: return 32;  // &&
        case 110: return 33;  // ==
        case 115: return 34;  // !=
        case 111: return 35;  // <
        case 112: return 36;  // <=
        case 113: return 37;  // >
        case 114: return 38;  // >=
        case 105: return 39;  // +
        case 106: return 40;  // -
        case 107: return 41;  // *
        case 108: return 42;  // /
        case 128: return 43;  // %
        // --- fin de entrada ---
        case FIN_ENTRADA: return 44;  // $

        default:  return -1;  // código que no es terminal de la gramática
    }
}

int Parser::codigoTokenActual() const {
    if (posActual < tokens.size()) {
        return codigoDelToken(tokens[posActual]);
    }
    return FIN_ENTRADA;  // se agotaron los tokens: token virtual $
}

std::string Parser::lexemaActual() const {
    if (posActual < tokens.size()) {
        return tokens[posActual].lexema;
    }
    return "$";
}

int Parser::lineaActual() const {
    if (posActual < tokens.size()) {
        return tokens[posActual].linea;
    }
    // Fin de archivo: usar la línea del último token
    if (!tokens.empty()) {
        return tokens.back().linea;
    }
    return 0;
}

int Parser::columnaActual() const {
    if (posActual < tokens.size()) {
        return tokens[posActual].columna;
    }
    if (!tokens.empty()) {
        return tokens.back().columna;
    }
    return 0;
}

std::string Parser::nombreSimbolo(int codigo) const {
    switch (codigo) {
        // --- no terminales (1-29) ---
        case NT_PROGRAM:   return "PROGRAM";
        case NT_DECLARA:   return "DECLARA";
        case NT_LID:       return "LID";
        case NT_TIPO:      return "TIPO";
        case NT_ESTATUTOS: return "ESTATUTOS";
        case NT_EST_ASIG:  return "EST_ASIG";
        case NT_EST_IF:    return "EST_IF";
        case NT_SINO:      return "SINO";
        case NT_EST_WHILE: return "EST_WHILE";
        case NT_EST_DO:    return "EST_DO";
        case NT_EST_LOOP:  return "EST_LOOP";
        case NT_EST_BREAK: return "EST_BREAK";
        case NT_EST_WRITE: return "EST_WRITE";
        case NT_LEXPR:     return "LEXPR";
        case NT_EST_READ:  return "EST_READ";
        case NT_LIDR:      return "LIDR";
        case NT_EXPR:      return "EXPR";
        case NT_EXPR_P:    return "EXPR'";
        case NT_EXPR2:     return "EXPR2";
        case NT_EXPR2_P:   return "EXPR2'";
        case NT_EXPR3:     return "EXPR3";
        case NT_EXPR4:     return "EXPR4";
        case NT_EXPR4_P:   return "EXPR4'";
        case NT_OPREL:     return "OPREL";
        case NT_EXPR5:     return "EXPR5";
        case NT_EXPR5_P:   return "EXPR5'";
        case NT_TERM:      return "TERM";
        case NT_TERM_P:    return "TERM'";
        case NT_FACT:      return "FACT";
        // --- terminales: id y constantes ---
        case 101: return "id";
        case 102: return "cteentera";
        case 103: return "ctereal";
        case 104: return "ctenotacion";
        case 125: return "ctecaracter";
        case 126: return "ctestring";
        // --- terminales: palabras reservadas ---
        case PR_CLASS:   return "class";
        case PR_DEF:     return "def";
        case PR_INT:     return "int";
        case PR_FLOAT:   return "float";
        case PR_CHAR:    return "char";
        case PR_BOOL:    return "bool";
        case PR_STRING:  return "string";
        case PR_IF:      return "if";
        case PR_ELSE:    return "else";
        case PR_WHILE:   return "while";
        case PR_DO:      return "do";
        case PR_DOWHILE: return "dowhile";
        case PR_LOOP:    return "loop";
        case PR_BREAK:   return "break";
        case PR_INPUT:   return "input";
        case PR_OUTPUT:  return "output";
        // --- terminales: delimitadores y operadores ---
        case 119: return "(";
        case 120: return ")";
        case 129: return "{";
        case 130: return "}";
        case 131: return ":";
        case 123: return ";";
        case 124: return ",";
        case 109: return "=";
        case 116: return "!";
        case 118: return "||";
        case 117: return "&&";
        case 110: return "==";
        case 115: return "!=";
        case 111: return "<";
        case 112: return "<=";
        case 113: return ">";
        case 114: return ">=";
        case 105: return "+";
        case 106: return "-";
        case 107: return "*";
        case 108: return "/";
        case 128: return "%";
        case FIN_ENTRADA: return "$";

        default: return "?(" + std::to_string(codigo) + ")";
    }
}

std::string Parser::esperadosDe(int codigoNoTerminal) const {
    // Los terminales esperados son las columnas con celda != -1 en la fila
    // del no terminal: salen DIRECTO de la matriz predictiva (sus FIRST,
    // más los FOLLOW si tiene producción épsilon)
    std::string resultado;
    if (codigoNoTerminal < 1 || codigoNoTerminal > NUM_NO_TERMINALES) {
        return resultado;
    }

    // Tabla inversa columna -> código de terminal (mismo orden que columnaDe)
    static const int CODIGO_POR_COLUMNA[NUM_TERMINALES] = {
        101, 102, 103, 104, 125, 126,                         // 0-5
        PR_CLASS, PR_DEF, PR_INT, PR_FLOAT, PR_CHAR,          // 6-10
        PR_BOOL, PR_STRING, PR_IF, PR_ELSE, PR_WHILE,         // 11-15
        PR_DO, PR_DOWHILE, PR_LOOP, PR_BREAK, PR_INPUT,       // 16-20
        PR_OUTPUT, 119, 120, 129, 130, 131, 123, 124,         // 21-28
        109, 116, 118, 117, 110, 115, 111, 112, 113, 114,     // 29-38
        105, 106, 107, 108, 128, FIN_ENTRADA                  // 39-44
    };

    for (int col = 0; col < NUM_TERMINALES; col++) {
        if (MATRIZ_PREDICTIVA[codigoNoTerminal - 1][col] != -1) {
            if (!resultado.empty()) {
                resultado += ", ";
            }
            resultado += nombreSimbolo(CODIGO_POR_COLUMNA[col]);
        }
    }
    return resultado;
}

void Parser::reportarError(int codigo, const std::string& esperado, const std::string& encontrado) {
    ErrorSintactico error(lineaActual(), columnaActual(), codigo, esperado, encontrado);
    errores.push_back(error);
}

void Parser::registrarPaso(const std::string& accion) {
    // Formato de la traza (estilo "Pila de Ejecución" vista en clase):
    //   PILA: <contenido> | ENTRADA: <lexema actual> | ACCION: <accion>
    std::string paso = "PILA: " + pilaToString() +
                       " | ENTRADA: " + lexemaActual() +
                       " | " + accion;
    traza.push_back(paso);
}

std::string Parser::pilaToString() const {
    // std::stack no permite iterar: se copia y se vacía la copia.
    // El tope queda a la IZQUIERDA del string resultante.
    std::stack<int> copia = pila;
    std::string resultado;
    while (!copia.empty()) {
        if (!resultado.empty()) {
            resultado += " ";
        }
        resultado += nombreSimbolo(copia.top());
        copia.pop();
    }
    return resultado;
}
