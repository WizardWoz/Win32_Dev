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
  2.窗口过程函数WndProc，处理窗口的各种消息
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT msgID, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, msgID, wParam, lParam);
}
/*
  1.定义WinMain函数，作为Windows应用程序的入口点
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
	wc.lpszClassName = TEXT("Main");	//窗口类名
	wc.lpszMenuName = NULL;	//窗口菜单名
	wc.style = CS_HREDRAW | CS_VREDRAW;	//窗口风格
	RegisterClass(&wc);	//将以上赋值全部写入操作系统

	//4.创建窗口CreateWindow，在内存中创建窗口
	HWND hWnd = CreateWindow(TEXT("Main"), TEXT("window"), WS_OVERLAPPEDWINDOW, 100, 100, 500, 500, NULL, NULL, hIns, NULL);

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