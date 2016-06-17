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

//ȫ�ֱ�������
HDC g_hdc = NULL, g_mdc = NULL, g_bdc = NULL;
HBITMAP g_hSprite, g_hBackground;
DWORD g_tPre = 0, g_tNow = 0;
int g_iX = 0, g_iY = 0;
int g_iNum = 0;

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
	wndClass.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);//ָ��һ����ɫ��ˢ���
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
	PlaySound(L"�ɽ�����ԭ�澰��.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

	//5.��Ϣ�ַ�
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//�ᱻ�������϶��������� �� GDI_7�Ա��£�
		g_tNow = GetTickCount();
		if (g_tNow - g_tPre > 100)
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
	g_hSprite = (HBITMAP)LoadImage(NULL, L"goright.bmp", IMAGE_BITMAP, 480, 108, LR_LOADFROMFILE);

	//2.�������ݵ�DC
	g_mdc = CreateCompatibleDC(g_hdc);//���������豸�������ڴ�DC, ���������ĸ��豸����
	g_bdc = CreateCompatibleDC(g_hdc);//�����󱸻�����
	bmp = CreateCompatibleBitmap(g_hdc, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(g_mdc, bmp);//��Ҫ�ȸ� mdc һ�Ż���

	g_iY = 350;
	g_iX = 0;
	g_iNum = 0;

	return TRUE;
}

VOID Game_Paint(HWND hwnd)
{
	if (g_iX > WINDOW_WIDTH)
	{
		g_iX = 0;
	}
	if (g_iNum > 7)
	{
		g_iNum = 0;
	}

	//3.ѡ��λͼ����
	SelectObject(g_bdc, g_hBackground);
	//4.��ͼ
	BitBlt(g_mdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_bdc, 0, 0, SRCCOPY);

	SelectObject(g_bdc, g_hSprite);
	//4.��ͼ
	//BitBlt(g_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_mdc, 0, 0, SRCCOPY);
	TransparentBlt(g_mdc, g_iX, g_iY, 60, 108, g_bdc, g_iNum * 60, 0, 60, 108, RGB(255, 0, 0));

	BitBlt(g_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_mdc, 0, 0, SRCCOPY);

	g_iX += 5;
	g_iNum++;
}

BOOL Game_Cleanup(HWND hwnd)
{
	//5.ɾ��������DC��λͼ��Դ
	DeleteObject(g_hSprite);
	KillTimer(hwnd, 1);
	DeleteObject(g_bdc);
	DeleteObject(g_mdc);
	ReleaseDC(hwnd, g_hdc);
	return TRUE;
}
