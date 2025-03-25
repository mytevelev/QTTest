#include "dialog.h"
#include "./ui_dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    mt = new Sek (this);// build new object
    connect(mt,&Sek::Update,this , &Dialog::onUpdate);

    connect(ui->pb_lap, //       object source of the signal
            &QAbstractButton::clicked, // signal method fron class object
            mt ,             // object to recieve signal
            &Sek::FixLap); // slot method from class object

    connect(mt, //       object source of the signal
            &Sek::NewLap, // signal method fron class object
            this ,             // object to recieve signal
            &Dialog::on_NewLap); // slot method from class object

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_3_clicked()
{
    close();
}


void Dialog::on_pb_startstop_clicked()
{
    if( ui->pb_startstop->text() == QString("Start"))
    {
        ui->pb_lap->setEnabled(true);
        ui->pb_startstop->setText("Stop");
        mt->Start();
    }
    else
    {
        ui->pb_startstop->setText("Start");
        mt->Stop();ui->pb_lap->setEnabled(false);
    };
}

void Dialog::onUpdate(double v)
{
    ui->lcdNumber->display( v );

}


void Dialog::on_pb_clear_clicked()
{
    mt->Clear();
    ui->textBrowser->clear();
}




void Dialog::on_NewLap()
{

   QVector <QString> allaps = mt->GetLaps();
    QString lp;

    for(int i=0; i < allaps.size(); i++ )
   {
        lp += QString ("%1. %2\n").arg(i+1 ).arg(allaps[i]);


   };

   ui->textBrowser->setPlainText(lp);
}

