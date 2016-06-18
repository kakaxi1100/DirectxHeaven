#include <Windows.h>
#include <time.h>
#include <wchar.h>
#include <stdio.h>

//添加依赖
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "Msimg32.lib")

//定义宏
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE L"Hello World!"

//定义sprite结构体
struct Sprites
{
	int x, y;
	int exist;
};

//全局变量声明
HDC g_hdc = NULL, g_mdc = NULL, g_bdc = NULL;
HBITMAP g_hHero, g_hSwordBlade, g_hBackground;
DWORD g_tPre = 0, g_tNow = 0;
Sprites bullets[30];

int g_iX, g_iY, g_iBulletNum;
int g_iBackgroundX;

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
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//指定一个灰色画刷句柄
	wndClass.lpszClassName = NULL;//不需要下拉菜单
	wndClass.lpszClassName = L"HWWND";//指定窗口类的名字

									  //2.注册窗口类
	if (!RegisterClassEx(&wndClass)) return -1;

	//3.创建窗体
	HWND hwnd = CreateWindow(L"HWWND", WINDOW_TITLE, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

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
	PlaySound(L"仙剑三·原版战斗3.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

	//5.消息分发
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);//这个会把消息发送到winproc处理函数
		}
		//会被阻塞（拖动窗体试试 和 GDI_7对比下）
		g_tNow = GetTickCount();
		if (g_tNow - g_tPre >= 5)
		{
			Game_Paint(hwnd);
			g_tPre = g_tNow;
		}
	}

	//6.注销窗体
	UnregisterClass(L"HWWND", hInstance);

	return 0;
}

//消息处理函数
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//PAINTSTRUCT paintStruct;
	switch (message)
	{
	case WM_TIMER:
		Game_Paint(hwnd);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			DestroyWindow(hwnd);//销毁窗体并发送一个WM_DESTROY消息
		}
		break;
	case WM_LBUTTONDOWN:
		for (int i = 0; i < 30; i++)
		{
			if (!bullets[i].exist)
			{
				bullets[i].exist = 1;
				bullets[i].x = g_iX;
				bullets[i].y = g_iY;
				g_iBulletNum++;
				break;
			}
		}
		break;
	case WM_MOUSEMOVE:
		g_iX = LOWORD(lParam);
		g_iY = HIWORD(lParam);

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
	g_hBackground = (HBITMAP)LoadImage(NULL, L"bg.bmp", IMAGE_BITMAP, WINDOW_WIDTH, WINDOW_HEIGHT, LR_LOADFROMFILE);
	g_hHero = (HBITMAP)LoadImage(NULL, L"swordman.bmp", IMAGE_BITMAP, 317, 283, LR_LOADFROMFILE);
	g_hSwordBlade = (HBITMAP)LoadImage(NULL, L"swordblade.bmp", IMAGE_BITMAP, 100, 26, LR_LOADFROMFILE);
	//2.建立兼容的DC
	g_mdc = CreateCompatibleDC(g_hdc);//建立兼容设备环境的内存DC, 参数是与哪个设备兼容
	g_bdc = CreateCompatibleDC(g_hdc);//建立后备缓冲区
	bmp = CreateCompatibleBitmap(g_hdc, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(g_mdc, bmp);//需要先给 mdc 一张画布

	g_iX = 300;
	g_iY = 100;
	g_iBulletNum = 0;

	POINT pt, lt, rb;
	RECT rect;
	pt.x = g_iX;
	pt.y = g_iY;
	ClientToScreen(hwnd, &pt);//窗口坐标转化为屏幕坐标
	SetCursorPos(pt.x, pt.y);//设置鼠标再屏幕的位置
							 //ShowCursor(false);//隐藏光标
							 //限制鼠标移动区域
	GetClientRect(hwnd, &rect);//取得客户区矩形区域
							   //GetWindowRect(hwnd, &rect);//取得窗口矩形区域, 这里得到的rect就是屏幕坐标，所以用这个的时候，不需要做ClientToScreen转换
	lt.x = rect.left;
	lt.y = rect.top;
	rb.x = rect.right;
	rb.y = rect.bottom;

	ClientToScreen(hwnd, &lt);
	ClientToScreen(hwnd, &rb);

	rect.left = lt.x;
	rect.bottom = rb.y;
	rect.right = rb.x;
	rect.top = lt.y;

	ClipCursor(&rect);

	Game_Paint(hwnd);
	return TRUE;
}

VOID Game_Paint(HWND hwnd)
{
	if (g_iBackgroundX > 800)
	{
		g_iBackgroundX = 0;
	}
	SelectObject(g_bdc, g_hBackground);
	BitBlt(g_mdc, g_iBackgroundX, 0, WINDOW_WIDTH - g_iBackgroundX, 600, g_bdc, 0, 0, SRCCOPY);
	BitBlt(g_mdc, 0, 0, g_iBackgroundX, 600, g_bdc, WINDOW_WIDTH - g_iBackgroundX, 0, SRCCOPY);

	//3.选用位图对象
	SelectObject(g_bdc, g_hHero);
	//4.贴图
	TransparentBlt(g_mdc, g_iX, g_iY, 317, 283, g_bdc, 0, 0, 317, 283, RGB(0, 0, 0));

	SelectObject(g_bdc, g_hSwordBlade);
	for (int i = 0; i < 30; i++)
	{
		if (bullets[i].exist)
		{
			if (bullets[i].x < -100)
			{
				bullets[i].exist = 0;
			}
			else
			{
				TransparentBlt(g_mdc, bullets[i].x, bullets[i].y, 100, 26, g_bdc, 0, 0, 100, 26, RGB(0, 0, 0));
				bullets[i].x -= 10;
			}
		}
	}

	g_iBackgroundX += 5;

	HFONT hFont;
	wchar_t str[20] = {};
	hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, TEXT("微软雅黑"));  //创建字体
	SelectObject(g_mdc, hFont);
	SetBkMode(g_mdc, TRANSPARENT);
	SetTextColor(g_mdc, RGB(255, 255, 0));

	swprintf_s(str, L"鼠标X坐标为%d    ", g_iX);
	TextOut(g_mdc, 0, 0, str, wcslen(str));
	swprintf_s(str, L"鼠标Y坐标为%d    ", g_iY);
	TextOut(g_mdc, 0, 20, str, wcslen(str));
	TextOut(g_mdc, 0, 20, str, wcslen(str)); swprintf_s(str, L"射出的子弹个数%d    ", g_iBulletNum);
	TextOut(g_mdc, 0, 40, str, wcslen(str));

	BitBlt(g_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_mdc, 0, 0, SRCCOPY);
}

BOOL Game_Cleanup(HWND hwnd)
{
	//5.删除创建的DC和位图资源
	DeleteObject(g_hBackground);
	KillTimer(hwnd, 1);
	DeleteObject(g_bdc);
	DeleteObject(g_mdc);
	ReleaseDC(hwnd, g_hdc);
	return TRUE;
}
