#include "calculate.h"

calculate::calculate()
{

}

void calculate::LagrangianInsert()
{
    if(insert.size() == 0) return;

    dots.resize(0);

    // 初始化二维数组，存储所有点的差商
    // 避免重复计算

    //大约优化了300us
    QVector<double> mul;
    double di;
    int size = insert.size();

    for(int i = 0;i < size;i++)
    {
        di = insert[i].y;
        for(int j = 0;j < size;j++)
        {
          if(i != j)
          {
              di /= (insert[i].x - insert[j].x);
          }
        }

        mul.push_back(di);
    }


    double sum = 0;

    QVector<double> prefix(insert.size()),suffix(insert.size());

    //开始和结束的位置与点无关，和屏幕有关

    double startX = insert[0].x,endX = insert[0].x;

    for(auto & i : insert)
    {
        if(i.x < startX) startX = i.x;
        if(i.x > endX) endX = i.x;
    }

    double start = scr.range.x0;
    double end = scr.range.x1;

    for(double i = start;i <= end;)
    {
        sum = 0;
        prefix[0] = suffix[size-1]= 1;

        //用前缀和后缀积，解决问题
        //不能只用一个total乘一起，有0会产生问题
        for(int j = 1,m = size - 2;j < size;j++,m--)
        {
            prefix[j] = prefix[j-1] * (i - insert[j-1].x);
            suffix[m] = suffix[m+1] * (i - insert[m+1].x);
        }

        for(int j = 0;j < size;j++)
        {
            sum += mul[j] * prefix[j] * suffix[j];
        }

        dots.push_back({i,sum});

//        if(startX <= i && i <= endX)
//        {
//            i += 0.01 / scale;
//        }
//        else
//        {
//            i += 0.25 / scale;
//        }
        i += 0.01;
    }

    qDebug() << dots.size();
}
