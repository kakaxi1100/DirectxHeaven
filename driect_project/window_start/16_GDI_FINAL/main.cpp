/********
֪ʶ�㣺
һ�������ת��
 ClientToScreen();
 ScreenToClient();

����GDI �������API
 HPEN CreatePen(__in int fnPenStyle, __in int nWidth, __in COLORREF crColor)//��������, ��ʽ ��Ⱥ���ɫ
 COLORREF RGB(BYTE byRed, BYTE, byGreen, BYTE byBlue);

 HBRUSH CreateSolidBrush(__in COLORREF crColor);//������ˢ
 HBRUSH CreateHatchBrush(__in int fnStyle, __in COLORREF clrref)//������Ӱ��ˢ

 ���ĸ��豸�������ֻ��ʻ�
 HGDIOBJ SelectObject(__in HDC hdc, __in HGDIOBJ hgdiobj);
 BOOL DeleteObject(__in HGDIOBJ bObject);

 ����ͼ�κ�����
 LinTo() MoveToEx() Rectangle()

 �������
 BOOL TextOut(HDC hdc, int nXStart, int nYStart, LPCTSTR lpString, int cbString)//DC, λ�ã� �ַ������ַ������� �� wcslen(str) ������

 int DrawText(HDC hDC, LPCTSTR lpString, int nCount, LPRECT lpRect, UINT uFormat);//DC, �ַ���, �ַ�������, ��������, ��дģʽ

 ����������ɫ
 COLORREF SetTextColor(HDC hdc, COLORREF crColor);

 �������ֱ���͸��
 int SetBkMode(HDC hdc, int IBkMode)//DC, Ҫ���õ�ģʽ������ΪTRANSPARENT

 ���崴��
 HFONT CreateFont(
	int nHeight,//�߶�
	int nWidth,//���
	int nEscapement,
	int nOrientation,//�Ƕ�
	int fnWeight,//����
	DWORD fdwItalic,//�Ƿ�б��
	DWORD fdwStrikeOut,//�Ƿ��ɾ����
	DWORD fdwCharSet,//������ַ���
	DWORD fdwOutputPrecision,//�������
	DWORD fdwClipPrecision,//�ü��ľ���
	DWORD fdwQuality,//���������
	DWORD fdwPitchAndFamily,//��������ַ���
	LPCTSTR lpszFace//��������
 )

 λͼ����
 1.����λͼ// ���, ����, ���ص�����, ָ���洢�Ŀ�Ⱥ͸߶�, ���صķ�ʽ
 HANDLE LoadImage(HINSTANCE hinst, LPCTSTR lpszName, UINT uType, int cxDesired, int cyDesired, UINT fuLoad);

 2.��������DC
 ����Ҫ�봰�ڵ�DC�����ݣ�Ϊ���ǽ��ڴ�DC �����ת�� ����DC ��������
 HDC CreateCompatibleDC(__in HDC hdc);  DeleteDC(__IN HDC hdc);

 3.ѡ��λͼ
 SelectObject();

 4.������ͼ
 BOOL BitBlt(
	__in HDC hdcDest, //Ŀ���豸�������
	__in int nXDest, //Ŀ��DC��x����
	__in int nYDest, //Ŀ��DC��y����
	__in int nWidth, //����Ŀ��DC�Ŀ��
	__in int nHeight, //����Ŀ��DC�ĸ߶�
	__in HDC hdcSrc, //Դ�豸�������
	__in int nXSrc, //ԴDC��x����
	__in int nYSrc, //ԴDC��y����
	__in DWORD dwRop //��ͼ��ʽ
 )

 ͸����ͼ��������ϵ
 1.͸�����ַ�
 ����������ͼ
 ͸�����ַ�Ҫ����ͼ��һ���Ǻڵ׵������ɫͼ����һ���ǰ׵׵������ɫͼ
 ����һ�����ñ�����׵׵ĺ�ɫͼ��&�����������ڱ����ϾͿ۳��˺�ɫ������ͼ����Ϊ 0&any = 0 1&any = any
 ����������úڵ׵������ɫͼ����ͬһ��λ���뱳���� or ������ 0|any = any 
 ������ɫ������ͼ��Ƕ���˱���

 ����������Ҫ�������� BitBlt() ����

 2.͸��ɫ�ʷ�
 ͸��ɫ�ʷ�������ͼʱ��������ĳ����ɫΪ͸��ɫ�ĺ��������õ� TransparentBlt ����
 BOOL TransparentBlt(
	__in HDC hdcDest, //Ŀ���豸
	__in int xoriginDest,//Ŀ��������Ͻǵ�X������
	__in int yoriginDest,//Ŀ��������Ͻǵ�Y������
	__in int wDest,//Ŀ����εĿ��
	__in int hDest,//Ŀ����εĸ߶�
	__in HDC hdcSrc,//ԴDC�ľ��
	__in int xoriginSrc,//Դ�������Ͻǵ�X������
	__in int yoriginSrc,//Դ�������Ͻǵ�Y������
	__in int wSrc,//Դ���εĿ��
	__in int hSrc,//Դ���εĸ߶�
	__in UINT crTransparent//ָ����Ϊ͸��ɫ��RGB��ɫֵ
 )


������ʱ��
WindowsAPI�� SetTimer() ������ʱ���������� WM_TIMER ��Ϣ
UINT_PTR WINAPI SetTimer(
	_In_ HWND hWnd, 
	_In_ UINT_PTR nIDEvent, //��ʱ������
	_In_ UINT uElapse, //ʱ����
	_In_ TIMERPROC lpTimerFunc //Ҫ����ĺ�����Ҳ����ָ��ΪNULL����Ҫ����ĺ���д��WM_TIMER�£���һ����
)

ɾ����ʱ��
BOOL WINAPI KillTimer(_In_opt_ HWND hWnd, _In_ UINT_PTR uIDEvent);

һ�㶨ʱ������������Ⱦ�ã���Ⱦ���ǲ���ѭ��������ʾ���������������һ�ε�ʱ�䣬��������ô��Ⱦ��whileѭ������Ĵ���


�ġ���Ϸ�еĽ�������
1.������Ϣ
���������ºͷſ�ʱ�ᴥ��������Ϣ��WM_KEYDOWN��WM_KEYUP

���� wparam �����˰�������ֵ

2.�����Ϣ����
�и��ֵ������Ϣ���������ɡ�

����lparam�ĸ�λ�ֽڱ��������X�����꣬��λ�ֽڱ���������Y���꣬ȡ�øߵ�λ�ĺ�������
WORD LOWDRD(DWORD dwValue);
WORD HIWDRD(DWORD dwValue);

wparam ��¼����갴���Լ�����Ctrl��Shift��״̬��Ϣ

WM_MOUSEWHEEL��Ϣ��wparam���ĸ�λ�ֽڱ�ʾת���Ŀ̶ȣ���λ�ֽڼ�¼����갴���Լ�����Ctrl��Shift��״̬��Ϣ

3.�������صĳ��ú���
BOOL WINAPI SetCursorPos(_In_ int X, _In_ int Y);//�趨����λ��

BOOL ClientToScreen(__in HWND hWnd, __inout LPPOINT lpPoint);//�ͻ�������ת������Ļ���꣨ע�⻹��һ����������
BOOL ScreenToClient(__in HWND hWnd, __inout LPPOINT lpPoint);//��Ļ����ת�����ͻ�������

int WINAPI ShowCursor(_In_ BOOL bShow);//true��ʾ���, false ���ع��

HWND WINAPI SetCapture(_In_ HWND hWnd);//��Ҫ��Ϊ���ڴ����Ⲷ�����up�¼���ע���ʱ����һ��Ҫ�ǻ�ý���
BOOL WINAPI ReleaseCapture(void);

BOOL WINAPI ClipCursor(_In_opt_ const RECT *lpRect); //�����������ƶ�����
typedef struct  tagRECT{
	LONG left;
	LONG top;
	LONG right;
	LONG bottom;
}RECT;

BOOL WINAPI GetWindowRect(_In_ HWND hWnd, _Out_ LPRECT lpRect);//ȡ�ô��ڵľ�������
BOOL WINAPI GetClientRect(_In_ HWND hWnd, _Out_ LPRECT lpRect);//ȡ�ÿͻ����ľ�������

********/

