#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include "Lexer.h"
class MainWindow : public QMainWindow {
     /**Q_Objet: Avisar al compilador que esta clase tendra eventos y slots de Qt 
     (slot es una función que se ejecuta en respuesta a una señal, como un clic de botón)
     */
    Q_OBJECT
private:
    // WIDGETS PRINCIPALES
    
    /**
     * Editor de código fuente (panel izquierdo superior)
     * Aquí el usuario escribe o carga el código .lia a analizar
     */

    QTextEdit* editorTexto;
    
    /**
     * Panel de tokens reconocidos (panel derecho superior)
     * Muestra la lista de tokens generados por el análisis
     * Formato: "Línea X: e lexema [GRAMEMA]"
     */

    QTextEdit* panelTokens;
    
    /**
     * Panel de información auxiliar (panel derecho medio)
     * Se mantiene como marcador de posición
     */

    QTextEdit* panelSintaxis;
    
    /**
     * Panel de errores léxicos (panel derecho inferior)
     * Muestra los errores encontrados durante el análisis
     * Formato: "Error en línea X, columna Y: descripción"
     */

    QTextEdit* panelErrores;
    
    // ========== BOTONES DE CONTROL ==========
    
    QPushButton* btnAbrir;      // Abrir archivo .lia
    QPushButton* btnGuardar;    // Guardar archivo .lia
    QPushButton* btnLimpiar;    // Limpiar editor y paneles
    QPushButton* btnAnalizar;   // Ejecutar análisis léxico
    QPushButton* btnSalir;      // Cerrar aplicación
    
    // ========== BACKEND ==========
    
    /**
     * Motor del analizador léxico
     * Realiza el análisis del código y genera tokens/errores
     */
    
    Lexer* lexer;
    
    // ESTADO DE LA APLICACIÓN 
                                //Qstring es la clase de Qt para manejar texto (similar a std::string pero con soporte Unicode y otras funcionalidades)
    QString archivoActual;      // Ruta del archivo actualmente abierto
    bool modificado;            // Indica si el texto ha sido modificado
    
public:
    /**
     * @brief Constructor de la ventana principal
     * @param parent Widget padre (nullptr para ventana independiente)
     */
    MainWindow(QWidget* parent = nullptr); 
    
    /**
     * @brief Destructor
     */
    ~MainWindow();
    
private slots: 
    /**
     * @brief Slot: Abrir archivo .lia
     * 
     * TODO: Implementar
     * - Mostrar QFileDialog para seleccionar archivo .lia
     * - Verificar si hay cambios sin guardar (confirmarCambios)
     * - Leer contenido del archivo
     * - Cargar en editorTexto
     * - Actualizar archivoActual
     * - Marcar modificado = false
     */
    void onAbrirArchivo();
    
    /**
     * @brief Slot: Guardar archivo .lia
     * 
     * TODO: Implementar
     * - Si archivoActual está vacío, pedir nombre con QFileDialog
     * - Escribir contenido de editorTexto al archivo
     * - Marcar modificado = false
     * - Mostrar mensaje de confirmación
     */
    void onGuardarArchivo();
    
    /**
     * @brief Slot: Limpiar editor y paneles
     * 
     * TODO: Implementar
     * - Verificar si hay cambios sin guardar (confirmarCambios)
     * - Limpiar editorTexto
     * - Limpiar todos los paneles (limpiarPaneles)
     * - Reiniciar archivoActual
     * - Marcar modificado = false
     */
    void onLimpiar();
    
    /**
     * @brief Slot: Ejecutar análisis léxico
     * 
     * TODO: Implementar
     * - Obtener texto de editorTexto
     * - Pasarlo al lexer: lexer->setSourceCode(texto)
     * - Ejecutar análisis: lexer->Analiza()
     * - Obtener resultados: lexer->getTokens() y lexer->getErrors()
     * - Actualizar paneles con los resultados
     */
    void onAnalizar();
    
