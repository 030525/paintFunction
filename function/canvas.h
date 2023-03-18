#ifndef CANVAS_H
#define CANVAS_H

#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

#include <QPainterPath>


#include "calculate.h"

QT_BEGIN_NAMESPACE
namespace Ui { class canvas; }
QT_END_NAMESPACE




class canvas : public QWidget
{
    Q_OBJECT
public:
    canvas(QWidget *parent = nullptr)
        : QWidget(parent),origin({-1,-1}),last_pos(origin),px(50),scale(1.0),unit_px(px)
    {
        start();
    }
private slots:
    void changeScreen();//直接一个就够了

signals:
    void giveScreen(const Screen & s);

private:
    void unit_change();
    void range_change();

protected:
    void start();

    void resizeEvent(QResizeEvent *event) override
    {
        //全屏的时候，widget不会自动resize，导致origin坐标出问题
        //初始化的时候，无法用this。width，因为每加载好
        origin = QPoint(width() / 2, height() / 2);

        changeScreen();
        QWidget::resizeEvent(event);

    }

    void wheelEvent(QWheelEvent * event) override
    {
        int delta = event->angleDelta().y();
        if (delta > 0) {
            // 向上滚动，放大绘图区域
            if(scale < 30)
            {
                scale *= 1.1;
                unit_change();
            }
        } else if (delta < 0) {
            // 向下滚动，缩小绘图区域
            if(scale > 0.2)
            {
                scale /= 1.1;
                unit_change();
            }
        }

        changeScreen();
        qDebug() << "in wheelEvent " << scale;
        update();
    }

    void mousePressEvent(QMouseEvent * event) override
    {


        if(event->button() == Qt::LeftButton)
        {
            QApplication::setOverrideCursor(Qt::PointingHandCursor);
            last_pos = event->pos();
        }

    }
    void mouseMoveEvent(QMouseEvent *event) override
    {
        if(event->buttons() & Qt::LeftButton)
        {
            origin += event->pos() - last_pos;
            last_pos = event->pos();

            changeScreen();
            update();
        }
    }
    void mouseReleaseEvent(QMouseEvent * event) override
    {
        QApplication::restoreOverrideCursor();
    }

    void paintEvent(QPaintEvent *event) override
    {
        paintBackground();
    }
private://画背景版
    void addArrow(const QPoint &p);
    void addCross(const QPoint & p);

    void paintBackground();

private://画图
    void paintDot(const QVector<QPoint> & f);






private:
    calculate * cal;

    QPoint origin;
    QPoint last_pos;

    int px;
    double scale;
    //单位长度1，对应多少个像素
    int unit_px;
    Range range;
};

inline void canvas::changeScreen()
{
    unit_change();
    range_change();

    emit giveScreen({range,origin,unit_px});
}

inline void canvas::unit_change()
{
    //一格单位值时，多少个像素
    unit_px = px * scale;
}

inline void canvas::range_change()
{
    range.x0 = (0 - origin.x()) / unit_px - 2;
    range.x1 = (this->width() - origin.x()) / unit_px + 2;
}


#endif // CANVAS_H
