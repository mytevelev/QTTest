#include <QCoreApplication>
#include <QSqlDatabase>
#include <QtNetwork>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QSqlDatabase dtb;
    QNetworkAccessManager access;
    return a.exec();
}
