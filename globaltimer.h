#ifndef GLOBALTIMER_H
#define GLOBALTIMER_H

#include <QObject>
#include <QTimer>

class GlobalTimer : public QObject
{
    Q_OBJECT

public:
    static GlobalTimer* instance();
    QTimer* getTimer();

private:
    explicit GlobalTimer(QObject *parent = nullptr);
    static GlobalTimer* m_instance;
    QTimer m_timer;
};

#endif // GLOBALTIMER_H
