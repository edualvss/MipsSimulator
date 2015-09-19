#include <QApplication>
#include "Control/AppControl.h"
#include "View/MainWindow.h"

#include <systemc>

#ifdef DEBUG_METHODS
    #include <iostream>
#endif

// Only because SystemC >= 2.3 need it
int sc_main(int, char **) {
    return 0;
}

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

