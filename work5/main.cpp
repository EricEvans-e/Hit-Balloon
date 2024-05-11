#include<stdio.h>
#include<math.h>
#include<time.h>
#include<stdlib.h>
#include<Windows.h>      //双缓冲
#include <graphics.h>      //easyX图形库
#include <process.h>        //使用进程处理函数需要用到该头文件，包含过程控制的函数(给鼠标开辟线程会用到)
#include <conio.h>

#include<mmsystem.h>               //这两个是载入音乐的头文件
#pragma comment(lib,"winmm.lib")   //
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

#define NUM 15


int x, y;    //炮口坐标
DWORD t1, t2, tt1, tt2;   //给一个时间限定
int score = 0;
char strscore[5];     //设置存储分数的数组 0-9999
MOUSEMSG msg = { 0 };     //定义鼠标信息    设为全局变量，主函数里开辟新的线程


//子弹的结构（结构体数组）
struct Bullet
{
    int x, y;         //子弹坐标
    double vx, vy;    //速度分量
    int r;            //子弹半径
    int color;        //子弹颜色
    int flag;         //子弹是否存活
}bullet[NUM];

//气球的结构(结构体数组)
struct  Balloon
{
    int x, y;   //气球坐标
    int color;  //气球颜色
    int flag;   //气球是否存在的标识

}balloon[NUM];

int main()
{
    void gamepaint();
    void GameInit();
    void CreateBullet(double a);
    void MoveBullet();
    void CreatBalloon();
    void MoveBalloon();
    void Boom();
    void HideCursor();    //双缓冲 
    void Mouse();


    initgraph(720, 480);    //创建窗口（宽720，高480）
    srand((unsigned int)time(NULL));            //种下随机数种子。 固定那些地方产生哪些颜色

    mciSendString("music.mp3", 0, 0, 0);  //打开音乐
    mciSendString("play music.mp3", 0, 0, 0);    //播放音乐

    GameInit();

    t1 = GetTickCount();   //获取系统时间
    tt1 = GetTickCount();


    _beginthread((_beginthread_proc_type)Mouse, 0, 0);  //Mouse函数开启了一个线程，Mouse是鼠标函数的地址（调用了Mouse函数），相当于main函数和鼠标函数同步执行
    while (1)
    {
        sprintf_s(strscore, "%d", score);   //将分数写入到界面. sprintf不安全，课考虑用sprintf_s

        gamepaint();


        tt2 = GetTickCount();
        if (tt2 - tt1 > 500)
            CreatBalloon();   //产生气球

        t2 = GetTickCount();
        if (t2 - t1 > 20)        //t1-t2>20ms
        {
            MoveBullet();   //子弹移动
            MoveBalloon();  //气球移动
            Boom();         //子弹气球碰撞爆炸
            t1 = t2;
        }
    }
    HideCursor();      //双缓冲函数调用

    exit(0);
    return 0;
}

//游戏绘制函数
void gamepaint()
{
    BeginBatchDraw();      //开始绘制
    setlinestyle(PS_SOLID, 2);  //设置线条样式与宽度 （实线，像素2）
    setlinecolor(WHITE);    //设置线条颜色
    setfillcolor(RGB(177, 173, 175));  //右边菜单栏
    fillrectangle(640, 0, 719, 479);

    IMAGE img;			   //创建IMAGE对象     //左边游戏区
    loadimage(&img, "E:\\c语言\\气球小游戏\\图片素材\\sky.jpg", 640, 479);//绝对地址载入图片  
    putimage(0, 0, &img);


    circle(320, 460, 60);    //圆心（320，460）  r=60

    setlinecolor(BLACK);   //线条颜色
    setfillcolor(BLACK);   //填充颜色
    fillcircle(320, 460, 10);  //炮台（小圆）

    setlinestyle(PS_SOLID, 8);   //(实线，像素8）
    line(x, y, 320, 460);    //炮口

    setbkmode(0);  //透明字体
    outtextxy(650, 100, "Score:");      //要将项目->属性->字符集设置为多节字符集
    outtextxy(650, 150, strscore);     //分数


    //设计气球
    setlinestyle(PS_SOLID, 2);  //（实线，像素2）
    setlinecolor(WHITE);    //设置线条颜色

    for (int i = 0; i < NUM; i++)
    {
        if (balloon[i].flag == 1)
        {
            setfillcolor(balloon[i].color);    //(画图工具里的红绿蓝三色)
            fillellipse(balloon[i].x, balloon[i].y, balloon[i].x + 30, balloon[i].y + 50);   //(圆心（100,100），宽度30，高度50)
            arc(balloon[i].x + 5, balloon[i].y + 5, balloon[i].x + 25, balloon[i].y + 45, 0, 1.2);   //圆弧（（）位置，（）宽度和高度，弧度0—1.2π）
            arc(balloon[i].x + 15, balloon[i].y + 35, balloon[i].x + 35, balloon[i].y + 65, 3.14, 4.4);
        }
    }



    //子弹画出来
    for (int i = 0; i < NUM; i++)
    {
        if (bullet[i].flag == 1)
        {
            setlinecolor(bullet[i].color);   //设置线条颜色
            setfillcolor(bullet[i].color);   //设置填充颜色
            fillcircle(bullet[i].x, bullet[i].y, bullet[i].r);  //子弹色位置和大小
        }
    }

    EndBatchDraw(); //结束绘制
}


