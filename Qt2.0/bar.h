#ifndef BAR_H
#define BAR_H

#include <QColor>
#include <QRect>

/*
 * 滑块类
 * 包含属性：位置、速度、大小
 * 包含方法：构造函数、析构函数、初始化、设置/获得速度、更新
 */
class Bar
{
public:
    Bar(qreal left, qreal top, qreal width, qreal height);
    ~Bar();
    QRectF* getPosition() const;
    qreal getV() const;
    void setV(qreal V);
    void update(qreal leftLimit, qreal rightLimit);
    void initial();
private:
    QRectF *position;
    qreal left;
    qreal top;
    qreal width;
    qreal height;
    qreal v;
};

#endif // BAR_H
