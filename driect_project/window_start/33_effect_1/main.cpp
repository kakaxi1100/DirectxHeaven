#include <Windows.h>
#include <time.h>
#include <wchar.h>
#include <stdio.h>
#include <math.h>
//�������
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "Msimg32.lib")

//�����
#define WINDOW_WIDTH 650
#define WINDOW_HEIGHT 510
#define WINDOW_TITLE L"Random Pop!"
#define MAX_COINS 100
#define ACC_Y 0.5f
#define BASE_VEL 17.0f
#define SPEED_X 3
#define SPEED_Y 1.5
#define PI 3.14159265f

//�ṹ��
struct Coin
{
	bool isActive;
	float x, y, vx, vy;
};

//ȫ�ֱ�������
HDC g_hdc = NULL, g_mdc = NULL, g_bdc = NULL;
HBITMAP g_hBackground, g_hCoin;
DWORD g_tPre = 0, g_tNow = 0;
Coin Coins[MAX_COINS];

int count = 0;

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
	HWND hwnd = CreateWindow(L"HWWND", WINDOW_TITLE, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 
		CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

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
	//PlaySound(L"�ɽ����»�.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

	//5.��Ϣ�ַ�
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);//��������Ϣ���͵�winproc������
		}
		else
		{
			//�ᱻ�������϶��������� �� GDI_7�Ա��£�
			g_tNow = GetTickCount();
			if (g_tNow - g_tPre > 1)
			{
				Game_Paint(hwnd);
				g_tPre = g_tNow;
			}
		}
	}

	//6.ע������
	UnregisterClass(L"HWWND", hInstance);

	return 0;
}

//��Ϣ������
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
	g_hBackground = (HBITMAP)LoadImage(NULL, L"4.bmp", IMAGE_BITMAP, 640, 480, LR_LOADFROMFILE);
	g_hCoin = (HBITMAP)LoadImage(NULL, L"3.bmp", IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);

	//2.�������ݵ�DC
	g_mdc = CreateCompatibleDC(g_hdc);//���������豸�������ڴ�DC, ���������ĸ��豸����
	g_bdc = CreateCompatibleDC(g_hdc);//�����󱸻�����
	bmp = CreateCompatibleBitmap(g_hdc, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(g_mdc, bmp);//��Ҫ�ȸ� mdc һ�Ż���

	for (int i = 0; i < MAX_COINS; ++i)
	{
		Coins[i].isActive = false;
	}

	return TRUE;
}

VOID Game_Paint(HWND hwnd)
{	
	float fRand_r, fRand_t;
	//3.ѡ��λͼ����
	SelectObject(g_bdc, g_hBackground);
	//4.��ͼ
	BitBlt(g_mdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_bdc, 0, 0, SRCCOPY);

	SelectObject(g_bdc, g_hCoin);
	for (int i = 0; i < MAX_COINS; ++i)
	{
		//�˶�
		if (Coins[i].isActive == true)
		{
			Coins[i].x += Coins[i].vx;
			Coins[i].y += Coins[i].vy;
			Coins[i].vy += ACC_Y;
			//�����������Ч
			if (Coins[i].x >= 640 || Coins[i].x <= -64 || Coins[i].y <= -64 || Coins[i].y >= 480)
			{
				Coins[i].isActive = false;
			}
			
		}
		//����
		if (Coins[i].isActive == false)
		{
			Coins[i].isActive = true;
			Coins[i].x = (640 - 64)*0.5;
			Coins[i].y = 480 - 64;

			//Ӧ�õ����������ͬ�����������Ч��Ҳ��ͬ���ڵ�һ�ε�ʱ��ǳ�����

			//1 ���������������
			/*Coins[i].vx = rand()*SPEED_X / (float)RAND_MAX - SPEED_X*0.5;
			Coins[i].vy = rand()*SPEED_Y / (float)RAND_MAX - SPEED_Y*0.5 - BASE_VEL;*/

			//2 �ó���õ����������
			/*Coins[i].vx = rand() % (SPEED_X + 1) - SPEED_X*0.5;
			Coins[i].vy = rand() % (SPEED_Y + 1) - SPEED_Y*0.5 - BASE_VEL;*/

			//3 ��̬�ֲ����
			fRand_r = sqrtf(-2.0f * logf((float)(rand() + 1) / (RAND_MAX + 1)));	// ��-2ln(a)
			fRand_t = 2.0f * PI * (float)rand() / RAND_MAX;								// 2��b
			Coins[i].vx = (fRand_r * cosf(fRand_t)) * SPEED_X;				// �������Vx�ĳ�ʼֵ
			Coins[i].vy = (fRand_r * sinf(fRand_t)) * SPEED_Y - BASE_VEL;	// �������Vy�ĳ�ʼֵ
		}
		TransparentBlt(g_mdc, Coins[i].x, Coins[i].y, 64, 64, g_bdc, 0, 0, 64, 64, RGB(0, 0, 0));
	}
	/*if (count % 2 == 0) {
		for (int i = 0; i < MAX_COINS; ++i)
		{
			//����
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
	//5.ɾ��������DC��λͼ��Դ
	DeleteObject(g_hBackground);
	DeleteObject(g_bdc);
	DeleteObject(g_mdc);
	ReleaseDC(hwnd, g_hdc);
	return TRUE;
}
