#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QKeyEvent>

#include "game.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void keyPressEvent(QKeyEvent *event);
private slots:
    void on_begin();
    void on_pause();
    void on_about();
    void on_win();
    void on_failed();
    void on_update();
private:
    QPushButton *beginGameBtn;
    QPushButton *quitGameBtn;
    QPushButton *pauseGameBtn;
    QPushButton *aboutBtn;
    QComboBox *chooseDifficultyComboBox;
    QLabel *scoreLabel;
    QLabel *scoreValueLabel;
    QTimer *timer;
    Game *game;
    bool isBegin;
    bool isPause;
};

#endif // MAINWINDOW_H
