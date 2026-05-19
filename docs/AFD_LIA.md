# Autómata Finito Determinístico (AFD) - Lenguaje LIA

## Convenciones de Estados

- **Estados 0-99**: Estados de transición (intermedios)
- **Estados 100-199**: Estados de aceptación (token reconocido)
- **Estados 500+**: Estados de error

## Conjuntos de Caracteres

- **L**: Letras mayúsculas (A-Z)
- **l**: letras minúsculas (a-z)
- **d**: dígitos (0-9)
- **_**: guión bajo
- **Whitespace**: espacio, \t, \n

## Tokens y Estados de Aceptación

| Estado | Token | Descripción |
|--------|-------|-------------|
| 100 | Palabra Reservada | class, if, while, etc. |
| 101 | Identificador | Variables y funciones |
| 102 | Constante Entera | Números enteros |
| 103 | Constante Flotante | Números decimales |
| 104 | Constante Científica | Notación científica |
| 105 | Operador Suma | + |
| 106 | Operador Resta | - |
| 107 | Operador Multiplicación | * |
| 108 | Operador División | / |
| 109 | Operador Asignación | = |
| 110 | Operador Igualdad | == |
| 111 | Operador Menor | < |
| 112 | Operador Menor Igual | <= |
| 113 | Operador Mayor Igual | >= |
| 114 | Operador Mayor | > |
| 115 | Operador Diferente | != |
| 116 | Operador NOT | ! |
| 117 | Operador AND | && |
| 118 | Operador OR | \|\| |
| 119 | Paréntesis Abre | ( |
| 120 | Paréntesis Cierra | ) |
| 121 | Corchete Abre | [ |
| 122 | Corchete Cierra | ] |
| 123 | Punto y Coma | ; |
| 124 | Coma | , |
| 125 | Constante Carácter | 'c' |
| 126 | Constante String | "texto" |
| 127 | Comentario | $...\\n |
| 128 | Operador Módulo | % |
| 129 | Llave Abre | { |
| 130 | Llave Cierra | } |
| 131 | Dos Puntos | : |

## Estados de Error

| Estado | Error | Descripción |
|--------|-------|-------------|
| 500 | Error General | Carácter no reconocido |
| 501 | Error Flotante | Esperaba dígitos después del punto |
| 502 | Error Científica | Esperaba +, - o dígito después de E/e |
| 503 | Error Científica | Esperaba dígito en exponente |
| 504 | Error AND | Esperaba segundo & |
| 505 | Error Carácter | Comilla simple sin cerrar |
| 506 | Error Whitespace | Carácter de control inválido |
| 507 | Error String | Comilla doble sin cerrar |

## Diagrama del AFD (Representación Textual)

### 1. Palabras Reservadas e Identificadores

```
Estado 0 --[L|l]--> Estado 1 (acepta como 100 si es palabra reservada)
                              (acepta como 101 si es identificador)
Estado 1 --[L|l|d]--> Estado 1 (loop)
Estado 1 --[_]--> Estado 2
Estado 2 --[L|l|d]--> Estado 1
Estado 2 --[_]--> Error (dos guiones bajos consecutivos)
```

### 2. Constantes Numéricas

```
Estado 0 --[d]--> Estado 3
Estado 3 --[d]--> Estado 3 (loop) --> acepta 102 (entero)
Estado 3 --[.]--> Estado 4
Estado 4 --[d]--> Estado 5
Estado 5 --[d]--> Estado 5 (loop) --> acepta 103 (flotante)
Estado 5 --[E|e]--> Estado 6
Estado 6 --[+|-]--> Estado 7
Estado 6 --[d]--> Estado 8
Estado 7 --[d]--> Estado 8
Estado 8 --[d]--> Estado 8 (loop) --> acepta 104 (científica)
```

### 3. Operadores Aritméticos

```
Estado 0 --[+]--> acepta 105 (suma)
Estado 0 --[-]--> acepta 106 (resta)
Estado 0 --[*]--> acepta 107 (multiplicación)
Estado 0 --[/]--> acepta 108 (división)
Estado 0 --[%]--> acepta 128 (módulo)
```

### 4. Operadores Relacionales

```
Estado 0 --[=]--> Estado 9
Estado 9 --[=]--> acepta 110 (igualdad)
Estado 9 --[otro]--> acepta 109 (asignación)

Estado 0 --[<]--> Estado 11
Estado 11 --[=]--> acepta 112 (menor igual)
Estado 11 --[otro]--> acepta 111 (menor)

Estado 0 --[>]--> Estado 12
Estado 12 --[=]--> acepta 113 (mayor igual)
Estado 12 --[otro]--> acepta 114 (mayor)

Estado 0 --[!]--> Estado 13
Estado 13 --[=]--> acepta 115 (diferente)
Estado 13 --[otro]--> acepta 116 (NOT)
```

### 5. Operadores Lógicos

```
Estado 0 --[&]--> Estado 14
Estado 14 --[&]--> acepta 117 (AND)
Estado 14 --[otro]--> Error 504

Estado 0 --[|]--> Estado 9
Estado 9 --[|]--> acepta 118 (OR)
Estado 9 --[otro]--> Error (pipe solo no válido)
```

### 6. Delimitadores

```
Estado 0 --[(]--> acepta 119
Estado 0 --[)]--> acepta 120
Estado 0 --[[]--> acepta 121
Estado 0 --[]]--> acepta 122
Estado 0 --[;]--> acepta 123
Estado 0 --[,]--> acepta 124
Estado 0 --[{]--> acepta 129
Estado 0 --[}]--> acepta 130
Estado 0 --[:]--> acepta 131
```

### 7. Constantes Carácter

```
Estado 0 --[']--> Estado 15
Estado 15 --[cualquier_char]--> Estado 16
Estado 16 --[']--> acepta 125
Estado 16 --[otro]--> Error 505
```

### 8. Constantes String

```
Estado 0 --["]--> Estado 17
Estado 17 --[cualquier_char]--> Estado 17 (loop)
Estado 17 --["]--> acepta 126
Estado 17 --[EOF]--> Error 507
```

### 9. Comentarios

```
Estado 0 --[$]--> Estado 19
Estado 19 --[cualquier_char]--> Estado 19 (loop)
Estado 19 --[\n]--> acepta 127
```

### 10. Espacios en Blanco

```
Estado 0 --[\t|\n|espacio]--> Estado 0 (descarta, no genera token)
```

## Notas de Implementación

1. **Retroceso (Backtrack)**: Algunos estados de aceptación requieren retroceder un carácter:
   - Estado 100 (palabra reservada/identificador): si termina con delimitador
   - Estado 102 (entero): si termina con operador o delimitador
   - Estado 109 (asignación): si el siguiente no es '='

2. **Prioridad**: Las palabras reservadas tienen prioridad sobre identificadores

3. **Modo Pánico**: Al encontrar un error, el lexer debe continuar analizando para reportar múltiples errores

4. **Buffer de Lexema**: Mantener un buffer para acumular caracteres del token actual
