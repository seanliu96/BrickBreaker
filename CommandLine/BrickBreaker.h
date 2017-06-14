const static int ROW = 24, COLUMN = 20;

bool pause = 0;


class Bar
{
public:
    Bar();
    //移动滑块
    void moveBar();
    //显示滑块
    void showBar();
    //得到滑块左边的坐标
    int getBarLeft();
    //得到滑块右边的坐标
    int getBarRight();
private:
    int barLeft, barRight;
};


class Bricks
{
public:
    Bricks(int level);
    //设置时间间隙
    int timeGap;
    //更新
    void update(int barLeft, int barRight);
    //显示
    void show();
    //是否挂了
    bool isDead();
    //是否结束
    bool isOver();
private:
    int x, y;
    int leftOrRight, upOrDown;
    int color[COLUMN][ROW];
    int score;

};