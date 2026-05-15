#include <QApplication>
#include "MainWindow.h"

/**
 * @file main.cpp
 * @brief Punto de entrada de la aplicación del analizador léxico LIA
 * 
 * Este archivo contiene la función main que inicializa la aplicación Qt
 * y muestra la ventana principal.
 */

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Configuración de la aplicación
    app.setApplicationName("Analizador Léxico LIA");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("Universidad");
    
    // Crear y mostrar ventana principal
    MainWindow ventana;
    ventana.show();
    
    // Ejecutar el loop de eventos de Qt
    return app.exec();
}
