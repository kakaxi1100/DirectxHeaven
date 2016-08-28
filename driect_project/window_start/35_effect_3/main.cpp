#include <Windows.h>
#include <time.h>
#include <wchar.h>
#include <stdio.h>
#include <math.h>
#include <chrono>

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
struct Ball
{
	float x, y, angle;
	float w;
	float v;
	bool isLive;
};

//全局变量声明
HDC g_hdc = NULL, g_mdc = NULL, g_bdc = NULL;
HBITMAP g_hBackground, g_hBall;
DWORD g_tPre = 0, g_tNow = 0;
Ball balls[100] = {0};
float g_angle = 0.0f;
std::chrono::time_point<std::chrono::system_clock> tick;

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
			//这里处理游戏tick
			auto diff = std::chrono::system_clock::now() - tick;

			if (std::chrono::duration<double, std::milli>(diff).count() >= 33)
			{
				Game_Paint(hwnd);
				g_tPre = g_tNow;
				tick = std::chrono::system_clock::now();
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
	g_hBackground = (HBITMAP)LoadImage(NULL, L"2.bmp", IMAGE_BITMAP, 640, 480, LR_LOADFROMFILE);
	g_hBall = (HBITMAP)LoadImage(NULL, L"1.bmp", IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);

	//2.建立兼容的DC
	g_mdc = CreateCompatibleDC(g_hdc);//建立兼容设备环境的内存DC, 参数是与哪个设备兼容
	g_bdc = CreateCompatibleDC(g_hdc);//建立后备缓冲区
	bmp = CreateCompatibleBitmap(g_hdc, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(g_mdc, bmp);//需要先给 mdc 一张画布

	for (int i = 0; i < 100; ++i)
	{
		Ball ball;
		ball.angle = 0;
		ball.x = 320 - 32;
		ball.y = 240 - 32;
		ball.w = 0;
		ball.v = 10;
		ball.isLive = false;
		balls[i] = ball;
	}
	return TRUE;
}

Ball* getBall()
{
	for (int i = 0; i < 100; ++i)
	{
		if (balls[i].isLive == false)
		{
			return &balls[i];
		}
	}
	return NULL;
}

VOID Game_Paint(HWND hwnd)
{

	//3.选用位图对象
	SelectObject(g_bdc, g_hBackground);
	//4.贴图 背景
	BitBlt(g_mdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_bdc, 0, 0, SRCCOPY);

	//球
	SelectObject(g_bdc, g_hBall);

	Ball* ball = getBall();
	if (ball != NULL)
	{
		ball->isLive = true;
		g_angle += 0.2;
		ball->angle = g_angle;
		ball->x = 320 - 32;
		ball->y = 240 - 32;
	}

	for (int i = 0; i < 100; i++)
	{
		if (balls[i].isLive == true)
		{
			balls[i].x += balls[i].v * cosf(balls[i].angle);
			balls[i].y += balls[i].v * sinf(balls[i].angle);
			TransparentBlt(g_mdc, balls[i].x, balls[i].y, 64, 64, g_bdc, 0, 0, 64, 64, RGB(0, 0, 0));	
		}
		if (balls[i].x <= -32 || balls[i].x >= 640 || balls[i].y <= -32 || balls[i].y >= 480)
		{
			balls[i].isLive = false;
		}
	}
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
