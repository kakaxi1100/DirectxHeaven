/********
知识点：

零、补遗
#pragma comment(lib,"winmm.lib") //调用PlaySound函数所需库文件

一、坐标点转换
 ClientToScreen();
 ScreenToClient();

二、GDI 几个相关API
 HPEN CreatePen(__in int fnPenStyle, __in int nWidth, __in COLORREF crColor)//创建画笔, 样式 宽度和颜色
 COLORREF RGB(BYTE byRed, BYTE, byGreen, BYTE byBlue);

 HBRUSH CreateSolidBrush(__in COLORREF crColor);//创建画刷
 HBRUSH CreateHatchBrush(__in int fnStyle, __in COLORREF clrref)//创建阴影画刷

 在哪个设备上用哪种画笔画
 HGDIOBJ SelectObject(__in HDC hdc, __in HGDIOBJ hgdiobj);
 BOOL DeleteObject(__in HGDIOBJ bObject);

 绘制图形和线条
 LinTo() MoveToEx() Rectangle()

 文字输出
 BOOL TextOut(HDC hdc, int nXStart, int nYStart, LPCTSTR lpString, int cbString)//DC, 位置， 字符串，字符串长度 用 wcslen(str) 来计算

 int DrawText(HDC hDC, LPCTSTR lpString, int nCount, LPRECT lpRect, UINT uFormat);//DC, 字符串, 字符串长度, 矩形区域, 书写模式

 设置文字颜色
 COLORREF SetTextColor(HDC hdc, COLORREF crColor);

 设置文字背景透明
 int SetBkMode(HDC hdc, int IBkMode)//DC, 要设置的模式，常设为TRANSPARENT

 字体创建
 HFONT CreateFont(
	int nHeight,//高度
	int nWidth,//宽度
	int nEscapement,
	int nOrientation,//角度
	int fnWeight,//磅数
	DWORD fdwItalic,//是否斜体
	DWORD fdwStrikeOut,//是否带删除线
	DWORD fdwCharSet,//所需的字符集
	DWORD fdwOutputPrecision,//输出精度
	DWORD fdwClipPrecision,//裁剪的精度
	DWORD fdwQuality,//输出的质量
	DWORD fdwPitchAndFamily,//字体间距的字符集
	LPCTSTR lpszFace//字体名称
 )

 位图绘制(HBITMAP)
 1.加载位图// 句柄, 名称, 加载的类型, 指定存储的宽度和高度, 加载的方式
 HANDLE LoadImage(HINSTANCE hinst, LPCTSTR lpszName, UINT uType, int cxDesired, int cyDesired, UINT fuLoad);

 2.建立兼容DC
 这里要与窗口的DC做兼容，为的是将内存DC 方便的转到 窗口DC 用来绘制
 HDC CreateCompatibleDC(__in HDC hdc);  DeleteDC(__IN HDC hdc);
 
 3.选用位图
 SelectObject();

 4.进行贴图
 BOOL BitBlt(
	__in HDC hdcDest, //目标设备环境句柄
	__in int nXDest, //目的DC的x坐标
	__in int nYDest, //目的DC的y坐标
	__in int nWidth, //贴到目的DC的宽度
	__in int nHeight, //贴到目的DC的高度
	__in HDC hdcSrc, //源设备环境句柄
	__in int nXSrc, //源DC的x坐标
	__in int nYSrc, //源DC的y坐标
	__in DWORD dwRop //贴图方式
 )

 透明贴图的两套体系
 1.透明遮罩法
 假如是人物图
 透明遮罩法要两张图，一张是黑底的人物彩色图，另一张是白底的人物黑色图
 步骤一：先让背景与白底的黑色图做&操作，这样在背景上就扣出了黑色的人物图，因为 0&any = 0 1&any = any
 步骤二：再让黑底的人物彩色图，在同一个位置与背景做 or 操作， 0|any = any 
 这样彩色的人物图就嵌入了背景

 所以这样做要调用两次 BitBlt() 函数

 2.透明色彩法
 透明色彩法是在贴图时可以设置某种颜色为透明色的函数这里用到 TransparentBlt 函数 需要加 Msimg32.lib库
 BOOL TransparentBlt(
	__in HDC hdcDest, //目标设备
	__in int xoriginDest,//目标矩形左上角的X轴坐标
	__in int yoriginDest,//目标矩形左上角的Y轴坐标
	__in int wDest,//目标矩形的宽度
	__in int hDest,//目标矩形的高度
	__in HDC hdcSrc,//源DC的句柄
	__in int xoriginSrc,//源矩形左上角的X轴坐标
	__in int yoriginSrc,//源矩形左上角的Y轴坐标
	__in int wSrc,//源矩形的宽度
	__in int hSrc,//源矩形的高度
	__in UINT crTransparent//指定视为透明色的RGB颜色值
 )
 如:
 TransparentBlt(g_mdc, 0, 0, 64, 64, g_bdc, 0, 0, 64, 64, RGB(0, 0, 0));

三、定时器
WindowsAPI中 SetTimer() 建立定时器，并发送 WM_TIMER 消息
UINT_PTR WINAPI SetTimer(
	_In_ HWND hWnd, 
	_In_ UINT_PTR nIDEvent, //定时器代号
	_In_ UINT uElapse, //时间间隔
	_In_ TIMERPROC lpTimerFunc //要处理的函数，也可以指定为NULL，把要处理的函数写在WM_TIMER下，都一样。
)

删除定时器
BOOL WINAPI KillTimer(_In_opt_ HWND hWnd, _In_ UINT_PTR uIDEvent);

一般定时器不用来做渲染用，渲染还是采用循环动画显示。即计算这次与上一次的时间，来决定怎么渲染。while循环里面的处理


四、游戏中的交互处理
1.键盘消息
当按键按下和放开时会触发两个消息，WM_KEYDOWN和WM_KEYUP

其中 wparam 保存了按键代码值

2.鼠标消息处理
有各种的鼠标消息，具体查书吧。

其中lparam的高位字节保存了鼠标X的坐标，低位字节保存了鼠标的Y坐标，取得高低位的函数如下
WORD LOWDRD(DWORD dwValue);
WORD HIWDRD(DWORD dwValue);

wparam 记录了鼠标按键以及键盘Ctrl与Shift的状态信息

WM_MOUSEWHEEL消息的wparam，的高位字节表示转动的刻度，低位字节记录了鼠标按键以及键盘Ctrl与Shift的状态信息

3.与鼠标相关的常用函数
BOOL WINAPI SetCursorPos(_In_ int X, _In_ int Y);//设定光标的位置

BOOL ClientToScreen(__in HWND hWnd, __inout LPPOINT lpPoint);//客户区坐标转换到屏幕坐标（注意还有一个窗口区）
BOOL ScreenToClient(__in HWND hWnd, __inout LPPOINT lpPoint);//屏幕坐标转换到客户区坐标

int WINAPI ShowCursor(_In_ BOOL bShow);//true显示光标, false 隐藏光标

HWND WINAPI SetCapture(_In_ HWND hWnd);//主要是为了在窗体外捕获鼠标up事件，注意此时窗体一定要是获得焦点
BOOL WINAPI ReleaseCapture(void);

BOOL WINAPI ClipCursor(_In_opt_ const RECT *lpRect); //限制鼠标光标的移动区域
typedef struct  tagRECT{
	LONG left;
	LONG top;
	LONG right;
	LONG bottom;
}RECT;

BOOL WINAPI GetWindowRect(_In_ HWND hWnd, _Out_ LPRECT lpRect);//取得窗口的矩形区域
BOOL WINAPI GetClientRect(_In_ HWND hWnd, _Out_ LPRECT lpRect);//取得客户区的矩形区域

五、消除闪烁
1.创建两个兼容与窗口兼容的DC mdc, bufdc.
2.创建与窗口兼容的位图
HBITMAP CreateCompatibleBitmap(_In_ HDC hdc, _In_ int nWidth, _In_ int nHeight);//创建一个空的和窗口兼容的位图对象
3.将空位图放到mdc中
SelectObject(mdc, bmp);
4.将load的图片赋给bufdc
5.在BitBlt bufdc的内容到mdc
6.最后再将mdc的内容BitBlt 到hdc

六、擦除背景
RECT clientArea;
GetClientRect(hwnd, &clientArea);//取得客户区
HBRUSH brush = (HBRUSH)GetStockObject(WHITE_BRUSH);//创建画刷
FillRect(gmdc, &clientArea, brush);//把客户区刷一遍

********/
#pragma comment(lib,"winmm.lib")

