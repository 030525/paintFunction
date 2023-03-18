#ifndef CALCULATE_H
#define CALCULATE_H
#include <QObject>
#include <QVector>
#include <QDebug>

#include "mystruct.h"
#include "timer.h"


class calculate :public QObject
{
    Q_OBJECT
public:
    calculate();
    void giveDots(QVector<QPoint> & f);//误差 value to px
    void giveInsert(QVector<QPoint> & f);

    void getInsert(const QVector<Value> & f);
public slots:
    void getScreen(const Screen & s);

private:
    void  LagrangianInsert();

private:
    QVector<Value> dots;//传出点

    QVector<Value> insert;//传入插值点

    Screen scr;
};

inline void calculate::giveDots(QVector<QPoint> &f)
{
    f.resize(0);
    for(int i = 0;i < dots.size();i++)
    {
        f.push_back({qRound(dots[i].x * scr.unit_px) + scr.origin.x(),scr.origin.y() - qRound(dots[i].y * scr.unit_px)});
    }

}

inline void calculate::giveInsert(QVector<QPoint> &f)
{
    f.resize(0);
    for(int i = 0;i < insert.size();i++)
    {
        f.push_back({qRound(insert[i].x * scr.unit_px) + scr.origin.x(),scr.origin.y() - qRound(insert[i].y * scr.unit_px)});
    }
}


inline void calculate::getInsert(const QVector<Value> &f)
{
    this->insert = f;
}

inline void calculate::getScreen(const Screen &s)
{
    scr = s;
}


#endif // CALCULATE_H
