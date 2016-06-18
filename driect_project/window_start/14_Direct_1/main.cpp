#include <Windows.h>
#include <time.h>
#include <wchar.h>
#include <stdio.h>

//�������
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "Msimg32.lib")

//�����
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE L"Hello World!"

//����sprite�ṹ��
struct Sprites
{
	int x, y;
	int exist;
};

//ȫ�ֱ�������
HDC g_hdc = NULL, g_mdc = NULL, g_bdc = NULL;
HBITMAP g_hHero, g_hSwordBlade, g_hBackground;
DWORD g_tPre = 0, g_tNow = 0;
Sprites bullets[30];

int g_iX, g_iY, g_iBulletNum;
int g_iBackgroundX;

//��������
//������Ϣ����
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

//Game Flow ����
BOOL Game_Init(HWND hwnd);
VOID Game_Paint(HWND hwnd);
BOOL Game_Cleanup(HWND hwnd);

//������
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//1.���һ�������Ĵ�����
	WNDCLASSEX wndClass = { 0 };//����һ��������
	wndClass.cbSize = sizeof(WNDCLASSEX);//���ýṹ��Ĵ�С
	wndClass.style = CS_HREDRAW | CS_VREDRAW;//���ô�����ʽ
	wndClass.lpfnWndProc = WndProc;//������Ϣ������
	wndClass.cbClsExtra = 0;//������ĸ����ڴ棬һ������Ϊ0
	wndClass.cbWndExtra = 0;//���ڵĸ����ڴ棬һ������Ϊ0
	wndClass.hInstance = hInstance;//��������ʵ���ĳ���ľ��
	wndClass.hIcon = (HICON)LoadImage(NULL, L"icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);//����һ��ͼ��
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//ָ��һ����ɫ��ˢ���
	wndClass.lpszClassName = NULL;//����Ҫ�����˵�
	wndClass.lpszClassName = L"HWWND";//ָ�������������

									  //2.ע�ᴰ����
	if (!RegisterClassEx(&wndClass)) return -1;

	//3.��������
	HWND hwnd = CreateWindow(L"HWWND", WINDOW_TITLE, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

	//4.�����ƶ�����ʾ�͸���
	//MoveWindow(hwnd, 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, true);
	ShowWindow(hwnd, nShowCmd);
	//UpdateWindow(hwnd);

	//��ʼ����Ϸ��Դ
	if (!Game_Init(hwnd))
	{
		MessageBox(hwnd, L"Faild!", L"Message", 0);
		return FALSE;
	}
	PlaySound(L"�ɽ�����ԭ��ս��3.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

	//5.��Ϣ�ַ�
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);//��������Ϣ���͵�winproc������
		}
		//�ᱻ�������϶��������� �� GDI_7�Ա��£�
		g_tNow = GetTickCount();
		if (g_tNow - g_tPre >= 5)
		{
			Game_Paint(hwnd);
			g_tPre = g_tNow;
		}
	}

	//6.ע������
	UnregisterClass(L"HWWND", hInstance);

	return 0;
}

//��Ϣ������
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
			DestroyWindow(hwnd);//���ٴ��岢����һ��WM_DESTROY��Ϣ
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

	//1.����λͼ
	g_hBackground = (HBITMAP)LoadImage(NULL, L"bg.bmp", IMAGE_BITMAP, WINDOW_WIDTH, WINDOW_HEIGHT, LR_LOADFROMFILE);
	g_hHero = (HBITMAP)LoadImage(NULL, L"swordman.bmp", IMAGE_BITMAP, 317, 283, LR_LOADFROMFILE);
	g_hSwordBlade = (HBITMAP)LoadImage(NULL, L"swordblade.bmp", IMAGE_BITMAP, 100, 26, LR_LOADFROMFILE);
	//2.�������ݵ�DC
	g_mdc = CreateCompatibleDC(g_hdc);//���������豸�������ڴ�DC, ���������ĸ��豸����
	g_bdc = CreateCompatibleDC(g_hdc);//�����󱸻�����
	bmp = CreateCompatibleBitmap(g_hdc, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(g_mdc, bmp);//��Ҫ�ȸ� mdc һ�Ż���

	g_iX = 300;
	g_iY = 100;
	g_iBulletNum = 0;

	POINT pt, lt, rb;
	RECT rect;
	pt.x = g_iX;
	pt.y = g_iY;
	ClientToScreen(hwnd, &pt);//��������ת��Ϊ��Ļ����
	SetCursorPos(pt.x, pt.y);//�����������Ļ��λ��
							 //ShowCursor(false);//���ع��
							 //��������ƶ�����
	GetClientRect(hwnd, &rect);//ȡ�ÿͻ�����������
							   //GetWindowRect(hwnd, &rect);//ȡ�ô��ھ�������, ����õ���rect������Ļ���꣬�����������ʱ�򣬲���Ҫ��ClientToScreenת��
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

	//3.ѡ��λͼ����
	SelectObject(g_bdc, g_hHero);
	//4.��ͼ
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
	hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, TEXT("΢���ź�"));  //��������
	SelectObject(g_mdc, hFont);
	SetBkMode(g_mdc, TRANSPARENT);
	SetTextColor(g_mdc, RGB(255, 255, 0));

	swprintf_s(str, L"���X����Ϊ%d    ", g_iX);
	TextOut(g_mdc, 0, 0, str, wcslen(str));
	swprintf_s(str, L"���Y����Ϊ%d    ", g_iY);
	TextOut(g_mdc, 0, 20, str, wcslen(str));
	TextOut(g_mdc, 0, 20, str, wcslen(str)); swprintf_s(str, L"������ӵ�����%d    ", g_iBulletNum);
	TextOut(g_mdc, 0, 40, str, wcslen(str));

	BitBlt(g_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_mdc, 0, 0, SRCCOPY);
}

BOOL Game_Cleanup(HWND hwnd)
{
	//5.ɾ��������DC��λͼ��Դ
	DeleteObject(g_hBackground);
	KillTimer(hwnd, 1);
	DeleteObject(g_bdc);
	DeleteObject(g_mdc);
	ReleaseDC(hwnd, g_hdc);
	return TRUE;
}
