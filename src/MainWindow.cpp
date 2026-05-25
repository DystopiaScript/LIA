#include "MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFont>
#include <QFileInfo>
/**
 * @file MainWindow.cpp
 * @brief Implementación de la ventana principal
 */

// CONSTRUCTOR Y DESTRUCTOR

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), lexer(nullptr), archivoActual(""), modificado(false) {
    
    // Crear instancia del lexer
    lexer = new Lexer();
    
    // Configurar la interfaz
    setupUI();
    
    // Conectar señales y slots
    setupConnections();
}

MainWindow::~MainWindow() {
    // Liberar memoria del lexer
    if (lexer != nullptr) {
        delete lexer;
        lexer = nullptr;
    }
}

// CONFIGURACIÓN DE LA INTERFAZ

void MainWindow::setupUI() {
    // Widget central
    QWidget* centralWidget = new QWidget(this);  // Widget contenedor para el layout principal
    setCentralWidget(centralWidget); // Establecer el widget central de la ventana principal
    
    // Layout principal horizontal
    QHBoxLayout* layoutPrincipal = new QHBoxLayout(centralWidget);
    
    // ========== PANEL IZQUIERDO ==========
    QVBoxLayout* layoutIzquierdo = new QVBoxLayout();
    
    // Editor de texto
    editorTexto = new QTextEdit();
    editorTexto->setFont(QFont("Courier New", 10));
    editorTexto->setPlaceholderText("Escribe o carga tu código LIA aquí...");
    layoutIzquierdo->addWidget(editorTexto); // Agregar editor al layout izquierdo arriba del todo del lado izquierdo
    
    // Botones
    QHBoxLayout* layoutBotones = new QHBoxLayout(); // Layout horizontal para los botones de control dena
    btnAbrir = new QPushButton("Abrir"); 
    btnGuardar = new QPushButton("Guardar");
    btnLimpiar = new QPushButton("Limpiar");
    btnAnalizar = new QPushButton("Analizar");
    btnSalir = new QPushButton("Salir");
    
    // Estilo para el botón Analizar (destacado)
    btnAnalizar->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; }");
    
    layoutBotones->addWidget(btnAbrir);
    layoutBotones->addWidget(btnGuardar);
    layoutBotones->addWidget(btnLimpiar);
    layoutBotones->addWidget(btnAnalizar);
    layoutBotones->addWidget(btnSalir);
    layoutIzquierdo->addLayout(layoutBotones); // Agregar layout de botones al layout izquierdo abajo del editor
    
    // ========== PANEL DERECHO ==========
    QVBoxLayout* layoutDerecho = new QVBoxLayout();
    
    // Panel de Tokens
    QLabel* labelTokens = new QLabel("Tokens:");
    labelTokens->setStyleSheet("font-weight: bold; font-size: 12pt;");
    layoutDerecho->addWidget(labelTokens);
    
    panelTokens = new QTextEdit();
    panelTokens->setReadOnly(true);
    panelTokens->setFont(QFont("Courier New", 9));
    layoutDerecho->addWidget(panelTokens);
    
    // Panel auxiliar
    QLabel* labelSintaxis = new QLabel("Sintaxis:");
    labelSintaxis->setStyleSheet("font-weight: bold; font-size: 12pt;");
    layoutDerecho->addWidget(labelSintaxis);
    
    panelSintaxis = new QTextEdit();
    panelSintaxis->setReadOnly(true);
    panelSintaxis->setFont(QFont("Courier New", 9));
    panelSintaxis->setPlainText("Proximamente");
    layoutDerecho->addWidget(panelSintaxis);
    
    // Panel de Errores
    QLabel* labelErrores = new QLabel("Errores:");
    labelErrores->setStyleSheet("font-weight: bold; font-size: 12pt;");
    layoutDerecho->addWidget(labelErrores);
    
    panelErrores = new QTextEdit();
    panelErrores->setReadOnly(true);
    panelErrores->setFont(QFont("Courier New", 9));
    layoutDerecho->addWidget(panelErrores);
    
    // ========== AGREGAR PANELES AL LAYOUT PRINCIPAL ==========
    layoutPrincipal->addLayout(layoutIzquierdo, 2);  // 2/3 del ancho
    layoutPrincipal->addLayout(layoutDerecho, 1);    // 1/3 del ancho
    
    // ========== CONFIGURAR VENTANA ==========
    setWindowTitle("Analizador Léxico LIA");
    resize(1200, 700);
}

