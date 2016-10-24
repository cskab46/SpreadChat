#ifndef JOINDIALOG_H
#define JOINDIALOG_H

#include <QDialog>

namespace Ui {
class JoinDialog;
}

class JoinDialog : public QDialog
{
    Q_OBJECT

public:
    explicit JoinDialog(QWidget* parent = 0);
    ~JoinDialog();

    QString groupName() const;

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::JoinDialog* ui;
};

#endif // JOINDIALOG_H
