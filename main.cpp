#include <QApplication>
#include "Control/AppControl.h"
#include "View/MainWindow.h"

#ifdef DEBUG_METHODS
    #include <iostream>
#endif

int main(int argc, char* argv[]) {
#ifdef DEBUG_METHODS
    std::cout << "Main" << std::endl;
#endif

    QApplication app(argc,argv);
    app.setOrganizationName("EAS");
    app.setApplicationName("MipsSimulator");
    MainWindow mw;
    AppControl appCtrl(&mw);
    appCtrl.startApp();
    return app.exec();
}
