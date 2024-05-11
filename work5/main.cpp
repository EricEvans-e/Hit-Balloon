#include<stdio.h>
#include<math.h>
#include<time.h>
#include<stdlib.h>
#include<Windows.h>      //˫����
#include <graphics.h>      //easyXͼ�ο�
#include <process.h>        //ʹ�ý��̴�������Ҫ�õ���ͷ�ļ����������̿��Ƶĺ���(����꿪���̻߳��õ�)
#include <conio.h>

#include<mmsystem.h>               //���������������ֵ�ͷ�ļ�
#pragma comment(lib,"winmm.lib")   //
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

#define NUM 15


int x, y;    //�ڿ�����
DWORD t1, t2, tt1, tt2;   //��һ��ʱ���޶�
int score = 0;
char strscore[5];     //���ô洢���������� 0-9999
MOUSEMSG msg = { 0 };     //���������Ϣ    ��Ϊȫ�ֱ������������￪���µ��߳�


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
    void GameInit();
    void CreateBullet(double a);
    void MoveBullet();
    void CreatBalloon();
    void MoveBalloon();
    void Boom();
    void HideCursor();    //˫���� 
    void Mouse();


    initgraph(720, 480);    //�������ڣ���720����480��
    srand((unsigned int)time(NULL));            //������������ӡ� �̶���Щ�ط�������Щ��ɫ

    mciSendString("music.mp3", 0, 0, 0);  //������
    mciSendString("play music.mp3", 0, 0, 0);    //��������

    GameInit();

    t1 = GetTickCount();   //��ȡϵͳʱ��
    tt1 = GetTickCount();


    _beginthread((_beginthread_proc_type)Mouse, 0, 0);  //Mouse����������һ���̣߳�Mouse����꺯���ĵ�ַ��������Mouse���������൱��main��������꺯��ͬ��ִ��
    while (1)
    {
        sprintf_s(strscore, "%d", score);   //������д�뵽����. sprintf����ȫ���ο�����sprintf_s

        gamepaint();


        tt2 = GetTickCount();
        if (tt2 - tt1 > 500)
            CreatBalloon();   //��������

        t2 = GetTickCount();
        if (t2 - t1 > 20)        //t1-t2>20ms
        {
            MoveBullet();   //�ӵ��ƶ�
            MoveBalloon();  //�����ƶ�
            Boom();         //�ӵ�������ײ��ը
            t1 = t2;
        }
    }
    HideCursor();      //˫���庯������

    exit(0);
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

    IMAGE img;			   //����IMAGE����     //�����Ϸ��
    loadimage(&img, "E:\\c����\\����С��Ϸ\\ͼƬ�ز�\\sky.jpg", 640, 479);//���Ե�ַ����ͼƬ  
    putimage(0, 0, &img);


    circle(320, 460, 60);    //Բ�ģ�320��460��  r=60

    setlinecolor(BLACK);   //������ɫ
    setfillcolor(BLACK);   //�����ɫ
    fillcircle(320, 460, 10);  //��̨��СԲ��

    setlinestyle(PS_SOLID, 8);   //(ʵ�ߣ�����8��
    line(x, y, 320, 460);    //�ڿ�

    setbkmode(0);  //͸������
    outtextxy(650, 100, "Score:");      //Ҫ����Ŀ->����->�ַ�������Ϊ����ַ���
    outtextxy(650, 150, strscore);     //����


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



    //�ӵ�������
    for (int i = 0; i < NUM; i++)
    {
        if (bullet[i].flag == 1)
        {
            setlinecolor(bullet[i].color);   //����������ɫ
            setfillcolor(bullet[i].color);   //���������ɫ
            fillcircle(bullet[i].x, bullet[i].y, bullet[i].r);  //�ӵ�ɫλ�úʹ�С
        }
    }

    EndBatchDraw(); //��������
}


//��Ϸ�ĳ�ʼ������
void GameInit()
{
    //�ӵ�
    int i;
    for (i = 0; i < NUM; i++)
    {
        bullet[i].r = 5;
        bullet[i].color = RGB(rand() % 257, rand() % 257, rand() % 257); //�ӵ���ɫ���
        bullet[i].flag = 0;  //�����(����);
    }

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


//�����ӵ�
void CreateBullet(double a)
{
    //����
    for (int i = 0; i < NUM; i++)
    {
        if (bullet[i].flag == 0)
        {
            bullet[i].x = x;
            bullet[i].y = y;
            bullet[i].vx = 15 * cos(a);    //15���Ƿ����ٶ�
            bullet[i].vy = 15 * sin(a);
            bullet[i].flag = 1;   //��ס�ڴ棬�൱���ӵ����ȥ����û�������û�ӵ��ˡ�
            break;
        }
    }
}


//�ӵ��ƶ�
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

    //�ƶ�֮�󣬷ɳ�����   flag=0
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


//�����ƶ�
void MoveBalloon()
{
    //�ƶ�
    for (int i = 0; i < NUM; i++)
    {
        if (balloon[i].flag == 1)
        {
            balloon[i].y -= 2;    //�����ƶ�

            if (rand() % 2 == 0)   //����ʵ�������ƶ���  rand()%2 = =0,����1/2��
                balloon[i].x += 1;
            else
                balloon[i].x -= 1;
        }
    }

    //����
    for (int i = 0; i < NUM; i++)
    {
        if (balloon[i].flag == 1)
        {
            if (balloon[i].x + 30 < 0 || balloon[i].x > 640 || balloon[i].y + 50 < 0)  //�±߽粻�㣬��Ϊ�ܻ�Ʈ����
            {
                balloon[i].flag = 0;
            }
        }
    }
}


//�����򣬱�ը
void Boom()
{
    for (int i = 0; i < NUM; i++)   //��������
    {
        if (balloon[i].flag == 1)   //ѡ�ڽ�����ߵ�����
        {
            for (int j = 0; j < NUM; j++)   //�����ӵ�  
            {
                if (bullet[j].flag == 1)
                {
                    if (bullet[j].x > balloon[i].x && bullet[j].x<balloon[i].x + 30 && bullet[j].y>balloon[i].y && bullet[j].y < balloon[i].x + 50)  //�ж��ӵ��Ƿ��������غ�
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
        if (MouseHit())    //����������Ϣ
        {
            msg = GetMouseMsg();   //��ȡ�����Ϣ��msg
            double a = atan2(460.0 - msg.y, msg.x - 320);
            x = 320 + 70 * cos(a);
            y = 460 - 70 * sin(a);

            if (msg.uMsg == WM_LBUTTONDOWN)   //���������
            {
                CreateBullet(a);
            }
        }
    }
}



//˫����
void HideCursor()
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(handle, &CursorInfo);//��ȡ����̨�����Ϣ
    CursorInfo.bVisible = false; //���ؿ���̨���
    SetConsoleCursorInfo(handle, &CursorInfo);//���ÿ���̨���״̬
}


