/**
Direct �Ĵ���
1.���ӿ�
DirectX ��ѭCOM�淶��������Ҫ�Ȼ�ȡ�ӿ�
2.ȡ��Ϣ
3.������
4.���豸

Direct3D ��Ⱦ
1.�������
Clear ��������̨�����������ݽ������
HRESULT Clear(
	[in] DWORD Count, //����ָ���ڶ��������ĸ���
	[in] const D3DRECT *pRects, //һ���������飬����������Ҫ��յ�Ŀ���������
	[in] DWORD Flags, //ָ��������Ҫ��յĻ���������3��������Ͽ����� "|" ���ӣ�D3DCLEAR_STENCIL, D3DCLEAR_TARGET, D3DCLEAR_ZUBUFFER ���Ƿֱ��ʾ ģ�滺��������ɫ����������Ȼ�������
	[in] D3DCOLOR Color, //���������������ɫ������֮��ÿ�����ض�Ӧ����ɫֵ���������ɫ��D3DCOLOR���ʾ��D3DCOLOR_XRGB(R, G, B) ��0-255֮��ȡֵ
	[in] float Z, //����ָ�������Ȼ�������ÿ�����ض�Ӧ�����ֵ [0,1]
	[in] DWORD Stencil //����ָ�����ģ�建������ÿ�����ض�Ӧ��ģ��ֵ������ȡֵ��Χ��0��2��n�η���1������n��ģ�建������
)

���������������Է��� D3DCOLOR �ĺ�
D3DCOLOR_ARGB(a, r, g, b);
D3DCOLOR_RGBA(r, g, b, a);
D3DCOLPRVALUE �ṹ��
D3DXCOLOR �ṹ��

2.ȡ�û�ͼ�豸
HRESULT BeginScene();// ��ʼ��ͼ����EndScene()��Ӧ

3.��ʽ����
�Լ����ⷢ��

4.�ͷŻ�ͼ�豸
HRESULT EndScene(); // ������ͼ����BeginScene()��Ӧ

5.��ת��ʾ
HRESULT Present(
	[in] const RECT *pSourceRect, // ָ����Դ���������ָ��
	[in] const RECT *pDestRect, // ָ����Ŀ����������ָ��
	[in] HWND hDestWindowOverride, //ָ��ǰ���ƵĴ��ھ���������ΪNULL����Ϊ hDeviceWindows��ֵ
	[in] const RGNDATA *pDIrtyRegion//ָ����С���������ָ��
);

֪ʶ�㣺
1. ��������
HRESULT D3DXCreateFont(
	__in LPDIRECT3DDEVICE9 pDevice, // Direct �豸
	__in INT Height, // ����ĸ߶�
	__in UINT Width, // ����Ŀ��
	__in UINT Weight, // ����Ĵֶ�
	__in UNIT MipLevels, // ���Ӧ���Ǻ�����ӳ���й�
	__in BOOL Italic, // �Ƿ���б��
	__in DWORD CharSet, // �ַ���
	__in DWORD OutputPrecision, // �ı����� 
	__in DWORD Quality, // �������
	__in DWORD PitchAndFamily, // ���������ţ�  0
	__in LPCTSTR pFacename, // ������
	__out LPD3DXFONT *ppFont // �������
);

INT DrawText(
	[in] LPD3DXSPRITE pSprite, // ID3DXSprite ����ӿ�
	[in] LPCTSTR pString, 
	[in] INT Count,
	[in] LPRECT pRect,
	[in] DWORD Format,
	[in] D3DCOLOR Color
)
**/

#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>

//�������
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

//�����
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE L"Hello World!"

#define SAFE_RELEASE(p) {if(p){(p)->Release(); (p)=NULL;}}

//ȫ�ֱ�������
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;
LPD3DXFONT g_pFont = NULL;

//��������
//������Ϣ����
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

//Game Flow ����
HRESULT Direct3D_Init(HWND hwnd); // Direct3D �ĳ�ʼ��
HRESULT Objects_Init(HWND hwnd); // Ҫ���Ƶ��������Դ��ʼ��
VOID Direct3D_Render(HWND hwnd); // ��Ⱦ
VOID Direct3D_CleanUp(); //������Դ

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
	UpdateWindow(hwnd);

	//��ʼ����Ϸ��Դ
	if (S_OK == Direct3D_Init(hwnd)) {
	}
	else {
		MessageBox(hwnd, L"Direct��ʼ��ʧ�ܣ�", NULL, 0);
	}

	PlaySound(L"NightElf3.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

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
			//ѭ����Ⱦ
			Direct3D_Render(hwnd);
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
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			DestroyWindow(hwnd);//���ٴ��岢����һ��WM_DESTROY��Ϣ
		}
		break;
	case WM_DESTROY:
		Direct3D_CleanUp();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

HRESULT Direct3D_Init(HWND hwnd)
{

	//1.����Direct3D �ӿڶ���
	LPDIRECT3D9 pD3D = NULL;
	if (NULL == (pD3D = Direct3DCreate9(D3D_SDK_VERSION))) //�������ʧ�ܷ���NULL
	{
		return E_FAIL;
	}

	//2.ȡ��Ӳ���豸��Ϣ
	//�Կ�����Ϣ
	D3DCAPS9 caps; //�������Կ���������
	int vp = 0;
	//��һ���������Կ������
	if (FAILED(pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))
	{
		return E_FAIL;
	}

	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)//���֧��Ӳ����������, �Ͳ�����Ӳ�����㣻 �����֧�־Ͳ����������
	{
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else 
	{
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	//3.�������
	D3DPRESENT_PARAMETERS d3dpp;//��ʵ���Ǵ��������ֻ�������ֱ�Ӵ����Ǿ�̫�������Ը���һ���ṹ��ר�����ò�����
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = WINDOW_WIDTH;
	d3dpp.BackBufferHeight = WINDOW_HEIGHT;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 2;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = 0;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	//4.����Direct3D�豸�ӿ�
	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, vp, &d3dpp, &g_pd3dDevice))) // g_pd3dDevice �������
	{
		return E_FAIL;
	}

	SAFE_RELEASE(pD3D);

	if (!(S_OK == Objects_Init(hwnd))) return E_FAIL;
	return S_OK;
}

HRESULT Objects_Init(HWND hwnd)
{
	if (FAILED(D3DXCreateFont(g_pd3dDevice, 36, 0, 0, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY, 0, L"Ҷ�Ѹ�ԲȤ��ͨ��", &g_pFont))) 
	{
		return E_FAIL;
	}
	srand(timeGetTime());
	return S_OK;
}

void Direct3D_Render(HWND hwnd)
{
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	RECT formatRect;
	GetClientRect(hwnd, &formatRect);
	g_pd3dDevice->BeginScene();
	//��һ������
	formatRect.top = 100;
	// ָ��-1 ʱ�� �����Զ��ж��ַ�������
	g_pFont->DrawText(NULL, _T("����һ��ʲô�����Ⱑ��������������"), -1, &formatRect, DT_CENTER, D3DCOLOR_XRGB(68, 139, 256));

	//��������400����д����������
	formatRect.top = 400;
	g_pFont->DrawText(0, _T("�����㰡�������������������յģ�"), -1, &formatRect, DT_CENTER,
		D3DCOLOR_XRGB(rand() % 256, rand() % 256, rand() % 256));//�������RGBֵ���������������˰�������Ч

	g_pd3dDevice->EndScene();

	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

void Direct3D_CleanUp()
{
	//�ͷ�COM�ӿڶ���
	SAFE_RELEASE(g_pFont)
	SAFE_RELEASE(g_pd3dDevice)
}