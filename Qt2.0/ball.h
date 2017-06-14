#ifndef BALL_H
#define BALL_H

#include <QColor>
#include <QRect>

/*
 * 小球类
 * 包含属性：位置、速度、大小、重力加速度
 * 包含方法：构造函数、析构函数、初始化、设置/获得速度、设置重力加速度、更新
 */
class Ball
{
public:
    Ball(qreal left, qreal top, qreal diameter);
    ~Ball();
    QRectF* getPosition() const;
    qreal getVx() const;
    qreal getVy() const;
    void setVx(qreal vx);
    void setVy(qreal vy);
    void update();
    void initial();
    void setG(qreal g);
private:
    QRectF *position;
    qreal left;
    qreal top;
    qreal diameter;
    qreal vx;
    qreal vy;
    qreal g;
};

#endif // BALL_H
