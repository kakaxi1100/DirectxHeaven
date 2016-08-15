/**
��������
�����������Ŀ��, ����Ϊ�˼��ٴ洢�ռ������
����,����ö��㻺��Ļ�,��ô��¼һ��������: ��Ҫ6����12��������,��36������. ����ʵ����һ��������ֻ�� 8������. ԭ�������
�кܶ��������ǹ������.��������ֻ��Ҫ���� 8 ������,Ȼ��,�������ķ�ʽ����ʾ��12��������, �����������˽�Լ�洢�ռ�.�������������
������.

IDirectDDevice9::DrawIndexedPrimitive()

HRESULT DrawIndexedPrimitive(
	[in] D3DPRIMITIVETYPE Type, // ��ʾҪ���Ƶ�ͼԪ����
	[in] INT BaseVertexIndex, // ���㻺�������λ, ���λ�ý���Ϊ�����������ʼ��
	[in] UINT MinIndex, // �����������Сֵ, һ����0, ������������ 0,1,2,3...����������
	[in] UINT NumVertices, // ���㻺���еĶ��ٸ�������Ϊ����
	[in] UINT StartIndex, // ��ʾ�����������еĵڼ���������ʼ����ͼ��
	[in] UINT PrimitiveCount //Ҫ���Ƶ�ͼԪ����
);

ʹ����������Ĳ���Ͷ��㻺����һ�µ� (����Ҳ֪����, ���Ƕ�����һ���������)
1.��ƶ����ʽ
struct CUSTOMVERTEX
{
	FLOAT X, Y, Z, rhw;
	DWORD color;
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

2.�������㻺���Լ���������
���㻺���� 17 ��ʱ����ܹ���, ����ֻ������������
IDirect3DIndexBuffer9
HRESULT CreateIndexBuffer(
	[in] UINT Length, // ��������Ĵ�С
	[in] DWORD Usage, // ������Ϣ, �ü����ü����ֵ�
	[in] D3DFORMAT Format, // ����������ÿ�������Ĵ�С
	[in] D3DPOOL Pool, // 17 �н���
	[out, retval] IDirect3DIndexBuffer9 **ppIndexBuffer, // �������˵��, ������������ĵ�ַ��
	[in] HANDLE *pShareHandle // NULL
)

D3DFORMAT ��һ��ö������, ��һ����ֵ, ����ֻ�о�����Ϊ������������õ���
typdef enum _D3DFORMAT{
	...
	D3DFMT_INDEX16 = 101,
    D3DFMT_INDEX32 = 102,
	...
}

3.��ֵ���㻺���Լ���������
���㻺�� 17 �Ѿ����ܹ���, ����ֻ������������
����ļ��������Ͷ��㻺��һ����˼

HRESULT Lock(
	[in] UINT OffsetToLock,
	[in] UINT SizeToLock,
	[out] VOID **ppbData,
	[in] DWORD Flags
);

HRESULT Unlock()

��ֵ��2�ַ�ʽ��
	1.	VOID *pIndices = NULL;
		g_pIndexBuf->Lock(0, 0, (void**)&pIndices, 0);
		pIndices[0] = 0, pIndices[1] = 2, pIndices[2] = 3;
		...
		g_pIndexBuf->Unlock();

	2.	WORD Indices[] = {0,1,2, 0,2,3, 0,3,4,...};
		WORD *pIndices = NULL;
		g_pIndexBuf->Lock(0,0,(void**)&pIndices, 0);
		memcpy(pIndices, Indices, sizeof(Indices));
		g_pIndexBuf->Unlock();

4.����ͼ��
	HRESULT SetStreamSource(
		[in] UINT StreamNumber, //0
		[in] IDirect3DVertexBuffer9 *pStreamData, //��������Ƕ��㻺��ָ����
		[in] UINT OffsetInBytes, //0
		[in] UINT Stride //���㻺���е�ÿ������ṹ�Ĵ�С
	)

	HRESULT SetFVF(
		[in] DWORD FVF //���õ�FVF�ĸ�ʽ
	)

	// ָ����������
	HRESULT SetIndices(
		[in] IDirect3DIndexBuffer9 *pIndexData
	)

	HRESULT DrawIndexedPrimitive(
		[in] D3DPRIMITIVETYPE Type, // ��ʾҪ���Ƶ�ͼԪ����
		[in] INT BaseVertexIndex, // ���㻺�������λ, ���λ�ý���Ϊ�����������ʼ��
		[in] UINT MinIndex, // �����������Сֵ, һ����0, ������������ 0,1,2,3...����������
		[in] UINT NumVertices, // ���㻺���еĶ��ٸ�������Ϊ����
		[in] UINT StartIndex, // ��ʾ�����������еĵڼ���������ʼ����ͼ��
		[in] UINT PrimitiveCount //Ҫ���Ƶ�ͼԪ����
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
LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;

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
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(18 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT,
		&g_pVertexBuffer, NULL)))
	{
		return E_FAIL;
	}

	if (FAILED(g_pd3dDevice->CreateIndexBuffer(48 * sizeof(WORD), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT,
		&g_pIndexBuffer, NULL)))
	{
		return E_FAIL;
	}

	CUSTOMVERTEX Vertices[17];
	Vertices[0].x = 400;
	Vertices[0].y = 300;
	Vertices[0].z = 0.0f;
	Vertices[0].rhw = 1.0f;
	Vertices[0].color = D3DCOLOR_XRGB(rand() % 256, rand() % 256, rand() % 256);
	for (int i = 0; i<16; i++)
	{
		Vertices[i + 1].x = (float)(250 * sin(i*3.14159 / 8.0)) + 400;
		Vertices[i + 1].y = -(float)(250 * cos(i*3.14159 / 8.0)) + 300;
		Vertices[i + 1].z = 0.0f;
		Vertices[i + 1].rhw = 1.0f;
		Vertices[i + 1].color = D3DCOLOR_XRGB(rand() % 256, rand() % 256, rand() % 256);
	}

	VOID* pVertices;
	if (FAILED(g_pVertexBuffer->Lock(0, sizeof(Vertices), (void**)&pVertices, 0)))
		return E_FAIL;
	memcpy(pVertices, Vertices, sizeof(Vertices));
	g_pVertexBuffer->Unlock();

	//��������
	WORD Indices[] = { 0,1,2, 0,2,3, 0,3,4, 0,4,5, 0,5,6, 0,6,7, 0,7,8, 0,8,9, 0,9,10, 0,10,11 ,0,11,12, 0,12,13 ,0,13,14 ,0,14,15 ,0,15,16, 0, 16,1 };
	// �����������
	WORD *pIndices = NULL;
	g_pIndexBuffer->Lock(0, 0, (void**)&pIndices, 0);
	memcpy(pIndices, Indices, sizeof(Indices));
	g_pIndexBuffer->Unlock();

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
	g_pd3dDevice->SetIndices(g_pIndexBuffer);
	g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 17, 0, 16);

	formatRect.top = 400;
	g_pFont->DrawText(0, _T("��ã���������ҵİ����������ɽ���»�������"), -1, &formatRect, DT_CENTER,
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