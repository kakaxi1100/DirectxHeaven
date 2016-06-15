#include <Windows.h>
#include <time.h>

//�������
#pragma comment(lib, "winmm.lib")

//�����
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE L"Hello World!"

//ȫ�ֱ�������
HDC g_hdc = NULL, g_mdc = NULL;
HBITMAP g_hBackground, g_hSprite1, g_hSprite2;

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
	PlaySound(L"��ƿʱ��1.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

	//5.��Ϣ�ַ�
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
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
	case WM_PAINT:
		g_hdc = BeginPaint(hwnd, &paintStruct);
		Game_Paint(hwnd);
		EndPaint(hwnd, &paintStruct);
		//ValidateRect(hwnd, NULL);
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
	srand((unsigned)time(NULL));
	g_hdc = GetDC(hwnd);

	//1.����λͼ
	g_hBackground = (HBITMAP)LoadImage(NULL, L"bg.bmp", IMAGE_BITMAP, 800, 600, LR_LOADFROMFILE);
	g_hSprite1 = (HBITMAP)LoadImage(NULL, L"character1.bmp", IMAGE_BITMAP, 640, 579, LR_LOADFROMFILE);
	g_hSprite2 = (HBITMAP)LoadImage(NULL, L"character2.bmp", IMAGE_BITMAP, 800, 584, LR_LOADFROMFILE);
	//2.�������ݵ�DC
	g_mdc = CreateCompatibleDC(g_hdc);//���������豸�������ڴ�DC, ���������ĸ��豸����

	Game_Paint(hwnd);
	ReleaseDC(hwnd, g_hdc);
	return TRUE;
}

VOID Game_Paint(HWND hwnd)
{
	//3.ѡ��λͼ����
	SelectObject(g_mdc, g_hBackground);
	//4.��ͼ
	BitBlt(g_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_mdc, 0, 0, SRCCOPY);

	SelectObject(g_mdc, g_hSprite1);
	BitBlt(g_hdc, 50, WINDOW_HEIGHT - 579, 320, 640, g_mdc, 320, 0, SRCAND);//������ͼ���뱳������ ���米���� 3  ������ 1 0 ��ô����֮�� ���� 3 0 ��ֻʣ���˺�ɫ����
	BitBlt(g_hdc, 50, WINDOW_HEIGHT - 579, 320, 640, g_mdc, 0, 0, SRCPAINT);//��ʱ��ԭͼ�����λ�õ�����ͼ���򣬾Ž�ԭͼ����ɫ�ʵĲ��ָ���䵽�˺�ɫ���֡�

	SelectObject(g_mdc, g_hSprite2);
	BitBlt(g_hdc, 450, WINDOW_HEIGHT - 584, 400, 584, g_mdc, 400, 0, SRCAND);
	BitBlt(g_hdc, 450, WINDOW_HEIGHT - 584, 400, 584, g_mdc, 0, 0, SRCPAINT);
}

BOOL Game_Cleanup(HWND hwnd)
{
	//5.ɾ��������DC��λͼ��Դ
	DeleteObject(g_hSprite2);
	DeleteObject(g_hSprite1);
	DeleteObject(g_hBackground);
	DeleteObject(g_mdc);
	return TRUE;
}
