#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->radioButton->setText("Button One");
    ui->radioButton->setChecked(true);
    ui->radioButton_2->setText("Button Two");
    ui->groupBox->setTitle("Select:");
    ui->pushButton->setText("Close");
    ui->comboBox->addItem("One");
    ui->comboBox->addItem("Two");
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(10);
    ui->progressBar->setValue(3);
    ui->pushButton_2->setText("Add");
    connect(ui->pushButton_2,&QPushButton::clicked, this , &MainWindow::OnAdd); // add function

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnAdd()
{
    qDebug() << " OnAdd ";
}

void MainWindow::on_pushButton_clicked()
{
    close();
}


void MainWindow::on_pushButton_2_clicked()
{
   int v =  ui->progressBar->value();
  //  qDebug() << v ;
    v++;
   ui->progressBar->setValue(v);
}

