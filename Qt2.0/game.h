#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QList>
#include <QColor>
#include "ball.h"
#include "bar.h"
#include "brick.h"

const static int M1 = 1;
const static int M2 = 5;
const static int M3 = INT_MAX - M1 - M2;
/*
 * 游戏类
 * 包含属性：一个小球、小球的图标、一个滑块、滑块的图标、一个砖块列表、文件有效、输、赢、重力加速度、分数
 * 包含方法：构造函数、析构函数、初始化、获得赢的情况、获得输的情况、获得分数、设置重力加速度、画图像
 */
class Game : public QWidget
{
    Q_OBJECT
public:
    explicit Game(int width, int height, QWidget *parent = 0);
    ~Game();
    Ball *ball;
    Bar *bar;
    QList<Brick*> bricks;
    void initial();
    bool getIsWin() const;
    bool getIsFailed() const;
    int getScore() const;
    void setG(qreal g);
protected:
    void paintEvent(QPaintEvent *event);
signals:

public slots:
    void updateGame();
private:
    int row;
    int column;
    int score;
    int width;
    int height;
    bool fileOk;
    bool isWin;
    bool isFailed;
    qreal g;
    QPixmap barPixmap;
    QPixmap ballPixmap;
    QColor randomColor();
};

#endif // GAME_H
