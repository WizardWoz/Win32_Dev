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

/*
* 窗口创建成功之后，显示之前被调用
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

void OnPaint(HWND hWnd)
{
	const WCHAR* pszText = L"WM_PAINT\n";
	WriteConsole(g_hOutput, pszText, lstrlenW(pszText), NULL, NULL);
	PAINTSTRUCT ps = { 0 };
	HDC hdc = BeginPaint(hWnd, &ps);	//开始绘图，返回绘图设备句柄HDC
	TextOut(hdc, 200, 200, L"hello", 5);	//在指定位置绘制文本
	EndPaint(hWnd, &ps);	//结束绘图
	//以上绘制图的代码，必须在处理WM_PAINT消息时调用
}

void OnKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	WCHAR szText[256] = { 0 };
	wsprintf(szText, L"WM_KEYDOWN：键码值=%d\n", wParam);
	WriteConsole(g_hOutput, szText, lstrlen(szText), NULL, NULL);
}

void OnKeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	WCHAR szText[256] = { 0 };
	wsprintf(szText, L"WM_KEYUP：键码值=%d\n", wParam);
	WriteConsole(g_hOutput, szText, lstrlen(szText), NULL, NULL);
}

void OnChar(HWND hWnd, WPARAM wParam)
{
	WCHAR szText[256] = { 0 };
	wsprintf(szText, L"WM_CHAR：字符=%c\n", wParam);
	WriteConsole(g_hOutput, szText, lstrlen(szText), NULL, NULL);
}

void OnLButtonDown(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	WCHAR szText[256] = { 0 };
	wsprintf(szText, L"WM_LBUTTONDOWN：其他按键状态=%d；鼠标位置X=%d,Y=%d\n", wParam, LOWORD(lParam), HIWORD(lParam));
	WriteConsole(g_hOutput, szText, lstrlen(szText), NULL, NULL);
}

void OnLButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	WCHAR szText[256] = { 0 };
	wsprintf(szText, L"WM_LBUTTONUP：其他按键状态=%d；鼠标位置X=%d,Y=%d\n", wParam, LOWORD(lParam), HIWORD(lParam));
	WriteConsole(g_hOutput, szText, lstrlen(szText), NULL, NULL);
}

void OnMouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	WCHAR szText[256] = { 0 };
	wsprintf(szText, L"WM_MouseMove：其他按键状态=%d；鼠标位置X=%d,Y=%d\n", wParam, LOWORD(lParam), HIWORD(lParam));
	WriteConsole(g_hOutput, szText, lstrlen(szText), NULL, NULL);
}

void OnLButtonDblClk(HWND hWnd)
{
	const WCHAR* szText = L"WM_LBUTTONDBLCLK.\n";
	WriteConsole(g_hOutput, szText, lstrlen(szText), NULL, NULL);
}

void OnMouseWheel(HWND hWnd, WPARAM wParam)
{
	short nDelta = HIWORD(wParam);	//偏移量一般是120的+、-倍数
	WCHAR szText[256] = { 0 };
	wsprintf(szText, L"WM_MOUSEWHEEL:nDelta=%d\n", nDelta);
	WriteConsole(g_hOutput, szText, lstrlen(szText), NULL, NULL);
}

void OnTimer(HWND hWnd, WPARAM wParam)
{
	WCHAR szText[256] = { 0 };
	wsprintf(szText, L"WM_TIMER：定时器ID=%d\n", wParam);
	WriteConsole(g_hOutput, szText, lstrlen(szText), NULL, NULL);
	//根据定时器ID进入不同分支执行
	switch (wParam)
	{
	case 1:
		break;
	case 2:
		break;
	default:
		break;
	}
}

/*
  2.窗口过程函数WndProc，处理窗口的各种消息（PostMessage、SendMessage产生的）
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
		6.WM_PAINT消息：当窗口需要绘制的时候发送，wParam=0，lParam=0

		自定义消息：ID范围0x0400-0x7FFF（31743个），需要我们发送和处理；自定义消息宏WM_USER
	*/
	/*
	* 键盘消息：wParam=虚拟按键码 Virtual Key（无法区分大小写），lParam=按键信息（例如按下次数、是否按下Ctrl等）
	* 1.WM_KEYDOWN：按下按键时发送；2.WM_KEYUP：释放按键时发送；3.WM_SYSKEYDOWN：按下系统按键时发送（Alt、F10）；4.WM_SYSKEYUP：释放系统按键时发送（Alt、F10）
	*/
	/*
	* 字符消息 WM_CHAR：TranslateMessage在转换WM_KEYDOWN消息时，对于可见字符产生，不可见字符无此消息
	* 附带信息：wParam=输入字符的ASCII码或Unicode码，lParam=按键信息（例如按下次数、是否按下Ctrl等）
	*/
	/*
	* 鼠标消息：
	* 1.WM_LBUTTONDOWN：按下左键时发送；2.WM_LBUTTONUP：释放左键时发送；3.WM_RBUTTONDOWN：按下右键时发送；4.WM_RBUTTONUP：释放右键时发送；
	* 5.WM_MOUSEMOVE：鼠标光标移动时发送（根据移动速度产生一系列的消息）
	* wParam：其他按键的状态（例Ctrl/Shift等），lParam：鼠标光标位置（窗口坐标系），LOWORD(lParam)水平位置，HIWORD(lParam)垂直位置
	* 
	* 使用鼠标双击消息前，需要在注册窗口类中添加CS_DBLCLKS风格
	* 6.WM_LBUTTONDBLCLK：双击左键时发送；7.WM_RBUTTONDBLCLK：双击右键时发送；
	* 鼠标左键双击后消息产生顺序：WM_LBUTTONDOWN -> WM_LBUTTONUP -> WM_LBUTTONDBLCLK -> WM_LBUTTONUP
	* 
	* 8.WM_MOUSEWHEEL：滚动鼠标滚轮时发送
	* LOWORD(wParam)：其他按键的状态；HIWORD(wParam)：滚轮偏移量，通过正负值表示滚动方向，正为向前滚动，负为向后滚动
	* LOWORD(lParam)：鼠标当前位置（屏幕坐标系）的X坐标；HIWORD(lParam)：鼠标当前位置（屏幕坐标系）的Y坐标
	*/
	/*
	* 定时器消息：程序中创建的定时器到达时间间隔时，会向程序发送一个WM_TIMER消息。精度是毫秒，但准确度很低；例如设置时间间隔为1000ms，但会在非1000ms收到消息
	* wParam：定时器ID；lParam：定时器处理函数的指针
	*/
	switch (msgID)
	{
	case WM_TIMER:
		OnTimer(hWnd, wParam);
		break;
	case WM_MOUSEWHEEL:
		OnMouseWheel(hWnd, wParam);
		break;
	case WM_LBUTTONDBLCLK:
		OnLButtonDblClk(hWnd);
		break;
	case WM_MOUSEMOVE:
		OnMouseMove(hWnd, wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
		//InvalidateRect函数参数：1.HWND hWnd：窗口句柄；2.CONST RECT* lpRect：指向RECT结构体的指针，指定需要重绘的区域，NULL表示整个窗口；3.BOOL bErase：是否擦除背景
		InvalidateRect(hWnd, NULL, TRUE); //使整个窗口无效，调用一次产生一个WM_PAINT消息
		OnLButtonDown(hWnd, wParam, lParam);
		break;
	case WM_LBUTTONUP:
		OnLButtonUp(hWnd, wParam, lParam);
		break;
	case WM_CHAR:
		OnChar(hWnd, wParam);
		break;
	case WM_KEYDOWN:
		OnKeyDown(hWnd, wParam, lParam);
		break;
	case WM_KEYUP:
		OnKeyUp(hWnd, wParam, lParam);
		break;
		//窗口无效区域 InvalidateRect：需要重新绘制的区域
		//WM_PAINT处理步骤：1.开始绘图 BeginPaint（返回绘图设备句柄HDC） 2.绘图 3.结束绘图 EndPaint
	case WM_PAINT:
		OnPaint(hWnd);
		break;
	case WM_CREATE:	//窗口创建成功，显示之前消息
		OnCreate(hWnd, lParam); //自定义函数，处理窗口创建时的消息
		SetTimer(hWnd, 1, 1000, NULL);		//定时器1
		SetTimer(hWnd, 2, 2000, NULL);		//定时器2
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
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;	//窗口风格，水平、垂直方向有变化时重绘窗口
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
	ShowWindow(hWnd, SW_SHOW);	//显示窗口，通过SendMessage向自定义消息处理函数WndProc发出WM_PAINT消息
	UpdateWindow(hWnd);	//更新窗口

	//6.消息循环Message Loop，从消息队列中获取消息并分发给窗口过程函数WndProc处理
	//SendMessage宏（调用SendMessageW函数）是同步发送消息到WndProc自定义消息处理函数，直接调用窗口过程函数WndProc处理消息，会等候消息处理的结果
	//PostMessage宏（调用PostMessageW函数）是异步发送消息，将消息放入系统消息队列，等待消息循环处理，消息发出后立即返回，不等侯消息处理的结果
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

	/*TranslateMessage(&nMsg)
	{
		if (nMsg.message!=WM_KEYDOWN)
		{
			return;
		}
		根据nMsg.wParam（键码值）可知哪个按键被按下
		if (不可见字符的按键)
		{
			return;
		}
		查看大写锁定键（CapsLock）是否处于打开状态
		if (打开)
		{
		//会往系统消息队列发送WM_CHAR消息
			PostMessage(nMsg.hWnd, WM_CHAR, 大写A字符ASCII码 = 65, 0);
		}
		else
		{
			PostMessage(nMsg.hWnd, WM_CHAR, 小写a字符ASCII码 = 97, 0);
		}
	}*/
	while (true)
	{
		//有消息时才处理消息
		if (PeekMessage(&nMsg, NULL, 0, 0, PM_NOREMOVE)) 
		{
			//GetMessage宏获取消息的顺序：
			//1.程序（线程）消息队列；2.向系统消息队列获取，若有则转发至当前程序（线程）消息队列；
			//3.当前进程所有窗口需要重新绘制的区域WM_PAINT（会放到系统队列）；4.到时的定时器WM_TIMER（会放到系统队列）
			//如果以上四个都不满足，则整理程序资源、内存等，然后进入休眠，等待新的消息到来；而PeekMessage会返回FALSE，交出程序控制权
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
			//WriteConsole(g_hOutput, L"no message\n", lstrlenW(L"no message\n"), NULL, NULL);
		}
	}

	/*
	  Windows平台下的消息组成：
	  1.窗口句柄			HWND hwnd;
	  2.消息ID			UINT message;
	  3.消息参数wParam	WPARAM wParam;
	  4.消息参数lParam	LPARAM lParam;
	  5.产生的时间		DWORD time;
	  6.产生时的鼠标位置	POINT pt;

	  消息作用：当系统通知窗口工作时，就采用消息的方式派发给窗口
	*/
	/*
	* 消息队列
	* 1.用于存放消息的队列；2.消息在队列中先进先出；3.所有窗口程序都具有消息队列；4.程序可以从队列中获取消息
	* 
	* 分类
	* 1.系统消息队列：由系统维护，存放系统产生的消息，例如：鼠标、键盘；所有进程产生的消息首先进系统消息队列，每隔一段时间系统消息队列会对应转发到相应的程序消息队列
	* 2.程序消息队列：属于每一个应用程序（线程）的消息队列，由应用程序（线程）进行维护；
	* 
	* 消息传递路径：PostMessage/SendMessage -> 系统消息队列 -> 程序消息队列 -> PeekMessage/GetMessage -> TranslateMessage -> DispatchMessage -> WndProc
	*/

	/*
	* 创建定时器 UINT_PTR SetTimer 
	* 参数：1.窗口句柄HWND hWnd；2.UINT_PTR nIDEvent定时器ID；3.UINT uElapse时间间隔；4.TIMERPROC lpTimerFunc定时器处理函数的指针（一般不使用，为NULL）
	* 关闭定时器 BOOL KillTimer
	* 参数：1.窗口句柄HWND hWnd；2.UINT_PTR nIDEvent定时器ID
	*/

	return 0;
}