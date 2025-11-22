#include <Windows.h>
#include <stdio.h>
#define WM_MYMESSAGE (WM_USER+1001)

//调试程序的方法：附加一个控制台窗口输出
HANDLE g_hOutput = 0;	//接受标准输出句柄

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

void OnCreate(HWND hWnd, LPARAM lParam)
{
	CREATESTRUCT* pCS = (CREATESTRUCT*)lParam;
	const WCHAR* pszText = (WCHAR*)pCS->lpCreateParams;
	MessageBox(NULL, pszText, L"提示", MB_OK);
	PostMessage(hWnd, WM_MYMESSAGE, 1, 2);
	SendMessage(hWnd, WM_MYMESSAGE, 3, 4);
	CreateWindowEx(0, L"EDIT", L"hello", WS_CHILD | WS_VISIBLE | WS_BORDER, 0, 0, 200, 200, hWnd, NULL, 0, NULL);
}

void OnSize(HWND hWnd, LPARAM lParam)
{
	short nWidth = LOWORD(lParam);
	short nHeight = HIWORD(lParam);
	WCHAR szText[256] = { 0 };
	wsprintf(szText, L"width=%d,height=%d", nWidth, nHeight);
	WriteConsole(g_hOutput, szText, (DWORD)wcslen(szText) * sizeof(WCHAR), NULL, NULL);
}

void OnMyMessage(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	WCHAR szText[256] = { 0 };
	wsprintf(szText,L"自定义消息被处理：wParam=%d,lParam=%d", wParam, lParam);
	MessageBox(hWnd, szText, L"Info", MB_OK);
}

