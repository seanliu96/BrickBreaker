#ifndef BRICK_H
#define BRICK_H

#include <QColor>
#include <QRect>

/*
 * 砖块类
 * 包含属性：位置、颜色、速度、大小、重力加速度、是否有效
 * 包含方法：构造函数、析构函数、初始化、设置/获得速度、设置重力加速度、更新、设置/获得有效
 */
class Brick
{
public:
    Brick(qreal left, qreal top, qreal width, qreal height,  QColor color);
    ~Brick();
    QRectF* getPosition() const;
    qreal getVx() const;
    qreal getVy() const;
    QColor getColor() const;
    bool getLive() const;
    void setVx(qreal Vx);
    void setVy(qreal Vy);
    void update();
    void setG(qreal g);
    void setLive(bool live);
private:
    QRectF *position;
    QColor color;
    qreal vx;
    qreal vy;
    qreal g;
    bool live;
};

#endif // BRICK_H
