#include "ball.h"
#include <cmath>

//构造函数
Ball::Ball(qreal left, qreal top, qreal diameter)
{
    this->left = left;
    this->top = top;
    this->diameter = diameter;
    this->position = new QRectF(this->left, this->top, this->diameter, this->diameter);
    initial();
}

//析构函数
Ball::~Ball()
{
    delete position;
}

//获得位置
QRectF* Ball::getPosition() const
{
    return this->position;
}

//获得水平方向速度
qreal Ball::getVx() const
{
    return this->vx;
}

//获得竖直方向速度
qreal Ball::getVy() const
{
    return this->vy;
}

//设置水平方向速度
void Ball::setVx(qreal Vx)
{
    this->vx = Vx;
}

//设置竖直方向速度
void Ball::setVy(qreal Vy)
{
    this->vy = Vy;
}

//初始化
void Ball::initial()
{
    this->position->setLeft(this->left);
    this->position->setTop(this->top);
    this->position->setWidth(this->diameter);
    this->position->setHeight(this->diameter);
    this->vy = -sqrt(2* 440 * g);
    this->vx = (qrand()* 1.0 - 0.5) * 2 / RAND_MAX * this->vy;
}

//更新
void Ball::update()
{
    vy += g;
    if(vx > sqrt(2* 440 * g))
        vx = sqrt(2* 440 * g);
    if(vy < -sqrt(2* 440 * g))
        vy = -sqrt(2* 440 * g);
    this->position->setTop(this->position->top() + this->vy);
    this->position->setBottom(this->position->bottom() + this->vy);
    this->position->setLeft(this->position->left() + this->vx);
    this->position->setRight(this->position->right() + this->vx);
}

//设置重力加速度
void Ball::setG(qreal g)
{
    this->g = g;
}
