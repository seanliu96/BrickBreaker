#include "brick.h"

//构造函数
Brick::Brick(qreal left, qreal top, qreal width, qreal height, QColor color)
{
    this->position = new QRectF(left, top, width, height);
    this->color = color;
    this->vx = 0;
    this->vy = 0;
    this->live = 1;
}

//析构函数
Brick::~Brick()
{
    delete position;
}

//获得位置
QRectF* Brick::getPosition() const
{
    return this->position;
}

//获得水平速度
qreal Brick::getVx() const
{
    return this->vx;
}

//获得颜色
QColor Brick::getColor() const
{
    return this->color;
}

//获得竖直速度
qreal Brick::getVy() const
{
    return this->vy;
}

//设置水平速度
void Brick::setVx(qreal Vx)
{
    this->vx = Vx;
}

//设置竖直速度
void Brick::setVy(qreal Vy)
{
    this->vy = Vy;
}

//更新
void Brick::update()
{
    //静止
    if(this->vy ==0 && this->vx ==0)
        return;

    //无效
    if(!(this->live))
        return;

    //更新位置
    this->position->setTop(this->position->top() + this->vy);
    this->position->setBottom(this->position->bottom() + this->vy);
    this->position->setLeft(this->position->left() + this->vx);
    this->position->setRight(this->position->right() + this->vx);

    //更新速度
    vy += g;
}

//设置重力加速度
void Brick::setG(qreal g)
{
    this->g = g;
}

//获得有效
bool Brick::getLive() const
{
    return this->live;
}

//设置有效
void Brick::setLive(bool live)
{
    this->live = live;
}
