#include "game.h"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QPainter>
#include <QtDebug>
#include <cmath>

//构造函数
Game::Game(int width, int height, QWidget *parent) : QWidget(parent)
{
    setFixedSize(width, height);
    qreal barHeight = 13.5 * 2;
    qreal barWidth = 30 * 2;
    qreal ballDiameter = 1.0 * width / 30;
    QMatrix rotation270;
    rotation270.rotate(270);
    ballPixmap = QPixmap("resources/ball.png");
    barPixmap = QPixmap("resources/bar.png").transformed(rotation270,Qt::SmoothTransformation);
    ballPixmap.scaled(QSize(ballDiameter, ballDiameter));
    barPixmap.scaled(QSize(barWidth, barHeight));
    this->width = width;
    this->height = height;
    ball = new Ball(width / 2 - ballDiameter / 2, height - barHeight - ballDiameter, ballDiameter);
    bar = new Bar(width / 2 - barWidth / 2, height - barHeight, barWidth, barHeight);
    score = 0;
}

//析构函数
Game::~Game()
{
    delete ball;;
    delete bar;
    bricks.clear();
}

//更新游戏
void Game::updateGame()
{
    if(!fileOk)
        return;
    if(isWin)
        return;
    if(isFailed)
        return;
    bar->update(rect().left(), rect().right());
    QRectF *ballPosition = ball->getPosition();
    QRectF *barPosition = bar->getPosition();
    QRectF *brickPosition = NULL, *brick1Position = NULL;
    qreal ballVx = ball->getVx(), ballVy = ball->getVy(), barV = bar->getV(), brickVx, brickVy, brick1Vx, brick1Vy;

    //小球撞到滑块，小球反向，同时在X方向获得一个速度
    if(ballPosition->intersects(*barPosition))
    {
        ballVy = -sqrt(2 * 440 * g);
        ballVx = ballVx + 0.5 * barV;
        barV *= 0.5;
    }

    //小球碰到边界
    if(ballPosition->top() < rect().top())
        ballVy = qAbs(ballVy);
    //小球到达底面，失败
    else if(ballPosition->bottom() > rect().bottom())
    {
        isFailed = 1;
        return;
    }
    if(ballPosition->left() < rect().left())
    {
        ballVx = qAbs(ballVx);
    }
    else if(ballPosition->right() > rect().right())
    {
        ballVx = -qAbs(ballVx);
    }
    QList<Brick*>::iterator it = bricks.begin();
    while(it!=bricks.end())
    {
        if(!(*it)->getLive())
        {
            ++it;
            continue;
        }
        brickPosition = (*it)->getPosition();
        brickVx = (*it)->getVx();
        brickVy = (*it)->getVy();

        //砖块撞到滑块
        if(barPosition->intersects(*brickPosition))
        {
            brickVy = -sqrt(440 * g);;
            brickVx = brickVx + 0.5 * barV;
            barV *= 0.5;
        }

        //砖块碰到其他边界
        if(brickPosition->top() > rect().bottom())
        {
            (*it)->setLive(0);
            ++it;
            continue;
        }

        if(brickPosition->top() < rect().top())
            brickVy = qAbs(brickVy);
        if(brickPosition->left() < rect().left())
            brickVx = qAbs(brickVx);
        else if(brickPosition->right() > rect().right())
            brickVx = -qAbs(brickVx);

        //根据动量守恒和能量守恒定理
        //v1'=((m1-m2)*v1+2m2v2)/(m1+m2)
        //v2'=((m2-m1)*v2+2m1v1)/(m1+m2)

        //砖块撞到小球
        if(ballPosition->intersects(*brickPosition))
        {
            qreal ballCenterX = 1.0 * (ballPosition->left() + ballPosition->right()) / 2;
            qreal ballCenterY = 1.0 * (ballPosition->top() + ballPosition->bottom()) / 2;
            qreal brickCenterX = 1.0 * (brickPosition->left() + brickPosition->right()) / 2;
            qreal brickCenterY = 1.0 * (brickPosition->top() + brickPosition->bottom()) / 2;
            qreal temp;
            ++score;
            if(brickVx == 0 && brickVy == 0)
            {
                if(qAbs(ballCenterX - brickCenterX) / (ballPosition->width() + brickPosition->width()) >= qAbs(ballCenterY - brickCenterY) / (ballPosition->height() + brickPosition->height()))
                {
                    temp = ((M1 - M3) * ballVx) / (M1 + M3);
                    brickVy = (2 * M1 * ballVx ) / (M1 + M3);
                    ballVx = temp;
                }
                else
                {
                    temp = ((M1 - M3) * ballVy) / (M1 + M3);
                    brickVy = (2 * M1 * ballVy ) / (M1 + M3);
                    ballVy = temp;
                }
            }
            else
            {
                if(qAbs(ballCenterX - brickCenterX) /(ballPosition->width() + brickPosition->width()) >= qAbs(ballCenterY - brickCenterY) / (ballPosition->height() + brickPosition->height()))
                {
                    temp = ((M1 - M2) * ballVx + 2 * M2 * brickVx ) / (M1 + M2);
                    brickVx = ((M2 - M1) * brickVx + 2 * M1 * ballVx ) / (M1 + M2);
                    ballVx = temp;
                }
                else
                {
                    temp = ((M1 - M2) * ballVy + 2 * M2 * brickVy ) / (M1 + M2);
                    brickVy = ((M2 - M1) * brickVy + 2 * M1 * ballVy ) / (M1 + M2);
                    ballVy = temp;
                }
                (*it)->setLive(0);
                ++it;
                continue;
            }

            while(ballPosition->intersects(*brickPosition))
            {
                ball->setVx(ballVx / 100);
                ball->setVy(ballVy / 100);
                (*it)->setVx(brickVx / 100);
                (*it)->setVy(brickVy / 100);
                ball->update();
                (*it)->update();
                ballPosition = ball->getPosition();
                brickPosition = (*it)->getPosition();
            }
        }

        if(brickVx!=0 || brickVy!=0)
        {
            //砖块撞到砖块
            QList<Brick*>::iterator it1 = bricks.begin();
            while(it1!=bricks.end())
            {
                if(!(*it1)->getLive())
                {
                    ++it1;
                    continue;
                }
                if(it1 == it)
                {
                    ++it1;
                    continue;
                }

                brick1Position = (*it1)->getPosition();
                brick1Vx = (*it1)->getVx();
                brick1Vy = (*it1)->getVy();

                //相撞
                if(brickPosition->intersects(*brick1Position))
                {
                    qreal brickCenterX = 1.0 * (brickPosition->left() + brickPosition->right()) / 2;
                    qreal brickCenterY = 1.0 * (brickPosition->top() + brickPosition->bottom()) / 2;
                    qreal brick1CenterX = 1.0 * (brick1Position->left() + brick1Position->right()) / 2;
                    qreal brick1CenterY = 1.0 * (brick1Position->top() + brick1Position->bottom()) / 2;
                    qreal temp;
                    if(brick1Vx == 0 && brick1Vy == 0)
                    {
                        if(qAbs(brickCenterX - brick1CenterX) / (brickPosition->width() + brick1Position->width()) >= qAbs(brickCenterY - brick1CenterY) / (brickPosition->height() + brick1Position->height()))
                        {
                            temp = ((M2 - M3) * brickVx ) / (M2 + M3);
                            brick1Vx = (2 * M2 * brickVx ) / (M2 + M3);
                            brickVy = temp;
                        }
                        else
                        {
                            temp = ((M2 - M3) * brickVy ) / (M2 + M3);
                            brick1Vy = ( 2 * M2 * brickVy ) / (M2 + M3);
                            brickVy = temp;
                        }
                    }
                    else
                    {
                        if(qAbs(brickCenterX - brick1CenterX) / (brickPosition->width() + brick1Position->width()) > qAbs(brickCenterY - brick1CenterY) / (brickPosition->height() + brick1Position->height()))
                        {
                            qreal temp = brickVx;
                            brickVx = brick1Vx;
                            brick1Vx = temp;
                        }
                        else
                        {
                            temp = brickVy;
                            brickVy = brick1Vy;
                            brick1Vy = temp;
                        }
                    }
                    while(brickPosition->intersects(*brick1Position))
                    {
                        (*it)->setVx(brickVx / 100);
                        (*it)->setVy(brickVy / 100);
                        (*it)->update();
                        (*it1)->setVx(brick1Vx / 100);
                        (*it1)->setVy(brick1Vy / 100);
                        (*it1)->update();
                        brickPosition = (*it)->getPosition();
                        brick1Position = (*it1)->getPosition();
                    }
                }
                (*it1)->setVx(brick1Vx);
                (*it1)->setVy(brick1Vy);
                ++it1;
            }
        }
        //砖块撞到滑块
        if(barPosition->intersects(*brickPosition))
        {
            brickVy = -sqrt(440 * g);
            brickVx = brickVx + 0.5 * barV;
            barV *= 0.5;
        }

        //砖块碰到其他边界
        if(brickPosition->top() > rect().bottom())
        {
            (*it)->setLive(0);
            ++it;
            continue;
        }
        if(brickPosition->top() < rect().top())
            brickVy = qAbs(brickVy);
        if(brickPosition->left() < rect().left())
            brickVx = qAbs(brickVx);
        else if(brickPosition->right() > rect().right())
            brickVx = -qAbs(brickVx);
        (*it)->setVx(brickVx);
        (*it)->setVy(brickVy);
        ++it;
    }

    //小球撞到滑块，小球反向，同时在X方向获得一个速度
    if(ballPosition->intersects(*barPosition))
    {
        ballVy = -sqrt(2* 440 * g);
        ballVx = ballVx + 0.5 * barV;
        barV *= 0.5;
    }

    //小球碰到其他边界
    if(ballPosition->top() < rect().top())
        ballVy = qAbs(ballVy);
    //小球到达底面，失败
    else if(ballPosition->bottom() > rect().bottom())
    {
        isFailed = 1;
        return;
    }
    if(ballPosition->left() < rect().left())
    {
        ballVx = qAbs(ballVx);
    }
    else if(ballPosition->right() > rect().right())
    {
        ballVx = -qAbs(ballVx);
    }

    ball->setVx(ballVx);
    ball->setVy(ballVy);
    ball->update();
    //bar->update(rect().left(), rect().right());
    it = bricks.begin();
    int lives = 0;
    while(it!=bricks.end())
    {
        if((*it)->getLive())
        {
            ++lives;
            (*it)->update();
        }
        ++it;
    }
    update();
    if(!lives)
        isWin = 1;
}

