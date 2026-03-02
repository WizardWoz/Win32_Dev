#include <Windows.h>
#include <stdio.h>

/*
 * 绘图基础
 * 1.绘图设备/绘图上下文绘图描述表DC（Device Context）：一个结构体，包含了绘图设备的各种参数信息；每个窗口都有一个与之对应的设备DC，基于设备DC进行绘图
 * 2.绘图设备句柄HDC：DC句柄，表示绘图设备
 * 3.GDI(Windows Graphic Device Interface)：Win32提供的绘图API
 * 4.颜色使用 COLORREF类型(DWORD)，RGB宏函数创建颜色值；COLORREF nColor = RGB(255, 0, 0);	//红色
 *	 获取RGB值：BYTE r = GetRValue(nColor);	BYTE g = GetGValue(nColor);	BYTE b = GetBValue(nColor);
 * 5.基本图形绘制函数：
 * (1)画点：BeginPaint函数获取DC句柄，EndPaint函数释放DC句柄；GetPixel获取指定点的颜色，SetPixel设置指定点的颜色
 * (2)画线：MoveToEx设置起点坐标，LineTo设置终点坐标并绘制线段；当前点是上一次绘图时的最后一点，初始为(0,0)；
 * (3)封闭图形：能够用画刷填充的图形（例：矩形、椭圆、多边形等），需要调用BeginPath函数开始路径，调用EndPath函数结束路径，调用StrokeAndFillPath函数绘制并填充图形
 * 
 * 6.GDI绘图对象
 * (1)画笔：HPEN，定义了线条的颜色、线形（直线/虚线/点型）、线粗等参数；
 *	  <1>创建画笔函数CreatePen
 *	  PS_SOLID（实线，支持多个像素宽度）、PS_DASH（虚线）、PS_DOT（点线）、PS_DASHDOT（虚点线）、PS_DASHDOTDOT（虚点点线）
 *	  <2>将画笔应用到DC中SelectObject函数，返回值是原来DC中应用的画笔句柄，注意保存
 *	  <3>绘图
 *	  <4>取出DC中的画笔，使用SelectObject函数将原来的画笔重新应用到DC中
 *	  <5>释放画笔DeleteObject函数，只能删除不被DC使用的画笔；所以释放前必须将画笔从DC中取出（第4步）
 * (2)画刷：HBRUSH，定义了填充图形的颜色、填充样式等参数；DC默认使用一个白色实心画刷
 *    <1>创建画刷函数CreateSolidBrush（实心画刷）、CreateHatchBrush（交叉线画刷）、CreatePatternBrush（图案画刷）等
 *	  <2>将画刷应用到DC中SelectObject函数，返回值是原来DC中应用的画刷句柄，注意保存
 *	  <3>绘图
 *	  <4>取出DC中的画刷，使用SelectObject函数将原来的画刷重新应用到DC中
 *	  <5>释放画刷DeleteObject函数，只能删除不被DC使用的画刷；所以释放前必须将画刷从DC中取出（第4步）
 * (3)使用GetStockObject获取系统维护的画刷、画笔，如不使用画刷填充，则需要使用NULL_BRUSH参数获取不填充的画刷，该方法返回的对象无需DeleteObject
*/

HINSTANCE g_hInstance = 0;	//保存WinMain的第一个参数（当前程序实例句柄）
//调试程序的方法：附加一个控制台窗口输出
HANDLE g_hOutput = 0;	//接受标准输出句柄

void DrawPit(HDC hdc)
{
	//绘制点，坐标(0,0)到(99,99)，颜色从黑色到黄色渐变
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			SetPixel(hdc, i, j, RGB(i, j, 0));
		}
	}
}

void DrawLine(HDC hdc)
{
	MoveToEx(hdc, 100, 100, NULL);	//设置起点坐标(20,20)，如果不调用MoveToEx函数，默认起点坐标为(0,0)
	LineTo(hdc, 200, 200);	//设置终点坐标(200,200)并绘制线段，此时窗口当前点为(200,200)
	LineTo(hdc, 0, 200);	//设置终点坐标(0,200)并绘制线段
	LineTo(hdc, 100, 100);	//设置终点坐标(100,100)并绘制线段，此时窗口当前点为(100,100)
	//看似是一个封闭图形，但并不能被画刷填充，所以并不是真正意义的封闭图形
}