#include <Windows.h>

//�����
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE L"Hello World!"
//��������
//������Ϣ����
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
BOOL Game_Cleanup(HWND hwnd);

//lpCmdLine �����в���
//nCmdShow ָ�����򴰿������ʾ
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//1.���һ��������
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
	ShowWindow(hwnd, nCmdShow);
	//UpdateWindow(hwnd);
	//5.��Ϣ�ַ�
	//��Ϣ����Ӧ����һ���������ȼ��Ķ��нṹ����Ϊ��Щ��Ϣ����������ġ�
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);//��������Ϣ���͵�winproc������
		}
		//���ﴦ����Ϸtick
	}
	//6.ע������
	UnregisterClass(L"HWWND", hInstance);
	return 0;
}

//��Ϣ������
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	// ȥ���豸������������ַ���
	//1. HDC BeginPaint(__in HWND hwnd, __out LPPINTSTRUCT lpPaint) �� BOOL EndPaint(__in HWND hwnd, __in const PAINTSTRUCT *lpPaint)
	//2. HDC GetDC(__in HWND hWnd) �� int ReleaseDC(__in HWND hWnd, __in HDC hDC)
	case WM_PAINT:
		TEXT("haha");
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

	default://����һ����Ҫ����д�����
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

BOOL Game_Init(HWND hwnd)
{
	return TRUE;
}

VOID Game_Paint(HWND hwnd)
{

}

BOOL Game_Cleanup(HWND hwnd)
{
	return TRUE;
}