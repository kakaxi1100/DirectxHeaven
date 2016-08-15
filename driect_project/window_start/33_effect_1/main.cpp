#include <Windows.h>
#include <time.h>
#include <wchar.h>
#include <stdio.h>
#include <math.h>
//添加依赖
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "Msimg32.lib")

//定义宏
#define WINDOW_WIDTH 650
#define WINDOW_HEIGHT 510
#define WINDOW_TITLE L"Random Pop!"
#define MAX_COINS 100
#define ACC_Y 0.5f
#define BASE_VEL 17.0f
#define SPEED_X 3
#define SPEED_Y 1.5
#define PI 3.14159265f

//结构体
struct Coin
{
	bool isActive;
	float x, y, vx, vy;
};

//全局变量声明
HDC g_hdc = NULL, g_mdc = NULL, g_bdc = NULL;
HBITMAP g_hBackground, g_hCoin;
DWORD g_tPre = 0, g_tNow = 0;
Coin Coins[MAX_COINS];

int count = 0;

//函数声明
//处理消息函数
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

//Game Flow 函数
BOOL Game_Init(HWND hwnd);
VOID Game_Paint(HWND hwnd);
BOOL Game_Cleanup(HWND hwnd);

//主函数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//1.设计一个完整的窗口类
	WNDCLASSEX wndClass = { 0 };//定义一个窗口类
	wndClass.cbSize = sizeof(WNDCLASSEX);//设置结构体的大小
	wndClass.style = CS_HREDRAW | CS_VREDRAW;//设置窗口样式
	wndClass.lpfnWndProc = WndProc;//设置消息处理函数
	wndClass.cbClsExtra = 0;//窗口类的附加内存，一般设置为0
	wndClass.cbWndExtra = 0;//窗口的附加内存，一般设置为0
	wndClass.hInstance = hInstance;//包含窗体实例的程序的句柄
	wndClass.hIcon = (HICON)LoadImage(NULL, L"icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);//设置一个图标
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);//指定一个灰色画刷句柄
	wndClass.lpszClassName = NULL;//不需要下拉菜单
	wndClass.lpszClassName = L"HWWND";//指定窗口类的名字

	//2.注册窗口类
	if (!RegisterClassEx(&wndClass)) return -1;

	//3.创建窗体
	HWND hwnd = CreateWindow(L"HWWND", WINDOW_TITLE, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 
		CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

	//4.窗体移动，显示和更新
	//MoveWindow(hwnd, 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, true);
	ShowWindow(hwnd, nShowCmd);
	//UpdateWindow(hwnd);

	//初始化游戏资源
	if (!Game_Init(hwnd))
	{
		MessageBox(hwnd, L"Faild!", L"Message", 0);
		return FALSE;
	}
	//PlaySound(L"仙剑·温慧.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

	//5.消息分发
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);//这个会把消息发送到winproc处理函数
		}
		else
		{
			//会被阻塞（拖动窗体试试 和 GDI_7对比下）
			g_tNow = GetTickCount();
			if (g_tNow - g_tPre > 1)
			{
				Game_Paint(hwnd);
				g_tPre = g_tNow;
			}
		}
	}

	//6.注销窗体
	UnregisterClass(L"HWWND", hInstance);

	return 0;
}

//消息处理函数
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;
	switch (message)
	{
	//case WM_PAINT:
	//	//BeginPaint(hwnd, &paintStruct);
	//	Game_Paint(hwnd);
	//	//EndPaint(hwnd, &paintStruct);
	//	break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			DestroyWindow(hwnd);//销毁窗体并发送一个WM_DESTROY消息
		}
		break;
	case WM_DESTROY:
		Game_Cleanup(hwnd);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

