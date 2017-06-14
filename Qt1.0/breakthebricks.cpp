#include "Breakthebricks.h"
#include "ui_breakthebricks.h"
#include <QDateTime>
#include <QKeyEvent>
#include <QMessageBox>


BreakTheBricks::BreakTheBricks(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BreakTheBricks)
{
    ui->setupUi(this);
    QLabel *temp;
    mutex.lock();
    for(int i = 0; i < COLUMN; ++i)
    {
        int j = 0;
        for(; j < ROW; ++j)
        {
            temp = new QLabel(ui->brick);
            temp->setText(tr(""));
            temp->setMinimumSize(QSize(10, 10));
            temp->setMaximumSize(QSize(10, 10));
            temp->setGeometry(i * 11 + 1, j * 11 + 1, 10, 10);
            temp->setStyleSheet("border:0px;background-color:white");
            brick[i][j] = temp;
        }
    }
    mutex.unlock();
    connect(ui->beginBtn, SIGNAL(clicked(bool)), this, SLOT(onBeginGame()));
    connect(ui->pauseBtn, SIGNAL(clicked(bool)), this, SLOT(onPause()));
    connect(ui->exitBtn, SIGNAL(clicked(bool)), this, SLOT(close()));
}

BreakTheBricks::~BreakTheBricks()
{
    delete ui;
}

void BreakTheBricks::drawBricks()
{
    mutex.lock();
    for(int i = 0; i < COLUMN; ++i)
    {
        int j = 0;
        for(; j < ROW; ++j)
        {
            QString temp = "border:0px;background-color:";
            temp += colorStr[color[i][j]];
            brick[i][j]->setStyleSheet(temp);
        }
    }
    for(int i = barLeft; i <= barRight; ++i)
        brick[i][ROW - 1]->setStyleSheet("border:0px;background-color:red");
    mutex.unlock();
}



bool BreakTheBricks::isDead()
{
    return y == ROW - 1;
}

bool BreakTheBricks::isOver()
{
    return score == COLUMN * ROW / 2;
}

void BreakTheBricks::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    if(key != Qt::Key_Escape && pause)
        return;
    switch(key)
    {
    case Qt::Key_A:
    case Qt::Key_Left:
        if(barLeft > 0 && !pause)
        {
            --barLeft;
            --barRight;
        }
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        if(barRight < COLUMN - 1 &&!pause)
        {
            ++barLeft;
            ++barRight;
        }
        break;
    case Qt::Key_Escape:
        pause = !pause;
    }
    if(!pause)
    {
        mutex.lock();
        if(barLeft > 0)
        {
            brick[barLeft - 1][ROW - 1]->setStyleSheet("border:0px;background-color:white");
        }
        if (barRight < COLUMN - 1)
        {
            brick[barRight + 1][ROW - 1]->setStyleSheet("border:0px;background-color:white");
        }
        brick[barLeft][ROW - 1]->setStyleSheet("border:0px;background-color:red");
        brick[barRight][ROW - 1]->setStyleSheet("border:0px;background-color:red");
        mutex.unlock();
    }
}

void BreakTheBricks::onBeginGame()
{
    qsrand(QDateTime::currentDateTime().toTime_t());
    score = 0;
    x = COLUMN / 2;
    y = ROW - 2;
    upOrDown = -1;
    leftOrRight = qrand() %2 * 2 - 1;
    pause = 0;
    barLeft = COLUMN / 2 - 3;
    barRight = COLUMN / 2 + 3;
    for(int i = 0; i < COLUMN; ++i)
    {
        int j = 0;
        for(; j < ROW / 2; ++j)
            color[i][j] = qrand() % 15 + 1;
        for(; j < ROW ; ++j)
            color[i][j] = 0;
    }

    switch(ui->difficultyComboBox->currentIndex())
    {
    case 0:
        difficulty = 200;break;
    case 1:
        difficulty = 100;break;
    case 2:
        difficulty = 50;break;
    }

    drawBricks();
    while(!isDead() && !isOver())
    {
        QTime t;
        t.start();
        update();
        while(t.elapsed() < difficulty)
            QCoreApplication::processEvents();
    }
    gameOver();
}