/*
  2.窗口过程函数WndProc，处理窗口的各种消息
  传参时只传消息的四个组成部分：窗口句柄、消息ID、消息参数wParam、消息参数lParam
  其实不用关心其他两个组成部分：产生的时间、产生时的鼠标位置

  函数名，参数名可以改，但是返回值类型和参数类型不能改
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT msgID, WPARAM wParam, LPARAM lParam)
{
	/*
		系统消息：ID范围0x0000-0x03FF（1024个），只需要我们发送或者只需要我们处理
		1.WM_DESTROY消息：窗口被销毁时发送，wParam=0，lParam=0，常用于在窗口销毁之前做相应的善后处理（例：资源、内存）
		2.WM_SYSCOMMAND消息：点击窗口右上角的按钮时发送，wParam=具体点击的位置（例：关闭SC_CLOSE、最大化SC_MAXIMIZE），
		lParam=鼠标光标位置，LOWORD(lParam)水平位置，HIWORD(lParam)垂直位置；常用于窗口关闭时提示用户处理
		3.WM_CREATE消息：窗口创建成功但还未显示，wParam=0，lParam=指向CREATESTRUCT结构体的指针，通过该指针可以获取CreateWindowEx函数的全部12个参数
		常用于初始化窗口的参数、资源等，包括创建子窗口
		4.WM_SIZE消息：窗口大小发生变化后，wParam=窗口大小变化的原因，lParam=窗口变化后的大小，LOWORD(lParam)宽度，HIWORD(lParam)高度
		常用于窗口大小变化后，调整窗口内各个部分的布局
		5.WM_QUIT消息：程序员控制发送时间，wParam=PostQuitMessage函数的参数，lParam=0，常用于结束消息循环，退出程序
		不需要程序员自定义函数处理

		自定义消息：ID范围0x0400-0x7FFF（31743个），需要我们发送和处理；自定义消息宏WM_USER
	*/
	switch (msgID)
	{
	case WM_CREATE:	//窗口创建消息
		OnCreate(hWnd, lParam); //自定义函数，处理窗口创建时的消息
		break;
	case WM_DESTROY:	//窗口销毁消息
		//PostQuitMessage(0);	//向消息队列发送退出消息（可以使GetMessage宏返回0），结束程序
		//PostQuitMessage内部调用PostMessage发送WM_QUIT消息
		//SendMessage(hWnd, WM_QUIT, 0, 0); //同步发送退出消息，结束程序
		PostMessage(hWnd, WM_QUIT, 0, 0); //异步发送退出消息，结束程序
		break;
	case WM_SYSCOMMAND:	//系统命令消息
		//点击窗口关闭按钮后，产生WM_SYSCOMMAND消息，之后执行默认窗口过程函数DefWindowProc，最终销毁窗口产生WM_DESTROY消息
		//case WM_DESTROY调用PostQuitMessage退出消息循环，结束程序
		//MessageBox(hWnd, L"系统命令消息", L"提示", MB_OK);	//弹出消息框提示用户，是阻塞函数
		if (wParam == SC_CLOSE)
		{
			int nRet = MessageBox(hWnd, L"是否退出", L"Info", MB_YESNO);
			if (nRet == IDYES)
			{

			}
			else
			{
				return 0;
			}
		}
		break;
	case WM_SIZE:	//窗口大小变化消息
		OnSize(hWnd, lParam);
	//处理自定义消息
	case WM_MYMESSAGE:
		OnMyMessage(hWnd, wParam, lParam);
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
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	//绘制窗口背景的画刷句柄
	wc.hCursor = NULL;		//窗口光标句柄，NULL表示使用系统默认光标
	wc.hIcon = NULL;		//窗口图标句柄，NULL表示使用系统默认图标
	wc.hInstance = hIns;	//当前模块的实例句柄
	wc.lpfnWndProc = WndProc;	//窗口过程处理函数
	//宽字节字符wchar_t每个字符占2个字节，窄字节字符char每个字符占1个字节
	//定义宽字节字符串时，需要在字符串前加L，例如L"Main"
	wc.lpszClassName = L"Main";	//窗口类名
	wc.lpszMenuName = NULL;	//窗口菜单的资源ID字符串
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
	WCHAR* pszText = (WCHAR*)L"hello data";
	HWND hWnd = CreateWindow(TEXT("Main"),	//已经注册的窗口类名称（注意）
		__TEXT("window"),		//窗口标题栏名称
		WS_OVERLAPPEDWINDOW,	//窗口风格
		//WS_OVERLAPPEDWINDOW包含了许多其他风格
		100,	//窗口左上角X坐标
		100,	//窗口左上角Y坐标
		500,	//窗口宽度
		500,	//窗口高度
		NULL,	//父窗口句柄
		NULL,	//菜单句柄
		hIns,	//当前模块的实例句柄（注意）
		pszText);	//创建窗口附加参数

	//8.创建子窗口
	//(1)创建时要设置父窗口句柄
	//(2)创建风格要包含WS_CHILD|WS_VISIBLE
	wc = { 0 };
	wc.cbClsExtra = 0;		//窗口类的附加数据缓冲区，赋值多少，就开辟多少字节的缓冲区
	wc.cbWndExtra = 0;		//窗口的附加数据缓冲区，赋值多少，就开辟多少字节的缓冲区
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	//绘制窗口背景的画刷句柄
	wc.hCursor = NULL;		//窗口光标句柄，NULL表示使用系统默认光标
	wc.hIcon = NULL;		//窗口图标句柄，NULL表示使用系统默认图标
	wc.hInstance = hIns;	//当前模块的实例句柄
	wc.lpfnWndProc = DefWindowProc;	//窗口过程处理函数，子窗口使用默认窗口过程函数
	//宽字节字符wchar_t每个字符占2个字节，窄字节字符char每个字符占1个字节
	//定义宽字节字符串时，需要在字符串前加L，例如L"Main"
	wc.lpszClassName = L"Child";	//窗口类名
	wc.lpszMenuName = NULL;	//窗口菜单的资源ID字符串
	wc.style = CS_HREDRAW | CS_VREDRAW;	//窗口风格，水平、垂直方向有变化时重绘窗口
	//常见的窗口风格：CS_DBLCLKS（双击消息）、CS_NOCLOSE（无关闭按钮）
	//注册窗口类的函数，注册成功后返回非0值，注册失败返回0
	RegisterClass(&wc);	//将以上赋值全部写入操作系统

	HWND hChildWnd1 = CreateWindowExW(0,	//扩展风格
		L"Child",	//已经注册的窗口类名称（注意）
		L"child window1",		//窗口标题栏名称
		WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW,	//窗口风格
		0,	//窗口左上角X坐标
		0,	//窗口左上角Y坐标
		200,	//窗口宽度
		200,	//窗口高度
		hWnd,	//父窗口句柄（注意）
		NULL,	//菜单句柄
		hIns,	//当前模块的实例句柄（注意）
		NULL);	//创建窗口附加参数
	HWND hChildWnd2 = CreateWindowExW(0,	//扩展风格
		L"Child",	//已经注册的窗口类名称（注意）
		L"child window2",		//窗口标题栏名称
		WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW,	//窗口风格
		200,	//窗口左上角X坐标
		0,	//窗口左上角Y坐标
		200,	//窗口宽度
		200,	//窗口高度
		hWnd,	//父窗口句柄（注意）
		NULL,	//菜单句柄
		hIns,	//当前模块的实例句柄（注意）
		NULL);	//创建窗口附加参数

	//5.显示窗口ShowWindow，将内存中的窗口显示到屏幕上
	ShowWindow(hWnd, SW_SHOW);	//显示窗口
	UpdateWindow(hWnd);	//更新窗口

	//6.消息循环Message Loop，从消息队列中获取消息并分发给窗口过程函数WndProc处理
	//SendMessage宏（调用SendMessageW函数）是同步发送消息，直接调用窗口过程函数WndProc处理消息，会等候消息处理的结果
	//PostMessage宏（调用PostMessageW函数）是异步发送消息，将消息放入消息队列，等待消息循环处理，消息发出后立即返回，不等侯消息处理的结果
	MSG nMsg = { 0 };
	//关闭窗口后程序仍存在于内存中，必须通过GetMessage宏（调用GetMessageW函数）返回0退出消息循环，结束程序
	//GetMessage宏从消息队列中获取所有消息，返回值大于0表示获取到有效消息，等于0表示获取到退出消息WM_QUIT，负值表示出错
	//LPMSG参数是一个指向MSG结构体的指针，用于存储获取到的消息；HWND hwnd参数是指定从哪个窗口的消息队列中获取消息，NULL表示从当前线程的消息队列中获取所有窗口的消息
	//UINT wMsgFilterMin参数和UINT wMsgFilterMax参数用于指定要获取的消息范围，0表示获取所有消息
	//while (GetMessage(&nMsg, NULL, 0, 0))	//从消息队列中获取消息
	//{
	//	//翻译消息，将按键消息（可见字符按键）转换为字符消息
	//	TranslateMessage(&nMsg);
	//	//分发消息，调用该消息所属窗口的窗口过程处理函数，例如第2步中的WndProc函数（窗口的窗口过程处理函数）
	//	DispatchMessage(&nMsg);
	//	//7.各种消息处理
	//}
	while (true)
	{
		//有消息时才处理消息
		if (PeekMessage(&nMsg, NULL, 0, 0, PM_NOREMOVE)) 
		{
			if (GetMessage(&nMsg,NULL,0,0))
			{
				TranslateMessage(&nMsg);
				DispatchMessage(&nMsg);
			}
			else
			{
				return 0; //收到WM_QUIT消息，退出程序
			}
		}
		//没有消息时，执行其他任务
		else
		{
			//往控制台窗口输出信息
			WriteConsole(g_hOutput, L"no message\n", lstrlenW(L"no message\n"), NULL, NULL);
		}
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