/**
���㻺��

֪ʶ��:
1.��ƶ��㻺��
	�����ʽ FVF, ����������Զ��嶥����Ϣ, �������ݱ���ΪD3Dָ��������, ��Ϊ��������֪����ô��ȡ��
	��ʽ��:
	D3DFVF_XYZ
	D3DFVF_XYZRHW
	D3DFVF_XYZB1~5
	D3DFVF_NORMAL
	D3DFVF_DIFFUSE
	D3DFVF_SPECULAR
	D3DFVF_TEXT1~8
	����ĸ�ʽ�������ѡ�����Ƕ���ṹ��˳����������һ�£��磺
	struct CUSTOMVERTEX
	{
		float x, y, z;//D3DFVF_XYZ
		DWORD color;//D3DFVF_DIFFUSE
	}

	#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

2.�������㻺��
	�����ӿڶ��� (retval ��ʾ return value)
	IDirect3DVertextBuffer9
	(LPDIRECT3DVERTEXBUFFER9)
	�������㻺��
	HRESULT CreateVertexBuffer(
		[in] UINT Length, //���㻺��Ĵ�С, ���ֽ�Ϊ��λ,����N������ N*�����������ݶ���Ĵ�С
		[in] DWORD Usage, //һЩ��������
		[in] DWORD FVF, //����ָ���ĸ�ʽ
		[in] D3DPOOL pool, //һ��ö������, ����ָ���洢���㻺����ڴ�λ��
		[out, retval] IDirect3DVertextBuffer9 **ppVertexBuffer, //���㻺�����
		[in] HANDLE *pSharedHanle 
	);

	typedef enum D3DPOOL{
		D3DPOOL_DEFAULT = 0,
		D3DPOOL_MANAGED = 1,
		D3DPOOL_SYSTEMMEN = 2,
		D3DPOOL_SCRATCH = 3,
		D3DPOOL_FORCE_DWORD = 0X7fffffff
	} D3DPOOL, *LPD3DPOOL;

	AGP �Ǳ����Դ棬��ռ���ڴ���Ϊ��ʾ�ĵط�
	�Դ�
	ϵͳ�ڴ�

3.���㻺�渳ֵ
	�ڷ��ʶ���֮ǰ��Ҫlock,�Լ����Ӧ����lock���ڴ沿��,���������Ҫ������
	IDirect3DVertextBuffer9::Lock();
	IDirect3DVertextBuffer9::Unlock();

	HRESULT Lock(
		[in] UINT OffsetToLock, //��ʼλ�õ�ƫ����
		[in] UINT SizeToLock, //Ҫ����������, ���ֽ���
		[out] VOID **ppbData, //ָ�������Ĵ洢������ʼ��ַ��ָ��
		[in] DWORD Flags //�����ķ�ʽ
	);

	��ֵ�����2�ַ�ʽ��
	1. g_pVertextBuf->Lock(0,0,(void**)&pVertices, 0);
	   pVertices[0] = CUSTOMVERTEX(-80.0f, -80.0f, 0.0f, 1.0f, D3DCOLOR_XRGB(255,0,0));
	   pVertices[1] = CUSTOMVERTEX(-80.0f, 80.0f, 0.0f, 1.0f, D3DCOLOR_XRGB(0,255,0));
	   ...
	   g_pVertextBuf->Unlock();

	2.CUSTOMVERTEX vertices[] = {
									{-80.0f, -80.0f, 0.0f, 1.0f, D3DCOLOR_XRGB(255,0,0)},
									{-80.0f, 80.0f, 0.0f, 1.0f, D3DCOLOR_XRGB(0,255,0)},
									...
								};
	  g_pVertextBuf->Lock(0,0,(void**)&pVertices, 0);
	  memcpy(pVertices, vertices, sizeof(vertices));
	  g_pVertextBuf->Unlock();

	  PS:  g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, false);//�ص���������, �������Ƿ�˳ʱ��, �����ζ�����ʾ

4.ͼ�εĻ���
	PS:	 g_pd3dDevice->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_GOURAUD);//������Ⱦ״̬

	����ͼ��Ҫ����3������
	IDirect3DDevice9::SetStreamSource
	IDirect3DDevice9::SetFVF
	IDirect3DDevice9::DrawPrimitive

	HRESULT SetStreamSource(
		[in] UINT StreamNumber, //0
		[in] IDirect3DVertexBuffer9 *pStreamData, //��������Ƕ��㻺��ָ����
		[in] UINT OffsetInBytes, //0
		[in] UINT Stride //���㻺���е�ÿ������ṹ�Ĵ�С
	)

	HRESULT SetFVF(
		[in] DWORD FVF //���õ�FVF�ĸ�ʽ
	)
	��:
	SetFVF(D3DFVF_CUSTOMVERTEX);

	HRESULT DrawPrimitive(
		[in] D3DPRIMITIVETYPE PrimitiveType, //ö�����ͣ� ��ʾҪ���Ƶ�ͼԪ����
		[in] UINT StartVertex, //�Ӷ��㻺���ж�ȡ�������ݵ���ʼ����λ��
		[in] UINT PrimitiveCount //ָ��Ҫ���Ƶ�ͼԪ����
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
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)
#define SAFE_RELEASE(p) {if(p){(p)->Release(); (p)=NULL;}}

//ȫ�ֱ�������
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;
LPD3DXFONT g_pFont = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;

//�ṹ������
struct CUSTOMVERTEX {
	FLOAT x, y, z, rhw;
	DWORD color;
};

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

	PlaySound(L"����.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

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

	//���㻺�����
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(6 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT,
		&g_pVertexBuffer, NULL)))
	{
		return E_FAIL;
	}

	CUSTOMVERTEX vertices[] =
	{
		{ 300.0f, 100.0f, 0.0f, 1.0f, D3DCOLOR_XRGB(rand() % 256, rand() % 256, rand() % 256) },
		{ 500.0f, 100.0f, 0.0f, 1.0f,  D3DCOLOR_XRGB(rand() % 256, rand() % 256, rand() % 256)},
		{ 300.0f, 300.0f, 0.0f, 1.0f,  D3DCOLOR_XRGB(rand() % 256, rand() % 256, rand() % 256)},
		{ 300.0f, 300.0f, 0.0f, 1.0f,  D3DCOLOR_XRGB(rand() % 256, rand() % 256, rand() % 256)},
		{ (float)(800.0*rand() / (RAND_MAX + 1.0)) , (float)(600.0*rand() / (RAND_MAX + 1.0)) , 0.0f, 1.0f,  D3DCOLOR_XRGB(rand() % 256, rand() % 256, rand() % 256)},
		{ (float)(800.0*rand() / (RAND_MAX + 1.0)) , (float)(600.0*rand() / (RAND_MAX + 1.0)) , 0.0f, 1.0f,  D3DCOLOR_XRGB(rand() % 256, rand() % 256, rand() % 256)}

	};

	VOID* pVertices;
	if (FAILED(g_pVertexBuffer->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
		return E_FAIL;
	memcpy(pVertices, vertices, sizeof(vertices));
	g_pVertexBuffer->Unlock();

	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, false);   //�ص���������
	return S_OK;
}

void Direct3D_Render(HWND hwnd)
{
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	//����һ�����Σ����ڻ�ȡ�����ھ���
	RECT formatRect;
	GetClientRect(hwnd, &formatRect);
	g_pd3dDevice->BeginScene();
	g_pd3dDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);//������Ⱦ״̬
	g_pd3dDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(CUSTOMVERTEX));
	g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	formatRect.top = 400;
	g_pFont->DrawText(0, _T("�����㰡�������������������յģ�"), -1, &formatRect, DT_CENTER,
		D3DCOLOR_XRGB(rand() % 256, rand() % 256, rand() % 256));//�������RGBֵ���������������˰�������Ч
	g_pd3dDevice->EndScene();

	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

void Direct3D_CleanUp()
{
	//�ͷ�COM�ӿڶ���
	SAFE_RELEASE(g_pVertexBuffer)
	SAFE_RELEASE(g_pFont)
	SAFE_RELEASE(g_pd3dDevice)
}