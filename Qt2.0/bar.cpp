#include "bar.h"
#include <QtDebug>

//构造函数
Bar::Bar(qreal left, qreal top, qreal width, qreal height)
{
    this->position = new QRectF(left, top, width, height);
    this->left = left;
    this->top = top;
    this->width = width;
    this->height = height;
    this->v = 0;
}

//析构函数
Bar::~Bar()
{
    delete position;
}

//获得位置
QRectF* Bar::getPosition() const
{
    return this->position;
}

//获得速度
qreal Bar::getV() const
{
    return this->v;
}

//设置速度
void Bar::setV(qreal V)
{
    this->v = V;
}

//更新
void Bar::update(qreal leftLimit, qreal rightLimit)
{
    //判断是否到了边界
    if(this->position->left() + this->v < leftLimit)
        this->v = leftLimit - this->position->left();
    if(this->position->right() + this->v > rightLimit)
        this->v = rightLimit - this->position->right();

    //更新速度
    this->position->setLeft(this->position->left() + this->v);
    this->position->setRight(this->position->right() + this->v);
    this->v = this->v * 0.8;
}

//初始化
void Bar::initial()
{
    this->position->setLeft(this->left);
    this->position->setTop(this->top);
    this->position->setWidth(this->width);
    this->position->setHeight(this->height);
    this->v = 0;
}
