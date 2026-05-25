#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Configuración de la aplicación
    app.setApplicationName("Analizador Léxico LIA");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("Universidad");
    
    // Crear y mostrar ventana principal
    MainWindow ventana; // Crear instancia de la ventana principal
    ventana.show();    // Mostrar la ventana
    
    // Ejecutar el loop de eventos de Qt
    return app.exec(); // Iniciar la aplicación y esperar eventos (clics, etc.)
}
