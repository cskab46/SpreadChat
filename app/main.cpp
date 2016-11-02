#include <QApplication>
#include "ConnectionDialog.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
#ifdef Q_OS_WIN
    QPalette palette;
    palette.setColor(QPalette::Window, Qt::white);
    app.setPalette(palette);
#endif
    ConnectionDialog dialog;
    dialog.show();
    return app.exec();
}
