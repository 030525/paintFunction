#ifndef CANVAS_H
#define CANVAS_H

#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QPoint>

#include <QPainterPath>
#include <QPainterPathStroker>
#include <QtMath>

#include <Qwt/qwt.h>
#include "timer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class canvas; }
QT_END_NAMESPACE


typedef struct
{
    double x;
    double y;
}Value;

class canvas : public QWidget
{
    Q_OBJECT
public:
    canvas(QWidget *parent = nullptr)
        : QWidget(parent),origin({-1,-1}),last_pos(origin),px(50),scale(1.0),unit_px(px)
    {
        //改变窗口大小origin不改变
        //不按下鼠标也能捕获滚轮
        //setMouseTracking(true);
    }

protected:

    void resizeEvent(QResizeEvent *event) override
    {
        //全屏的时候，widget不会自动resize，导致origin坐标出问题
        //初始化的时候，无法用this。width，因为每加载好
        origin = QPoint(width() / 2, height() / 2);
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
        qDebug() << scale;
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
            update();
        }
    }
    void mouseReleaseEvent(QMouseEvent * event) override
    {
        QApplication::restoreOverrideCursor();
    }

    void paintEvent(QPaintEvent *event) override
    {
        int w = this->width(),h = this->height();
        int ox = origin.x(),oy = origin.y();
        int Px = px * scale;

        QPainter p(this);
        QPen pen,pen1;
        QBrush brush;

        pen.setColor(QColor(173, 216, 230));
        pen.setStyle(Qt::DashLine);
        pen1.setWidth(4);
        brush.setStyle(Qt::SolidPattern);

        p.setPen(pen);

        //画背景
        for(int i = ox;i > 0;i -=Px)
        {
            p.drawLine(i,0,i,h);
        }
        for(int i = ox;i <= w;i +=Px)
        {
            p.drawLine(i,0,i,h);
        }

        for(int j = oy;j > 0;j -=Px)
        {
            p.drawLine(0,j,w,j);
        }
        for(int j = oy;j <= h;j +=Px)
        {
            p.drawLine(0,j,w,j);
        }

        //坐标轴
        p.setPen(pen1);
        p.drawLine(ox,0,ox,h);
        p.drawLine(0,oy,w,oy);

        //画个原点
        p.setBrush(brush);
        p.drawEllipse(ox-5,oy-5,10,10);

        p.end();
        //坐标轴添加箭头
        if(oy >=0)addArrow({ox,0});
        if(oy <= h)addArrow({ox,h});
        if(ox >= 0)addArrow({0,oy});
        if(ox <= w)addArrow({w,oy});


        //坐标轴刻度
        if(scale > 0.6)
        {
        for(int i = ox;i > 0;i -=Px)
        {
            addCross({i,oy});
        }
        for(int i = ox;i <= w;i +=Px)
        {
            addCross({i,oy});
        }

        for(int j = oy;j > 0;j -=Px)
        {
            addCross({ox,j});
        }
        for(int j = oy;j <= h;j +=Px)
        {
            addCross({ox,j});
        }
        }

        QVector<Value> f;
        f.push_back({0,10});
        f.push_back({-3,7});
        f.push_back({-2,6});
        f.push_back({2,6});
        f.push_back({3,7});
        f.push_back({4.8});
        f.push_back({5,9});

        painLagrangian(f);


        qDebug() << "px " << px * scale;

    }

    void addArrow(const QPoint &p)
    {
        QPainter painter(this);
        QPen pen;
        pen.setCapStyle(Qt::SquareCap);
        pen.setJoinStyle(Qt::MiterJoin);
        pen.setWidth(4);

        painter.setPen(pen);

        int l = 15;

        painter.drawLine(p,{p.x() + l,p.y() + l});
        painter.drawLine(p,{p.x() - l,p.y() + l});
        painter.drawLine(p,{p.x() + l,p.y() - l});
        painter.drawLine(p,{p.x() - l,p.y() - l});
     }

    void addCross(const QPoint & p)
    {
        QPainter painter(this);
        QPen pen;
        pen.setWidth(3);

        painter.setPen(pen);

        //int l = 5;
        int l = px * scale / 6;
        painter.drawLine(p,{p.x() + l,p.y()});
        painter.drawLine(p,{p.x() - l,p.y()});
        painter.drawLine(p,{p.x(),p.y() + l});
        painter.drawLine(p,{p.x(),p.y() - l});
    }

    void painLagrangian(const QVector<Value> & f)
    {
        Timer t;
        t.start();

        QPainter painter(this);
        QPen pen;
        QBrush brush;

        pen.setWidth(4);
        brush.setStyle(Qt::SolidPattern);
        painter.setBrush(brush);

        for(auto & i : f)
        {
            painter.drawEllipse(valueToPoint(i.x,i.y),6,6);
        }

        // 初始化二维数组，存储所有点的差商
        // 避免重复计算

        //大约优化了300us
        QVector<double> mul;
        double di;
        int size = f.size();

        for(int i = 0;i < size;i++)
        {
            di = f[i].y;
            for(int j = 0;j < size;j++)
            {
              if(i != j)
              {
                  di /= (f[i].x - f[j].x);
              }
            }

            mul.push_back(di);
        }


        double value = this->width() / unit_px;

        QVector<QPoint> v,v1;
        double sum = 0,sum1 = 0;

        double end = f[f.size() - 1].x;


        QVector<double> prefix(f.size()),suffix(f.size());
        QVector<double> prefix1(f.size()),suffix1(f.size());

        for(double i = 0;i <= value;)
        {
            sum = 0;
            sum1 = 0;
            prefix[0] = suffix[size-1]= 1;
            prefix1[0] = suffix1[size-1]= 1;

            //用前缀和后缀积，解决问题
            //不能只用一个total乘一起，有0会产生问题
            for(int j = 1,m = size - 2;j < size;j++,m--)
            {
                prefix[j] = prefix[j-1] * (i - f[j-1].x);
                suffix[m] = suffix[m+1] * (i - f[m+1].x);

                prefix1[j] = prefix1[j-1] * (-i - f[j-1].x);
                suffix1[m] = suffix1[m+1] * (-i - f[m+1].x);
            }

            for(int j = 0;j < size;j++)
            {
                sum += mul[j] * prefix[j] * suffix[j];
                sum1 += mul[j] * prefix1[j] * suffix1[j];
            }

            v.push_back(valueToPoint(i,sum));
            v1.push_back(valueToPoint(-i,sum1));


            if(i < end + 5) i += 0.05;
            else i += 0.02;
        }




        painter.setPen(pen);
        painter.drawPolyline(v);
        painter.drawPolyline(v1);
        t.end();

    }

    inline void valueToPoint(const double & x, const double & y,QVector<QPoint> & f)
    {
        f.push_back({qRound((origin.x() + x * unit_px)),qRound(origin.y() - y * unit_px)});
    }
    inline QPoint  valueToPoint(const double &x,const double & y)
    {
        return QPoint({qRound((origin.x() + x * unit_px)),qRound(origin.y() - y * unit_px)});
    }

    inline void unit_change()
    {
        //一格单位值时，多少个像素
        unit_px = px * scale;
    }

private:


    QPoint origin;
    QPoint last_pos;

    int px;
    double scale;
    //单位长度1，对应多少个像素
    int unit_px;
};

#endif // CANVAS_H
