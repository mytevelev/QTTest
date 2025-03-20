#ifndef GREET_H
#define GREET_H

#include <QObject>

class Greet : public QObject
{
    Q_OBJECT
public:
    explicit Greet(QObject *parent = nullptr);

signals:    /// source of interrupt

    void StartTimer();



};

#endif // GREET_H
