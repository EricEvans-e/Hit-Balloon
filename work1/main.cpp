#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include <graphics.h>      //easyXͼ�ο�


#pragma comment(lib,"winmm.lib")   //

#define NUM 15


int x, y;    //�ڿ�����


//�ӵ��Ľṹ���ṹ�����飩
struct Bullet
{
    int x, y;         //�ӵ�����
    double vx, vy;    //�ٶȷ���
    int r;            //�ӵ��뾶
    int color;        //�ӵ���ɫ
    int flag;         //�ӵ��Ƿ���
}bullet[NUM];

//����Ľṹ(�ṹ������)
struct  Balloon
{
    int x, y;   //��������
    int color;  //������ɫ
    int flag;   //�����Ƿ���ڵı�ʶ

}balloon[NUM];

int main()
{
    void gamepaint();

    initgraph(720, 480);    //�������ڣ���720����480��




    while (1)
    {
        gamepaint();
    }

    getchar();
    return 0;
}

//��Ϸ���ƺ���
void gamepaint()
{
    BeginBatchDraw();      //��ʼ����
    setlinestyle(PS_SOLID, 2);  //����������ʽ���� ��ʵ�ߣ�����2��
    setlinecolor(WHITE);    //����������ɫ
    setfillcolor(RGB(177, 173, 175));  //�ұ߲˵���
    fillrectangle(640, 0, 719, 479);

    circle(320, 460, 60);    //Բ�ģ�320��460��  r=60

    setlinecolor(BLACK);   //������ɫ
    setfillcolor(BLACK);   //�����ɫ
    fillcircle(320, 460, 10);  //��̨��СԲ��

    setlinestyle(PS_SOLID, 8);   //(ʵ�ߣ�����8��
    line(x, y, 320, 460);    //�ڿ�



    EndBatchDraw(); //��������
}

