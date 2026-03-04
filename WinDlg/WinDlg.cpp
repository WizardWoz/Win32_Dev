#include <Windows.h>
#include <stdio.h>
#include "resource.h"

/*
 * 普通窗口：自定义函数调用缺省函数
 * WndProc(...){
 *		DefWindowProc(...);
 * }
 * 对话框窗口：缺省函数（对话框的窗口处理函数）调用自定义函数
 * 缺省函数(...){
 *		自定义函数(...);
 * }
 *
 * 缺省函数：系统提供的函数，开发者不需要编写代码
 * INT CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);返回TRUE：缺省函数不需要处理消息，返回FALSE：缺省函数需要处理消息
 * 不需要显式调用缺省函数，系统会自动调用
 *
 * 对话框分类：
 * 1.模态对话框：调用时会阻塞父窗口，直到对话框被关闭
 * 2.非模态对话框：调用时不会阻塞父窗口，可以同时与父窗口交互
 * 对话框的使用：
 * 1.对话框窗口处理函数
 * 2.注册窗口类（不常用）
 * 3.创建对话框DialogBox函数，谁创建对话框谁来实现窗口处理函数
 * 4.对话框的关闭EndDialog函数，发送WM_QUIT消息，wParam=0，lParam=0
*/

/*
 * 模式对话框
 * 1.创建对话框：DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DialogProc);是一个阻塞函数，直到对话框被关闭才会返回
 * 2.关闭对话框：EndDialog(hDlg, 100);发送WM_QUIT消息，wParam=0，lParam=0；DialogBox函数返回值为100
 * 
 * 无模式对话框
 * 1.创建对话框：CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DialogProc);是一个非阻塞函数，创建成功返回对话框窗口句柄，创建失败返回NULL
 *	 并且需要调用ShowWindow函数显示对话框窗口
 * 2.关闭对话框：DestroyWindow(hDlg);销毁对话框窗口；CreateDialog函数返回值为NULL
*/

HINSTANCE g_hInstance = 0;	//保存WinMain的第一个参数（当前程序实例句柄）
//调试程序的方法：附加一个控制台窗口输出
HANDLE g_hOutput = 0;	//接受标准输出句柄

/*
 * 对话框窗口处理函数
 * 1.函数名可以改，但返回值类型和参数类型不能改
*/
INT_PTR CALLBACK DialogProc(HWND hDlg, UINT msgID, WPARAM wParam, LPARAM lParam)
{
	switch (msgID)
	{
	case WM_INITDIALOG:
		MessageBox(hDlg, L"Dialog Box WM_INITDIALOG", L"Info", MB_OK);
		break;
	case WM_CREATE:
		MessageBox(hDlg, L"Dialog Box WM_CREATE", L"Info", MB_OK);
		break;
	case WM_SYSCOMMAND:
		if (wParam==SC_CLOSE)
		{
			//销毁模式对话框；第二个参数是DialogBox函数返回值，通常设置为0；同时解除主窗口的阻塞状态，继续执行DialogBox函数后续代码
			EndDialog(hDlg, 100);	//发送WM_QUIT消息，wParam=0，lParam=0
			//不能使用DestroyWindow函数销毁对话框窗口，因为主窗口仍处于阻塞状态，DestroyWindow函数无法解除主窗口的阻塞状态
			//DestroyWindow(hDlg);	
		}
	default:
		break;
	}
	return FALSE;	//将消息交给真正的对话框窗口处理函数的后续代码再处理
}

void OnCommand(HWND hWnd, WPARAM wParam)
{
	switch (LOWORD(wParam))
	{
	case ID_MODEL:
		int nRet = DialogBox(g_hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DialogProc);	//发送WM_QUIT消息，wParam=0，lParam=0
		if (nRet==100)
		{
			MessageBox(hWnd, L"Successful!", L"Info", MB_OK);
		}
		break;
	}
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
		//3.在处理窗口创建消息时设置菜单
	case WM_COMMAND:
		OnCommand(hWnd, wParam);
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
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	//绘制窗口背景的画刷句柄
	wc.hCursor = NULL;		//窗口光标句柄，NULL表示使用系统默认光标
	wc.hIcon = NULL;		//窗口图标句柄，NULL表示使用系统默认图标
	wc.hInstance = hIns;	//当前模块的实例句柄
	wc.lpfnWndProc = WndProc;	//窗口过程处理函数
	//宽字节字符wchar_t每个字符占2个字节，窄字节字符char每个字符占1个字节
	//定义宽字节字符串时，需要在字符串前加L，例如L"Main"
	wc.lpszClassName = L"Main";	//窗口类名
	//1.注册窗口类时设置菜单
	wc.lpszMenuName = (LPCWSTR)IDR_MENU1;	//窗口菜单的资源ID字符串
	//wc.lpszMenuName = NULL;
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
		L"Windows",		//窗口标题栏名称
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