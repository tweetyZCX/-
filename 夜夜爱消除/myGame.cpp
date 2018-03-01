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

posType currCursor;//当前光标的位置

posType ballsArray[180];//存放同色小球坐标

int ballsNumber = 0;//同色小球个数

int score = 0;//初始分数为零

//宏，如果按下键了，后面的括号计算出来非零值。我们用keydown这样一个宏来替代后面一长条的函数
#define KEY_DOWN(vk_c) (GetAsyncKeyState(vk_c)&0x8000)
				  
void init(void);//初始化界面
void gamebegain(void);//游戏开始界面
void gameplay(void);//玩游戏具体过程
void close(void);//释放资源
void drawcursor(posType , COLORREF );//绘制光标
void drawtime(int);//重绘时间字符串
void drawscore(int );//绘制分数
void getsamecolorballs(posType ,COLORREF);//获取同色小球，并把他们加到数组里面，同时增加同色小球个数
int isok(posType, COLORREF);//判断新的小球是否合法
void drawwarnning(posType);//绘制警告语句
void ballsfall();//小球下落
//void checkblackballs();//检查未填上的黑洞？
void sort();//给小球排序，纵坐标小的在前面

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
	//绘制边框
	setlinecolor(RGB(50, 50, 50));//设置边框颜色为灰色
	setlinestyle(PS_SOLID/*实线*/,10/*线条粗细*/);//设置边框样式
	rectangle(255, 45, 745, 655);
	//绘制180个小球
	
	setlinestyle(PS_SOLID, 1);
	
	srand((unsigned)time(NULL));//提供随机数种子
	for (int x = 280; x < 740; x += 40)
	{
		for (int y = 70; y <= 630; y += 40)
		{
			int i=rand() % 6;//提供0~5的随机数
			setlinecolor(colorArray[i]);
			setfillcolor(colorArray[i]);
			fillcircle(x, y, 18);
		}
	}
	//绘制光标
	currCursor.x = 480;
	currCursor.y = 390;
	drawcursor(currCursor, WHITE);
	//绘制时间
	drawtime(30);
	//绘制分数
	drawscore(0);
}

void gameplay(void)
{
	//不断等待！监听用户动作，hold住！
	for (int i = 299 ; i >= 0 ;i--)
	{
		if (i % 10 ==0)
		{
			drawtime(i / 10);

		}
		//如果上下左右方向键按下时
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
			//获取光标周围同色小球坐标，存入数组里面
			getsamecolorballs(currCursor, getpixel(currCursor.x, currCursor.y));

			//将数组中元素依次至黑一段时间
			if (ballsNumber >1)//只能让多个小球一起消失
			{
				for (int i = 0; i < ballsNumber; i++)
				{
					setlinecolor(BLACK);
					setfillcolor(BLACK);
					fillcircle(ballsArray[i].x, ballsArray[i].y, 18);
				}
				//暂停一下，方便看到黑洞
				Sleep(500);
				//上方小球落下
				ballsfall();
				//checkblackballs();
				//刷新分数
				score += ballsNumber;
				drawscore(score);
			}
			else//无法让一个小球消失
			{
				drawwarnning(currCursor);//打印警告
			}
			ballsNumber = 0;//让同色小球数归零
		}
		Sleep(100);//让程序等待用户的按键动作，不然光标会突破天际！
	}
	//游戏结束，清理屏幕
	cleardevice();
	
}

void close(void)
{
	settextcolor(RGB(255, 0, 0));
	settextstyle(50, 0, _T("黑体"));
	TCHAR s[] = _T("GAME OVER !");//必须这样写，不然会乱码！
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
	settextstyle(25, 0, _T("黑体"));
	//TCHAR s[] = _T("剩余时间： 30s");//必须这样写，不然会乱码！
	swprintf_s(s, _T("剩余时间： %2ds"), sec);
	outtextxy(29, 50,s);
}

void drawscore(int score)
{
	TCHAR s[30];
	settextcolor(GREEN);
	settextstyle(25, 0, _T("黑体"));
	swprintf_s(s, _T("得分： %d分"), score);
	outtextxy(29, 600, s);
}

void getsamecolorballs(posType curr , COLORREF color)
{
	ballsArray[ballsNumber].x = curr.x;//先把当前光标坐标加入数组中
	ballsArray[ballsNumber].y = curr.y;
	ballsNumber++;//此时同色小球个数为一了
/*
	//光标上方的球
	posType upBall;
	upBall.x = curr.x;
	upBall.y = curr.y - 40;
	if (getpixel(upBall.x , upBall.y) == color)
	{
		ballsArray[ballsNumber] = upBall;
		ballsNumber++;
	}

	//下方的球
	posType downBall;
	downBall.x = curr.x;
	downBall.y = curr.y + 40;
	if (getpixel(downBall.x, downBall.y) == color)
	{
		ballsArray[ballsNumber] = downBall;
		ballsNumber++;
	}
	//左边的球
	posType leftBall;
	leftBall.x = curr.x - 40;
	leftBall.y = curr.y;
	if (getpixel(leftBall.x, leftBall.y) == color)
	{
		ballsArray[ballsNumber] = leftBall;
		ballsNumber++;
	}
	//右边的球
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
		case 0 ://上
			temp.x = curr.x;
			temp.y = curr.y - 40;
			break;
		case 1 ://下
			temp.x = curr.x;
			temp.y = curr.y + 40;
			break;
		case 2 ://左
			temp.x = curr.x - 40;
			temp.y = curr.y ;
			break;
		case 3://右
			temp.x = curr.x + 40;
			temp.y = curr.y;
			break;
		}
		if (isok(temp,color))//如果检验合格
		{
			getsamecolorballs(temp, color);//那么就加入数组吧！
		}
	}
	
}

int isok(posType temp, COLORREF color)
{
	if (getpixel(temp.x, temp.y) == color)//如果颜色相同
	{
		for (int i = 0; i < ballsNumber; i++)//在数组中搜寻已加入的小球列表
		{
			if (temp.x == ballsArray[i].x && temp.y == ballsArray[i].y)
			{
				return 0;//如果已经加入数组了，不合格
			}
		}
		return 1;//如果没加过，那么合格，哈哈哈
	}
	else//如果颜色不相同
	{
		return 0;//那么不合格吧！
	}
	
}

void drawwarnning(posType curr)
{
	TCHAR s[30];
	settextcolor(WHITE);
	settextstyle(20, 0, _T("黑体"));
	swprintf_s(s, _T("小球（%d，%d)无法消除！"), curr.x,curr.y);
	outtextxy(380, 10, s);
}

void ballsfall()
{
	sort();
	srand((unsigned)time(NULL));//提供随机数种子
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
		j = rand() % 6;//提供0~5的随机数
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
				//交换纵坐标
				ballsArray[i].y = ballsArray[i].y ^ ballsArray[j].y;
				ballsArray[j].y = ballsArray[i].y ^ ballsArray[j].y;
				ballsArray[i].y = ballsArray[i].y ^ ballsArray[j].y;
				//交换横坐标
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
			int j = rand() % 6;//提供0~5的随机数
			setlinecolor(colorArray[j]);
			setfillcolor(colorArray[j]);
			fillcircle(ballsArray[i].x, 70, 18);
		}
	}
}
*/