#include <Windows.h>
#include <wchar.h>
#include <chrono>
#include "Map.h"
#include "Pacman.h"
#include "Beans.h"
#include "HitTest.h"
#include "Ghost.h"

//定义宏
#define WINDOW_WIDTH 820
#define WINDOW_HEIGHT 680
#define WINDOW_TITLE L"Hello GDI!"

//函数声明
//处理消息函数
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
BOOL Game_Cleanup(HWND hwnd);
BOOL Game_Init(HWND hwnd);
VOID Game_Paint(HWND hwnd);

//声明全局变量
HDC ghdc = nullptr, gmdc = nullptr, buffDC = nullptr;
HBITMAP beanBitmap = nullptr , gameBitmap = nullptr, emptyBitmap = nullptr;
std::unique_ptr<Map> map = nullptr;
std::unique_ptr<Pacman> pacman = nullptr;
std::unique_ptr<Beans> beans = nullptr;
std::unique_ptr<Ghost> ghost = nullptr;
HFONT hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, L"叶根友圆趣卡通体");

std::chrono::time_point<std::chrono::system_clock> tick;
//lpCmdLine 命令行参数
//nCmdShow 指定程序窗口如何显示
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//1.设计一个窗口类
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
	ShowWindow(hwnd, nCmdShow);
	//UpdateWindow(hwnd);
	Game_Init(hwnd);

	//5.消息分发
	//消息队列应该是一个带有优先级的队列结构，因为有些消息是立即处理的。
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);//这个会把消息发送到winproc处理函数
		}
		//这里处理游戏tick
		auto diff = std::chrono::system_clock::now() - tick;
		
		if (std::chrono::duration<double, std::milli>(diff).count() >= 16)
		{
			ghost->update();
			pacman->update();
			HitTest::HitObject(*pacman, *ghost, *map, *beans);
			Game_Paint(hwnd);
			tick = std::chrono::system_clock::now();
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
	// 去的设备环境句柄的两种方法
	//1. HDC BeginPaint(__in HWND hwnd, __out LPPINTSTRUCT lpPaint) 和 BOOL EndPaint(__in HWND hwnd, __in const PAINTSTRUCT *lpPaint)
	//2. HDC GetDC(__in HWND hWnd) 和 int ReleaseDC(__in HWND hWnd, __in HDC hDC)
	/*case WM_PAINT:
		ghdc = BeginPaint(hwnd, &paintStruct);
		Game_Paint(hwnd);
		EndPaint(hwnd, &paintStruct);
		ValidateRect(hwnd, NULL);
		break;*/
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			DestroyWindow(hwnd);//销毁窗体并发送一个WM_DESTROY消息
		}
		if (wParam == VK_LEFT)
		{
			pacman->setDir(2);
		}
		if (wParam == VK_RIGHT)
		{
			pacman->setDir(0);
		}
		if (wParam == VK_UP)
		{
			pacman->setDir(3);
		}
		if (wParam == VK_DOWN)
		{
			pacman->setDir(1);
		}
		break;
	case WM_DESTROY:
		Game_Cleanup(hwnd);
		PostQuitMessage(0);
		break;

	default://这里一定不要忘记写这个！
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

