/**
Direct 的创建
1.创接口
DirectX 遵循COM规范，所以需要先获取接口
2.取信息
3.填内容
4.创设备

Direct3D 渲染
1.清理操作
Clear 函数将后台缓冲区的内容进行清空
HRESULT Clear(
	[in] DWORD Count, //用来指定第二个参数的个数
	[in] const D3DRECT *pRects, //一个矩形数组，表名我们需要清空的目标矩形区域
	[in] DWORD Flags, //指定我们需要清空的缓冲区，有3种类型组合可以用 "|" 链接，D3DCLEAR_STENCIL, D3DCLEAR_TARGET, D3DCLEAR_ZUBUFFER 它们分别表示 模版缓冲区，颜色缓冲区，深度缓冲区。
	[in] D3DCOLOR Color, //用于我们在清空颜色缓冲区之后每个像素对应的颜色值，这里的颜色用D3DCOLOR宏表示，D3DCOLOR_XRGB(R, G, B) 在0-255之间取值
	[in] float Z, //用于指定清空深度缓冲区后每个像素对应的深度值 [0,1]
	[in] DWORD Stencil //用于指定清空模板缓冲区中每个像素对应的模版值。它的取值范围是0到2的n次方减1。其中n是模板缓冲的深度
)

还有其它几个可以返回 D3DCOLOR 的宏
D3DCOLOR_ARGB(a, r, g, b);
D3DCOLOR_RGBA(r, g, b, a);
D3DCOLPRVALUE 结构体
D3DXCOLOR 结构体

2.取得绘图设备
HRESULT BeginScene();// 开始绘图，与EndScene()对应

3.正式绘制
自己随意发挥

4.释放绘图设备
HRESULT EndScene(); // 结束绘图，与BeginScene()对应

5.翻转显示
HRESULT Present(
	[in] const RECT *pSourceRect, // 指向复制源矩形区域的指针
	[in] const RECT *pDestRect, // 指向复制目标矩形区域的指针
	[in] HWND hDestWindowOverride, //指向当前绘制的窗口句柄，如果设为NULL，则为 hDeviceWindows的值
	[in] const RGNDATA *pDIrtyRegion//指向最小更新区域的指针
);

知识点：
1. 创建字体
HRESULT D3DXCreateFont(
	__in LPDIRECT3DDEVICE9 pDevice, // Direct 设备
	__in INT Height, // 字体的高度
	__in UINT Width, // 字体的宽度
	__in UINT Weight, // 字体的粗度
	__in UNIT MipLevels, // 这个应该是和纹理映射有关
	__in BOOL Italic, // 是否是斜体
	__in DWORD CharSet, // 字符集
	__in DWORD OutputPrecision, // 文本精度 
	__in DWORD Quality, // 输出质量
	__in DWORD PitchAndFamily, // 字体索引号，  0
	__in LPCTSTR pFacename, // 字体名
	__out LPD3DXFONT *ppFont // 填充数据
);

INT DrawText(
	[in] LPD3DXSPRITE pSprite, // ID3DXSprite 对象接口
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

//添加依赖
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

//定义宏
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE L"Hello World!"

#define SAFE_RELEASE(p) {if(p){(p)->Release(); (p)=NULL;}}

//全局变量声明
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;
LPD3DXFONT g_pFont = NULL;

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

	PlaySound(L"NightElf3.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

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
	return S_OK;
}

void Direct3D_Render(HWND hwnd)
{
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	RECT formatRect;
	GetClientRect(hwnd, &formatRect);
	g_pd3dDevice->BeginScene();
	//第一段文字
	formatRect.top = 100;
	// 指定-1 时， 它会自动判断字符串长度
	g_pFont->DrawText(NULL, _T("这是一个什么鬼玩意啊啊啊啊啊啊啊！"), -1, &formatRect, DT_CENTER, D3DCOLOR_XRGB(68, 139, 256));

	//在纵坐标400处，写第三段文字
	formatRect.top = 400;
	g_pFont->DrawText(0, _T("闪死你啊啊啊啊啊啊啊啊，狗日的！"), -1, &formatRect, DT_CENTER,
		D3DCOLOR_XRGB(rand() % 256, rand() % 256, rand() % 256));//采用随机RGB值，做出“闪闪惹人爱”的特效

	g_pd3dDevice->EndScene();

	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

void Direct3D_CleanUp()
{
	//释放COM接口对象
	SAFE_RELEASE(g_pFont)
	SAFE_RELEASE(g_pd3dDevice)
}