//画图像
void Game::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if(!isWin && !isFailed)
    {
        painter.setPen(Qt::NoPen);
        painter.drawPixmap(ball->getPosition()->left(), ball->getPosition()->top(), ball->getPosition()->width(), ball->getPosition()->height(),ballPixmap);
        painter.drawPixmap(bar->getPosition()->left(), bar->getPosition()->top(), bar->getPosition()->width(), bar->getPosition()->height(),barPixmap);
        for(int i=0;i<bricks.size();++i)
        {
            if(bricks[i]->getLive())
                painter.fillRect(*(bricks[i]->getPosition()), bricks[i]->getColor());
        }
    }
}

//初始化
void Game::initial()
{
    fileOk = 0;
    score = 0;
    isWin=0;
    isFailed=0;
    ball->initial();
    bar->initial();
    bricks.clear();
    QFile file("maps/map.txt");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream input(&file);
        QList<QString> lines;
        row = 0;
        column = 0;
        while(!input.atEnd())
        {
            lines.append(input.readLine());
            if(column < lines[lines.size()-1].length())
                column = lines[lines.size()-1].length();
        }
        row = lines.size();
        qreal brickWidth = 1.0 * width / column - 3;
        qreal brickHeight = 1.0 * height / 2 / row - 3;
        for(int i=0;i<lines.size();++i)
        {
            for(int j=0;j<lines[i].length();++j)
            {
                if(lines[i][j] != ' ')
                {
                    bricks.append(new Brick( j * (brickWidth + 3), i * (brickHeight + 3), brickWidth, brickHeight, randomColor()));
                    bricks[bricks.size()-1]->setG(g);
                }
            }
        }

        fileOk = 1;
        file.close();
    }
    else
    {
        QMessageBox::information(this,tr("Error"),tr("请检查地图是否合法"));
        return;
    }
}

//随机颜色
QColor Game::randomColor()
{
    return QColor(qrand() % 256, qrand() % 256, qrand()% 256);
}

//获得赢
bool Game::getIsWin() const
{
    return isWin;
}

//获得输
bool Game::getIsFailed() const
{
    return isFailed;
}

//获得分数
int Game::getScore() const
{
    return score;
}

//设置重力加速度
void Game::setG(qreal g)
{
    this->g = g;
    ball->setG(g);
    for(int i =0;i<bricks.size();++i)
        bricks[i]->setG(g);
}
