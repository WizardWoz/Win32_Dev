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
	  2.应用程序全局窗口类：应用程序定义的窗口类，当前应用程序的所有模块（主模块、库模块）都可以使用
	  3.应用程序局部窗口类：应用程序定义的窗口类，当前应用程序的本模块可以使用
	*/
	
	//当前项目使用系统窗口类

	//4.创建窗口CreateWindow，在内存中创建窗口
	//TEXT宏根据#ifdef  UNICODE，来决定创建宽字节字符窗口还是窄字节字符窗口
	//L"Main" = TEXT("Main") =  __TEXT("Main") =（取决于是否定义了UNICODE）"Main"
	//HWND hWnd = CreateWindow(TEXT("Edit"), __TEXT("window"), WS_OVERLAPPEDWINDOW, 100, 100, 500, 500, NULL, NULL, hIns, NULL);
	HWND hWnd = CreateWindow(TEXT("Button"), __TEXT("window"), WS_OVERLAPPEDWINDOW, 100, 100, 500, 500, NULL, NULL, hIns, NULL);

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