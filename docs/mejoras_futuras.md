# Mejoras Futuras Opcionales - Analizador Léxico LIA

Este documento lista características opcionales que pueden mejorar la experiencia del usuario pero no son requisitos obligatorios para la entrega inicial.

## 🎨 Mejoras de UI/UX

### 1. Numeración de Líneas en el Editor
**Descripción**: Mostrar números de línea en el margen izquierdo del editor de código.

**Beneficios**:
- Facilita identificar errores por número de línea
- Mejora la navegación en archivos grandes
- Aspecto más profesional

**Implementación en Qt**:
- Usar `QPlainTextEdit` con widget personalizado para números de línea
- O usar librerías como QCodeEditor

**Prioridad**: Media (nice to have)

---

### 2. Resaltado de Sintaxis (Syntax Highlighting)
**Descripción**: Colorear palabras reservadas, operadores, strings, comentarios, etc.

**Beneficios**:
- Mejora legibilidad del código
- Ayuda a identificar errores visualmente
- Experiencia más profesional

**Implementación en Qt**:
- Usar `QSyntaxHighlighter` con reglas personalizadas
- Definir colores para cada tipo de token

**Prioridad**: Baja (nice to have, requiere más tiempo)

---

### 3. Temas de Color (Dark Mode / Light Mode)
**Descripción**: Permitir al usuario elegir entre tema claro y oscuro.

**Beneficios**:
- Reduce fatiga visual
- Preferencia personal del usuario

**Implementación en Qt**:
- Usar QStyleSheet para definir paletas de colores
- Botón o menú para cambiar tema

**Prioridad**: Baja (cosmético)

---

### 4. Búsqueda y Reemplazo en el Editor
**Descripción**: Funcionalidad Ctrl+F para buscar texto en el código.

**Beneficios**:
- Útil para archivos grandes
- Funcionalidad estándar en editores

**Implementación en Qt**:
- Diálogo de búsqueda con QLineEdit
- Usar `QTextEdit::find()` para buscar

**Prioridad**: Baja

---

## 📊 Mejoras de Funcionalidad

### 5. Exportar Resultados a Archivo
**Descripción**: Guardar tokens y errores en un archivo de texto o CSV.

**Beneficios**:
- Permite análisis posterior
- Útil para reportes

**Formatos sugeridos**:
- `tokens.txt` - Lista de tokens
- `errores.txt` - Lista de errores
- `reporte.csv` - Formato tabular

**Prioridad**: Media

---

### 6. Historial de Archivos Recientes
**Descripción**: Menú "Archivos Recientes" para abrir archivos usados recientemente.

**Beneficios**:
- Ahorra tiempo al usuario
- Funcionalidad estándar en aplicaciones

**Implementación en Qt**:
- Guardar lista en QSettings
- Menú dinámico con últimos 5-10 archivos

**Prioridad**: Baja

---

### 7. Estadísticas Detalladas en Panel Sintaxis
**Descripción**: Mostrar métricas del código analizado.

**Posibles métricas**:
- Total de tokens reconocidos
- Total de líneas procesadas
- Total de palabras reservadas encontradas
- Total de identificadores únicos
- Total de constantes numéricas
- Tiempo de análisis (ms)
- Distribución de tipos de tokens (gráfico)

**Prioridad**: Media (pendiente definir con el profesor)

---

### 8. Modo de Análisis Continuo
**Descripción**: Analizar automáticamente mientras el usuario escribe (con delay).

**Beneficios**:
- Feedback inmediato
- Detecta errores en tiempo real

**Implementación en Qt**:
- Conectar señal `textChanged()` con timer
- Analizar después de 500ms de inactividad

**Prioridad**: Baja (puede ser molesto)

---

## 🐛 Mejoras de Depuración

### 9. Modo Debug del Autómata
**Descripción**: Visualizar paso a paso las transiciones del AFD.

**Beneficios**:
- Útil para entender el funcionamiento
- Ayuda a depurar la matriz de transición

**Información a mostrar**:
- Estado actual
- Carácter leído
- Siguiente estado
- Lexema acumulado

**Prioridad**: Baja (más para desarrollo que para usuario final)

---

### 10. Resaltar Errores en el Código
**Descripción**: Marcar con color rojo o subrayado los caracteres que causaron errores.

**Beneficios**:
- Identificación visual inmediata de errores
- Mejora UX

**Implementación en Qt**:
- Usar `QTextEdit::setExtraSelections()` para resaltar
- Sincronizar con lista de errores

**Prioridad**: Media

---

## 🎯 Mejoras de Rendimiento

### 11. Análisis en Hilo Separado
**Descripción**: Ejecutar análisis léxico en un thread separado para no bloquear la UI.

**Beneficios**:
- UI responsiva durante análisis de archivos grandes
- Mejor experiencia de usuario

**Implementación en Qt**:
- Usar `QThread` o `QtConcurrent::run()`
- Emitir señales para actualizar UI

**Prioridad**: Baja (solo necesario para archivos muy grandes)

---

### 12. Caché de Resultados
**Descripción**: No re-analizar si el código no ha cambiado.

**Beneficios**:
- Ahorra procesamiento
- Respuesta instantánea

**Implementación**:
- Guardar hash del código analizado
- Comparar antes de re-analizar

**Prioridad**: Muy baja

---

## 📝 Notas de Implementación

### Priorización Sugerida (Post-Entrega)

**Fase 1 - Mejoras Básicas**:
1. Numeración de líneas
2. Estadísticas detalladas en panel Sintaxis
3. Exportar resultados a archivo

**Fase 2 - Mejoras Visuales**:
4. Resaltar errores en el código
5. Resaltado de sintaxis básico
6. Temas de color

**Fase 3 - Mejoras Avanzadas**:
7. Historial de archivos recientes
8. Búsqueda y reemplazo
9. Análisis en hilo separado

### Consideraciones

- **No implementar antes de la entrega inicial**: Enfocarse en requisitos obligatorios
- **Consultar con el profesor**: Algunas mejoras pueden ser requisitos ocultos
- **Documentar decisiones**: Si se implementa algo, actualizar requirements.md
- **Testing**: Cada mejora debe ser probada antes de integrar

---

## 🔄 Proceso de Actualización

Cuando el profesor defina requisitos adicionales el lunes:

1. Revisar este documento
2. Mover requisitos confirmados a `requirements.md`
3. Actualizar `design.md` si es necesario
4. Agregar tareas correspondientes a `tasks.md`
5. Actualizar este documento con nuevas ideas

---

**Última actualización**: Fecha de creación del proyecto
**Estado**: Pendiente de revisión con el profesor
