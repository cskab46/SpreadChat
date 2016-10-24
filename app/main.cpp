#include <QApplication>
#include "ConnectionDialog.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    ConnectionDialog dialog;
    dialog.show();
    return app.exec();
}