void MainWindow::setupConnections() {
    // Conectar botones a sus slots
    connect(btnAbrir, &QPushButton::clicked, this, &MainWindow::onAbrirArchivo);
    connect(btnGuardar, &QPushButton::clicked, this, &MainWindow::onGuardarArchivo);
    connect(btnLimpiar, &QPushButton::clicked, this, &MainWindow::onLimpiar);
    connect(btnAnalizar, &QPushButton::clicked, this, &MainWindow::onAnalizar);
    connect(btnSalir, &QPushButton::clicked, this, &MainWindow::onSalir);
    
    // Conectar cambios en el editor
    connect(editorTexto, &QTextEdit::textChanged, this, &MainWindow::onTextoModificado);
}

// ========== SLOTS DE BOTONES ==========

void MainWindow::onAbrirArchivo() {
    // Verificar cambios sin guardar
    if (!confirmarCambios()) {
        return;
    }
    
    // Mostrar diálogo de archivo
    QString filename = QFileDialog::getOpenFileName( // QFileDialog es una clase de Qt para mostrar diálogos de selección de archivos
        this,
        "Abrir archivo LIA", // Título del diálogo
        "", // Directorio inicial (vacío para usar el último)
        "Archivos LIA (*.lia)" // Filtros de archivos 
    );
    
    // Si el usuario canceló, retornar
    if (filename.isEmpty()) {
        return;
    }
    
    // Abrir archivo
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "No se pudo abrir el archivo:\n" + filename);
        return;
    }
    
    // Leer contenido
    QTextStream in(&file); // QTextStream es una clase de Qt para leer/escribir texto de archivos o strings
    QString contenido = in.readAll(); // Lee todo el contenido del archivo como un QString
    file.close();
    
    // Cargar en editor
    editorTexto->setPlainText(contenido);
    
    // Actualizar estado
    archivoActual = filename;
    modificado = false;
    
    // Limpiar paneles de resultados
    limpiarPaneles();
    
    // Actualizar título de ventana
    setWindowTitle("Analizador Léxico LIA - " + QFileInfo(filename).fileName());
}

void MainWindow::onGuardarArchivo() {
    // Si no hay archivo actual, pedir nombre
    if (archivoActual.isEmpty()) {
        archivoActual = QFileDialog::getSaveFileName(
            this,
            "Guardar archivo LIA",
            "",
            "Archivos LIA (*.lia)"
        );
    }
    
    // Si el usuario canceló, retornar
    if (archivoActual.isEmpty()) {
        return;
    }
    
    // Abrir archivo para escritura
    QFile file(archivoActual);  // QFile es una clase de Qt para manejar archivos
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "No se pudo guardar el archivo:\n" + archivoActual);
        return;
    }
    
    // Escribir contenido
    QTextStream out(&file); //out es un QTextStream para escribir texto en el archivo )
    out << editorTexto->toPlainText(); //toPlainText() obtiene el texto sin formato del editor
    file.close();
    
    // Actualizar estado
    modificado = false;
    
    // Actualizar título de ventana
    setWindowTitle("Analizador Léxico LIA - " + QFileInfo(archivoActual).fileName());
    
    // Mostrar confirmación
    QMessageBox::information(this, "Éxito", "Archivo guardado correctamente");
}

void MainWindow::onLimpiar() {
    // Verificar cambios sin guardar
    if (!confirmarCambios()) {
        return;
    }
    
    // Limpiar editor
    editorTexto->clear();
    
    // Limpiar paneles
    limpiarPaneles();
    
    // Reiniciar estado
    archivoActual = "";
    modificado = false;
    
    // Actualizar título de ventana
    setWindowTitle("Analizador Léxico LIA");
}

