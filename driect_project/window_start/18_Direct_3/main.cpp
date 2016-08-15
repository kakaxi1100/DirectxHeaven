/**
索引缓存
用索引缓存的目的, 就是为了减少存储空间的消耗
比如,如果用顶点缓存的话,那么记录一个正方体: 需要6个面12个三角形,共36个顶点. 但是实际上一个正方体只有 8个顶点. 原因就在于
有很多三角形是共顶点的.所以我们只需要定义 8 个顶点,然后,用索引的方式来表示这12个三角形, 这样就做到了节约存储空间.这就是索引缓存
的意义.

IDirectDDevice9::DrawIndexedPrimitive()

HRESULT DrawIndexedPrimitive(
	[in] D3DPRIMITIVETYPE Type, // 表示要绘制的图元类型
	[in] INT BaseVertexIndex, // 顶点缓存的索引位, 这个位置将作为索引缓存的起始点
	[in] UINT MinIndex, // 索引数组的最小值, 一般是0, 这样索引就是 0,1,2,3...这样往后排
	[in] UINT NumVertices, // 顶点缓存中的多少个顶点做为索引
	[in] UINT StartIndex, // 表示从索引缓存中的第几个索引开始绘制图形
	[in] UINT PrimitiveCount //要绘制的图元个数
);

使用索引缓存的步骤和顶点缓存是一致的 (想想也知道嘛, 就是多用了一个数组而已)
1.设计定点格式
struct CUSTOMVERTEX
{
	FLOAT X, Y, Z, rhw;
	DWORD color;
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

2.创建顶点缓存以及索引缓存
顶点缓存在 17 的时候介绍过了, 这里只介绍索引缓存
IDirect3DIndexBuffer9
HRESULT CreateIndexBuffer(
	[in] UINT Length, // 索引缓存的大小
	[in] DWORD Usage, // 附加信息, 裁剪不裁剪各种的
	[in] D3DFORMAT Format, // 索引缓存中每个索引的大小
	[in] D3DPOOL Pool, // 17 有介绍
	[out, retval] IDirect3DIndexBuffer9 **ppIndexBuffer, // 这个不用说了, 索引缓存数组的地址咯
	[in] HANDLE *pShareHandle // NULL
)

D3DFORMAT 是一个枚举类型, 有一大坨值, 这里只列举我认为这个函数可能用到的
typdef enum _D3DFORMAT{
	...
	D3DFMT_INDEX16 = 101,
    D3DFMT_INDEX32 = 102,
	...
}

3.赋值顶点缓存以及索引缓存
顶点缓存 17 已经介绍过了, 这里只介绍索引缓存
这里的枷锁解锁和顶点缓存一个意思

HRESULT Lock(
	[in] UINT OffsetToLock,
	[in] UINT SizeToLock,
	[out] VOID **ppbData,
	[in] DWORD Flags
);

HRESULT Unlock()

赋值的2种方式：
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

4.绘制图形
	HRESULT SetStreamSource(
		[in] UINT StreamNumber, //0
		[in] IDirect3DVertexBuffer9 *pStreamData, //这个不就是顶点缓存指针吗
		[in] UINT OffsetInBytes, //0
		[in] UINT Stride //顶点缓存中的每个顶点结构的大小
	)

	HRESULT SetFVF(
		[in] DWORD FVF //设置的FVF的格式
	)

	// 指定索引数组
	HRESULT SetIndices(
		[in] IDirect3DIndexBuffer9 *pIndexData
	)

	HRESULT DrawIndexedPrimitive(
		[in] D3DPRIMITIVETYPE Type, // 表示要绘制的图元类型
		[in] INT BaseVertexIndex, // 顶点缓存的索引位, 这个位置将作为索引缓存的起始点
		[in] UINT MinIndex, // 索引数组的最小值, 一般是0, 这样索引就是 0,1,2,3...这样往后排
		[in] UINT NumVertices, // 顶点缓存中的多少个顶点做为索引
		[in] UINT StartIndex, // 表示从索引缓存中的第几个索引开始绘制图形
		[in] UINT PrimitiveCount //要绘制的图元个数
	)

**/

#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>

//添加依赖
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

//定义宏
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE L"Hello World!"
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)
#define SAFE_RELEASE(p) {if(p){(p)->Release(); (p)=NULL;}}

//全局变量声明
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;
LPD3DXFONT g_pFont = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;

//结构体声明
struct CUSTOMVERTEX {
	FLOAT x, y, z, rhw;
	DWORD color;
};

//函数声明
//处理消息函数
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

