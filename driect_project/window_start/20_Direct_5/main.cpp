/**
看到 14.6.... 未完, 待续....






光照
四大光照类型
	一、环境光(Ambient Light)
		设置环境光
		IDirect3DDevice9::SetRenderState
		HRESULT SetRenderState(
			[in] D3DRENDERSTATETYPE State, //设置
			[in] DWORD              Value //填一个颜色值
		);

		D3DRENDERSTATETYPE 是一个枚举类型 也是超级多内容
		typedef enum D3DRENDERSTATETYPE {
			D3DRS_ZENABLE                     = 7,
			D3DRS_FILLMODE                    = 8,
			D3DRS_SHADEMODE                   = 9,
			D3DRS_ZWRITEENABLE                = 14,
			D3DRS_ALPHATESTENABLE             = 15,
			D3DRS_LASTPIXEL                   = 16,
			D3DRS_SRCBLEND                    = 19,
			D3DRS_DESTBLEND                   = 20,
			D3DRS_CULLMODE                    = 22,
			....
			D3DRS_SEPARATEALPHABLENDENABLE    = 206,
			D3DRS_SRCBLENDALPHA               = 207,
			D3DRS_DESTBLENDALPHA              = 208,
			D3DRS_BLENDOPALPHA                = 209,
			D3DRS_FORCE_DWORD                 = 0x7fffffff
		} D3DRENDERSTATETYPE, *LPD3DRENDERSTATETYPE;

		如：
		g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(36,36,3600));

	二、漫反射光(Diffuse Light)

	三、镜面反射光(Specular Light)
		镜面放射默认是关闭的, 要开启的话可以这样:
		g_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, true);

	四、自发光(Emissive Light)
		自发光影响着一个对象的颜色。但是自发光属性创建的材质并不发射出能被场景内其它对象放射的光。

光源
	光源涉及到的方法及结构体:

	typedef struct D3DLIGHT9 {
		D3DLIGHTTYPE  Type; //光源类型
		D3DCOLORVALUE Diffuse; //漫反射颜射值
		D3DCOLORVALUE Specular; //镜面反射颜色值
		D3DCOLORVALUE Ambient; //环境光颜色值
		D3DVECTOR     Position; //光源的位置
		D3DVECTOR     Direction; //光照的方向
		float         Range; //光照的范围, This member does not affect directional lights. 
		float         Falloff; //聚光灯类型参数
		float         Attenuation0; //衰减系数
		float         Attenuation1;
		float         Attenuation2;
		float         Theta;//聚光灯类型参数
		float         Phi;//聚光灯类型参数
	} D3DLIGHT9, *LPD3DLIGHT;

	typedef enum D3DLIGHTTYPE {
		D3DLIGHT_POINT        = 1,
		D3DLIGHT_SPOT         = 2,
		D3DLIGHT_DIRECTIONAL  = 3,
		D3DLIGHT_FORCE_DWORD  = 0x7fffffff
	} D3DLIGHTTYPE, *LPD3DLIGHTTYPE;

	衰减系数公式:
	D为光源到顶点的距离
	衰减值 = 1/(A0 + A1*D + A2 + D²)

	1.设置光源
	IDirect3DDevice9::SetLight
	HRESULT SetLight(
	  [in]       DWORD     Index, //0~7之间，表示1~8个光源
	  [in] const D3DLIGHT9 *pLight //这个就是上面的结构体啦
	);

	2.开启光源
	IDirect3DDevice9::LightEnable
	HRESULT LightEnable(
		[in] DWORD LightIndex,//0~7之间，表示1~8个光源
		[in] BOOL  bEnable //true/false 表示启用或禁用第一个参数指定的光源
	);

三大光源
	一、点光源(Point Light)
		点光源具有颜色和位置, 但是没有方向, 它向所有方向发射的光都是一样的。

	二、平行光(Directional Light)
		平行光是从无穷远处发出的一组平行、均匀的光线, 只具有颜色和方向, 不收衰减和范围的影响。

	三、聚光灯(Spot Light)
		聚光灯由一个明亮的内锥体和大一点的外锥体组成。
		Falloff用于控制光强如何从内锥体的外侧向外锥体的内侧减弱的。

材质
	物体表面的材质属性决定了它能反射什么颜色的光线以及能反射多少。

	材质涉及到的方法及结构体:

	typedef struct D3DMATERIAL9 {
		D3DCOLORVALUE Diffuse; // Value specifying the diffuse color of the material.
		D3DCOLORVALUE Ambient; // Value specifying the ambient color of the material.
		D3DCOLORVALUE Specular; // Value specifying the specular color of the material. 
		D3DCOLORVALUE Emissive; // Value specifying the emissive color of the material.
		float         Power; // Floating-point value specifying the sharpness of specular highlights. The higher the value, the sharper the highlight.
	} D3DMATERIAL9, *LPD3DMATERIAL9;

	typedef struct _D3DCOLORVALUE {
		float r;
		float g;
		float b;
		float a;
	} D3DCOLORVALUE;

	物体顶点颜色的亮度总和公式:
	Itotal = Iambient+Idiffuse+Ispecular+Iemissive

	设置材质:
	IDirect3DDevice9::SetMaterial
	HRESULT SetMaterial(
		[in] const D3DMATERIAL9 *pMaterial
	);

	IDirect3DDevice9::GetMaterial
	HRESULT GetMaterial(
		[out] D3DMATERIAL9 *pMaterial
	);

顶点法线

	struct CUSTOMVERTEX {
		FLOAT x, y, z;
		FLOAT nx, ny, nz;
		DWORD color;
	｝
	#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE)
	
	向量规范化
	g_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);

14.6.....未完待续
};
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
#define WINDOW_TITLE L"光!"
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
	g_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
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
	SAFE_RELEASE(g_pIndexBuffer)
	SAFE_RELEASE(g_pVertexBuffer)
	SAFE_RELEASE(g_pFont)
	SAFE_RELEASE(g_pd3dDevice)
}