BOOL Game_Init(HWND hwnd)
{
	HBITMAP bmp;

	srand((unsigned)time(NULL));
	g_hdc = GetDC(hwnd);

	//1.加载位图
	g_hBackground = (HBITMAP)LoadImage(NULL, L"4.bmp", IMAGE_BITMAP, 640, 480, LR_LOADFROMFILE);
	g_hCoin = (HBITMAP)LoadImage(NULL, L"3.bmp", IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);

	//2.建立兼容的DC
	g_mdc = CreateCompatibleDC(g_hdc);//建立兼容设备环境的内存DC, 参数是与哪个设备兼容
	g_bdc = CreateCompatibleDC(g_hdc);//建立后备缓冲区
	bmp = CreateCompatibleBitmap(g_hdc, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(g_mdc, bmp);//需要先给 mdc 一张画布

	for (int i = 0; i < MAX_COINS; ++i)
	{
		Coins[i].isActive = false;
	}

	return TRUE;
}

VOID Game_Paint(HWND hwnd)
{	
	float fRand_r, fRand_t;
	//3.选用位图对象
	SelectObject(g_bdc, g_hBackground);
	//4.贴图
	BitBlt(g_mdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_bdc, 0, 0, SRCCOPY);

	SelectObject(g_bdc, g_hCoin);
	for (int i = 0; i < MAX_COINS; ++i)
	{
		//运动
		if (Coins[i].isActive == true)
		{
			Coins[i].x += Coins[i].vx;
			Coins[i].y += Coins[i].vy;
			Coins[i].vy += ACC_Y;
			//超出画面后无效
			if (Coins[i].x >= 640 || Coins[i].x <= -64 || Coins[i].y <= -64 || Coins[i].y >= 480)
			{
				Coins[i].isActive = false;
			}
			
		}
		//重置
		if (Coins[i].isActive == false)
		{
			Coins[i].isActive = true;
			Coins[i].x = (640 - 64)*0.5;
			Coins[i].y = 480 - 64;

			//应用的随机方法不同，产生的随机效果也不同，在第一次的时候非常明显

			//1 包含浮点数的随机
			/*Coins[i].vx = rand()*SPEED_X / (float)RAND_MAX - SPEED_X*0.5;
			Coins[i].vy = rand()*SPEED_Y / (float)RAND_MAX - SPEED_Y*0.5 - BASE_VEL;*/

			//2 用除余得到整数的随机
			/*Coins[i].vx = rand() % (SPEED_X + 1) - SPEED_X*0.5;
			Coins[i].vy = rand() % (SPEED_Y + 1) - SPEED_Y*0.5 - BASE_VEL;*/

			//3 正态分布随机
			fRand_r = sqrtf(-2.0f * logf((float)(rand() + 1) / (RAND_MAX + 1)));	// √-2ln(a)
			fRand_t = 2.0f * PI * (float)rand() / RAND_MAX;								// 2πb
			Coins[i].vx = (fRand_r * cosf(fRand_t)) * SPEED_X;				// 随机设置Vx的初始值
			Coins[i].vy = (fRand_r * sinf(fRand_t)) * SPEED_Y - BASE_VEL;	// 随机设置Vy的初始值
		}
		TransparentBlt(g_mdc, Coins[i].x, Coins[i].y, 64, 64, g_bdc, 0, 0, 64, 64, RGB(0, 0, 0));
	}
	/*if (count % 2 == 0) {
		for (int i = 0; i < MAX_COINS; ++i)
		{
			//重置
			if (Coins[i].isActive == false)
			{
			Coins[i].isActive = true;
			Coins[i].x = (640 - 64)*0.5;
			Coins[i].y = 480 - 64;
			Coins[i].vx = rand()*SPEED_X / (float)RAND_MAX - SPEED_X*0.5;
			Coins[i].vy = rand()*SPEED_Y / (float)RAND_MAX - SPEED_Y*0.5 - BASE_VEL;
			}
		}
	}
	count++;*/
	BitBlt(g_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_mdc, 0, 0, SRCCOPY);
}

BOOL Game_Cleanup(HWND hwnd)
{
	//5.删除创建的DC和位图资源
	DeleteObject(g_hBackground);
	DeleteObject(g_bdc);
	DeleteObject(g_mdc);
	ReleaseDC(hwnd, g_hdc);
	return TRUE;
}
