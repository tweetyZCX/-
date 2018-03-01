#include<graphics.h>
#include<conio.h>
#include<time.h>
#include <stdio.h>

COLORREF colorArray[6] = {
	RGB(200,0,0),RGB(0,200,0),RGB(0,0,200),RGB(200,200,0),RGB(200,0,200),RGB(0,200,200)
};

typedef struct position
{
	int x;
	int y;
}posType;

posType currCursor;//��ǰ����λ��

posType ballsArray[180];//���ͬɫС������

int ballsNumber = 0;//ͬɫС�����

int score = 0;//��ʼ����Ϊ��

//�꣬������¼��ˣ���������ż����������ֵ��������keydown����һ�������������һ�����ĺ���
#define KEY_DOWN(vk_c) (GetAsyncKeyState(vk_c)&0x8000)
				  
void init(void);//��ʼ������
void gamebegain(void);//��Ϸ��ʼ����
void gameplay(void);//����Ϸ�������
void close(void);//�ͷ���Դ
void drawcursor(posType , COLORREF );//���ƹ��
void drawtime(int);//�ػ�ʱ���ַ���
void drawscore(int );//���Ʒ���
void getsamecolorballs(posType ,COLORREF);//��ȡͬɫС�򣬲������Ǽӵ��������棬ͬʱ����ͬɫС�����
int isok(posType, COLORREF);//�ж��µ�С���Ƿ�Ϸ�
void drawwarnning(posType);//���ƾ������
void ballsfall();//С������
//void checkblackballs();//���δ���ϵĺڶ���
void sort();//��С������������С����ǰ��

int main()
{
	init();
	gamebegain();
	gameplay();
	close();
	return 0;
}

void init(void)
{
	initgraph(1000, 700);
}

void gamebegain(void)
{
	//���Ʊ߿�
	setlinecolor(RGB(50, 50, 50));//���ñ߿���ɫΪ��ɫ
	setlinestyle(PS_SOLID/*ʵ��*/,10/*������ϸ*/);//���ñ߿���ʽ
	rectangle(255, 45, 745, 655);
	//����180��С��
	
	setlinestyle(PS_SOLID, 1);
	
	srand((unsigned)time(NULL));//�ṩ���������
	for (int x = 280; x < 740; x += 40)
	{
		for (int y = 70; y <= 630; y += 40)
		{
			int i=rand() % 6;//�ṩ0~5�������
			setlinecolor(colorArray[i]);
			setfillcolor(colorArray[i]);
			fillcircle(x, y, 18);
		}
	}
	//���ƹ��
	currCursor.x = 480;
	currCursor.y = 390;
	drawcursor(currCursor, WHITE);
	//����ʱ��
	drawtime(30);
	//���Ʒ���
	drawscore(0);
}

void gameplay(void)
{
	//���ϵȴ��������û�������holdס��
	for (int i = 299 ; i >= 0 ;i--)
	{
		if (i % 10 ==0)
		{
			drawtime(i / 10);

		}
		//����������ҷ��������ʱ
		if (KEY_DOWN(VK_UP)&&currCursor.y>70)
		{
			drawcursor(currCursor, BLACK);
			currCursor.y -= 40;
			drawcursor(currCursor, WHITE);
		}
		else if (KEY_DOWN(VK_DOWN) && currCursor.y<630)
		{
			drawcursor(currCursor, BLACK);
			currCursor.y += 40;
			drawcursor(currCursor, WHITE);
		}
		else if (KEY_DOWN(VK_LEFT) && currCursor.x>280)
		{
			drawcursor(currCursor, BLACK);
			currCursor.x -= 40;
			drawcursor(currCursor, WHITE);
		}
		else if (KEY_DOWN(VK_RIGHT) && currCursor.x<720)
		{
			drawcursor(currCursor, BLACK);
			currCursor.x += 40;
			drawcursor(currCursor, WHITE);
		}
		else if (KEY_DOWN(VK_RETURN) || KEY_DOWN(VK_SPACE))
		{
			//��ȡ�����ΧͬɫС�����꣬������������
			getsamecolorballs(currCursor, getpixel(currCursor.x, currCursor.y));

			//��������Ԫ����������һ��ʱ��
			if (ballsNumber >1)//ֻ���ö��С��һ����ʧ
			{
				for (int i = 0; i < ballsNumber; i++)
				{
					setlinecolor(BLACK);
					setfillcolor(BLACK);
					fillcircle(ballsArray[i].x, ballsArray[i].y, 18);
				}
				//��ͣһ�£����㿴���ڶ�
				Sleep(500);
				//�Ϸ�С������
				ballsfall();
				//checkblackballs();
				//ˢ�·���
				score += ballsNumber;
				drawscore(score);
			}
			else//�޷���һ��С����ʧ
			{
				drawwarnning(currCursor);//��ӡ����
			}
			ballsNumber = 0;//��ͬɫС��������
		}
		Sleep(100);//�ó���ȴ��û��İ�����������Ȼ����ͻ����ʣ�
	}
	//��Ϸ������������Ļ
	cleardevice();
	
}

void close(void)
{
	settextcolor(RGB(255, 0, 0));
	settextstyle(50, 0, _T("����"));
	TCHAR s[] = _T("GAME OVER !");//��������д����Ȼ�����룡
	outtextxy(300, 300, s);
	drawscore(score);
	system("pause");
	//_getch();
	closegraph();
}


void drawcursor(posType position, COLORREF cr)
{
	setlinecolor(cr);
	rectangle(position.x - 20, position.y - 20, position.x + 20, position.y + 20);
}

