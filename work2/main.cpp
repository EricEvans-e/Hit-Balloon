#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include <graphics.h>      //easyX图形库
#include<time.h>


#pragma comment(lib,"winmm.lib")   //

#define NUM 15


int x, y;    //炮口坐标


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
    srand((unsigned int)time(NULL));            //种下随机数种子。 固定那些地方产生哪些颜色
    void CreatBalloon();
    void gamepaint();
    void GameInit();


    initgraph(720, 480);    //创建窗口（宽720，高480）




    while (1)
    {
        gamepaint();
    }

    getchar();
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

    circle(320, 460, 60);    //圆心（320，460）  r=60

    setlinecolor(BLACK);   //线条颜色
    setfillcolor(BLACK);   //填充颜色
    fillcircle(320, 460, 10);  //炮台（小圆）

    setlinestyle(PS_SOLID, 8);   //(实线，像素8）
    line(x, y, 320, 460);    //炮口

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


    EndBatchDraw(); //结束绘制
}

//游戏的初始化函数
void GameInit()
{

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