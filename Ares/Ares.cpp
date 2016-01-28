// Ares.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <mmsystem.h>  // very important and include WINMM.LIB too!
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TIMER_ID_1SEC 1
#define TIMER_ID_3SEC 2
#define WINDOW_CLASS_NAME "WINCLASS1"

int count = 0;
LRESULT CALLBACK WindowProc0(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	char buff[50];
	switch(msg) {
	case WM_CREATE:
		return 0;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		COLORREF old_fcolor, old_bcolor;
		int old_mode;
		old_fcolor=SetTextColor(hdc, RGB(0,255,0));
		old_bcolor=SetBkColor(hdc, RGB(0,0,0));
		old_mode=SetBkMode(hdc, TRANSPARENT);
		sprintf(buff, "hello %d", count);
		TextOut(hdc, 20,30, buff, strlen(buff));
		SetTextColor(hdc, old_fcolor);
		SetBkColor(hdc, old_bcolor);
		SetBkMode(hdc, old_mode);
		TextOut(hdc, 100,30, "world", strlen("hello"));
		EndPaint(hwnd, &ps);
		return 0;
		break;
	case WM_DESTROY:
		KillTimer(hwnd, TIMER_ID_1SEC);
		KillTimer(hwnd, TIMER_ID_3SEC);
		PostQuitMessage(0);
		return 0;
		break;
	case WM_TIMER:
		switch(wparam) {
		case TIMER_ID_1SEC:
			count++;
			break;
		case TIMER_ID_3SEC:
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return (DefWindowProc(hwnd, msg, wparam, lparam));
}

int APIENTRY WinMain0(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{

 	WNDCLASSEX winclass;
	HWND hwnd;
//	HWND hwnd1;
	MSG msg;
	HDC hdc;
	char buff[50];

	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = WindowProc0;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = hInstance;
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = WINDOW_CLASS_NAME;
	winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&winclass))
		return 0;
	
	if (!(hwnd = CreateWindowEx(NULL, WINDOW_CLASS_NAME,"initialization demo",
								WS_OVERLAPPEDWINDOW | WS_VISIBLE,
								0,0,
								400,400,
								NULL, NULL,hInstance,NULL)))
	{
		return 0;
	}

	/*if (!(hwnd1 = CreateWindowEx(NULL, WINDOW_CLASS_NAME,"initialization demo",
								WS_OVERLAPPEDWINDOW | WS_VISIBLE,
								100,100,
								400,300,
								NULL, NULL,hInstance,NULL)))
	{
		return 0;
	}*/
	//ShowWindow(hwnd, nCmdShow);
	//UpdateWindow(hwnd);

	SetTimer(hwnd, TIMER_ID_1SEC, 1000, NULL);
	SetTimer(hwnd, TIMER_ID_3SEC, 3000, NULL);
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0,0,PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
							
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		hdc = GetDC(hwnd);
		SetTextColor(hdc, RGB(rand()%255, rand()%255, rand()%255));
		SetBkColor(hdc, RGB(rand()%255, rand()%255, rand()%255));
		SetBkMode(hdc, TRANSPARENT);
		sprintf(buff, "this is %d", count);
		TextOut(hdc, 100,100, buff, strlen(buff));
		ReleaseDC(hwnd,hdc);
		/*hdc = GetDC(hwnd);
		for (int index = 0; index < 1000; index++)
		{
			int x = rand()%400;
			int y = rand()%400;
			COLORREF color = RGB(rand()%255, rand()%255, rand()%255);
			SetPixel(hdc, x, y ,color);
		}
		ReleaseDC(hwnd, hdc);*/
		/*if (PeekMessage(&msg, hwnd1, 0,0,PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
							
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}*/
		//Game_Main();
	}
	return msg.wParam;
}



