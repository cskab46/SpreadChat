#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>
#include <SpreadConnection.h>

namespace Ui {
class ConnectionDialog;
}

class ConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionDialog(QWidget* parent = 0);
    ~ConnectionDialog();

private:
    void setAllEnabled(bool enabled);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void onConnect();

private:
    Ui::ConnectionDialog* ui;
};

#endif // CONNECTIONDIALOG_H