BOOL Game_Init(HWND hwnd)
{
	ghdc = GetDC(hwnd);
	gmdc = CreateCompatibleDC(ghdc);
	buffDC = CreateCompatibleDC(ghdc);
	emptyBitmap = CreateCompatibleBitmap(ghdc, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(gmdc, emptyBitmap);

	gameBitmap = (HBITMAP)LoadImage(nullptr, L"1.bmp", IMAGE_BITMAP, 448, 128, LR_LOADFROMFILE);
	beanBitmap = (HBITMAP)LoadImage(nullptr, L"2.bmp", IMAGE_BITMAP, 96, 32, LR_LOADFROMFILE);

	map = std::make_unique<Map>(25, 20, 
		std::vector<int>{	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
							1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
							1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
							1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
							1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
							1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
							1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
							1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
							1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
							1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
							1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
							1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
							1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
							1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
							1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
							1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
							1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
							1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
							1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
							1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 });

	pacman = std::make_unique<Pacman>(18, 23);
	ghost = std::make_unique<Ghost>(1, 1, 1);
	beans = std::make_unique<Beans>();
	beans->init(*map);

	Game_Paint(hwnd);

	return TRUE;
}

VOID Game_Paint(HWND hwnd)
{
	/*SelectObject(gmdc, gameBitmap);
	BitBlt(ghdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, gmdc, 0, 0, SRCCOPY);*/
	//先清空gmdc的画布
	RECT clientArea;
	GetClientRect(hwnd, &clientArea);
	HBRUSH brush = (HBRUSH)GetStockObject(WHITE_BRUSH);
	FillRect(gmdc, &clientArea, brush);

	map->draw(gmdc, buffDC, gameBitmap);
	beans->draw(gmdc, buffDC, beanBitmap);
	pacman->draw(gmdc, buffDC, gameBitmap);
	ghost->draw(gmdc, buffDC, gameBitmap);

	SelectObject(gmdc, hFont);
	SetBkMode(gmdc, TRANSPARENT);
	SetTextColor(gmdc, RGB(62, 255, 80));
	wchar_t t[20];
	wsprintf(t, L"Score %d", HitTest::score);
	TextOut(gmdc, 30, 5, t, wcslen(t));

	BitBlt(ghdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, gmdc, 0, 0, SRCCOPY);
}

BOOL Game_Cleanup(HWND hwnd)
{
	DeleteObject(hFont);
	DeleteObject(gameBitmap);
	DeleteDC(buffDC);
	DeleteDC(gmdc);
	ReleaseDC(hwnd, ghdc);
	return TRUE;
}