void MainWindow::onAnalizar() {
    // Obtener texto del editor
    QString texto = editorTexto->toPlainText();
    
    // Verificar que no esté vacío
    if (texto.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Advertencia", "No hay código para analizar");
        return;
    }
    
    // Asegurar newline final para que el lexer cierre el último token
    if (!texto.endsWith('\n')) {
        texto.append('\n');
    }

    // Convertir a std::string
    std::string codigo = texto.toStdString();
    
    // Pasar al lexer
    lexer->setSourceCode(codigo); // setSourceCode es una función que asigna el código fuente al lexer para que pueda analizarlo
    
    // Ejecutar análisis
    lexer->Analiza();
    
    // Obtener resultados
    const std::vector<Token>& tokens = lexer->getTokens();
    const std::vector<Error>& errors = lexer->getErrors();
    
    // Actualizar paneles
    actualizarPanelTokens(tokens);
    actualizarPanelErrores(errors);
}

void MainWindow::onSalir() {
    // Verificar cambios sin guardar
    if (!confirmarCambios()) {
        return;
    }
    
    // Cerrar ventana
    close();
}

void MainWindow::onTextoModificado() {
    // Marcar como modificado
    modificado = true;
    
    // Actualizar título de ventana con asterisco
    QString titulo = "Analizador Léxico LIA";
    if (!archivoActual.isEmpty()) {
        titulo += " - *" + QFileInfo(archivoActual).fileName(); 
    } else {
        titulo = "*" + titulo;
    }
    setWindowTitle(titulo);
}

// FUNCIONES DE ACTUALIZACIÓN DE PANELES 

void MainWindow::actualizarPanelTokens(const std::vector<Token>& tokens) {
    // Limpiar panel
    panelTokens->clear();
    
    // Si no hay tokens, mostrar mensaje
    if (tokens.empty()) {
        panelTokens->setPlainText("No se generaron tokens");
        return;
    }
    
    // Construir texto con todos los tokens
    QString texto;
    for (const Token& token : tokens) { //por cada token en la lista de tokens
        texto += QString("Línea %1, Col %2: %3 [%4]\n")  //+= concatena al texto existente con el nuevo token formateado y 
                .arg(token.linea)                       // asi se va construyendo el texto completo con todos los tokens
                .arg(token.columna)                     //formato: "Línea X, Col Y: lexema [GRAMEMA]"
                .arg(QString::fromStdString(token.lexema))
                .arg(QString::fromStdString(token.getGramemaName()));
    }
    
    // Establecer texto en el panel
    panelTokens->setPlainText(texto);
}

void MainWindow::actualizarPanelErrores(const std::vector<Error>& errors) {
    // Limpiar panel
    panelErrores->clear();
    
    // Si no hay errores, mostrar mensaje de éxito
    if (errors.empty()) {
        panelErrores->setPlainText("Análisis completado sin errores");
        return;
    }
    
    // Construir texto con todos los errores
    QString texto;
    for (const Error& error : errors) {
        texto += QString("Error en línea %1, columna %2:\n")
                .arg(error.linea)
                .arg(error.columna);
        texto += QString("  %1\n")
                .arg(QString::fromStdString(error.descripcion));
        if (!error.caracterProblematico.empty()) {
            texto += QString("  Carácter problemático: '%1'\n\n")
                    .arg(QString::fromStdString(error.caracterProblematico));
        } else {
            texto += "\n";
        }
    }
    
    // Establecer texto en el panel
    panelErrores->setPlainText(texto);
}

void MainWindow::limpiarPaneles() {
    // Limpiar todos los paneles de resultados
    panelTokens->clear();
    panelSintaxis->setPlainText("Proximamente");
    panelErrores->clear();
}

// FUNCIONES AUXILIARES

bool MainWindow::confirmarCambios() {
    // Si no hay modificaciones, continuar sin preguntar
    if (!modificado) {
        return true;
    }
    
    // Mostrar diálogo de confirmación
    QMessageBox::StandardButton respuesta = QMessageBox::question(
        this,
        "Cambios sin guardar",
        "¿Desea guardar los cambios antes de continuar?",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel
    );
    
    // Procesar respuesta
    if (respuesta == QMessageBox::Save) {
        onGuardarArchivo();
        return true;
    } else if (respuesta == QMessageBox::Discard) {
        return true;
    } else {  // Cancel
        return false;
    }
}
