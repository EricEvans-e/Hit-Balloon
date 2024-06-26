#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include <graphics.h>      //easyX图形库


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
    void gamepaint();

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



    EndBatchDraw(); //结束绘制
}