void DrawRect(HDC hdc)
{
	//绘制矩形，左上角坐标(300,300)，宽度100，高度50
	Rectangle(hdc, 200, 200, 300, 250);	//右下角坐标(300,250) = (左上角X坐标+宽度，左上角Y坐标+高度)
}

void DrawEllipse(HDC hdc)
{
	//绘制椭圆，外接矩形左上角坐标(300,300)，宽度100，高度50，画出来的是外接矩形的内切椭圆
	Ellipse(hdc, 300, 300, 400, 350);	//外接矩形右下角坐标(400,350) = (左上角X坐标+宽度，左上角Y坐标+高度)
}

void OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps = { 0 };
	HDC hdc = BeginPaint(hWnd, &ps);
	//1.创建一个红色实线画笔，宽度为2像素
	HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	//创建一个绿色实心画刷
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 255, 0));
	//创建一个蓝色交叉线画刷
	//HBRUSH hBrush2 = CreateHatchBrush(HS_CROSS, RGB(0, 0, 255));
	//创建一个透明画刷
	HGDIOBJ hBrush3 = GetStockObject(NULL_BRUSH);
	//2.将画笔应用到DC中，保存原来DC中应用的画笔句柄
	HGDIOBJ hOldPen = SelectObject(hdc, hPen);
	HGDIOBJ hOldBrush = SelectObject(hdc, hBrush);
	//HGDIOBJ hOldBrush2 = SelectObject(hdc, hBrush2);
	//HGDIOBJ hOldBrush3 = SelectObject(hdc, hBrush3);
	//3.绘图（注意：必须在应用了画笔的DC中绘图，否则无法使用该画笔的参数）
	DrawPit(hdc);		//绘制点
	DrawLine(hdc);		//绘制线段
	DrawRect(hdc);		//绘制矩形
	DrawEllipse(hdc);	//绘制椭圆
	//4.取出DC中的画笔，使用SelectObject函数将原来的画笔重新应用到DC中
	SelectObject(hdc, hOldPen);
	//取出DC中的画刷，使用SelectObject函数将原来的画刷重新应用到DC中
	SelectObject(hdc, hOldBrush);
	//SelectObject(hdc, hOldBrush2);
	//SelectObject(hdc, hOldBrush3);
	//5.释放画笔
	DeleteObject(hPen);
	//释放画刷
	DeleteObject(hBrush);
	//DeleteObject(hBrush2);
	//DeleteObject(hBrush3);	//使用GetStockObject获取的对象无需DeleteObject
	EndPaint(hWnd, &ps);
}

/*
  窗口创建过程：
  1.定义WinMain函数，作为Windows应用程序的入口点
  2.定义窗口过程函数WndProc，处理窗口的各种消息
  3.注册窗口类WNDCLASS wc，向操作系统内核写入一些数据
  4.创建窗口CreateWindow，在内存中创建窗口
  5.显示窗口ShowWindow，将内存中的窗口显示到屏幕上
  6.消息循环Message Loop，从消息队列中获取消息并分发给窗口过程函数WndProc处理
  7.各种消息处理
*/
/*
  系统调用函数的参数类型：
  LPSTR==char*；LPCSTR==const char*；
  LPWSTR==wchar_t*；LPCWSTR==const wchar_t*；
  LPCTSTR==TCHAR*；LPCTSTR==const TCHAR*；
*/

