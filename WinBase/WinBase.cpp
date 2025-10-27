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
	WNDCLASS wc = { 0 };
	wc.cbClsExtra = 0;		//赋值多少，就开辟多少字节的缓冲区
	wc.cbWndExtra = 0;		//赋值多少，就开辟多少字节的缓冲区
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	//窗口背景色
	wc.hCursor = NULL;		//使用系统默认光标
	wc.hIcon = NULL;		//使用系统默认图标
	wc.hInstance = hIns;	//当前应用程序实例句柄
	wc.lpfnWndProc = WndProc;	//窗口过程函数
	//宽字节字符wchar_t每个字符占2个字节，窄字节字符char每个字符占1个字节
	//定义宽字节字符串时，需要在字符串前加L，例如L"Main"
	wc.lpszClassName = L"Main";	//窗口类名
	wc.lpszMenuName = NULL;	//窗口菜单名
	wc.style = CS_HREDRAW | CS_VREDRAW;	//窗口风格
	RegisterClass(&wc);	//将以上赋值全部写入操作系统

	//4.创建窗口CreateWindow，在内存中创建窗口
	//TEXT宏根据#ifdef  UNICODE，来决定创建宽字节字符窗口还是窄字节字符窗口
	//L"Main" = TEXT("Main") =  __TEXT("Main") =（取决于是否定义了UNICODE）"Main"
	HWND hWnd = CreateWindow(TEXT("Main"), __TEXT("window"), WS_OVERLAPPEDWINDOW, 100, 100, 500, 500, NULL, NULL, hIns, NULL);

	//5.显示窗口ShowWindow，将内存中的窗口显示到屏幕上
	ShowWindow(hWnd, SW_SHOW);	//显示窗口
	UpdateWindow(hWnd);	//更新窗口

	//6.消息循环Message Loop，从消息队列中获取消息并分发给窗口过程函数WndProc处理
	MSG nMsg = { 0 };
	while (GetMessage(&nMsg, NULL, 0, 0))	//从消息队列中获取消息
	{
		TranslateMessage(&nMsg);	//翻译消息
		DispatchMessage(&nMsg);		//分发消息，谁处理消息就发给谁，例如第2步中的WndProc函数
		//7.各种消息处理
	}
	return 0;
}