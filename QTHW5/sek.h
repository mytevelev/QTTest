#ifndef SEK_H
#define SEK_H

#include <QObject>
#include <QTimer>
#include <QVector>

class Sek : public QObject
{
    Q_OBJECT    ///MUST!
public:
    explicit Sek(QObject *parent = nullptr);    ///MUST! constructor

    QVector <QString> GetLaps()
    {

        return lap;
    };
public slots:       // possible interrupts
    void Start();
    void Stop();
    void Clear();
    void FixLap();

private slots:
    void Tick();



signals:
    void Update( double s);
    void NewLap();


private:
    int time = 0, last_lap =0;
    QVector <QString> lap;
    QTimer *t;


};

#endif // SEK_H
