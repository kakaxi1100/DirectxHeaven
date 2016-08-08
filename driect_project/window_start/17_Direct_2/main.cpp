/**
顶点缓存

知识点:
1.设计顶点缓存
	灵活顶点格式 FVF, 可以让玩家自定义顶点信息, 但是内容必须为D3D指定的内容, 因为这样它才知道怎么读取。
	格式有:
	D3DFVF_XYZ
	D3DFVF_XYZRHW
	D3DFVF_XYZB1~5
	D3DFVF_NORMAL
	D3DFVF_DIFFUSE
	D3DFVF_SPECULAR
	D3DFVF_TEXT1~8
	上面的格式可以随便选，但是定义结构的顺序必须和上面一致，如：
	struct CUSTOMVERTEX
	{
		float x, y, z;//D3DFVF_XYZ
		DWORD color;//D3DFVF_DIFFUSE
	}

	#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

2.创建顶点缓存
	声明接口对象 (retval 表示 return value)
	IDirect3DVertextBuffer9
	(LPDIRECT3DVERTEXBUFFER9)
	创建顶点缓存
	HRESULT CreateVertexBuffer(
		[in] UINT Length, //顶点缓存的大小, 以字节为单位,创建N个就是 N*单个顶点数据对象的大小
		[in] DWORD Usage, //一些附加属性
		[in] DWORD FVF, //顶点指定的格式
		[in] D3DPOOL pool, //一个枚举类型, 用来指定存储顶点缓存的内存位置
		[out, retval] IDirect3DVertextBuffer9 **ppVertexBuffer, //顶点缓存对象
		[in] HANDLE *pSharedHanle 
	);

	typedef enum D3DPOOL{
		D3DPOOL_DEFAULT = 0,
		D3DPOOL_MANAGED = 1,
		D3DPOOL_SYSTEMMEN = 2,
		D3DPOOL_SCRATCH = 3,
		D3DPOOL_FORCE_DWORD = 0X7fffffff
	} D3DPOOL, *LPD3DPOOL;

	AGP 非本地显存，即占用内存作为显示的地方
	显存
	系统内存

3.顶点缓存赋值
	在访问顶点之前先要lock,自己理解应该是lock了内存部分,访问完后需要解锁。
	IDirect3DVertextBuffer9::Lock();
	IDirect3DVertextBuffer9::Unlock();

	HRESULT Lock(
		[in] UINT OffsetToLock, //起始位置的偏移量
		[in] UINT SizeToLock, //要锁多大快区域, 按字节算
		[out] VOID **ppbData, //指向被锁定的存储区的起始地址的指针
		[in] DWORD Flags //锁定的方式
	);

	赋值顶点的2种方式：
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

	  PS:  g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, false);//关掉背面消隐, 即无论是否顺时针, 三角形都会显示

4.图形的绘制
	PS:	 g_pd3dDevice->SetRenderState(D3DRS_SHADEMODE,D3DSHADE_GOURAUD);//设置渲染状态

	绘制图形要调用3个函数
	IDirect3DDevice9::SetStreamSource
	IDirect3DDevice9::SetFVF
	IDirect3DDevice9::DrawPrimitive

	HRESULT SetStreamSource(
		[in] UINT StreamNumber, //0
		[in] IDirect3DVertexBuffer9 *pStreamData, //这个不就是顶点缓存指针吗
		[in] UINT OffsetInBytes, //0
		[in] UINT Stride //顶点缓存中的每个顶点结构的大小
	)

	HRESULT SetFVF(
		[in] DWORD FVF //设置的FVF的格式
	)
	如:
	SetFVF(D3DFVF_CUSTOMVERTEX);

	HRESULT DrawPrimitive(
		[in] D3DPRIMITIVETYPE PrimitiveType, //枚举类型， 表示要绘制的图元类型
		[in] UINT StartVertex, //从顶点缓存中读取顶点数据的起始索引位置
		[in] UINT PrimitiveCount //指定要绘制的图元数量
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
	g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	formatRect.top = 400;
	g_pFont->DrawText(0, _T("闪死你啊啊啊啊啊啊啊啊，狗日的！"), -1, &formatRect, DT_CENTER,
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