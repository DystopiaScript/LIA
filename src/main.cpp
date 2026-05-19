#include <QApplication>
#include "MainWindow.h"

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
