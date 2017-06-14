#ifndef BREAKTHEBRICKS_H
#define BREAKTHEBRICKS_H

#include <QMainWindow>
#include <QLabel>
#include <QMutex>

const int ROW = 26, COLUMN = 20;

namespace Ui {
class BreakTheBricks;
}

class BreakTheBricks : public QMainWindow
{
    Q_OBJECT

public:
    explicit BreakTheBricks(QWidget *parent = 0);
    ~BreakTheBricks();
protected:
    void keyPressEvent(QKeyEvent *event);
private:
    Ui::BreakTheBricks *ui;
    QLabel *brick[COLUMN][ROW];
    QString colorStr[16]={"white", "aqua", "black", "blue", "fuchsia", "gray", "green", "lime", "maroon", "navy", "olive", "purple", "red", "silver", "teal", "yellow"};
    QMutex mutex;
    int difficulty;
    int barLeft;
    int barRight;
    int leftOrRight;
    int upOrDown;
    int x, y;
    bool pause;
    int score;
    int color[COLUMN][ROW];
    void drawBricks();
    bool isDead();
    bool isOver();
    void update();
    void gameOver();
private slots:
    void onBeginGame();
    void onPause();
};

#endif // BREAKTHEBRICKS_H
