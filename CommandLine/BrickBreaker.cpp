#include "BrickBreaker.h"

#include <windows.h>
#include <process.h>
#include <conio.h>
#include <cstdio>
#include <ctime>
#include <cstdlib>

//����̨������
HANDLE handleConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

//�ٽ��
CRITICAL_SECTION cs;

bool over = 0;
//���ÿ���̨����
void goToXY(int x, int y);
//��ʼ��Ϸ
void gameBegin();
//��Ϸ����
void gameOver();

//�����߳�
unsigned int __stdcall _BarThread(void* args)
{
    Bar *bar = (Bar*) args;
    bar->showBar();
    bar->moveBar();
    _endthreadex(0);
}

int main()
{
    srand(time(0));
    gameBegin();
}

//���ÿ���̨����
void goToXY(int x, int y)
{
    COORD coord={(x<<1) + 30, y};
    SetConsoleCursorPosition(handleConsoleOutput, coord);
}

//���캯��
Bar::Bar()
{
    barLeft = COLUMN / 2 - 3;
    barRight = COLUMN / 2 + 3;
}

//�ƶ�����
void Bar::moveBar()
{
    int ch;
    //�������
    while (ch = _getch())
    {
        if(over)
            break;
        //��ͣ
        if(ch ==32)
        {
            pause = !pause;
        }
        if(pause)
            continue;
        //����
        switch(ch)
        {
            case 'A':
            case 'a':
            case 75:
                if(barLeft > 0)
                {
                    --barLeft;
                    --barRight;
                }
                break;
            case 'D':
            case 'd':
            case 77:
                if(barRight < COLUMN - 1)
                {
                    ++barLeft;
                    ++barRight;
                }
                break;
            break;
        }
        //���»���
        EnterCriticalSection(&cs);
        if(barLeft > 0)
        {
            goToXY(barLeft - 1, ROW -1);
            SetConsoleTextAttribute(handleConsoleOutput, 0);
            printf("%2s","");
        }
        else
            goToXY(barLeft , ROW -1);
        SetConsoleTextAttribute(handleConsoleOutput, 0x0F);
        printf("��");
        goToXY(barRight, ROW -1);
        SetConsoleTextAttribute(handleConsoleOutput, 0x0F);
        printf("��");
        if(barRight < COLUMN -1)
        {
            SetConsoleTextAttribute(handleConsoleOutput, 0);
            printf("%2s","");
        }
        LeaveCriticalSection(&cs); 
    }
}


//��ʾ����
void Bar::showBar()
{
    EnterCriticalSection(&cs);
    goToXY(barLeft , ROW -1);
    for(int i = barLeft; i <= barRight; ++i)
    {
        SetConsoleTextAttribute(handleConsoleOutput, 0x0F);
        printf("��");
    }
    LeaveCriticalSection(&cs);
}

//�õ������������
int Bar::getBarLeft()
{
    return barLeft;
}

//�õ������ұ�����
int Bar::getBarRight()
{
    return barRight;
}

//���캯��
Bricks::Bricks(int level)
{
    CONSOLE_CURSOR_INFO cursorInfo = { 1, FALSE };
    SetConsoleCursorInfo(handleConsoleOutput, &cursorInfo);
    SetConsoleTitleA("Break The Bricks");
    timeGap = 300 / level ;
    upOrDown = -1;
    leftOrRight = rand() % 2 * 2 -1;
    score = 0;
    pause = false;
    x = COLUMN / 2;
    y = ROW - 2;

    for(int i = 0; i < COLUMN; ++i)
    {
        int j = 0;
        for(; j < ROW / 2; ++j)
            color[i][j] = rand() % ((1<<8));
        for(; j < ROW ; ++j)
            color[i][j] = 0;
    }

}

//��Ϸ��ʼ
void gameBegin()
{
    InitializeCriticalSection(&cs);
    SetConsoleTextAttribute(handleConsoleOutput, 0x07);
    system("cls");
    int level = 5;
    EnterCriticalSection(&cs);
    goToXY(COLUMN / 2 - 3, ROW / 4 * 3);
    printf("Break The Bricks\n");
    goToXY(COLUMN / 2 - 10, ROW / 4 * 3 + 1);
    printf("Enter \"1~9\"(easy~hard) to select difficulty.\n");
    goToXY(COLUMN / 2 - 8, ROW / 4 * 3 + 2);
    printf("Enter any other key to exit.\n");
    LeaveCriticalSection(&cs);
    level = _getch();
    if(level >= '0' && level <= '9')
        level = level -'0';
    else
        exit(0);
    
    system("cls");
    unsigned int threadID=0;
    Bar *bar = new Bar;
    over = 0;
    Bricks bricks(level);
    bricks.show();

    //�����̣߳����ⲻ��ͬʱ���
    HANDLE barThread=(HANDLE)_beginthreadex(NULL,0,_BarThread,bar,0,&threadID);
    while(!bricks.isDead() && !bricks.isOver())
    {
        Sleep(bricks.timeGap);
        if(!pause)
        {
            bricks.update(bar->getBarLeft(), bar->getBarRight());
        }
    }

    //��Ϸ����
    delete bar;
    over = 1;
    gameOver();
}

//��Ϸ����
void gameOver()
{
    int ch;
    EnterCriticalSection(&cs);
    goToXY(COLUMN / 2 - 3, ROW / 4 * 3);
    printf("GameOver\n");
    goToXY(COLUMN / 2 - 8, ROW / 4 * 3 + 1);
    printf("Enter \"Enter\" to continue.\n");
    goToXY(COLUMN / 2 - 8, ROW / 4 * 3 + 2);
    printf("Enter any other key to exit.\n");
    goToXY(COLUMN / 2 - 8, ROW / 4 * 3 + 3);
    printf("Hint: Press the spacebar to pause.\n");
    
    LeaveCriticalSection(&cs);
    ch = _getch();
    //������Ϸ
    if(ch == 13)
        gameBegin();
    
}

