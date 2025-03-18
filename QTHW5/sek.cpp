#include "sek.h"
#include <QtDebug>

Sek::Sek(QObject *parent)
    : QObject{parent}
{

    t = new QTimer(this);
    connect(t, &QTimer::timeout,    // само прерывание по таймеру
            this,&Sek::Tick);           // обработчик проерывания

}

void Sek::Start()
{
    t->start(100);
}

void Sek::Stop()
{
    t->stop();
}

void Sek::Clear()
{

    time = 0;

    emit Update(0.0);   // call interrupt for update timer
}

void Sek::Tick()
{
    time++;
    double sec = static_cast<double>( time );
    sec /= 10.0;
    emit Update(sec);   // call interrupt for update timer
}

void Sek::FixLap()
{

    double sec = static_cast<double>( time );
    sec /= 10.0;
    QString l;
    l = QString::number( sec, 'f', 1 );
    qDebug() << l;
    lap.push_back(l);

    emit NewLap();

}