void drawtime(int sec)
{
	TCHAR s[30];
	settextcolor(RGB(255, 255, 0));
	settextstyle(25, 0, _T("����"));
	//TCHAR s[] = _T("ʣ��ʱ�䣺 30s");//��������д����Ȼ�����룡
	swprintf_s(s, _T("ʣ��ʱ�䣺 %2ds"), sec);
	outtextxy(29, 50,s);
}

void drawscore(int score)
{
	TCHAR s[30];
	settextcolor(GREEN);
	settextstyle(25, 0, _T("����"));
	swprintf_s(s, _T("�÷֣� %d��"), score);
	outtextxy(29, 600, s);
}

void getsamecolorballs(posType curr , COLORREF color)
{
	ballsArray[ballsNumber].x = curr.x;//�Ȱѵ�ǰ����������������
	ballsArray[ballsNumber].y = curr.y;
	ballsNumber++;//��ʱͬɫС�����Ϊһ��
/*
	//����Ϸ�����
	posType upBall;
	upBall.x = curr.x;
	upBall.y = curr.y - 40;
	if (getpixel(upBall.x , upBall.y) == color)
	{
		ballsArray[ballsNumber] = upBall;
		ballsNumber++;
	}

	//�·�����
	posType downBall;
	downBall.x = curr.x;
	downBall.y = curr.y + 40;
	if (getpixel(downBall.x, downBall.y) == color)
	{
		ballsArray[ballsNumber] = downBall;
		ballsNumber++;
	}
	//��ߵ���
	posType leftBall;
	leftBall.x = curr.x - 40;
	leftBall.y = curr.y;
	if (getpixel(leftBall.x, leftBall.y) == color)
	{
		ballsArray[ballsNumber] = leftBall;
		ballsNumber++;
	}
	//�ұߵ���
	posType rightBall;
	rightBall.x = curr.x + 40;
	rightBall.y = curr.y;
	if (getpixel(rightBall.x, rightBall.y) == color)
	{
		ballsArray[ballsNumber] = rightBall;
		ballsNumber++;
	}
	*/
	posType temp;
	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0 ://��
			temp.x = curr.x;
			temp.y = curr.y - 40;
			break;
		case 1 ://��
			temp.x = curr.x;
			temp.y = curr.y + 40;
			break;
		case 2 ://��
			temp.x = curr.x - 40;
			temp.y = curr.y ;
			break;
		case 3://��
			temp.x = curr.x + 40;
			temp.y = curr.y;
			break;
		}
		if (isok(temp,color))//�������ϸ�
		{
			getsamecolorballs(temp, color);//��ô�ͼ�������ɣ�
		}
	}
	
}

int isok(posType temp, COLORREF color)
{
	if (getpixel(temp.x, temp.y) == color)//�����ɫ��ͬ
	{
		for (int i = 0; i < ballsNumber; i++)//����������Ѱ�Ѽ����С���б�
		{
			if (temp.x == ballsArray[i].x && temp.y == ballsArray[i].y)
			{
				return 0;//����Ѿ����������ˣ����ϸ�
			}
		}
		return 1;//���û�ӹ�����ô�ϸ񣬹�����
	}
	else//�����ɫ����ͬ
	{
		return 0;//��ô���ϸ�ɣ�
	}
	
}

void drawwarnning(posType curr)
{
	TCHAR s[30];
	settextcolor(WHITE);
	settextstyle(20, 0, _T("����"));
	swprintf_s(s, _T("С��%d��%d)�޷�������"), curr.x,curr.y);
	outtextxy(380, 10, s);
}

void ballsfall()
{
	sort();
	srand((unsigned)time(NULL));//�ṩ���������
	int j;
	for (int i = 0 ; i < ballsNumber ; i++)
	{
		for (int y = ballsArray[i].y ; y >70 ; y -= 40)
		{
			COLORREF colorShouldBe = getpixel(ballsArray[i].x, y - 40);
			setlinecolor(colorShouldBe);
			setfillcolor(colorShouldBe);
			fillcircle(ballsArray[i].x, y, 18);
		}
		j = rand() % 6;//�ṩ0~5�������
		setlinecolor(colorArray[j]);
		setfillcolor(colorArray[j]);
		fillcircle(ballsArray[i].x, 70, 18);
	}
}
void sort()
{
	for (int i = 0; i < ballsNumber - 1; i++)
	{
		for (int j = i +1; j < ballsNumber; j++)
		{
			if (ballsArray[i].y > ballsArray[j].y)
			{
				//����������
				ballsArray[i].y = ballsArray[i].y ^ ballsArray[j].y;
				ballsArray[j].y = ballsArray[i].y ^ ballsArray[j].y;
				ballsArray[i].y = ballsArray[i].y ^ ballsArray[j].y;
				//����������
				ballsArray[i].x = ballsArray[i].x ^ ballsArray[j].x;
				ballsArray[j].x = ballsArray[i].x ^ ballsArray[j].x;
				ballsArray[i].x = ballsArray[i].x ^ ballsArray[j].x;
			}
		}
	}
}

/*
void checkblackballs()
{
	COLORREF color ;
	for (int i = 0; i < ballsNumber; i++)
	{
		color = getpixel(ballsArray[i].x, ballsArray[i].y );
		if (color ==BLACK)
		{
			for (int y = ballsArray[i].y; y >70; y -= 40)
			{
				COLORREF colorShouldBe = getpixel(ballsArray[i].x, y - 40);
				setlinecolor(colorShouldBe);
				setfillcolor(colorShouldBe);
				fillcircle(ballsArray[i].x, y, 18);
			}
			int j = rand() % 6;//�ṩ0~5�������
			setlinecolor(colorArray[j]);
			setfillcolor(colorArray[j]);
			fillcircle(ballsArray[i].x, 70, 18);
		}
	}
}
*/