//��ʾש��
void Bricks::show()
{
    EnterCriticalSection(&cs);
    for(int j = 0;j <ROW-1; ++j)
    {
        goToXY(0, j);
        for(int i = 0; i < COLUMN; ++i)
        {
            SetConsoleTextAttribute(handleConsoleOutput, color[i][j]);
            printf("��");
        }
    }
    LeaveCriticalSection(&cs);

}

//����ש��
void Bricks::update(int barLeft, int barRight)
{
    //����ǰλ����ɫ��Ϊ��ɫ
    EnterCriticalSection(&cs);
    color[x][y] = 0;
    goToXY(x, y);
    SetConsoleTextAttribute(handleConsoleOutput, 0);
    printf("%2s","");
    LeaveCriticalSection(&cs);
    
    //�ı䷽��
    if(x == 0)
        leftOrRight = 1;
    if(y == 0)
        upOrDown = 1;
    if(x == COLUMN -1)
        leftOrRight = -1;
     if(y == ROW-2 && (x + leftOrRight >= barLeft && x + leftOrRight <= barRight))
        upOrDown = -1;

    //��������
    if(color[x][y + upOrDown] !=0 || color[x + leftOrRight][y]!= 0)
    {
        //�������ҵķ���
        if(color[x][y + upOrDown] != 0)
        {
            //���������ը
            if(rand() % 6 == 0)
            {
                for(int i = x - 1; i <= x + 1; ++i)
                {
                    for(int j = y + upOrDown - 1; j <= y + upOrDown + 1; ++j)
                    {
                        if(i >= 0 && i < COLUMN && j >= 0 && j < ROW)
                        {
                            if(color[i][j])
                            {
                                ++score;
                                color[i][j] = 0;
                                EnterCriticalSection(&cs);
                                goToXY(i, j);
                                SetConsoleTextAttribute(handleConsoleOutput, 0);
                                printf("%2s","");
                                LeaveCriticalSection(&cs);
                            }
                        }
                    }
                }
            }
            else
            {     
                ++score;
                color[x][y + upOrDown] = 0;
                EnterCriticalSection(&cs);
                goToXY(x, y + upOrDown);
                SetConsoleTextAttribute(handleConsoleOutput, 0);
                printf("%2s","");
                LeaveCriticalSection(&cs);
            }
            upOrDown = -upOrDown;
        }
        //�������µ�ש��
        if(color[x + leftOrRight][y] != 0)
        {
            if(rand() % 6 == 0)
            {
                for(int i = x + leftOrRight - 1; i <= x + leftOrRight + 1; ++i)
                {
                    for(int j = y - 1; j <= y + 1; ++j)
                    {
                        if(i >= 0 && i < COLUMN && j >= 0 && j < ROW)
                        {
                            if(color[i][j])
                            {
                                ++score;
                                color[i][j] = 0;
                                EnterCriticalSection(&cs);
                                goToXY(i, j);
                                SetConsoleTextAttribute(handleConsoleOutput, 0);
                                printf("%2s","");
                                LeaveCriticalSection(&cs);
                            }
                        }
                    }
                }
            }
            else
            {     
                ++score;
                color[x + leftOrRight][y] = 0;
                EnterCriticalSection(&cs);
                goToXY(x + leftOrRight, y);
                SetConsoleTextAttribute(handleConsoleOutput, 0);
                printf("%2s","");
                LeaveCriticalSection(&cs);
            }
            leftOrRight = -leftOrRight;
        }
    }
    //�����Խǵ�ש��
    if(color[x + leftOrRight][y + upOrDown] != 0)
    {
        if(rand() % 6 == 0)
        {
            for(int i = x + leftOrRight - 1; i <= x + leftOrRight + 1; ++i)
            {
                for(int j = y + upOrDown - 1; j <= y + upOrDown + 1; ++j)
                {
                    if(i >= 0 && i < COLUMN && j >= 0 && j < ROW)
                    {
                        if(color[i][j])
                        {
                            ++score;
                            color[i][j] = 0;
                            EnterCriticalSection(&cs);
                            goToXY(i, j);
                            SetConsoleTextAttribute(handleConsoleOutput, 0);
                            printf("%2s","");
                            LeaveCriticalSection(&cs);
                        }
                    }
                }
            }
        }
        else
        {     
            ++score;
            color[x + leftOrRight][y + upOrDown] = 0;
            EnterCriticalSection(&cs);
            goToXY(x + leftOrRight, y + upOrDown);
            SetConsoleTextAttribute(handleConsoleOutput, 0);
            printf("%2s","");
            LeaveCriticalSection(&cs);
        }
        upOrDown = -upOrDown;
        leftOrRight = -leftOrRight;
    }

    //��������
    x += leftOrRight;
    y += upOrDown;
    //��ʾש��
    color[x][y] = 0xF0;
    
    EnterCriticalSection(&cs);
    goToXY(x, y);
    SetConsoleTextAttribute(handleConsoleOutput, color[x][y]);
    printf("��");
    goToXY(COLUMN, ROW);
    SetConsoleTextAttribute(handleConsoleOutput, 0x0F);
    printf("score: %d", score);
    LeaveCriticalSection(&cs);
}

//�ж��Ƿ�����
bool Bricks::isDead()
{
    if(y == ROW - 1)
        return true;
    return false;
}

//ש���Ƿ�ȫ���������
bool Bricks::isOver()
{
    return (score == COLUMN * ROW / 2);
}