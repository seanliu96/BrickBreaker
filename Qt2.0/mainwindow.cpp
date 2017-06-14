#include "mainwindow.h"

#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QLineEdit>
#include <QtDebug>
#include <QDateTime>

QPushButton *beginGameBtn;
QPushButton *quitGameBtn;
QPushButton *pauseGameBtn;
QPushButton *aboutBtn;
QGridLayout *mainLayout;
QComboBox *chooseDifficultyComboBox;
QLabel *scoreLabel;
QLabel *scoreValueLabel;
QTimer timer;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Block Breaker "));
    setFixedSize(640, 480);
    game = new Game(500, 440);
    scoreLabel = new QLabel(tr("得分"));
    scoreValueLabel = new QLabel(QString::number(game->getScore()));
    scoreLabel->setMaximumHeight(30);
    scoreValueLabel->setMaximumHeight(30);
    chooseDifficultyComboBox = new QComboBox;
    chooseDifficultyComboBox->addItem(tr("简单"));
    chooseDifficultyComboBox->addItem(tr("一般"));
    chooseDifficultyComboBox->addItem(tr("困难"));
    QLineEdit *lineEdit = new QLineEdit;
    lineEdit->setReadOnly(true);
    lineEdit->setAlignment(Qt::AlignCenter);
    chooseDifficultyComboBox->setLineEdit(lineEdit);

    beginGameBtn = new QPushButton(QIcon("resources/beginGame.ico"),tr("开始游戏"));
    pauseGameBtn = new QPushButton(QIcon("resources/pauseGame1.ico"),tr("暂停游戏"));
    quitGameBtn = new QPushButton(QIcon("resources/quitGame.ico"),tr("退出游戏"));
    aboutBtn = new QPushButton(QIcon("resources/about.ico"),tr("关于游戏"));

    timer = new QTimer(this);
    timer->setInterval(10);
    qsrand(QDateTime::currentDateTime().toTime_t());


    QWidget *mainWidget = new QWidget();
    mainWidget->setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap("resources/background.jpg");
    palette.setBrush(QPalette::Window, QBrush(pixmap));
    mainWidget->setPalette(palette);

    this->setCentralWidget(mainWidget);
    QVBoxLayout *rightLayout = new QVBoxLayout();
    QHBoxLayout *scoreLayout = new QHBoxLayout();
    scoreLayout->addWidget(scoreLabel);
    scoreLayout->addWidget(scoreValueLabel);
    rightLayout->addLayout(scoreLayout);
    rightLayout->addWidget(chooseDifficultyComboBox);
    rightLayout->addWidget(beginGameBtn);
    rightLayout->addWidget(pauseGameBtn);
    rightLayout->addWidget(quitGameBtn);
    rightLayout->addWidget(aboutBtn);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addWidget(game);
    mainLayout->addLayout(rightLayout);
    mainWidget->setLayout(mainLayout);

    isBegin = 0;
    isPause = 0;


    connect(beginGameBtn, SIGNAL(clicked(bool)), this, SLOT(on_begin()));
    connect(pauseGameBtn, SIGNAL(clicked(bool)), this, SLOT(on_pause()));
    connect(quitGameBtn, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(aboutBtn, SIGNAL(clicked(bool)), this, SLOT(on_about()));
    connect(timer, SIGNAL(timeout()), this, SLOT(on_update()));
}


MainWindow::~MainWindow()
{
    delete beginGameBtn;
    delete quitGameBtn;
    delete pauseGameBtn;
    delete aboutBtn;
    delete chooseDifficultyComboBox;
    delete scoreLabel;
    delete scoreValueLabel;
    delete timer;
    delete game;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<isPause;
    qreal barV = game->bar->getV();
    switch (event->key())
    {
    case Qt::Key_Escape:
        on_pause();
        return;
    case Qt::Key_A:
    case Qt::Key_Left:
    case Qt::Key_J:
        if(isPause)
            return;
        qDebug()<<"left";
        barV -= 10;
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
    case Qt::Key_L:

        if(isPause)
            return;
        qDebug()<<"right";
        barV += 10;
        break;
    }
    game->bar->setV(barV);
}

void MainWindow::on_begin()
{
    switch (chooseDifficultyComboBox->currentIndex())
    {
    case 0:
        this->game->setG(0.005);
        break;
    case 1:
        this->game->setG(0.010);
        break;
    case 2:
        this->game->setG(0.020);
        break;
    }
    this->game->initial();
    isBegin = 1;
    if(isPause)
        on_pause();
    timer->start();
}

void MainWindow::on_pause()
{
    if(!isBegin)
        return;
    isPause = !isPause;
    if(isPause)
    {
        pauseGameBtn->setIcon(QIcon("resources/pauseGame2.ico"));
        timer->stop();
    }
    else
    {
        pauseGameBtn->setIcon(QIcon("resources/pauseGame1.ico"));
        timer->start();
    }

}

void MainWindow::on_about()
{
    QString aboutStr = tr("只是为了怀念第一个手机游戏而已\n");
    aboutStr += tr("作者： 刘昕\n");
    aboutStr += tr("联系方式： liux223@mail2.sysu.edu.cn\n\n");
    aboutStr += tr("使用说明：\n");
    aboutStr += tr("resources文件夹放置图片素材，用户可以自己更改\n");
    aboutStr += tr("maps文件夹放置地图，程序使用map.txt来生成砖块地图\n");
    aboutStr += tr("使用A和J来控制滑块向左移动\n");
    aboutStr += tr("使用D和L来控制滑块向右移动\n");
    aboutStr += tr("保证不会小球不会掉落地面，消灭全部砖块！\n");
    QMessageBox::about(this, tr("About"), aboutStr);
}

void MainWindow::on_win()
{
    timer->stop();
    QMessageBox winMessage(QMessageBox::NoIcon,tr("很强"),tr(""));
    winMessage.setIconPixmap(QPixmap("resources/ball.png"));
    winMessage.exec();
}

void MainWindow::on_failed()
{
    timer->stop();
    QMessageBox failedMessage(QMessageBox::NoIcon,tr("继续努力吧"),tr(""));
    failedMessage.setIconPixmap(QPixmap("resources/bar.png"));
    failedMessage.exec();
}

void MainWindow::on_update()
{
    if(game->getIsWin())
        on_win();
    else if(game->getIsFailed())
        on_failed();
    else
        game->updateGame();
    this->scoreValueLabel->setText(QString::number(game->getScore()));
}