//游戏的初始化函数
void GameInit()
{
    //子弹
    int i;
    for (i = 0; i < NUM; i++)
    {
        bullet[i].r = 5;
        bullet[i].color = RGB(rand() % 257, rand() % 257, rand() % 257); //子弹颜色随机
        bullet[i].flag = 0;  //不存货(闲置);
    }

    //气球
    for (int i = 0; i < NUM; i++)   //先给10个气球
    {
        if (i < 10)
        {
            balloon[i].x = rand() % 610;  //气球显示，边界窗口宽-气球宽 640-30
            balloon[i].y = rand() % 430;  //480-50
            balloon[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);
            balloon[i].flag = 1;  //气球存在了
        }
        else     //剩下5个气球
        {
            balloon[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);
            balloon[i].flag = 0;  //气球没存在界面内
        }
    }

}


//产生子弹
void CreateBullet(double a)
{
    //数组
    for (int i = 0; i < NUM; i++)
    {
        if (bullet[i].flag == 0)
        {
            bullet[i].x = x;
            bullet[i].y = y;
            bullet[i].vx = 15 * cos(a);    //15就是发射速度
            bullet[i].vy = 15 * sin(a);
            bullet[i].flag = 1;   //锁住内存，相当于子弹打出去，还没捡回来，没子弹了。
            break;
        }
    }
}


//子弹移动
void MoveBullet()
{
    for (int i = 0; i < NUM; i++)
    {
        if (bullet[i].flag == 1)
        {
            bullet[i].x += bullet[i].vx;
            bullet[i].y -= bullet[i].vy;
        }
    }

    //移动之后，飞出场外   flag=0
    for (int i = 0; i < NUM; i++)
    {
        if (bullet[i].flag == 1)
        {
            if (bullet[i].x < 0 || bullet[i].x>640 || bullet[i].y < 0 || bullet[i].y>480)
            {
                bullet[i].flag = 0;
            }
        }
    }

}


//产生气球
void CreatBalloon()
{
    for (int i = 0; i < NUM; i++)
    {
        if (balloon[i].flag == 0)
        {
            balloon[i].x = rand() % 610;   //新产生x坐标气球随机
            balloon[i].y = 480;          //新产生气球y坐标永远在最底下;
            balloon[i].flag = 1;   //锁住内存，相当于气球在界面内
            break;
        }
    }
}


//气球移动
void MoveBalloon()
{
    //移动
    for (int i = 0; i < NUM; i++)
    {
        if (balloon[i].flag == 1)
        {
            balloon[i].y -= 2;    //往上移动

            if (rand() % 2 == 0)   //气球实现左右移动。  rand()%2 = =0,概率1/2；
                balloon[i].x += 1;
            else
                balloon[i].x -= 1;
        }
    }

    //出界
    for (int i = 0; i < NUM; i++)
    {
        if (balloon[i].flag == 1)
        {
            if (balloon[i].x + 30 < 0 || balloon[i].x > 640 || balloon[i].y + 50 < 0)  //下边界不算，因为总会飘上来
            {
                balloon[i].flag = 0;
            }
        }
    }
}


//打气球，爆炸
void Boom()
{
    for (int i = 0; i < NUM; i++)   //遍历气球
    {
        if (balloon[i].flag == 1)   //选在界面里边的气球
        {
            for (int j = 0; j < NUM; j++)   //遍历子弹  
            {
                if (bullet[j].flag == 1)
                {
                    if (bullet[j].x > balloon[i].x && bullet[j].x<balloon[i].x + 30 && bullet[j].y>balloon[i].y && bullet[j].y < balloon[i].x + 50)  //判断子弹是否与气球重合
                    {
                        bullet[j].flag = 0;
                        balloon[i].flag = 0;
                        score += 10;
                    }
                }
            }
        }
    }
}

void Mouse()
{
    while (1)
    {
        if (MouseHit())    //如果有鼠标信息
        {
            msg = GetMouseMsg();   //获取鼠标信息到msg
            double a = atan2(460.0 - msg.y, msg.x - 320);
            x = 320 + 70 * cos(a);
            y = 460 - 70 * sin(a);

            if (msg.uMsg == WM_LBUTTONDOWN)   //点击鼠标左键
            {
                CreateBullet(a);
            }
        }
    }
}



//双缓冲
void HideCursor()
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(handle, &CursorInfo);//获取控制台光标信息
    CursorInfo.bVisible = false; //隐藏控制台光标
    SetConsoleCursorInfo(handle, &CursorInfo);//设置控制台光标状态
}


