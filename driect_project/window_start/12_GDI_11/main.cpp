#include <Windows.h>
#include <time.h>
#include <wchar.h>
#include <stdio.h>

//�������
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "Msimg32.lib")

//�����
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE L"Hello World!"
#define SPRITE_NUMBER 50

//����sprite�ṹ��
struct Sprites
{
	int x, y;
	int index;
	int direction;
};

//ȫ�ֱ�������
HDC g_hdc = NULL, g_mdc = NULL, g_bdc = NULL;
HBITMAP g_hSprite[4], g_hBackground;
Sprites sprite[SPRITE_NUMBER];
DWORD g_tPre = 0, g_tNow = 0;

//��������
//������Ϣ����
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
VOID BubSort();

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
	//PlaySound(L"OrcTheme.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

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
	g_hSprite[0] = (HBITMAP)LoadImage(NULL, L"11.bmp", IMAGE_BITMAP, 384, 96, LR_LOADFROMFILE);//��
	g_hSprite[1] = (HBITMAP)LoadImage(NULL, L"22.bmp", IMAGE_BITMAP, 384, 96, LR_LOADFROMFILE);//��
	g_hSprite[2] = (HBITMAP)LoadImage(NULL, L"33.bmp", IMAGE_BITMAP, 384, 96, LR_LOADFROMFILE);//��
	g_hSprite[3] = (HBITMAP)LoadImage(NULL, L"44.bmp", IMAGE_BITMAP, 384, 96, LR_LOADFROMFILE);//��

																							   //2.�������ݵ�DC
	g_mdc = CreateCompatibleDC(g_hdc);//���������豸�������ڴ�DC, ���������ĸ��豸����
	g_bdc = CreateCompatibleDC(g_hdc);//�����󱸻�����
	bmp = CreateCompatibleBitmap(g_hdc, WINDOW_WIDTH, WINDOW_HEIGHT);
	SelectObject(g_mdc, bmp);//��Ҫ�ȸ� mdc һ�Ż���

	for (int i = 0; i < SPRITE_NUMBER; i++)//��ʼ��sprite����
	{
		sprite[i].direction = rand() % 4;
		sprite[i].x = rand() % WINDOW_WIDTH;
		sprite[i].y = rand() % WINDOW_HEIGHT;
		sprite[i].index = 0;
	}

	return TRUE;
}

VOID Game_Paint(HWND hwnd)
{
	//3.ѡ��λͼ����
	SelectObject(g_bdc, g_hBackground);
	//4.��ͼ
	BitBlt(g_mdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_bdc, 0, 0, SRCCOPY);
	BubSort();
	for (int i = 0; i < SPRITE_NUMBER; i++)
	{
		Sprites s = sprite[i];

		if (s.x > WINDOW_WIDTH)
		{
			s.x = 0;
		}
		else if (s.x < 0)
		{
			s.x = WINDOW_WIDTH;
		}

		if (s.y > WINDOW_HEIGHT)
		{
			s.y = 0;
		}
		else if (s.y < 0)
		{
			s.y = WINDOW_HEIGHT;
		}
		if (s.index > 3)
		{
			s.index = 0;
		}

		//3.ѡ��λͼ����
		SelectObject(g_bdc, g_hSprite[s.direction]);
		//4.��ͼ
		TransparentBlt(g_mdc, s.x, s.y, 96, 96, g_bdc, s.index * 96, 0, 96, 96, RGB(0, 0, 0));

		sprite[i].index++;
		//���ȡ����
		int temp = rand() % 4;
		if (temp != s.direction)
		{
			sprite[i].direction = temp;
			sprite[i].index = 0;
		}

		switch (s.direction)
		{
		case 0:
			sprite[i].y += 10;
			break;
		case 1:
			sprite[i].x -= 10;
			break;
		case 2:
			sprite[i].x += 10;
			break;
		case 3:
			sprite[i].y -= 10;
			break;
		}
	}

	BitBlt(g_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_mdc, 0, 0, SRCCOPY);
}

BOOL Game_Cleanup(HWND hwnd)
{
	//5.ɾ��������DC��λͼ��Դ
	DeleteObject(g_hSprite[0]);
	DeleteObject(g_hSprite[1]);
	DeleteObject(g_hSprite[2]);
	DeleteObject(g_hSprite[3]);
	DeleteObject(g_hBackground);
	KillTimer(hwnd, 1);
	DeleteObject(g_bdc);
	DeleteObject(g_mdc);
	ReleaseDC(hwnd, g_hdc);
	return TRUE;
}

VOID BubSort()
{
	Sprites temp;
	for (int i = 0; i < SPRITE_NUMBER; i++)
	{
		for (int j = 0; j < SPRITE_NUMBER; j++)
		{
			if (sprite[i].y < sprite[j].y)
			{
				temp = sprite[i];
				sprite[i] = sprite[j];
				sprite[j] = temp;
			}
		}
	}
}