//Game Flow 函数
HRESULT Direct3D_Init(HWND hwnd); // Direct3D 的初始化
HRESULT Objects_Init(HWND hwnd); // 要绘制的物体的资源初始化
VOID Direct3D_Render(HWND hwnd); // 渲染
VOID Direct3D_CleanUp(); //清理资源

						 //主函数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//1.设计一个完整的窗口类
	WNDCLASSEX wndClass = { 0 };//定义一个窗口类
	wndClass.cbSize = sizeof(WNDCLASSEX);//设置结构体的大小
	wndClass.style = CS_HREDRAW | CS_VREDRAW;//设置窗口样式
	wndClass.lpfnWndProc = WndProc;//设置消息处理函数
	wndClass.cbClsExtra = 0;//窗口类的附加内存，一般设置为0
	wndClass.cbWndExtra = 0;//窗口的附加内存，一般设置为0
	wndClass.hInstance = hInstance;//包含窗体实例的程序的句柄
	wndClass.hIcon = (HICON)LoadImage(NULL, L"icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);//设置一个图标
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//指定一个灰色画刷句柄
	wndClass.lpszClassName = NULL;//不需要下拉菜单
	wndClass.lpszClassName = L"HWWND";//指定窗口类的名字

									  //2.注册窗口类
	if (!RegisterClassEx(&wndClass)) return -1;

	//3.创建窗体
	HWND hwnd = CreateWindow(L"HWWND", WINDOW_TITLE, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

	//4.窗体移动，显示和更新
	//MoveWindow(hwnd, 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, true);
	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	//初始化游戏资源
	if (S_OK == Direct3D_Init(hwnd)) {
	}
	else {
		MessageBox(hwnd, L"Direct初始化失败！", NULL, 0);
	}

	PlaySound(L"神秘.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

	//5.消息分发
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);//这个会把消息发送到winproc处理函数
		}
		else
		{
			//循环渲染
			Direct3D_Render(hwnd);
		}
	}

	//6.注销窗体
	UnregisterClass(L"HWWND", hInstance);

	return 0;
}

//消息处理函数
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//PAINTSTRUCT paintStruct;
	switch (message)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			DestroyWindow(hwnd);//销毁窗体并发送一个WM_DESTROY消息
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

	//1.创建Direct3D 接口对象
	LPDIRECT3D9 pD3D = NULL;
	if (NULL == (pD3D = Direct3DCreate9(D3D_SDK_VERSION))) //如果创建失败返回NULL
	{
		return E_FAIL;
	}

	//2.取得硬件设备信息
	//显卡的信息
	D3DCAPS9 caps; //被填充的显卡数据类型
	int vp = 0;
	//第一个参数是显卡的序号
	if (FAILED(pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))
	{
		return E_FAIL;
	}

	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)//如果支持硬件定点运算, 就采用用硬件计算； 如果不支持就采用软件计算
	{
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	//3.填充内容
	D3DPRESENT_PARAMETERS d3dpp;//其实就是传入参数，只不过如果直接传入那就太多了所以搞了一个结构来专门设置参数。
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

	//4.创建Direct3D设备接口
	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, vp, &d3dpp, &g_pd3dDevice))) // g_pd3dDevice 填充类型
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
		DEFAULT_QUALITY, 0, L"叶友根圆趣卡通体", &g_pFont)))
	{
		return E_FAIL;
	}
	srand(timeGetTime());

	//顶点缓存操作
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

	//索引数组
	WORD Indices[] = { 0,1,2, 0,2,3, 0,3,4, 0,4,5, 0,5,6, 0,6,7, 0,7,8, 0,8,9, 0,9,10, 0,10,11 ,0,11,12, 0,12,13 ,0,13,14 ,0,14,15 ,0,15,16, 0, 16,1 };
	// 填充索引数据
	WORD *pIndices = NULL;
	g_pIndexBuffer->Lock(0, 0, (void**)&pIndices, 0);
	memcpy(pIndices, Indices, sizeof(Indices));
	g_pIndexBuffer->Unlock();

	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, false);   //关掉背面消隐
	return S_OK;
}

void Direct3D_Render(HWND hwnd)
{
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	//定义一个矩形，用于获取主窗口矩形
	RECT formatRect;
	GetClientRect(hwnd, &formatRect);
	g_pd3dDevice->BeginScene();
	g_pd3dDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);//设置渲染状态
	g_pd3dDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(CUSTOMVERTEX));
	g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	g_pd3dDevice->SetIndices(g_pIndexBuffer);
	g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 17, 0, 16);

	formatRect.top = 400;
	g_pFont->DrawText(0, _T("苍茫的天涯是我的爱，绵绵的青山脚下花正开！"), -1, &formatRect, DT_CENTER,
		D3DCOLOR_XRGB(rand() % 256, rand() % 256, rand() % 256));//采用随机RGB值，做出“闪闪惹人爱”的特效
	g_pd3dDevice->EndScene();

	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

void Direct3D_CleanUp()
{
	//释放COM接口对象
	SAFE_RELEASE(g_pVertexBuffer)
		SAFE_RELEASE(g_pFont)
		SAFE_RELEASE(g_pd3dDevice)
}