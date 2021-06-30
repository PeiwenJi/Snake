#include"snake.h"//头文件

//全局变量
char name[10];
clock_t start, stop; //clock_t为clock()函数返回的变量类型
int duration = 0; //CLK_TCK为clock()函数的时间单位，即时钟打点
int speed = 10;//速度
int coolTime = -2;//道具冷却时间
bool speedJudge = false;//是否加速
int speedTime = 0;//加速时间
bool game = true;//是否退出游戏(注意不能初始化！！！)
bool reBegin = false;//是否重新开始，回到首页
int score = 0;//得分
int score1 = 0;//玩家一得分
int score2 = 0;//玩家二得分
struct food food[15];
struct food letter[26];//字母;/o
int flash = 0;//炸弹闪动
int timeFood = 0;//道具减少的时间
MOUSEMSG m;//鼠标
struct snake snake;//主玩家
struct snake ball;//小球
struct snake snake2;//玩家二（双人模式）
struct coor brr[200];//障碍物
struct coor treasure;//宝藏
struct coor wall[700];//边界
struct snake fakesnake; // 假的蛇（地图动中确定snake的形状）
int fakeX;//假蛇相对真蛇的x坐标
int fakeY;//假蛇相对真蛇的y坐标
IMAGE img[5];//5种模式的图片
bool choose = true; //是否还在模式选择中
int imagenum = 0;//目前图片的号码
struct letter snaletter;//吃到的字符串
struct letter ltlibrary;
int snaletternum = 0;
bool lettersame = false;
bool crechoose = true; //是否还在创意模式选择中


