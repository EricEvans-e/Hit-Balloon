#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include <graphics.h>      //easyXͼ�ο�
#include<time.h>


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
    srand((unsigned int)time(NULL));            //������������ӡ� �̶���Щ�ط�������Щ��ɫ
    void CreatBalloon();
    void gamepaint();
    void GameInit();


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

    //�������
    setlinestyle(PS_SOLID, 2);  //��ʵ�ߣ�����2��
    setlinecolor(WHITE);    //����������ɫ

    for (int i = 0; i < NUM; i++)
    {
        if (balloon[i].flag == 1)
        {
            setfillcolor(balloon[i].color);    //(��ͼ������ĺ�������ɫ)
            fillellipse(balloon[i].x, balloon[i].y, balloon[i].x + 30, balloon[i].y + 50);   //(Բ�ģ�100,100�������30���߶�50)
            arc(balloon[i].x + 5, balloon[i].y + 5, balloon[i].x + 25, balloon[i].y + 45, 0, 1.2);   //Բ��������λ�ã�������Ⱥ͸߶ȣ�����0��1.2�У�
            arc(balloon[i].x + 15, balloon[i].y + 35, balloon[i].x + 35, balloon[i].y + 65, 3.14, 4.4);
        }
    }


    EndBatchDraw(); //��������
}

//��Ϸ�ĳ�ʼ������
void GameInit()
{

    //����
    for (int i = 0; i < NUM; i++)   //�ȸ�10������
    {
        if (i < 10)
        {
            balloon[i].x = rand() % 610;  //������ʾ���߽細�ڿ�-����� 640-30
            balloon[i].y = rand() % 430;  //480-50
            balloon[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);
            balloon[i].flag = 1;  //���������
        }
        else     //ʣ��5������
        {
            balloon[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);
            balloon[i].flag = 0;  //����û���ڽ�����
        }
    }

}


//��������
void CreatBalloon()
{
    for (int i = 0; i < NUM; i++)
    {
        if (balloon[i].flag == 0)
        {
            balloon[i].x = rand() % 610;   //�²���x�����������
            balloon[i].y = 480;          //�²�������y������Զ�������;
            balloon[i].flag = 1;   //��ס�ڴ棬�൱�������ڽ�����
            break;
        }
    }
}