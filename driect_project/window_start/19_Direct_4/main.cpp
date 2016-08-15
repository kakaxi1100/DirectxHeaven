/**
四大变换
一、世界变换
	世界变换包括平移, 旋转和缩放, 它们的函数分别是:
	(D3DXMatrixTranslation)
	D3DXMATRIX* D3DXMatrixTranslation(
		_Inout_ D3DXMATRIX *pOut, //对应的矩阵
		_In_    FLOAT      x, //x轴偏移量
		_In_    FLOAT      y, //y轴偏移量
		_In_    FLOAT      z //z轴偏移量
	);
	例:
	D3DXMATRIX mTrans;//声明一个临时矩阵
	D3DXMatrixTranslation(&mTrans, 0,0,10);//对临时矩阵赋值
	//与实际要用到的矩阵做乘积
	D3DXMatrixMultiply(&mMatrix, &mMatrix, &mTrans);//其中mMatrix是我们实际用到的矩阵

	D3DXMATRIX* D3DXMatrixMultiply(
		_Inout_       D3DXMATRIX *pOut, //需要输出的那个矩阵
		_In_    const D3DXMATRIX *pM1, // 两个需要相乘的矩阵
		_In_    const D3DXMATRIX *pM2
	);


	(D3DXMatrixRotationX D3DXMatrixRotationY D3DXMatrixRotationZ)
	D3DXMATRIX* D3DXMatrixRotationX(
		_Inout_ D3DXMATRIX *pOut, //做为输出结果的矩阵
		_In_    FLOAT      Angle //要旋转的弧度值
	);
	例:
	D3DXMATRIX mTrans;//声明一个临时矩阵
	float fAngle=90*(2.0f*D3DX_PI)/360.0f;
	D3DXMatrixRotationY(&mTrans, fAngle);
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &mTrans);
	//与实际要用到的矩阵做乘积
	D3DXMatrixMultiply(&mMatrix, &mMatrix, &mTrans);

	(D3DXMatrixScaling)
	D3DXMATRIX* D3DXMatrixScaling(
		_Inout_ D3DXMATRIX *pOut, // 做为输出结果的矩阵
		_In_    FLOAT      sx, // 在X轴上缩放的比例
		_In_    FLOAT      sy, // 在Y轴上缩放的比例
		_In_    FLOAT      sz // 在Z轴上缩放的比例
	);
	D3DXMATRIX mTrans;//声明一个临时矩阵
	D3DXMatrixScaling(&mTrans, 1.0f, 1.0f, 5.0f);
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &mTrans);
	//与实际要用到的矩阵做乘积
	D3DXMatrixMultiply(&mMatrix, &mMatrix, &mTrans);

	应用矩阵:
	HRESULT SetTransform(
		[in] D3DTRANSFORMSTATETYPE State,
		[in] D3DMATRIX *pMatrix
	);

	D3DTRANSFORMSTATETYPE 是一个枚举类型,表示这个矩阵变换是四大变换前三大变换中的那一种, 原型如下:
	typedef enum D3DTRANSFORMSTATETYPE {
		D3DTS_VIEW         = 2, // 取景变换
		D3DTS_PROJECTION   = 3, // 投影变换
		D3DTS_TEXTURE0     = 16, // 某层纹理的变换
		D3DTS_TEXTURE1     = 17, //...
		D3DTS_TEXTURE2     = 18,
		D3DTS_TEXTURE3     = 19,
		D3DTS_TEXTURE4     = 20,
		D3DTS_TEXTURE5     = 21,
		D3DTS_TEXTURE6     = 22,
		D3DTS_TEXTURE7     = 23,
		D3DTS_FORCE_DWORD  = 0x7fffffff // 世界变换
	} D3DTRANSFORMSTATETYPE, *LPD3DTRANSFORMSTATETYPE;

二、取景变换(相机变换)
	设置取景变换矩阵, LH-left hand 左手坐标系, 还有一个对应的RH的函数表示的是右手坐标系
	D3DXMATRIX* D3DXMatrixLookAtLH(
		_Inout_       D3DXMATRIX  *pOut,// 做为输出结果的矩阵
		_In_    const D3DXVECTOR3 *pEye, // 指定虚拟摄像机在世界坐标系中的位置
		_In_    const D3DXVECTOR3 *pAt, // 观察点在世界坐标系中的位置
		_In_    const D3DXVECTOR3 *pUp // 定义当前世界的向上向量，通常为 [0, 1, 0]
	);
	//矢量结构
	typedef struct D3DXVECTOR3 {
		FLOAT x;
		FLOAT y;
		FLOAT z;
	} D3DXVECTOR3, *LPD3DXVECTOR3;

三、投影变换
	D3DXMATRIX* D3DXMatrixPerspectiveFovLH(
		_Inout_ D3DXMATRIX *pOut,// 做为输出结果的矩阵
		_In_    FLOAT      fovy, //视域角度, 即虚拟摄像机在Y轴上的成像角度
		_In_    FLOAT      Aspect, // 显示区域(应该是视口)的横纵比
		_In_    FLOAT      zn, // 进裁面距摄像机的位置
		_In_    FLOAT      zf // 远裁面距摄像机的位置
	);

四、视口变换
	视口即显示画面的区域，可以自己设置

	typedef struct D3DVIEWPORT9 {
		DWORD X; // 视口相对于窗口的X坐标
		DWORD Y; // 视口相对于窗口的Y坐标
		DWORD Width; // 视口的宽度
		DWORD Height; // 视口的高度
		float MinZ; // 视口在深度缓存中的最小深度值
		float MaxZ; // 视口在深度缓存中的最大深度值
	} D3DVIEWPORT9, *LPD3DVIEWPORT9;

	IDIRECT3DDEVICE9::SetViewPort()
	HRESULT SetViewport(
		[in] const D3DVIEWPORT9 *pViewport //视口信息
	);

其它用到的函数：

	//把矩阵转化为单位矩阵
	D3DXMATRIX* D3DXMatrixIdentity(
		_Inout_ D3DXMATRIX *pOut
	);

	//清屏函数 14 里面有介绍
	HRESULT Clear(
		[in]       DWORD    Count, // 指定pRects中 D3DRECT 的数量
		[in] const D3DRECT  *pRects, //Pointer to an array of D3DRECT structures that describe the rectangles to clear. Set a rectangle to the dimensions of the rendering target to clear the entire surface. Each rectangle uses screen coordinates that correspond to points on the render target. Coordinates are clipped to the bounds of the viewport rectangle. To indicate that the entire viewport rectangle is to be cleared, set this parameter to NULL and Count to 0.
		[in]       DWORD    Flags, // Combination of one or more D3DCLEAR flags that specify the surface(s) that will be cleared.
		[in]       D3DCOLOR Color, // Clear a render target to this ARGB color.
		[in]       float    Z, // Clear the depth buffer to this new z value which ranges from 0 to 1. See remarks.
		[in]       DWORD    Stencil // Clear the stencil buffer to this new value which ranges from 0 to 2ⁿ-1 (n is the bit depth of the stencil buffer). See remarks. 
	);

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
#define WINDOW_TITLE L"矩阵变换!"
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)
#define SAFE_RELEASE(p) {if(p){(p)->Release(); (p)=NULL;}}

//全局变量声明
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;
LPD3DXFONT g_pFont = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;

//结构体声明
struct CUSTOMVERTEX {
	FLOAT x, y, z;
	DWORD color;
};

//函数声明
//处理消息函数
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
VOID Matrix_Set();

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

VOID Matrix_Set()
{
	//世界变换矩阵的设置
	D3DXMATRIX matWorld, Rx, Ry, Rz;
	D3DXMatrixIdentity(&matWorld); //转成单位矩阵
	//绕轴旋转
	D3DXMatrixRotationX(&Rx, D3DX_PI*(::timeGetTime() / 1000.0f));
	D3DXMatrixRotationY(&Ry, D3DX_PI*(::timeGetTime() / 1000.0f / 2));
	D3DXMatrixRotationZ(&Rz, D3DX_PI*(::timeGetTime() / 1000.0f / 3));
	matWorld = Rx * Ry * Rz * matWorld;
	//matWorld = Ry*matWorld;
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	//相机变换
	D3DXMATRIX matView;
	D3DXVECTOR3 vEye(0.0f, 0.0f, -200.0f);
	D3DXVECTOR3 vAt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);//  计算出相机的变换矩阵
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	//投影变换
	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0f, 1.0f, 1.0f, 1000.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	//视口变换
	D3DVIEWPORT9 vp;
	vp.X = 0;
	vp.Y = 0;
	vp.Width = 800;
	vp.Height = 600;
	vp.MinZ = 0.0f; // 近裁面
	vp.MaxZ = 1.0f; // 远裁面
	g_pd3dDevice->SetViewport(&vp);
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
	d3dpp.BackBufferCount = 1;
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

	// 设置渲染状态
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);      //关闭光照
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);   //开启背面消隐

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
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(8 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT,
		&g_pVertexBuffer, NULL)))
	{
		return E_FAIL;
	}

	if (FAILED(g_pd3dDevice->CreateIndexBuffer(36 * sizeof(WORD), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT,
		&g_pIndexBuffer, NULL)))
	{
		return E_FAIL;
	}

	CUSTOMVERTEX Vertices[] =
	{
		{ -20.0f, 20.0f, -20.0f,  D3DCOLOR_XRGB(rand() % 256, rand() % 256, rand() % 256) },
		{ -20.0f, 20.0f, 20.0f,  D3DCOLOR_XRGB(rand() % 256, rand() % 256, rand() % 256) },
		{ 20.0f, 20.0f, 20.0f,  D3DCOLOR_XRGB(rand() % 256, rand() % 256, rand() % 256) },
		{ 20.0f, 20.0f, -20.0f,  D3DCOLOR_XRGB(rand() % 256, rand() % 256, rand() % 256) },
		{ -20.0f, -20.0f, -20.0f, D3DCOLOR_XRGB(rand() % 256, rand() % 256, rand() % 256) },
		{ -20.0f, -20.0f, 20.0f, D3DCOLOR_XRGB(rand() % 256, rand() % 256, rand() % 256) },
		{ 20.0f, -20.0f, 20.0f, D3DCOLOR_XRGB(rand() % 256, rand() % 256, rand() % 256) },
		{ 20.0f, -20.0f, -20.0f, D3DCOLOR_XRGB(rand() % 256, rand() % 256, rand() % 256) },

	};

	VOID* pVertices;
	if (FAILED(g_pVertexBuffer->Lock(0, sizeof(Vertices), (void**)&pVertices, 0)))
		return E_FAIL;
	memcpy(pVertices, Vertices, sizeof(Vertices));
	g_pVertexBuffer->Unlock();

	//索引数组
	WORD *pIndices = NULL;
	g_pIndexBuffer->Lock(0, 0, (void**)&pIndices, 0);

	// 顶面
	pIndices[0] = 0, pIndices[1] = 1, pIndices[2] = 2;
	pIndices[3] = 0, pIndices[4] = 2, pIndices[5] = 3;
	// 正面
	pIndices[6] = 0, pIndices[7] = 3, pIndices[8] = 7;
	pIndices[9] = 0, pIndices[10] = 7, pIndices[11] = 4;
	// 左侧面
	pIndices[12] = 0, pIndices[13] = 4, pIndices[14] = 5;
	pIndices[15] = 0, pIndices[16] = 5, pIndices[17] = 1;
	// 右侧面
	pIndices[18] = 2, pIndices[19] = 6, pIndices[20] = 7;
	pIndices[21] = 2, pIndices[22] = 7, pIndices[23] = 3;
	// 背面
	pIndices[24] = 2, pIndices[25] = 5, pIndices[26] = 6;
	pIndices[27] = 2, pIndices[28] = 1, pIndices[29] = 5;
	// 底面
	pIndices[30] = 4, pIndices[31] = 6, pIndices[32] = 5;
	pIndices[33] = 4, pIndices[34] = 7, pIndices[35] = 6;
	g_pIndexBuffer->Unlock();

	return S_OK;
}

void Direct3D_Render(HWND hwnd)
{
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	//定义一个矩形，用于获取主窗口矩形
	RECT formatRect;
	GetClientRect(hwnd, &formatRect);
	g_pd3dDevice->BeginScene();
	Matrix_Set();//调用封装了四大变换的函数，对Direct3D世界变换，取景变换，投影变换，视口变换进行设置
				 // 获取键盘消息并给予设置相应的填充模式
	if (::GetAsyncKeyState(0x31) & 0x8000f)         // 若数字键1被按下，进行线框填充
		g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	if (::GetAsyncKeyState(0x32) & 0x8000f)         // 若数字键2被按下，进行实体填充
		g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	g_pd3dDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(CUSTOMVERTEX));//把包含的几何体信息的顶点缓存和渲染流水线相关联
	g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);//指定我们使用的灵活顶点格式的宏名称
	g_pd3dDevice->SetIndices(g_pIndexBuffer);//设置索引缓存
	g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);//利用索引缓存配合顶点缓存绘制图形

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