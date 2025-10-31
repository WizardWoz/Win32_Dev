#include <Windows.h>

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
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT msgID, WPARAM wParam, LPARAM lParam)
{
	switch (msgID)
	{
	case WM_DESTROY:	//窗口销毁消息
		PostQuitMessage(0);	//向消息队列发送退出消息（可以使GetMessage宏返回0），结束程序
		break;
	default:
		break;
	}
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
		NULL);	//创建窗口附加参数

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
	MSG nMsg = { 0 };
	//关闭窗口后程序仍存在于内存中，必须通过GetMessage宏（调用GetMessageW函数）返回0退出消息循环，结束程序
	while (GetMessage(&nMsg, NULL, 0, 0))	//从消息队列中获取消息
	{
		TranslateMessage(&nMsg);	//翻译消息
		DispatchMessage(&nMsg);		//分发消息，谁处理消息就发给谁，例如第2步中的WndProc函数
		//7.各种消息处理
	}

	return 0;
}