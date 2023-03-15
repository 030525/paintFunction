#ifndef TIMER_H
#define TIMER_H
#include <QDebug>

#include <QElapsedTimer>

class Timer {
public:
    void start() {
        m_elapsedTimer.start();
    }

    void end() {
        //获取ns
        m_elapsed += m_elapsedTimer.nsecsElapsed();
        qDebug() << "it takes " << m_elapsed / 1000000000 << " s , "  << m_elapsed / 1000000 << " ms , " << m_elapsed / 1000 << " us ";
    }


private:
    QElapsedTimer m_elapsedTimer;
    qint64 m_elapsed = 0;
};

#endif // TIMER_H
