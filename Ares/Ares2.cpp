// Ares.cpp : Defines the entry point for the application.
//


#include "stdafx.h"

#define INITGUID 

#include <mmsystem.h>
#include <iostream.h> // include important C/C++ stuff
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h> 
#include <math.h>
#include <io.h>
#include <fcntl.h>
#include "ddraw.h"

#define TIMER_ID_1SEC 1
#define TIMER_ID_3SEC 2
#define WINDOW_CLASS_NAME "WINCLASS1"
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP 8

LPDIRECTDRAW7         lpdd7         = NULL; 
HWND      main_window_handle = NULL;
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	//char buff[50];
	switch(msg) {
	case WM_CREATE:
		return 0;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
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

int Game_Init()
{
	if (FAILED(DirectDrawCreateEx(NULL, (void**)&lpdd7,IID_IDirectDraw7, NULL)))
	{
		return 0;
	}
	if (FAILED(lpdd7->SetCooperativeLevel(main_window_handle, 
											DDSCL_FULLSCREEN | DDSCL_ALLOWMODEX |
											DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT)))
	{
		return 0;
	}

	lpdd7->SetDisplayMode(SCREEN_WIDTH, SCREEN_HEIGHT,SCREEN_BPP,0,0);
	return 1;
}
int Game_Main()
{
	return 1;
}
int Game_ShutDown()
{
	if (lpdd7)
	{
		lpdd7->Release();
		lpdd7 = NULL;
	}
	return 1;
}
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{

 	WNDCLASSEX winclass;
	HWND hwnd;
	MSG msg;
//	HDC hdc;
//	char buff[50];

	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = hInstance;
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = WINDOW_CLASS_NAME;
	winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&winclass))
		return 0;
	
	if (!(hwnd = CreateWindowEx(NULL, WINDOW_CLASS_NAME,"initialization demo",
								WS_OVERLAPPEDWINDOW | WS_VISIBLE,
								0,0,
								400,300,
								NULL, NULL,hInstance,NULL)))
	{
		return 0;
	}
	main_window_handle = hwnd;
	Game_Init();
//	SetTimer(hwnd, TIMER_ID_1SEC, 1000, NULL);
//	SetTimer(hwnd, TIMER_ID_3SEC, 3000, NULL);
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

		/*hdc = GetDC(hwnd);
		SetTextColor(hdc, RGB(rand()%255, rand()%255, rand()%255));
		SetBkColor(hdc, RGB(rand()%255, rand()%255, rand()%255));
		SetBkMode(hdc, TRANSPARENT);
		sprintf(buff, "this is %d", 1);
		TextOut(hdc, 100,100, buff, strlen(buff));
		ReleaseDC(hwnd,hdc);*/

		Game_Main();
	}

	Game_ShutDown();
	return msg.wParam;
}