void BreakTheBricks::update()
{
    if(pause)
        return;

    if(x == 0)
        leftOrRight = 1;
    if(x == COLUMN -1)
        leftOrRight = -1;
    if(y == 0)
        upOrDown = 1;
    if(y == ROW - 2 && (x + leftOrRight >= barLeft) && (x + leftOrRight <= barRight))
        upOrDown = -1;

    if(color[x][y + upOrDown] != 0)
    {
        if(qrand() % 3 == 0)
        {
            for(int i = x - 1; i <= x + 1; ++i)
            {
                for(int j = y - 1; j <= y + 1; ++j)
                {
                    if(i >= 0 && i < COLUMN && j >= 0 && j < ROW && (x != i || y != j ))
                    {
                        if(color[i][j] != 0)
                        {
                            mutex.lock();
                            ++score;
                            color[i][j] = 0;
                            brick[i][j]->setStyleSheet("border:0px;background-color:white");
                            mutex.unlock();
                        }
                    }
                }
            }
        }
        else
        {
            mutex.lock();
            ++score;
            color[x][y + upOrDown] = 0;
            brick[x][y + upOrDown]->setStyleSheet("border:0px;background-color:white");
            mutex.unlock();
        }
        upOrDown = -upOrDown;
    }
    if(color[x + leftOrRight][y] != 0)
    {
        if(qrand() % 3 == 0)
        {
            for(int i = x - 1; i <= x + 1; ++i)
            {
                for(int j = y - 1; j <= y + 1; ++j)
                {
                    if(i >= 0 && i < COLUMN && j >= 0 && j < ROW && (x != i || y != j ))
                    {
                        if(color[i][j] != 0)
                        {
                            mutex.lock();
                            ++score;
                            color[i][j] = 0;
                            brick[i][j]->setStyleSheet("border:0px;background-color:white");
                            mutex.unlock();
                        }
                    }
                }
            }
        }
        else
        {
            mutex.lock();
            ++score;
            color[x + leftOrRight][y] = 0;
            brick[x + leftOrRight][y]->setStyleSheet("border:0px;background-color:white");
            mutex.unlock();
        }
        leftOrRight = -leftOrRight;
    }

    if(color[x + leftOrRight][y + upOrDown] != 0)
    {
        if(qrand() % 3 == 0)
        {
            for(int i = x - 1; i <= x + 1; ++i)
            {
                for(int j = y - 1; j <= y + 1; ++j)
                {
                    if(i >= 0 && i < COLUMN && j >= 0 && j < ROW && (x != i || y != j ))
                    {
                        if(color[i][j] != 0)
                        {
                            mutex.lock();
                            ++score;
                            color[i][j] = 0;
                            brick[i][j]->setStyleSheet("border:0px;background-color:white");
                            mutex.unlock();
                        }
                    }
                }
            }
        }
        else
        {
            mutex.lock();
            ++score;
            color[x + leftOrRight][y + upOrDown] = 0;
            brick[x + leftOrRight][y + upOrDown]->setStyleSheet("border:0px;background-color:white");
            mutex.unlock();
        }
        upOrDown = -upOrDown;
        leftOrRight = -leftOrRight;
    }

    if(x == 0)
        leftOrRight = 1;
    if(x == COLUMN -1)
        leftOrRight = -1;
    if(y == 0)
        upOrDown = 1;
    if(y == ROW - 2 && (x + leftOrRight >= barLeft) && (x + leftOrRight <= barRight))
        upOrDown = -1;

    //更新坐标
    mutex.lock();
    brick[x][y]->setStyleSheet("border:0px;background-color:white");
    x += leftOrRight;
    y += upOrDown;
    brick[x][y]->setStyleSheet("border:0px;background-color:blue");
    mutex.unlock();
    //更新分数
    ui->scoreValueLabel->setText(QString::number(score));
}

void BreakTheBricks::gameOver()
{
    QMessageBox::information(this,"Game Over","你真的很弱诶");
}

void BreakTheBricks::onPause()
{
    pause = !pause;
}