/*
  2.窗口过程函数WndProc，处理窗口的各种消息
  传参时只传消息的四个组成部分：窗口句柄、消息ID、消息参数wParam、消息参数lParam
  其实不用关心其他两个组成部分：产生的时间、产生时的鼠标位置

  函数名，参数名可以改，但是返回值类型和参数类型不能改
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT msgID, WPARAM wParam, LPARAM lParam)
{
	/*
		WM_DESTROY消息：窗口被销毁时发送，wParam=0，lParam=0，常用于在窗口销毁之前做相应的善后处理（例：资源、内存）

		WM_SYSCOMMAND消息：点击窗口右上角的按钮时发送，wParam=具体点击的位置（例：关闭SC_CLOSE、最大化SC_MAXIMIZE），
		lParam=鼠标光标位置，LOWORD(lParam)水平位置，HIWORD(lParam)垂直位置；常用于窗口关闭时提示用户处理

		WM_CREATE消息：窗口创建成功但还未显示，wParam=0，lParam=指向CREATESTRUCT结构体的指针，通过该指针可以获取CreateWindowEx函数的全部12个参数
		常用于初始化窗口的参数、资源等，包括创建子窗口

		WM_SIZE消息：窗口大小发生变化后，wParam=窗口大小变化的原因，lParam=窗口变化后的大小，LOWORD(lParam)宽度，HIWORD(lParam)高度
		常用于窗口大小变化后，调整窗口内各个部分的布局

		WM_QUIT消息：程序员控制发送时间，wParam=PostQuitMessage函数的参数，lParam=0，常用于结束消息循环，退出程序
		不需要程序员自定义函数处理
	*/
	switch (msgID)
	{
	case WM_PAINT:
		OnPaint(hWnd);
		break;
	default:
		break;
	}
	//在自定义窗口过程函数中不处理的消息、子窗口消息等，都会调用默认窗口过程函数DefWindowProc处理
	return DefWindowProc(hWnd, msgID, wParam, lParam);
}
/*
  1.定义WinMain函数，作为Windows应用程序的入口点
  句柄：Windows操作系统中用于标识各种对象内存位置的一个唯一值，但绝不是指针（三个句柄例外）
  1.标准输入句柄 2.标准输出句柄 3.标准错误句柄；相当于Linux中的文件描述符0（标准输入设备）、1（标准输出设备）、2（标准错误设备）
  HINSTANCE实例句柄；HWND窗口句柄
*/
int CALLBACK WinMain(_In_ HINSTANCE hIns,	//当前程序的实例句柄
	_In_opt_ HINSTANCE hPrevIns,		//上一个同类程序的实例句柄，现已弃用，始终为NULL
	_In_ LPSTR lpCmdLine,		//以字符串形式传递给程序的命令行参数
	_In_ int nCmdShow)			//指定窗口如何显示的标志
{
	g_hInstance = hIns;	//保存WinMain的第一个参数（当前程序实例句柄）
	AllocConsole();	//分配控制台窗口
	g_hOutput = GetStdHandle(STD_OUTPUT_HANDLE);	//获取标准输出句柄

	//3.注册窗口类WNDCLASS wc，向操作系统内核写入一些数据
	//窗口类：包含了窗口各种参数信息的数据结构（结构体）
	//每个窗口都具有窗口类，基于窗口类创建窗口
	//每个窗口类都具有一个名称，使用前必须注册到系统

	/*
	  1.系统窗口类：系统已定义好的窗口类，所有应用程序直接使用即可；例如：按钮BUTTON、编辑框EDIT、列表框LISTBOX等
	  2.应用程序全局窗口类：应用程序定义的窗口类，当前应用程序的所有模块（主模块、库模块）都可以使用，
	  需要在窗口类风格wc.style中添加CS_GLOBALCLASS
	  3.应用程序局部窗口类：应用程序定义的窗口类，当前应用程序的本模块可以使用，窗口类风格wc.style不添加CS_GLOBALCLASS
	*/
	WNDCLASS wc = { 0 };
	wc.cbClsExtra = 0;		//窗口类的附加数据缓冲区，赋值多少，就开辟多少字节的缓冲区
	wc.cbWndExtra = 0;		//窗口的附加数据缓冲区，赋值多少，就开辟多少字节的缓冲区
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	//绘制窗口背景的画刷句柄，默认是一个白色实心画刷
	wc.hCursor = NULL;		//窗口光标句柄，NULL表示使用系统默认光标
	wc.hIcon = NULL;		//窗口图标句柄，NULL表示使用系统默认图标
	wc.hInstance = hIns;	//当前模块的实例句柄
	wc.lpfnWndProc = WndProc;	//窗口过程处理函数
	//宽字节字符wchar_t每个字符占2个字节，窄字节字符char每个字符占1个字节
	//定义宽字节字符串时，需要在字符串前加L，例如L"Main"
	wc.lpszClassName = L"Main";	//窗口类名
	//1.注册窗口类时设置菜单
	//wc.lpszMenuName = (LPCWSTR)IDR_MENU1;	//窗口菜单的资源ID字符串
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;	//窗口风格，水平、垂直方向有变化时重绘窗口
	//常见的窗口风格：CS_DBLCLKS（双击消息）、CS_NOCLOSE（无关闭按钮）
	//注册窗口类的函数，注册成功后返回非0值，注册失败返回0
	RegisterClass(&wc);	//将以上赋值全部写入操作系统

	//4.创建窗口CreateWindow，在内存中创建窗口，创建成功则返回窗口句柄
	//TEXT宏根据#ifdef  UNICODE，来决定创建宽字节字符窗口还是窄字节字符窗口
	//L"Main" = TEXT("Main") =  __TEXT("Main") =（取决于是否定义了UNICODE）"Main"
	//CreateWindow宏实际上调用的是CreateWindowEx函数，第一个参数是扩展风格，通常设置为0
	/*
	  1.CreateWindowEx函数根据传入的窗口类名称，在应用程序局部窗口类中查找，如果找到执行2，如果未找到执行3
	  2.比较应用程序局部窗口类与创建窗口时传入的HINSTANCE变量。如果相等，创建和注册的窗口类在同一进程（模块），创建窗口返回；如果不相等，继续执行3
	  3.CreateWindowEx函数在应用程序全局窗口类中查找，如果找到执行4，如果未找到执行5
	  4.使用找到的窗口类信息创建窗口返回
	  5.在系统窗口类中查找，如果找到则创建窗口返回，否则创建窗口失败
	*/
	
	
	HWND hWnd = CreateWindow(TEXT("Main"),	//已经注册的窗口类名称（注意）
		__TEXT("Window"),		//窗口标题栏名称
		WS_OVERLAPPEDWINDOW,	//窗口风格
		//WS_OVERLAPPEDWINDOW包含了许多其他风格
		100,	//窗口左上角X坐标
		100,	//窗口左上角Y坐标
		500,	//窗口宽度
		500,	//窗口高度
		NULL,	//父窗口句柄
		//2.创建窗口时设置菜单
		//hMenu,	//菜单句柄
		NULL,
		hIns,	//当前模块的实例句柄（注意）
		NULL);	//创建窗口附加参数

	//5.显示窗口ShowWindow，将内存中的窗口显示到屏幕上
	ShowWindow(hWnd, SW_SHOW);	//显示窗口
	UpdateWindow(hWnd);	//更新窗口

	//6.消息循环Message Loop，从消息队列中获取消息并分发给窗口过程函数WndProc处理
	MSG nMsg = { 0 };
	
	//关闭窗口后程序仍存在于内存中，必须通过GetMessage宏（调用GetMessageW函数）返回0退出消息循环，结束程序
	//GetMessage宏从消息队列中获取所有消息，返回值大于0表示获取到有效消息，等于0表示获取到退出消息WM_QUIT，负值表示出错
	//LPMSG参数是一个指向MSG结构体的指针，用于存储获取到的消息；HWND hwnd参数是指定从哪个窗口的消息队列中获取消息，NULL表示从当前线程的消息队列中获取所有窗口的消息
	//UINT wMsgFilterMin参数和UINT wMsgFilterMax参数用于指定要获取的消息范围，0表示获取所有消息
	while (GetMessage(&nMsg, NULL, 0, 0))	//从消息队列中获取消息
	{
		//翻译消息，将按键消息（可见字符按键）转换为字符消息
		TranslateMessage(&nMsg);
		//分发消息，调用该消息所属窗口的窗口过程处理函数，例如第2步中的WndProc函数（窗口的窗口过程处理函数）
		DispatchMessage(&nMsg);
		//7.各种消息处理
	}

	/*
	  Windows平台下的消息组成：
	  1.窗口句柄
	  2.消息ID
	  3.消息参数wParam
	  4.消息参数lParam
	  5.产生的时间
	  6.产生时的鼠标位置

	  消息作用：当系统通知窗口工作时，就采用消息的方式派发给窗口
	*/

	return 0;
}