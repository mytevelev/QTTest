#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "sek.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:      // reciever
    void on_pushButton_3_clicked();

    void on_pb_startstop_clicked();

    void onUpdate(double );

    void on_pb_clear_clicked();


    void on_NewLap();

private:
    Ui::Dialog *ui;
    Sek *mt;
};
#endif // DIALOG_H