    /**
     * @brief Slot: Cerrar aplicación
     * 
     * TODO: Implementar
     * - Verificar si hay cambios sin guardar (confirmarCambios)
     * - Cerrar la ventana: close()
     */
    void onSalir();
    
    /**
     * @brief Slot: Detectar cuando el texto es modificado
     * 
     * TODO: Implementar
     * - Marcar modificado = true
     * - Opcional: actualizar título de ventana con indicador *
     */
    void onTextoModificado();
    
private:
    /**
     * @brief Configura la interfaz de usuario
     * 
     * TODO: Implementar
     * - Crear todos los widgets
     * - Configurar layouts (QHBoxLayout principal, QVBoxLayout para cada panel)
     * - Configurar propiedades de widgets (fuentes, colores, tamaños)
     * - Agregar widgets a layouts
     * - Establecer widget central
     * - Configurar título de ventana y tamaño inicial
     */
    void setupUI();
    
    /**
     * @brief Conecta señales y slots
     * 
     * TODO: Implementar
     * - Conectar botones a sus slots correspondientes
     * - Conectar señal textChanged() del editor a onTextoModificado()
     * 
     * Ejemplo:
     * connect(btnAbrir, &QPushButton::clicked, this, &MainWindow::onAbrirArchivo);
     * connect(btnGuardar, &QPushButton::clicked, this, &MainWindow::onGuardarArchivo);
     * connect(btnLimpiar, &QPushButton::clicked, this, &MainWindow::onLimpiar);
     * connect(btnAnalizar, &QPushButton::clicked, this, &MainWindow::onAnalizar);
     * connect(btnSalir, &QPushButton::clicked, this, &MainWindow::onSalir);
     * connect(editorTexto, &QTextEdit::textChanged, this, &MainWindow::onTextoModificado);
     */
    void setupConnections();
    
    /**
     * @brief Actualiza el panel de tokens con los resultados del análisis
     * @param tokens Vector de tokens reconocidos
     * 
     * TODO: Implementar
     * - Limpiar panelTokens
     * - Iterar sobre tokens
     * - Para cada token, agregar línea con formato:
     *   "Línea X, Col Y: lexema [GRAMEMA]"
     * - Usar token.toString() o construir manualmente
     * 
     * Ejemplo de formato:
     * Línea 1, Col 1: class [PALABRA_RESERVADA]
     * Línea 1, Col 7: MiClase [IDENTIFICADOR]
     * Línea 1, Col 15: { [LLAVE_ABRE]
     */
    void actualizarPanelTokens(const std::vector<Token>& tokens);
    
    /**
     * @brief Actualiza el panel de errores con los errores encontrados
     * @param errors Vector de errores léxicos
     * 
     * TODO: Implementar
     * - Si no hay errores, mostrar mensaje de éxito:
     *   "Análisis completado sin errores"
     * - Si hay errores, listar cada uno con formato:
     *   "Error en línea X, columna Y:
     *      Descripción del error
     *      Carácter problemático: 'char'"
     * - Usar error.toString() o construir manualmente
     */
    void actualizarPanelErrores(const std::vector<Error>& errors);
    
    /**
     * @brief Limpia todos los paneles de resultados
     * 
     * TODO: Implementar
     * - Limpiar panelTokens
    * - Limpiar panelSintaxis
     * - Limpiar panelErrores
     */
    void limpiarPaneles();
    
    /**
     * @brief Confirma si el usuario quiere guardar cambios
     * @return true si puede continuar, false si debe cancelar
     * 
     * TODO: Implementar
     * - Si modificado == false, retornar true
     * - Si modificado == true, mostrar QMessageBox preguntando:
     *   "¿Desea guardar los cambios?"
     *   [Guardar] [No Guardar] [Cancelar]
     * - Si elige Guardar: llamar onGuardarArchivo() y retornar true
     * - Si elige No Guardar: retornar true
     * - Si elige Cancelar: retornar false
     */
    bool confirmarCambios();
};

#endif // MAINWINDOW_H
