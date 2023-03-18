#ifndef MYSTRUCT_H
#define MYSTRUCT_H

#include <QPoint>

typedef struct
{
    double x;
    double y;
}Value;

typedef struct
{
    double x0;
    double x1;
}Range;

typedef struct
{
    Range range;//x的定义域，一般要比屏幕大一点
    QPoint origin;
    int unit_px;
}Screen ;

#endif // MYSTRUCT_H