/**************************************************///主函数
int main()
{
	PlaySound(TEXT("music.wav"), 0, SND_FILENAME | SND_ASYNC | SND_LOOP);//随机数种子，后面随机产生蛇的食物要用到
	srand((unsigned int)time(NULL));
	while (game)
	{
		welcome();
		inall();
	}
	return 0;
}
//所有全局变量初始化
void inall()
{
	duration = 0; //CLK_TCK为clock()函数的时间单位，即时钟打点
	speed = 10;//速度
	coolTime = -2;//道具冷却时间
	speedJudge = false;//是否加速
	speedTime = 0;//加速时间
	reBegin = false;//是否重新开始，回到首页
	score = 0;//得分
	score1 = 0;//玩家一得分
	score2 = 0;//玩家二得分
	flash = 0;//炸弹闪动
	timeFood = 0;//道具减少的时间
	choose = true; //是否还在模式选择中
	imagenum = 0;//目前图片的号码
	snaletternum = 0;
	lettersame = false;
	crechoose = true; //是否还在创意模式选择中

}
/**************************************************///蛇
//初始化蛇
void insnake() {
	initgraph(640, 480);
	setbkcolor(RGB(135,206,250));//LightSkyBlue
	snake.xy[2].x = 0;
	snake.xy[2].y = 0;
	snake.xy[1].x = 10;
	snake.xy[1].y = 0;
	snake.xy[0].x = 20;
	snake.xy[0].y = 0;
	snake.num = 3;
	snake.way = right;
	for (int i=0;i< 15; i++)
	{
		food[i].flag = 0;
	}
}
//绘制蛇
void inisnake()
{
	setlinecolor(RGB(106, 90, 205));//SlateBlue
	setfillcolor(RGB(250, 128, 114));//Violet
	fillellipse(snake.xy[0].x, snake.xy[0].y, snake.xy[0].x + 10, snake.xy[0].y + 10);
	for (int i = 1; i < snake.num; i++)
	{
		setlinecolor(RGB(106, 90, 205));//SlateBlue
		setfillcolor(RGB(255, 228, 225));//MistyRose
		fillrectangle(snake.xy[i].x, snake.xy[i].y, snake.xy[i].x + 10, snake.xy[i].y + 10);
	}
}
//蛇的移动（键盘）
void way()
{
	//除了第一节蛇，后面每节都是前面一节蛇的前一次坐标
	for (int i = snake.num - 1; i > 0; i--)
	{
		snake.xy[i].x = snake.xy[i - 1].x;
		snake.xy[i].y = snake.xy[i - 1].y;
	}
	switch (snake.way)
	{
	case right:
		snake.xy[0].x += speed;
		break;
	case left:
		snake.xy[0].x -= speed;
		break;
	case down:
		snake.xy[0].y += speed;
		break;
	case up:
		snake.xy[0].y -= speed;
		break;
	default:
		break;
	}
}
//蛇的方向控制（键盘）
void snakemove()
{
	char ch = 0;
	ch = _getch();
	switch (ch)
	{
	case 72:
		if (snake.way != down)
			snake.way = up;
		break;
	case 75:
		if (snake.way != right)
			snake.way = left;
		break;
	case 77:
		if (snake.way != left)
			snake.way = right;
		break;
	case 80:
		if (snake.way != up)
			snake.way = down;
		break;
	case 32://暂停
		_getch();
		break;
	case 13://技能：加50分（用于闯关模式）
		score += 50;
		break;
	case 9://技能：宝藏可以看见,并停顿一秒（用于寻宝模式）
		fillrectangle(treasure.x, treasure.y, treasure.x + 10, treasure.y + 10);
		Sleep(1000);
		break;
	default:
		break;
	}
}
//判断蛇是否死亡
void snakedie()
{
	if (snake.xy[0].x > 640 || snake.xy[0].x < 0 || snake.xy[0].y > 480 || snake.xy[0].y < 0)
	{
		finish();

	}

	for (int i = 1; i < snake.num; i++)
	{
		if (snake.xy[0].x == snake.xy[i].x &&snake.xy[0].y == snake.xy[i].y)
		{
			finish();
		}
	}

}
/**************************************************///食物
//食物坐标获取
void foodxy()
{
	//蛇吃到食物，食物消失
	for (int i = 0; i < 15; i++)
	{
		if (food[i].flag == 0)
		{
			food[i].fdxy.x = -100;
			food[i].fdxy.y = -100;
		}
	}
	//食物随机出现和消失
	if (coolTime == -1) {
		for (int j = 0; j < 15; j++)
		{
			food[j].fdxy.x = -100;
			food[j].fdxy.y = -100;
		}
	}
	if (coolTime<20 && coolTime>0) {
		for (int j = 0; j < 15; j++)
		{
			if (int k = rand() % 200 == 1) {
				food[j].fdxy.x = rand() % 65 * 10;
				food[j].fdxy.y = rand() % 48 * 10;
				food[j].flag = 1;

				for (int i = 0; i > snake.num; i++)
				{
					if (food[j].fdxy.x == snake.xy[i].x && food[j].fdxy.y == snake.xy[i].y)
					{
						food[j].fdxy.x = rand() % 65 * 10;
						food[j].fdxy.y = rand() % 48 * 10;

					}
				}
			}
		}
	}
	if (coolTime > 20 && coolTime < 40) {
		for (int j = 0; j < 15; j++)
		{
			if (int k = rand() % 200 == 1) {
				food[j].fdxy.x = -100;
				food[j].fdxy.y = -100;
			}
		}
	}
	if (coolTime > 40) {
		coolTime = 0;
	}
}
//绘制食物
/*
Food(15个道具)：
0.	加10分（橙色矩形）
1.	加10分（橙色矩形）
2.	加10分（橙色矩形）
3.	加10分（橙色矩形）
4.	加15分（黄色矩形）
5.	加15分（黄色矩形）
6.	减速（绿色矩形）
7.	减速（绿色矩形）
8.	加速（紫色矩形）
9.	加速（紫色矩形）
10.	炸弹——死亡（黑色椭圆）
11.	分数清空（红色三角形）
12.	清屏（白色椭圆）
13.	蛇身变短——2格（棕色椭圆）
14.	减少时间——10秒（蓝色椭圆）
*/
void inifood()
{
	//food[0]
	setfillcolor(RGB(255, 165, 0));//Orange
	fillrectangle(food[0].fdxy.x, food[0].fdxy.y, food[0].fdxy.x + 10, food[0].fdxy.y + 10);
	//food[1]
	setfillcolor(RGB(255, 165, 0));//Orange
	fillrectangle(food[1].fdxy.x, food[1].fdxy.y, food[1].fdxy.x + 10, food[1].fdxy.y + 10);
	//food[2]
	setfillcolor(RGB(255, 165, 0));//Orange
	fillrectangle(food[2].fdxy.x, food[2].fdxy.y, food[2].fdxy.x + 10, food[2].fdxy.y + 10);
	//food[3]
	setfillcolor(RGB(255, 165, 0));//Orange
	fillrectangle(food[3].fdxy.x, food[3].fdxy.y, food[3].fdxy.x + 10, food[3].fdxy.y + 10);
	//food[4]
	setfillcolor(RGB(255, 255, 0));//Yellow
	fillrectangle(food[4].fdxy.x, food[4].fdxy.y, food[4].fdxy.x + 10, food[4].fdxy.y + 10);
	//food[5]
	setfillcolor(RGB(255, 255, 0));//Yellow
	fillrectangle(food[5].fdxy.x, food[5].fdxy.y, food[5].fdxy.x + 10, food[5].fdxy.y + 10); 
	//food[6]
	setfillcolor(RGB(0, 250, 154));//MedSpringGreen
	fillrectangle(food[6].fdxy.x, food[6].fdxy.y, food[6].fdxy.x + 10, food[6].fdxy.y + 10);
	//food[7]
	setfillcolor(RGB(0, 250, 154));//MedSpringGreen
	fillrectangle(food[7].fdxy.x, food[7].fdxy.y, food[7].fdxy.x + 10, food[7].fdxy.y + 10);
	//food[8]
	setfillcolor(RGB(147, 112, 219));//MediumPurple
	fillrectangle(food[8].fdxy.x, food[8].fdxy.y, food[8].fdxy.x + 10, food[8].fdxy.y + 10);
	//food[9]
	setfillcolor(RGB(147, 112, 219));//MediumPurple
	fillrectangle(food[9].fdxy.x, food[9].fdxy.y, food[9].fdxy.x + 10, food[9].fdxy.y + 10);
	//food[10]
	if (flash == 0) {
		flash = 1;
		setfillcolor(RGB(255, 255, 255));
	}
	else {
		flash = 0;
		setfillcolor(RGB(0, 0, 0));
	}	
	fillellipse(food[10].fdxy.x, food[10].fdxy.y, food[10].fdxy.x + 10, food[10].fdxy.y + 10);
	//food[11]
	setfillcolor(RGB(238, 99, 99));//red
	POINT pts[] = { {food[11].fdxy.x-5, food[11].fdxy.y+10}, {food[11].fdxy.x, food[11].fdxy.y}, {food[11].fdxy.x+5, food[11].fdxy.y+10} };
	fillpolygon(pts, 3);
	//food[12]
	setfillcolor(RGB(255, 250, 250));//White
	fillellipse(food[12].fdxy.x, food[12].fdxy.y, food[12].fdxy.x + 10, food[12].fdxy.y + 10);
	//food[13]
	setfillcolor(RGB(139, 90, 0));//Orange4
	fillellipse(food[13].fdxy.x, food[13].fdxy.y, food[13].fdxy.x + 10, food[13].fdxy.y + 10);
	//food[14]
	setfillcolor(RGB(16, 78, 139));//DodgerBlue4
	fillellipse(food[14].fdxy.x, food[14].fdxy.y, food[14].fdxy.x + 10, food[14].fdxy.y + 10);
}
//吃食物的判断
void eatfood()
{
	//0-3:加10分
	for (int i = 0; i < 4; i++) {
		if (snake.xy[0].x == food[i].fdxy.x && snake.xy[0].y == food[i].fdxy.y)
		{
			snake.num++;
			fakesnake.num++;
			score += 10;
			score1 += 10;
			food[i].flag = 0;
		}
	}
	//4-5:加15分
	for (int i = 4; i < 6; i++) {
		if (snake.xy[0].x == food[i].fdxy.x && snake.xy[0].y == food[i].fdxy.y)
		{
			snake.num++;
			fakesnake.num++;
			snake.num++;
			score += 15;
			score1 += 15;
			food[i].flag = 0;
		}
	}
	//6-7:减速
	if (snake.xy[0].x == food[6].fdxy.x && snake.xy[0].y == food[6].fdxy.y)
	{
		food[6].flag = 0;
		speed += 10;
		speedJudge = true;
	}
	if (snake.xy[0].x == food[7].fdxy.x && snake.xy[0].y == food[7].fdxy.y)
	{
		food[7].flag = 0;
		speed += 10;
		speedJudge = true;
	}
	//8-9:加速
	if (snake.xy[0].x == food[8].fdxy.x && snake.xy[0].y == food[8].fdxy.y)
	{
		food[8].flag = 0;
		speed += 20;
		speedJudge = true;
	}
	if (snake.xy[0].x == food[9].fdxy.x && snake.xy[0].y == food[9].fdxy.y)
	{
		food[9].flag = 0;
		speed += 20;
		speedJudge = true;
	}
	//10:炸弹
	if (snake.xy[0].x == food[10].fdxy.x && snake.xy[0].y == food[10].fdxy.y)
	{
		food[10].flag = 0;
		finish();
	}
	//11:清空分数
	if (snake.xy[0].x == food[11].fdxy.x && snake.xy[0].y == food[11].fdxy.y)
	{
		food[11].flag = 0;
		score = 0;
		score1 = 0;
	}
	//12:清屏
	if (snake.xy[0].x == food[12].fdxy.x && snake.xy[0].y == food[12].fdxy.y)
	{
		for (int i = 0; i < 15; i++) {
			food[i].flag = 0;
		}
	}
	//13:蛇身变短-2格
	if (snake.xy[0].x == food[13].fdxy.x && snake.xy[0].y == food[13].fdxy.y)
	{
		food[13].flag = 0;
		snake.num -= 2;
	}
	//14:减少时间-10s
	if (snake.xy[0].x == food[14].fdxy.x && snake.xy[0].y == food[14].fdxy.y)
	{
		food[14].flag = 0;
		timeFood -= 10;
	}	
}
/**************************************************///界面控制
//游戏过程中，按任意键可以对游戏进行暂停、开始(仅鼠标模式用到！！！)
void again()
{
	char ch = 0;//按键
	ch = _getch();
	if (ch == 32)
	{
		while (_getch() != 32);
	}
}
//欢迎界面
void welcome() {
	//属性列表
	struct MOUSEMSG myClick;
	bool quit = false;

	initgraph(640, 480);
	// 读取图片至绘图窗口
	IMAGE img;
	loadimage(NULL, _T("welcome.png"));
	// 按任意键退出
	_getch();
	loadimage(NULL, _T("catalogue.png"));
	//响应点击事件
	while (!quit) {
		myClick = GetMouseMsg();
		if (myClick.uMsg == WM_LBUTTONDOWN)
		{
			if (myClick.x > 260 && myClick.x < 397 && myClick.y>316 && myClick.y < 371)
			{
				loadimage(NULL, _T("author.png"));
				_getch();
				loadimage(NULL, _T("catalogue.png"));
			}
			if (myClick.x > 556 && myClick.x < 640 && myClick.y>440 && myClick.y < 480)
			{
				loadimage(NULL, _T("help.png"));
				_getch();
				loadimage(NULL, _T("catalogue.png"));
			}
			if (myClick.x > 138 && myClick.x < 520 && myClick.y>184 && myClick.y < 300) {
				quit = true;
				loadimage(NULL, _T("begin.png"));
				char s[10];
				InputBox(s, 10, "您的名字（10个字符内）");
				sscanf(s, "%s", &name);
				_getch();
				stylechoose();
			}
		}
		//出现选择框
		if (myClick.x > 260 && myClick.x < 397 && myClick.y>316 && myClick.y < 371)
		{
			setlinestyle(PS_DASH | PS_JOIN_BEVEL, 3);
			setlinecolor(RGB(250, 128, 114));
			line(260,365,397,365);
		}
		else if (myClick.x > 556 && myClick.x < 640 && myClick.y>440 && myClick.y < 480)
		{
			setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 1);
			setlinecolor(RGB(105, 105, 105));
			line(559, 473, 637, 473);
		}
		else if (myClick.x > 138 && myClick.x < 520 && myClick.y>184 && myClick.y < 300) 
			{
			setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 5);
			setlinecolor(RGB(250, 128, 114));
			rectangle(138, 184, 520, 300);
		}
		else
		{
			loadimage(NULL, _T("catalogue.png"));
		}
	}
}
//结束界面
void finish() 
{
	struct MOUSEMSG myClick;
	char grade[100] = { 0 };
	sprintf(grade, "%d", score);
	
	loadimage(NULL, _T("finish.png"));
	setbkmode(0);//文字背景透明
	settextcolor(BLACK);
	outtextxy(5, 5, "您的分数是：");
	outtextxy(5, 25, grade);

	while (game && (!reBegin)) {
		myClick = GetMouseMsg();
		if (myClick.uMsg == WM_LBUTTONDOWN)
		{
			if (myClick.x > 147 && myClick.x < 495 && myClick.y>187 && myClick.y < 300)
			{
				game = true;
				reBegin = true;
			}
			if (myClick.x > 271 && myClick.x < 368 && myClick.y>321 && myClick.y < 373)
			{
				game = false;
			}
		}
		//出现选择框
		if (myClick.x > 147 && myClick.x < 495 && myClick.y>187 && myClick.y < 300)
		{
			setlinecolor(RGB(250, 128, 114));
			setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 5);
			rectangle(147, 187, 495, 300);
		}
		else if (myClick.x > 271 && myClick.x < 368 && myClick.y>321 && myClick.y < 373)
		{
			setlinestyle(PS_DASH | PS_ENDCAP_FLAT, 3);
			setlinecolor(RGB(250, 128, 114));
			rectangle(271, 321, 368, 373);
		}
		else
		{
			loadimage(NULL, _T("finish.png"));
		}
	}
}
//显示信息
void text()
{
	char grade[100] = { 0 };
	sprintf(grade, "%d", score);
	setbkmode(0);//文字背景透明
	settextstyle(15, 0, _T("宋体"));

	//提示信息
	settextcolor(RGB(224, 255, 255));//LightCyan
	outtextxy(5, 440, "键盘的方向键控制");
	outtextxy(5, 460, "暂停：空格/继续：任意键");
	//分数
	settextcolor(RGB(65, 105, 225));//RoyalBlue
	outtextxy(570, 10, "分数：");
	outtextxy(610, 10, grade);
	//记录时间
	stop = clock();
	 duration = (int)(stop - start) / CLK_TCK + timeFood; //CLK_TCK为clock()函数的时间单位，即时钟打点
	char s[5];
	sprintf(s, "%d", duration);
	outtextxy(570, 25, "时间：");
	outtextxy(610, 25, s);
	//姓名
	settextcolor(RGB(224, 255, 255));//LightCyan
	outtextxy(5, 10, "欢迎进入贪吃蛇大作战");
	outtextxy(5, 25, name);
}
//切换界面(黄色)
void change()
{
	initgraph(640, 480);
	int x = 0;
	int y = 0;
	setfillcolor(RGB(255, 248, 220));
	for (int i = 0; i < 48; i++)
	{
		fillrectangle(x, y+i*10,x + 640, y + 10+i*10);
		Sleep(50);
	}
	closegraph();
}
//切换界面（黑色）
void change1()
{
	int x = 0;
	int y = 0;
	setfillcolor(RGB(0, 0, 0));
	for (int i = 0; i < 48; i++)
	{
		fillrectangle(x, y + i * 10, x + 640, y + 10 + i * 10);
		Sleep(50);
	}
}
//游戏倒计时界面
void startgame()
{
	initgraph(640, 480);
	loadimage(NULL, _T("3.png"));
	Sleep(500);
	loadimage(NULL, _T("0.png"));
	Sleep(500);
	loadimage(NULL, _T("2.png"));
	Sleep(500);
	loadimage(NULL, _T("0.png"));
	Sleep(500);
	loadimage(NULL, _T("1.png"));
	Sleep(500);
	loadimage(NULL, _T("0.png"));
	Sleep(500);
	loadimage(NULL, _T("start.png"));
	Sleep(300);
	change1();
	closegraph();
}
//游戏成功界面
void success()
{
	while (!(_kbhit()))
	{
		if (flash == 0)
		{
			loadimage(NULL, _T("success1.png"));
			Sleep(100);
			flash = 1;
		}
		else 
		{
			loadimage(NULL, _T("success2.png"));
			Sleep(100);
			flash = 0;
		}
	}
}
/**************************************************///模式选择
/*
按左右键切换模式，按ESC进入帮助，按Enter进入游戏
*/
//加载5种模式的图片
void inimage()
{
	loadimage(&img[0], _T("freeStyle.png"));
	loadimage(&img[1], _T("levelStyle.png"));
	loadimage(&img[2], _T("mapStyle.png"));
	loadimage(&img[3], _T("creatyStyle.png"));
	loadimage(&img[4], _T("togetherStyle.png"));
}
//模式切换
void stylechoose()
{
	inimage();
	putimage(0, 0, &img[0]);
	while (choose)
	{
		char ch = 0;
		//键盘控制图片切换		
		ch = _getch();
		switch (ch)
		{
		case 75://左键
			if (imagenum > 0)
			{
				imagenum -= 1;
			}
			break;
		case 77://右键
			if (imagenum < 4)
			{
				imagenum += 1;
			}
			break;
		case 13://回车
			choose = false;
			closegraph();
			begin();
			break;
		case 27://ESC键
			stylehelp();
			break;
		default:
			break;
		}
		putimage(0, 0, &img[imagenum]);
	}
}
//各种模式的游戏规则
void stylehelp()
{
	if (imagenum == 0)
	{
		loadimage(NULL, _T("freeStylehelp.png"));
		_getch();
	}
	if (imagenum == 1)
	{
		loadimage(NULL, _T("levelStylehelp.png"));
		_getch();
	}
	if (imagenum == 2)
	{
		loadimage(NULL, _T("mapStylehelp.png"));
		_getch();
	}
	if (imagenum == 3)
	{
		loadimage(NULL, _T("creatyStylehelp.png"));
		_getch();
	}
	if (imagenum == 4)
	{
		loadimage(NULL, _T("togetherStylehelp.png"));
		_getch();
	}
}
//游戏开始
void begin()
{
	if (imagenum == 0)
	{
		startgame();
		freeStyle();
	}
	if (imagenum == 1)
	{
		startgame();
		levelStyle();
	}
	if (imagenum == 2)
	{
		startgame();
		mapStyle();
	}
	if (imagenum == 3)
	{
		creatychoose();
	}
	if (imagenum == 4)
	{
		startgame();
		togetherStyle();
	}
}
//创意模式选择
void creatychoose()
{
	initgraph(640, 480);
	loadimage(NULL, _T("creatychoose.png"));
	while (crechoose) {
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			//小球模式
			if (m.x > 130 && m.x < 270 && m.y>150 && m.y < 200)
			{
				closegraph();
				crechoose = false;
				startgame();
				ballStyle();
			}
			//鼠标模式
			if (m.x > 370 && m.x < 510 && m.y>150 && m.y < 205)
			{
				closegraph();
				crechoose = false;
				startgame();
				ballStyleMS();
			}
			//寻宝模式
			if (m.x > 130 && m.x < 275 && m.y>255 && m.y < 305) 
			{
				closegraph();
				crechoose = false;
				startgame();
				seekStyle();
			}
			//双蛇模式
			if (m.x > 370 && m.x < 510 && m.y>255 && m.y < 305) 
			{
				closegraph();
				crechoose = false;
				startgame();
				twoStyle();
			}
			//字母模式
			if (m.x > 130 && m.x < 275 && m.y>350 && m.y < 400) 
			{
				closegraph();
				crechoose = false;
				startgame();
				letterStyle();
			}
			//随机模式
			if (m.x > 360 && m.x < 507 && m.y>344 && m.y < 400) 
			{
				closegraph();
				crechoose = false;
				startgame();
				randomStyle();
			}
		}
		//出现选择框
		//小球模式
		if (m.x > 130 && m.x < 270 && m.y>150 && m.y < 200)
		{
			setlinestyle(PS_DASHDOTDOT | PS_ENDCAP_FLAT, 2);
			setlinecolor(RGB(123, 104, 238));
			line(140, 193, 266, 193);
		}
		//鼠标模式
		else if (m.x > 370 && m.x < 510 && m.y>150 && m.y < 205)
		{
			setlinestyle(PS_DASHDOTDOT | PS_ENDCAP_FLAT, 2);
			setlinecolor(RGB(255, 127, 80));
			line(380, 193, 503, 193);
		}
		//寻宝模式
		else if (m.x > 130 && m.x < 275 && m.y>255 && m.y < 305)
		{
			setlinestyle(PS_DASHDOTDOT | PS_ENDCAP_FLAT, 2);
			setlinecolor(RGB(0, 191, 255));
			line(140, 298, 266, 298);
		}
		//双蛇模式
		else if (m.x > 370 && m.x < 510 && m.y>255 && m.y < 305)
		{
			setlinestyle(PS_DASHDOTDOT | PS_ENDCAP_FLAT, 2);
			setlinecolor(RGB(139, 115, 85));
			line(380, 296, 503, 296);
		}
		//字母模式
		else if (m.x > 130 && m.x < 275 && m.y>350 && m.y < 400)
		{
			setlinestyle(PS_DASHDOTDOT | PS_ENDCAP_FLAT, 2);
			setlinecolor(RGB(50, 205, 50));
			line(140, 391, 266, 391);
		}
		//随机模式
		else if (m.x > 360 && m.x < 507 && m.y>344 && m.y < 400) 
		{
			setlinestyle(PS_DASHDOTDOT | PS_ENDCAP_FLAT, 2);
			setlinecolor(RGB(96, 123, 139));
			line(380, 389, 500, 389);
		}
		else
		{
			loadimage(NULL, _T("creatychoose.png"));
		}
	}
}
/**************************************************///小球模式
//初始化小球
void insball()
{
	ball.xy[0].x = 100;
	ball.xy[0].y = 80;
	ball.num = 1;
	ball.way = right;
}
//绘制小球
void iniball() 
{
	setfillcolor(RGB(250, 128, 114));//Violet
	fillellipse(ball.xy[0].x, ball.xy[0].y, ball.xy[0].x + 10, ball.xy[0].y + 10);
}
//小球的移动（键盘）
void wayball()
{
	switch (ball.way)
	{
	case right:
		ball.xy[0].x += speed;
		break;
	case left:
		ball.xy[0].x -= speed;
		break;
	case down:
		ball.xy[0].y += speed;
		break;
	case up:
		ball.xy[0].y -= speed;
		break;
	default:
		break;
	}

}
//小球的方向控制（键盘）
void ballmove()
{
	char ch = 0;
	ch = _getch();
	switch (ch)
	{
	case 72:
		ball.way = up;
		break;
	case 75:
		ball.way = left;
		break;
	case 77:
		ball.way = right;
		break;
	case 80:
		ball.way = down;
		break;
	case 32:
		_getch();
		break;
	default:
		break;
	}
}
//蛇跟随小球移动
void sbmove() 
{
	if (abs(snake.xy[0].x - ball.xy[0].x) >= abs(snake.xy[0].y - ball.xy[0].y)) {
		if (snake.xy[0].x >= ball.xy[0].x ) {
			snake.way = left;
		}else{
			snake.way = right;
		}
	}else {
		if (snake.xy[0].y >= ball.xy[0].y ) {
			snake.way = up;
		}else{
			snake.way = down;
		}
	}
}
//小球撞墙反弹
void ballwall()
{
	if (ball.xy[0].x > 640 ) 
	{
		ball.way = left;
	}
	if (ball.xy[0].x < 0)
	{
		ball.way = right;
	}
	if (ball.xy[0].y > 480)
	{
		ball.way = up;
	}
	if (ball.xy[0].y < 0)
	{
		ball.way = down;
	}
}
//蛇死亡
void sbdie() 
{
	if (snake.xy[0].x > 640 || snake.xy[0].x < 0 || snake.xy[0].y > 480 || snake.xy[0].y < 0)
	{
		finish();

	}
}
/**************************************************///小球模式(鼠标)
//小球的方向控制（鼠标）
void ballmoveMS()
{
	m = GetMouseMsg();
	ball.xy[0].x=m.x;
	ball.xy[0].y = m.y;
}
//蛇的移动（鼠标）
void wayMS()
{
	//除了第一节蛇，后面每节都是前面一节蛇的前一次坐标
	switch (snake.way)
	{
	case right:
		for (int i = snake.num - 1; i > 0; i--)
		{
			snake.xy[i].x = snake.xy[i - 1].x-9;
			snake.xy[i].y = snake.xy[i - 1].y;
		}
		snake.xy[0].x += speed;
		break;
	case left:
		for (int i = snake.num - 1; i > 0; i--)
		{
			snake.xy[i].x = snake.xy[i - 1].x+9;
			snake.xy[i].y = snake.xy[i - 1].y;
		}
		snake.xy[0].x -= speed;
		break;
	case down:
		for (int i = snake.num - 1; i > 0; i--)
		{
			snake.xy[i].x = snake.xy[i - 1].x;
			snake.xy[i].y = snake.xy[i - 1].y-9;
		}
		snake.xy[0].y += speed;
		break;
	case up:
		for (int i = snake.num - 1; i > 0; i--)
		{
			snake.xy[i].x = snake.xy[i - 1].x;
			snake.xy[i].y = snake.xy[i - 1].y+9;
		}
		snake.xy[0].y -= speed;
		break;
	default:
		break;
	}
}
/**************************************************///障碍物
//障碍物初始化
void insbrr() 
{
	for (int i = 0; i < 200; i++) {
		brr[i].x = -1000;
		brr[i].y = -1000;
	}
}
//障碍物绘制
void inibrr() 
{
	for (int i = 0; i < 200; i++) {
		if (flash == 0) {
			flash = 1;
			setfillcolor(RGB(255, 255, 255));
		}
		else {
			flash = 0;
			setfillcolor(RGB(0, 0, 0));
		}
		fillrectangle(brr[i].x, brr[i].y, brr[i].x + 10, brr[i].y + 10);
	}	
}
//障碍物碰撞检测
void bomp() 
{
	for (int i = 1; i < 200; i++)
	{
		if (snake.xy[0].x == brr[i].x && snake.xy[0].y == brr[i].y)
		{

			finish();
		}
	}
}
//障碍物碰撞检测（玩家二）
void bomp2()
{
	for (int i = 1; i < 200; i++)
	{
		if (snake2.xy[0].x == brr[i].x && snake2.xy[0].y == brr[i].y)
		{

			finish();
		}
	}
}
/**************************************************///地图
//关卡一
void level_1() {
	insbrr();
	brr[0].x = 160;
	brr[0].y = 130;
	for (int i = 1; i < 13; i++) {
		brr[i].x = 160;
		brr[i].y = 160+10*(i-1);
	}
	brr[13].x = 160;
	brr[13].y = 300;
	brr[14].x = 320;
	brr[14].y = 240;
	brr[15].x = 480;
	brr[15].y = 130;
	for (int i = 16; i < 28; i++) {
		brr[i].x = 480;
		brr[i].y = 160 + 10 * (i - 16);
	}
	brr[28].x = 480;
	brr[28].y = 300;
	inibrr();
}
//关卡二
void level_2() {
	insbrr();
	brr[0].x = 310;
	brr[0].y = 400;
	for (int i = 1; i < 15; i++) {
		brr[i].x = brr[i - 1].x - 10;
		brr[i].y = brr[i - 1].y - 10;
	}
	brr[20].x = 320;
	brr[20].y = 400;
	for (int i = 21; i < 35; i++) {
		brr[i].x = brr[i - 1].x + 10;
		brr[i].y = brr[i - 1].y - 10;
	}
	brr[40].x = 320;
	brr[40].y = 80;
	for (int i = 41; i < 55; i++) {
		brr[i].x = brr[i - 1].x + 10;
		brr[i].y = brr[i - 1].y + 10;
	}
	brr[60].x = 310;
	brr[60].y = 80;
	for (int i = 61; i < 75; i++) {
		brr[i].x = brr[i - 1].x - 10;
		brr[i].y = brr[i - 1].y + 10;
	}
	brr[80].x = 50;
	brr[80].y = 50;
	for (int i = 81; i < 85; i++) {
		brr[i].x = brr[i - 1].x + 10;
		brr[i].y = brr[i - 1].y ;
	}
	for (int i = 85; i < 90; i++) {
		brr[i].x = brr[i - 1].x ;
		brr[i].y = brr[i - 1].y+10;
	}
	brr[90].x = 640-50;
	brr[90].y = 50;
	for (int i = 91; i < 95; i++) {
		brr[i].x = brr[i - 1].x - 10;
		brr[i].y = brr[i - 1].y;
	}
	for (int i = 95; i < 100; i++) {
		brr[i].x = brr[i - 1].x;
		brr[i].y = brr[i - 1].y + 10;
	}
	brr[100].x = 50;
	brr[100].y = 480-50;
	for (int i = 101; i < 105; i++) {
		brr[i].x = brr[i - 1].x + 10;
		brr[i].y = brr[i - 1].y;
	}
	for (int i = 105; i < 110; i++) {
		brr[i].x = brr[i - 1].x;
		brr[i].y = brr[i - 1].y - 10;
	}
	brr[110].x = 640-50;
	brr[110].y = 480 - 50;
	for (int i = 111; i < 115; i++) {
		brr[i].x = brr[i - 1].x - 10;
		brr[i].y = brr[i - 1].y;
	}
	for (int i = 115; i < 120; i++) {
		brr[i].x = brr[i - 1].x;
		brr[i].y = brr[i - 1].y - 10;
	}
	inibrr();
}
//关卡三
void level_3() 
{
	insbrr();
	brr[0].x = 280;
	brr[0].y = 170;
	for (int i = 1; i < 9; i++) {
		brr[i].x = brr[i - 1].x + 10;
		brr[i].y = brr[i - 1].y ;
	}
	brr[10].x = 280;
	brr[10].y = 310;
	for (int i = 11; i < 19; i++) {
		brr[i].x = brr[i - 1].x + 10;
		brr[i].y = brr[i - 1].y;
	}
	brr[19].x = 240;
	brr[19].y = 210;
	for (int i = 20; i < 26 ; i++) {
		brr[i].x = brr[i - 1].x ;
		brr[i].y = brr[i - 1].y + 10;
	}
	brr[26].x = 400;
	brr[26].y = 210;
	for (int i = 27; i < 33; i++) {
		brr[i].x = brr[i - 1].x;
		brr[i].y = brr[i - 1].y + 10;
	}
	brr[33].x = 150;
	brr[33].y = 120;
	for (int i = 34; i < 40; i++) {
		brr[i].x = brr[i - 1].x+10;
		brr[i].y = brr[i - 1].y ;
	}
	brr[41].x = 150;
	brr[41].y = 130;
	for (int i = 42; i < 47; i++) {
		brr[i].x = brr[i - 1].x;
		brr[i].y = brr[i - 1].y+10;
	}
	brr[47].x = 490;
	brr[47].y = 120;
	for (int i = 48; i < 54; i++) {
		brr[i].x = brr[i - 1].x - 10;
		brr[i].y = brr[i - 1].y;
	}
	brr[55].x = 490;
	brr[55].y = 130;
	for (int i = 56; i < 61; i++) {
		brr[i].x = brr[i - 1].x;
		brr[i].y = brr[i - 1].y + 10;
	}
	brr[62].x = 150;
	brr[62].y = 360;
	for (int i = 63; i < 69; i++) {
		brr[i].x = brr[i - 1].x + 10;
		brr[i].y = brr[i - 1].y;
	}
	brr[70].x = 150;
	brr[70].y = 350;
	for (int i = 71; i < 76; i++) {
		brr[i].x = brr[i - 1].x;
		brr[i].y = brr[i - 1].y - 10;
	}
	brr[77].x = 490;
	brr[77].y = 360;
	for (int i = 78; i < 84; i++) {
		brr[i].x = brr[i - 1].x - 10;
		brr[i].y = brr[i - 1].y;
	}
	brr[85].x = 490;
	brr[85].y = 350;
	for (int i = 86; i < 91; i++) {
		brr[i].x = brr[i - 1].x;
		brr[i].y = brr[i - 1].y - 10;
	}
	brr[91].x = 200;
	brr[91].y = 60;
	for (int i = 92; i < 116; i++) {
		brr[i].x = brr[i - 1].x+10;
		brr[i].y = brr[i - 1].y ;
	}
	brr[116].x = 200;
	brr[116].y = 420;
	for (int i = 117; i < 141; i++) {
		brr[i].x = brr[i - 1].x + 10;
		brr[i].y = brr[i - 1].y;
	}
	brr[141].x = 70;
	brr[141].y = 150;
	for (int i = 142; i < 160; i++) {
		brr[i].x = brr[i - 1].x;
		brr[i].y = brr[i - 1].y + 10;
	}
	brr[160].x = 570;
	brr[160].y = 150;
	for (int i = 161; i < 178; i++) {
		brr[i].x = brr[i - 1].x;
		brr[i].y = brr[i - 1].y + 10;
	}
	inibrr();
}
//小球模式地图
void mapballstyle()
{
	insbrr();
	brr[0].x = 150;
	brr[0].y = 110;
	brr[1].x = 160;
	brr[1].y = 110;
	brr[2].x = 150;
	brr[2].y = 120;
	brr[3].x = 160;
	brr[3].y = 120;

	brr[10].x = 470;
	brr[10].y = 350;
	brr[11].x = 480;
	brr[11].y = 350;
	brr[12].x = 470;
	brr[12].y = 360;
	brr[13].x = 480;
	brr[13].y = 360;

	brr[20].x = 310;
	brr[20].y = 230;
	brr[21].x = 320;
	brr[21].y = 230;
	brr[22].x = 310;
	brr[22].y = 240;
	brr[23].x = 320;
	brr[23].y = 240;

	brr[30].x = 150;
	brr[30].y = 350;
	brr[31].x = 160;
	brr[31].y = 350;
	brr[32].x = 150;
	brr[32].y = 360;
	brr[33].x = 160;
	brr[33].y = 360;

	brr[40].x = 470;
	brr[40].y = 110;
	brr[42].x = 470;
	brr[42].y = 120;
	brr[43].x = 480;
	brr[43].y = 120;
	brr[41].x = 480;
	brr[41].y = 110;
	inibrr();
}
//双蛇模式地图
void maptwostyle()
{
	insbrr();
	brr[0].x = 160;
	brr[0].y = 190;
	for (int i = 1; i < 10; i++) {
		brr[i].x = 160;
		brr[i].y = 190 + 10 * i;
	}

	brr[10].x = 320;
	brr[10].y = 190;
	for (int i = 11; i < 20; i++) {
		brr[i].x = 320;
		brr[i].y = 190 + 10 * (i-10);
	}

	brr[20].x = 480;
	brr[20].y = 190;
	for (int i = 21; i < 30; i++) {
		brr[i].x = 480;
		brr[i].y = 190 + 10 * (i-20);
	}

	brr[30].x = 220;
	brr[30].y = 120;
	for (int i = 31; i < 35; i++) {
		brr[i].x = 220 + 10 * (i-30);
		brr[i].y = 120;
	}

	brr[35].x = 380;
	brr[35].y = 120;
	for (int i = 35; i < 40; i++) {
		brr[i].x = 380 + 10 * (i-34);
		brr[i].y = 120;
	}

	brr[40].x = 220;
	brr[40].y = 360;
	for (int i = 41; i < 45; i++) {
		brr[i].x = 220 + 10 * (i - 40);
		brr[i].y = 360;
	}

	brr[45].x = 380;
	brr[45].y = 360;
	for (int i = 45; i < 50; i++) {
		brr[i].x = 380 + 10 * (i - 44);
		brr[i].y = 360;
	}

	brr[50].x = 60;
	brr[50].y = 120;
	for (int i = 51; i < 55; i++) {
		brr[i].x = 60 + 10 * (i - 50);
		brr[i].y = 120;
	}

	brr[55].x = 540;
	brr[55].y = 120;
	for (int i = 55; i < 60; i++) {
		brr[i].x = 540 + 10 * (i - 54);
		brr[i].y = 120;
	}

	brr[60].x = 60;
	brr[60].y = 360;
	for (int i = 61; i < 65; i++) {
		brr[i].x = 60 + 10 * (i - 60);
		brr[i].y = 360;
	}

	brr[65].x = 540;
	brr[65].y = 360;
	for (int i = 65; i < 70; i++) {
		brr[i].x = 540 + 10 * (i - 64);
		brr[i].y = 360;
	}
	inibrr();
}
//双人模式地图
void maptogetherstyle()
{
	insbrr();
	brr[0].x = 90;
	brr[0].y = 90;
	for (int i = 1; i < 15; i++) {
		brr[i].x = brr[i - 1].x + 10;
		brr[i].y = brr[i - 1].y + 10;
	}

	brr[20].x = 640-90;
	brr[20].y = 480-90;
	for (int i = 21; i < 35; i++) {
		brr[i].x = brr[i - 1].x - 10;
		brr[i].y = brr[i - 1].y - 10;
	}
	
	brr[40].x = 640 - 90;
	brr[40].y = 90;
	for (int i = 41; i < 55; i++) {
		brr[i].x = brr[i - 1].x - 10;
		brr[i].y = brr[i - 1].y + 10;
	}

	brr[60].x = 90;
	brr[60].y = 480 - 90;
	for (int i = 61; i < 75; i++) {
		brr[i].x = brr[i - 1].x + 10;
		brr[i].y = brr[i - 1].y - 10;
	}
	inibrr();
}
//地图动的地图(不加入循环，仅产生一次)
void mapmap()
{
	for (int j = 0; j < 200; j++)
	{

		brr[j].x = rand() % 192 * 10 - 640;
		brr[j].y = rand() % 144 * 10 - 480;
	}
}
/**************************************************///宝藏
//宝藏位置
void trsxy()
{
	treasure.x = rand() % 65 * 10;
	treasure.y = rand() % 48 * 10;
	setfillcolor(RGB(255, 248, 220));
	//fillrectangle(treasure.x, treasure.y, treasure.x + 10, treasure.y + 10);
}
//提示消息
void clue()
{
	setbkmode(0);//文字背景透明
	settextcolor(RGB(54, 54, 54));
	setlinecolor(RGB(54, 54, 54));
	rectangle(295,15,335,40);
	if (abs(snake.xy[0].x - treasure.x) >= abs(snake.xy[0].y - treasure.y)) {
		if (snake.xy[0].x >= treasure.x) {
			outtextxy(300, 20, "向左");
		}
		else {
			outtextxy(300, 20, "向右");
		}
	}
	else {
		if (snake.xy[0].y >= treasure.y) {
			outtextxy(300, 20, "向上");
		}
		else {
			outtextxy(300, 20, "向下");
		}
	}
	text();
}
//吃宝藏
void eattrs() {
	if (snake.xy[0].x == treasure.x && snake.xy[0].y == treasure.y)
	{
		snake.num++;
		score += 50;
		trsxy();
	}
}
/**************************************************///玩家二
//初始化蛇(玩家二)
void insnake2() {
	snake2.xy[2].x = 0;
	snake2.xy[2].y = 200;
	snake2.xy[1].x = 10;
	snake2.xy[1].y = 200;
	snake2.xy[0].x = 20;
	snake2.xy[0].y = 200;
	snake2.num = 3;
	snake2.way = right;
}
//绘制蛇(玩家二)
void inisnake2()
{
	setlinecolor(RGB(106, 90, 205));//SlateBlue
	setfillcolor(RGB(238, 154, 73));//Tan2
	fillellipse(snake2.xy[0].x, snake2.xy[0].y, snake2.xy[0].x + 10, snake2.xy[0].y + 10);
	for (int i = 1; i < snake2.num; i++)
	{
		setlinecolor(RGB(106, 90, 205));//SlateBlue
		setfillcolor(RGB(255, 231, 186));//Wheat1
		fillrectangle(snake2.xy[i].x, snake2.xy[i].y, snake2.xy[i].x + 10, snake2.xy[i].y + 10);
	}
}
//蛇的移动（键盘）(玩家二)
void way2()
{
	//除了第一节蛇，后面每节都是前面一节蛇的前一次坐标
	for (int i = snake2.num - 1; i > 0; i--)
	{
		snake2.xy[i].x = snake2.xy[i - 1].x;
		snake2.xy[i].y = snake2.xy[i - 1].y;
	}
	switch (snake2.way)
	{
	case right:
		snake2.xy[0].x += speed;
		break;
	case left:
		snake2.xy[0].x -= speed;
		break;
	case down:
		snake2.xy[0].y += speed;
		break;
	case up:
		snake2.xy[0].y -= speed;
		break;
	default:
		break;
	}
}
/*
玩家二用英文的大写键盘控制
*/
//蛇的方向控制（键盘）(玩家二)
void snakemove2()
{
	char ch = 0;//按键
	ch = _getch();
	switch (ch)
	{
	//空格暂停
	case 32	:	
		_getch();
		break;
	//玩家二
	case 87 :
		if (snake2.way != down)
			snake2.way = up;
		break;
	case 65 :
		if (snake2.way != right)
			snake2.way = left;
		break;
	case 68:
		if (snake2.way != left)
			snake2.way = right;
		break;
	case 83:
		if (snake2.way != up)
			snake2.way = down;
		break;
	//玩家一
	case 72:
		if (snake.way != down)
			snake.way = up;
		break;
	case 75:
		if (snake.way != right)
			snake.way = left;
		break;
	case 77:
		if (snake.way != left)
			snake.way = right;
		break;
	case 80:
		if (snake.way != up)
			snake.way = down;
		break;
	default:
		break;
	}
}
//判断蛇是否死亡(玩家二)
void snakedie2()
{
	if (snake2.xy[0].x > 640 || snake2.xy[0].x < 0 || snake2.xy[0].y > 480 || snake2.xy[0].y < 0)
	{
		finish();

	}

	for (int i = 1; i < snake2.num; i++)
	{
		if (snake2.xy[0].x == snake2.xy[i].x &&snake2.xy[0].y == snake2.xy[i].y)
		{
			finish();
		}
	}

}
//吃食物的判断(玩家二)
void eatfood2()
{
	//0-3:加10分
	for (int i = 0; i < 4; i++) {
		if (snake2.xy[0].x == food[i].fdxy.x && snake2.xy[0].y == food[i].fdxy.y)
		{
			snake2.num++;
			score += 10;
			score2 += 10;
			food[i].flag = 0;
		}
	}
	//4-5:加15分
	for (int i = 4; i < 6; i++) {
		if (snake2.xy[0].x == food[i].fdxy.x && snake2.xy[0].y == food[i].fdxy.y)
		{
			snake2.num++;
			snake2.num++;
			score += 15;
			score2 += 15;
			food[i].flag = 0;
		}
	}
	//6-7:减速
	if (snake2.xy[0].x == food[6].fdxy.x && snake2.xy[0].y == food[6].fdxy.y)
	{
		food[6].flag = 0;
		speed += 10;
		speedJudge = true;
	}
	if (snake2.xy[0].x == food[7].fdxy.x && snake2.xy[0].y == food[7].fdxy.y)
	{
		food[7].flag = 0;
		speed += 10;
		speedJudge = true;
	}
	//8-9:加速
	if (snake2.xy[0].x == food[8].fdxy.x && snake2.xy[0].y == food[8].fdxy.y)
	{
		food[8].flag = 0;
		speed += 20;
		speedJudge = true;
	}
	if (snake2.xy[0].x == food[9].fdxy.x && snake2.xy[0].y == food[9].fdxy.y)
	{
		food[9].flag = 0;
		speed += 20;
		speedJudge = true;
	}
	//10:炸弹
	if (snake2.xy[0].x == food[10].fdxy.x && snake2.xy[0].y == food[10].fdxy.y)
	{
		food[10].flag = 0;
		finish();
	}
	//11:清空分数
	if (snake2.xy[0].x == food[11].fdxy.x && snake2.xy[0].y == food[11].fdxy.y)
	{
		food[11].flag = 0;
		score = 0;
		score2 = 0;
	}
	//12:清屏
	if (snake2.xy[0].x == food[12].fdxy.x && snake2.xy[0].y == food[12].fdxy.y)
	{
		for (int i = 0; i < 15; i++) {
			food[i].flag = 0;
		}
	}
	//13:蛇身变短-2格
	if (snake2.xy[0].x == food[13].fdxy.x && snake2.xy[0].y == food[13].fdxy.y)
	{
		food[13].flag = 0;
		snake2.num -= 2;
	}
	//14:减少时间-10s
	if (snake2.xy[0].x == food[14].fdxy.x && snake2.xy[0].y == food[14].fdxy.y)
	{
		food[14].flag = 0;
		timeFood -= 10;
	}
}
/**************************************************///双人模式
//两条蛇的碰撞检测
void crash()
{
	for (int i = 0; i < snake.num; i++)
	{
		for (int j = 0; j < snake2.num; j++)
		{
			if (snake.xy[i].x == snake2.xy[j].x && snake.xy[i].y == snake2.xy[j].y)
			{
				finish();
			}
		}
	}
}
//显示信息(双人模式)
void text2()
{
	char grade1[100] = { 0 };
	char grade2[100] = { 0 };
	sprintf(grade1, "%d", score1);
	sprintf(grade2, "%d", score2);
	setbkmode(0);//文字背景透明
	settextstyle(15, 0, _T("宋体"));

	settextcolor(RGB(65, 105, 225));//RoyalBlue
	outtextxy(550, 50, "玩家一：");
	outtextxy(610, 50, grade1);
	outtextxy(550, 65, "玩家二：");
	outtextxy(610, 65, grade2);
}
/**************************************************///同时控制双蛇
//蛇的方向控制（键盘）(玩家二)
void snakemovetwo()
{
	char ch = 0;//按键
	ch = _getch();
	switch (ch)
	{
		//空格暂停
	case 32:
		_getch();
		break;
		//键盘控制双蛇
	case 72:
		if (snake.way != down)
		{
			snake.way = up;
			snake2.way = up;
		}
		break;
	case 75:
		if (snake.way != right)
		{
			snake.way = left;
			snake2.way = left;
		}
		break;
	case 77:
		if (snake.way != left)
		{
			snake.way = right;
			snake2.way = right;
		}
		break;
	case 80:
		if (snake.way != up)
		{
			snake.way = down;
			snake2.way = down;
		}
		break;
	default:
		break;
	}
}
/**************************************************///假蛇
//初始化假蛇
void infakesnake() {
	fakesnake.xy[2].x = 320;
	fakesnake.xy[2].y = 260;
	fakesnake.xy[1].x = 320;
	fakesnake.xy[1].y = 250;
	fakesnake.xy[0].x = 320;
	fakesnake.xy[0].y = 240;
	fakesnake.num = 3;
	fakesnake.way = up;
}
//假蛇的移动
void fakeway()
{
	//除了第一节蛇，后面每节都是前面一节蛇的前一次坐标
	for (int i = fakesnake.num - 1; i > 0; i--)
	{
		fakesnake.xy[i].x = fakesnake.xy[i - 1].x;
		fakesnake.xy[i].y = fakesnake.xy[i - 1].y;
	}
	switch (fakesnake.way)
	{
	case right:
		fakesnake.xy[0].x += speed;
		break;
	case left:
		fakesnake.xy[0].x -= speed;
		break;
	case down:
		fakesnake.xy[0].y += speed;
		break;
	case up:
		fakesnake.xy[0].y -= speed;
		break;
	default:
		break;
	}
}
//假蛇的方向控制
void fakesnakemove()
{
	char ch = 0;
	ch = _getch();
	switch (ch)
	{
	case 72:
		if (snake.way != down)
		{
			snake.way = up;
			fakesnake.way = up;
		}
		break;
	case 75:
		if (snake.way != right)
		{
			snake.way = left;
			fakesnake.way = left;
		}
		break;
	case 77:
		if (snake.way != left)
		{
			snake.way = right;
			fakesnake.way = right;
		}		
		break;
	case 80:
		if (snake.way != up)
		{
			snake.way = down;
			fakesnake.way = down;
		}
		break;
		//空格暂停
	case 32:
		_getch();
		break;
	default:
		break;
	}
}
//利用假蛇绘制真蛇
void inirealsnake()
{
	setlinecolor(RGB(106, 90, 205));//SlateBlue
	setfillcolor(RGB(250, 128, 114));//Violet
	fillellipse(snake.xy[0].x, snake.xy[0].y, snake.xy[0].x + 10, snake.xy[0].y + 10);
	fakeX = fakesnake.xy[0].x - snake.xy[0].x;
	fakeY = fakesnake.xy[0].y - snake.xy[0].y;
	for (int i = 1; i < snake.num; i++)
	{
		setlinecolor(RGB(106, 90, 205));//SlateBlue
		setfillcolor(RGB(255, 228, 225));//MistyRose
		fillrectangle(fakesnake.xy[i].x - fakeX, fakesnake.xy[i].y - fakeY, fakesnake.xy[i].x + 10 - fakeX, fakesnake.xy[i].y + 10 - fakeY);
	}
}
/**************************************************///地图移动
//地图移动
void mapway()
{
	
	switch (snake.way)
	{
	case right:
		for (int i = 0; i<200; i++)
		{
			brr[i].x -= speed;
		}
		for (int i = 0; i < 15; i++)
		{
			food[i].fdxy.x -= speed;
		}
		for (int i = 0; i < 700; i++)
		{
			wall[i].x -= speed;
		}
		break;
	case left:
		for (int i = 0; i < 200; i++)
		{
			brr[i].x += speed;
		}
		for (int i = 0; i < 15; i++)
		{
			food[i].fdxy.x += speed;
		}
		for (int i = 0; i < 700; i++)
		{
			wall[i].x += speed;
		}
		break;
	case down:
		for (int i = 0; i < 200; i++)
		{
			brr[i].y -= speed;
		}
		for (int i = 0; i < 15; i++)
		{
			food[i].fdxy.y -= speed;
		}
		for (int i = 0; i < 700; i++)
		{
			wall[i].y -= speed;
		}
		break;
	case up:
		for (int i = 0; i < 200; i++)
		{
			brr[i].y += speed;
		}
		for (int i = 0; i < 15; i++)
		{
			food[i].fdxy.y += speed;
		}
		for (int i = 0; i < 700; i++)
		{
			wall[i].y += speed;
		}
		break;
	default:
		break;
	}
}
//地图移动蛇的位置
void mapsnakexy()
{
	if (snake.way == up)
	{
		for (int i = snake.num - 1; i > 0; i--)
		{
			snake.xy[i].x = snake.xy[i - 1].x;
			snake.xy[i].y = snake.xy[i - 1].y;
		}
		for (int i = 1; i < snake.num; i++)
		{
			snake.xy[i].x = snake.xy[0].x;
			snake.xy[i].y = snake.xy[0].y + 10 * i;
		}
	}
	if (snake.way == down)
	{
		for (int i = 1; i < snake.num; i++)
		{
			snake.xy[i].x = snake.xy[0].x;
			snake.xy[i].y = snake.xy[0].y - 10 * i;
		}
	}
	if (snake.way == left)
	{
		for (int i = 1; i < snake.num; i++)
		{
			snake.xy[i].x = snake.xy[0].x + 10 * i;
			snake.xy[i].y = snake.xy[0].y;
		}
	}
	if (snake.way == right)
	{
		for (int i = 1; i < snake.num; i++)
		{
			snake.xy[i].x = snake.xy[0].x - 10 * i;
			snake.xy[i].y = snake.xy[0].y;
		}
	}
}
//地图移动食物坐标获取
void mapfoodxy()
{
	//蛇吃到食物，食物消失
	for (int i = 0; i < 15; i++)
	{
		if (food[i].flag == 0)
		{
			food[i].fdxy.x = -1000;
			food[i].fdxy.y = -1000;
		}
	}
	//食物随机出现和消失
	if (coolTime == -1) {
		for (int j = 0; j < 15; j++)
		{
			food[j].fdxy.x = -1000;
			food[j].fdxy.y = -1000;
		}
	}
	if (coolTime < 100 && coolTime>0) {
		for (int j = 0; j < 15; j++)
		{
			if (int k = rand() % 150 == 1) {
				food[j].fdxy.x = rand() % 65 * 10;
				food[j].fdxy.y = rand() % 48 * 10;
				food[j].flag = 1;

				for (int i = 0; i > snake.num; i++)
				{
					if (food[j].fdxy.x == snake.xy[i].x && food[j].fdxy.y == snake.xy[i].y)
					{
						food[j].fdxy.x = rand() % 192 * 10-640;
						food[j].fdxy.y = rand() % 144 * 10-480;

					}
				}
			}
		}
	}
	if (coolTime > 100 && coolTime < 200) {
		for (int j = 0; j < 15; j++)
		{
			if (int k = rand() % 150 == 1) {
				food[j].fdxy.x = -100;
				food[j].fdxy.y = -100;
			}
		}
	}
	if (coolTime > 200) {
		coolTime = 0;
	}
}
//大地图边界的位置
void mapwallxy()
{
	//初始化
	for (int i = 0; i < 700; i++)
	{
			wall[i].x = -1000;
			wall[i].y = -1000;
	}
	//大地图边界
	wall[0].x = -640;
	wall[0].y = -480;
	for (int i = 1; i < 192; i++)
	{
		wall[i].x = wall[i-1].x+10;
		wall[i].y = wall[i-1].y;
	}
	for (int i = 192; i < 335; i++)
	{
		wall[i].x = wall[i - 1].x ;
		wall[i].y = wall[i - 1].y+10;
	}
	for (int i = 335; i < 526; i++)
	{
		wall[i].x = wall[i - 1].x-10;
		wall[i].y = wall[i - 1].y ;
	}
	for (int i = 526; i < 669; i++)
	{
		wall[i].x = wall[i - 1].x;
		wall[i].y = wall[i - 1].y - 10;
	}
}
//绘制大地图边界
void iniwall()
{
	for (int i = 0; i < 700; i++) {
		if (flash == 0) {
			flash = 1;
			setfillcolor(RGB(255, 255, 255));
		}
		else {
			flash = 0;
			setfillcolor(RGB(0, 0, 0));
		}
		fillrectangle(wall[i].x, wall[i].y, wall[i].x + 10, wall[i].y + 10);
	}
}
//边界碰撞检测
void bompwall()
{
	for (int i = 0; i < 700; i++)
	{
		if (snake.xy[0].x == wall[i].x && snake.xy[0].y == wall[i].y)
		{	
			finish();
		}
	}
}
//利用假蛇判断真蛇是否咬到自己死亡
void mapsnakedie()
{
	for (int i = 1; i < fakesnake.num; i++)
	{
		if (fakesnake.xy[0].x == fakesnake.xy[i].x &&fakesnake.xy[0].y == fakesnake.xy[i].y)
		{
			finish();
		}
	}
}
/**************************************************///字母模式
/*
每个字母对应0-26，以数字形式存在数组里。
判断时只要判断两个数组的每个值是否相同，仅绘制时需要用到字母。
当蛇吃到其他字母时，吃的所有字母消失。
*/
//字母初始化
void inletter()
{
	for (int i = 0; i < 26; i++)
	{
		letter[i].flag = 0;
	}
	for (int i = 0; i < 15; i++)
	{
		snaletter.num[i] = 0;
	}
}
//字母坐标获取
void letterxy()
{
	//蛇吃到食物，食物消失
	for (int i = 0; i < 26; i++)
	{
		if (letter[i].flag == 0)
		{
			letter[i].fdxy.x = -1000;
			letter[i].fdxy.y = -1000;
		}
	}
	//食物随机出现和消失
	if (coolTime == -1) {
		for (int j = 0; j <26; j++)
		{
			letter[j].fdxy.x = -1000;
			letter[j].fdxy.y = -1000;
		}
	}
	if (coolTime < 20 && coolTime>0) {
		for (int j = 0; j < 26; j++)
		{
			if (int k = rand() % 100 == 1) {
				letter[j].fdxy.x = rand() % 65 * 10;
				letter[j].fdxy.y = rand() % 48 * 10;
				letter[j].flag = 1;

				for (int i = 0; i > snake.num; i++)
				{
					if (letter[j].fdxy.x == snake.xy[i].x && letter[j].fdxy.y == snake.xy[i].y)
					{
						letter[j].fdxy.x = rand() % 65 * 10;
						letter[j].fdxy.y = rand() % 48 * 10;

					}
				}
			}
		}
	}
	if (coolTime > 20 && coolTime < 40) {
		for (int j = 0; j < 15; j++)
		{
			if (int k = rand() % 100 == 1) {
				letter[j].fdxy.x = -100;
				letter[j].fdxy.y = -100;
			}
		}
	}
	if (coolTime > 40) {
		coolTime = 0;
	}
}
//绘制字母
void iniletter()
{
	setbkmode(0);//文字背景透明
	settextcolor(RGB(0, 0, 0));//RoyalBlue
	for (int i = 0; i < 26; i++)
	{		
		outtextxy(letter[i].fdxy.x, letter[i].fdxy.y, 'a'+i);
	}
}
//吃字母
void eatletter()
{
	for (int i = 0; i < 26; i++) {
		if (snake.xy[0].x == letter[i].fdxy.x && snake.xy[0].y == letter[i].fdxy.y)
		{
			snake.num++;
			letter[i].flag = 0;
			snaletter.num[snaletternum] = i+1;

			snaletternum += 1;
		}
	}
}
/*
1-26
a-z
分别对应
*/
//初始化字母库
void library()
{
	for (int i = 0; i < 15; i++)
	{
		ltlibrary.num[i] = 0;
	}
	ltlibrary.num[0] = 26;
	ltlibrary.num[1] = 9;
	ltlibrary.num[2] = 16;
	ltlibrary.num[3] = 5;
	ltlibrary.num[4] = 9;
	ltlibrary.num[5] = 23;
	ltlibrary.num[6] = 5;
	ltlibrary.num[7] = 14;
}
//字母与字母库判断
void same()
{
	if (snaletter.num[7] != 0)
	{
		for (int i = 0; i < 8; i++)
		{
			if (ltlibrary.num[i] != snaletter.num[i])
			{
				lettersame = false;
			}
			else 
			{
				lettersame = true;
			}
		}
	}
	for (int i = 0; i < sizeof(snaletter.num); i++)
	{
		//吃到错误字母，清空
		if (ltlibrary.num[i] != snaletter.num[i])
		{
			for (int i = 0; i < 15; i++)
			{
				snaletter.num[i] = 0;
			}
		}
	}
	if (lettersame == true)
	{
		success();
		finish();
	}
}
//显示信息
void lettertext()
{
	settextstyle(15, 0, _T("宋体"));
	setbkmode(0);//文字背景透明
	//提示信息
	settextcolor(RGB(224, 255, 255));//LightCyan
	outtextxy(5, 440, "键盘的方向键控制");
	outtextxy(5, 460, "暂停：空格/继续：任意键");
	//记录时间
	stop = clock();
	duration = (int)(stop - start) / CLK_TCK + timeFood; //CLK_TCK为clock()函数的时间单位，即时钟打点
	char s[5];
	sprintf(s, "%d", duration);
	outtextxy(570, 10, "时间：");
	outtextxy(610, 10, s);
	//姓名
	settextcolor(RGB(224, 255, 255));//LightCyan
	outtextxy(5, 10, "欢迎进入贪吃蛇大作战");
	outtextxy(5, 40, name);
	//单词
	outtextxy(280, 10, "需要吃的单词是：jipeiwen");
	//吃到的字母
	for (int i = 0; i < sizeof(snaletter.num); i++)
	{
		outtextxy(280+5*i, 40, "a"+snaletter.num[i]-1);
	}
}
/**************************************************///模式类型
/*
模式：
1.	自由模式
撞墙、咬到自己死亡
2.	闯关模式
3个关卡、有障碍、满100分进入下一关
3.	创意模式（可自由发挥）
（1）小球控制
键盘方向键控制小球运动，蛇追小球
（2）寻宝模式
宝藏不能看见，按正上方提示运动，吃到宝藏加10分
（3）小球控制（鼠标）
用鼠标控制小球运动，蛇追小球
（4）双蛇舞动
键盘方向键可以同时控制两条蛇运动
（5）字母模式
（6）随机模式
4. 地图动
5. 共生模式（双人）
两条蛇共同游戏，不能相互碰撞，道具共用
玩家一：用键盘的方向键控制；粉色小蛇
玩家二：用WASD控制；黄色小蛇

*/
//自由模式
void freeStyle()
{

	insnake();
	start = clock();
	while(game && (!reBegin))
	{
		if (speedJudge) {
			speedTime++;
			if (speedTime == 50) {
				speedJudge = false;
				speed = 10;
				speedTime = 0;
			}
		}
		coolTime++;
		cleardevice();
		foodxy();
		text();
		inifood();
		inisnake();
		snakedie();
		eatfood();
		way();
		Sleep(100);
		while (_kbhit())//这里是按上下左右键控制蛇的移动
		{
				//again();
				snakemove();	
		}
	}
	closegraph();
}
//闯关模式-1(闯关模式集成)
void levelStyle()
{
	insnake();
	start = clock();
	while (game && (!reBegin))
	{
		if (score >= 100)
		{
			change();
			score = 0;
			levelStyle_2();
		}
		if (speedJudge) {
			speedTime++;
			if (speedTime == 50) {
				speedJudge = false;
				speed = 10;
				speedTime = 0;
			}
		}
		coolTime++;
		cleardevice();
		level_1();
		bomp();
		foodxy();
		text();
		inifood();
		inisnake();
		snakedie();
		eatfood();
		way();
		Sleep(100);
		while (_kbhit())//这里是按上下左右键控制蛇的移动
		{
			//again();
			snakemove();
		}
	}
	closegraph();
}
//闯关模式-2
void levelStyle_2()
{
	insnake();
	start = clock();
	while (game && (!reBegin))
	{
		if (score >= 100) 
		{
			change();
			score = 0;
			levelStyle_3();
		}
		if (speedJudge) {
			speedTime++;
			if (speedTime == 50) {
				speedJudge = false;
				speed = 10;
				speedTime = 0;
			}
		}
		coolTime++;
		cleardevice();
		level_2();
		bomp();
		foodxy();
		text();
		inifood();
		inisnake();
		snakedie();
		eatfood();
		way();
		Sleep(75);
		while (_kbhit())//这里是按上下左右键控制蛇的移动
		{
			//again();
			snakemove();
		}
	}
	closegraph();
}
//闯关模式-3
void levelStyle_3()
{
	insnake();
	start = clock();
	while (game && (!reBegin))
	{
		if (score >= 100)
		{
			success();
			finish();
		}
		if (speedJudge) {
			speedTime++;
			if (speedTime == 50) {
				speedJudge = false;
				speed = 10;
				speedTime = 0;
			}
		}
		coolTime++;
		cleardevice();
		level_3();
		bomp();
		foodxy();
		text();
		inifood();
		inisnake();
		snakedie();
		eatfood();
		way();
		Sleep(50);
		while (_kbhit())//这里是按上下左右键控制蛇的移动
		{
			//again();
			snakemove();
		}
	}
	closegraph();
}
//创意模式-小球控制
void ballStyle() 
{
	insnake();
	insball();
	start = clock();
	while (game && (!reBegin))
	{	
		if (speedJudge) {
			speedTime++;
			if (speedTime == 50) {
				speedJudge = false;
				speed = 10;
				speedTime = 0;
			}
		}
		coolTime++;
		cleardevice();
		mapballstyle();
		bomp();
		foodxy();
		text();
		inifood();
		inisnake();
		iniball();
		wayball();
		way();
		sbdie();
		ballwall();
		eatfood();
		Sleep(100);
		while (_kbhit())//这里是按上下左右键控制蛇的移动
		{
			//again();
			ballmove();
			sbmove();
		}
	}
	closegraph();
}
//创意模式-寻宝模式
void seekStyle()
{
	insnake();
	trsxy();
	start = clock();
	while (game && (!reBegin))
	{
		cleardevice();
		//fillrectangle(treasure.x, treasure.y, treasure.x + 10, treasure.y + 10);
		clue();
		inisnake();
		snakedie();
		eattrs();
		way();
		Sleep(100);
		while (_kbhit())//这里是按上下左右键控制蛇的移动
		{
			//again();
			snakemove();
		}
	}
	closegraph();
}
//创意模式-小球控制(鼠标)
void ballStyleMS()
{
	insnake();
	insball();
	start = clock();
	speed = 1;
	while (game && (!reBegin))
	{
		if (speedJudge) {
			speedTime++;
			if (speedTime == 50) {
				speedJudge = false;
				speed = 10;
				speedTime = 0;
			}
		}
		coolTime++;
		cleardevice();
		inisnake();
		iniball();
		ballmoveMS();
		wayMS();
		sbmove();
		sbdie();
		ballwall();
		while (_kbhit())//这里是按上下左右键控制蛇的移动
		{
			again();			
			sbmove();
			//snakemove();
		}
	}
	closegraph();
}
//创意模式-双蛇舞动
void twoStyle()
{
	insnake();
	insnake2();
	snake2.xy[0].x = 20;
	snake2.xy[0].y = 100;
	start = clock();
	while (game && (!reBegin))
	{
		if (speedJudge) {
			speedTime++;
			if (speedTime == 50) {
				speedJudge = false;
				speed = 10;
				speedTime = 0;
			}
		}
		coolTime++;
		cleardevice();
		maptwostyle();
		bomp();
		bomp2();
		foodxy();
		text();
		//text2();
		inifood();
		inisnake();
		inisnake2();
		snakedie();
		snakedie2();
		//crash();
		eatfood();
		eatfood2();
		way();
		way2();
		Sleep(100);
		while (_kbhit())//这里是按上下左右键控制蛇的移动
		{
			//again();
			//snakemove();
			snakemovetwo();
		}
	}
	closegraph();
}
//创意模式-地图动
void mapStyle()
{
	insnake();
	infakesnake();
	snake.xy[0].x = 320;
	snake.xy[0].y = 240;
	snake.way = up;
	mapwallxy();
	mapmap();
	start = clock();
	//level_1();
	while (game && (!reBegin))
	{
		if (speedJudge) {
			speedTime++;
			if (speedTime == 50) {
				speedJudge = false;
				speed = 10;
				speedTime = 0;
			}
		}
		coolTime++;
		cleardevice();
		mapfoodxy();
		text();
		inifood();
		inibrr();
		mapsnakexy();
		inirealsnake();
		iniwall();
		bompwall();
		bomp();
		eatfood();
		mapway();
		fakeway();
		mapsnakedie();
		Sleep(100);
		while (_kbhit())//这里是按上下左右键控制蛇的移动
		{
			//again();
			fakesnakemove();
		}
	}
	closegraph();
}
//共生模式（双人）
void togetherStyle()
{
	insnake();
	insnake2();
	snake2.xy[0].x = 20;
	snake2.xy[0].y = 470;
	start = clock();
	while (game && (!reBegin))
	{
		if (speedJudge) {
			speedTime++;
			if (speedTime == 50) {
				speedJudge = false;
				speed = 10;
				speedTime = 0;
			}
		}
		coolTime++;
		cleardevice();
		maptogetherstyle();
		bomp();
		bomp2();
		foodxy();
		text();
		text2();
		inifood();
		inisnake();
		inisnake2();
		snakedie();
		snakedie2();
		crash();
		eatfood();
		eatfood2();
		way();
		way2();
		Sleep(100);
		while (_kbhit())//这里是按上下左右键控制蛇的移动
		{
			//again();
			//snakemove();
			snakemove2();
		}
	}
	closegraph();
}
//字母模式
void letterStyle()
{

	insnake();
	inletter();
	library();
	start = clock();
	while (game && (!reBegin))
	{
		if (speedJudge) {
			speedTime++;
			if (speedTime == 50) {
				speedJudge = false;
				speed = 10;
				speedTime = 0;
			}
		}
		coolTime++;
		cleardevice();
		same();//游戏成功判断
		letterxy();
		lettertext();
		iniletter();
		inisnake();
		snakedie();
		eatletter();
		way();
		Sleep(100);
		while (_kbhit())//这里是按上下左右键控制蛇的移动
		{
			//again();
			snakemove();
		}
	}
	closegraph();
}
//随机模式
void randomStyle()
{
	int i = rand() % 5;
	switch(i)
	{
		case 0:
			ballStyle();
			break;
		case 1:
			seekStyle();
			break;
		case 2:
			ballStyleMS();
			break;
		case 3:
			twoStyle();
			break;
		case 4:
			letterStyle();
			break;
		default:
			break;
}
}
/**************************************************///技能
/*
1. Enter：加50分（用于闯关模式）—见snakemove()
2. Tab：宝藏可以看见,并停顿一秒（用于寻宝模式）—见snakemove()
*/


