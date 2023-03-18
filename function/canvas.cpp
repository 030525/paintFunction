#include "canvas.h"
#include "ui_canvas.h"




void canvas::start()
{
    cal = new calculate();
    connect(this,&canvas::giveScreen,cal,&calculate::getScreen);
}

void canvas::addArrow(const QPoint &p)
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

void canvas::addCross(const QPoint &p)
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

void canvas::paintBackground()
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
//关闭painter


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
}

void canvas::paintDot(const QVector<QPoint> &f)
{
    QPainter paint(this);
    QBrush b;
    b.setStyle(Qt::SolidPattern);

    paint.setBrush(b);

    for(auto & i : f)
    {
        paint.drawEllipse(i.x()-4,i.y()-4,8,8);
    }

}

