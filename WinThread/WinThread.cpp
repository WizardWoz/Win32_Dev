#include <iostream>
#include <Windows.h>

/**
 * Windows线程：是可以执行代码的实例。系统是以线程为单位调度程序。一个程序中可以有多个线程实现多任务处理
 *
 * 线程的特点
 * 1. 轻量级：线程是比进程更小的执行单位，创建和销毁的开销较小。
 * 2. 共享内存：同一进程中的线程可以共享进程的内存空间。
 * 3. 独立性：线程之间相互独立，每个线程都有一个线程ID和栈空间，一个线程的异常不会影响其他线程。
 * 线程的状态
 * 1. 就绪状态：线程已经准备好，可以被调度执行。
 * 2. 运行状态：线程正在执行。
 * 3. 阻塞状态：线程因为某种原因被挂起，无法继续执行。
 * 线程的创建与管理
 * 1. 创建线程：使用CreateThread函数。
 * (1)定义线程处理函数：线程处理函数必须符合特定的函数签名，返回值为DWORD，参数为LPVOID类型。函数名字可以改
 * 2. 结束线程：使用ExitThread函数。
 * 3. 挂起线程SuspendThread函数，唤醒线程ResumeThread函数。
 * 4. 结束指定线程TerminateThread函数，结束当前线程ExitThread函数。
 * 3. 线程同步：使用信号量、互斥锁等机制。
 * 4. 线程调度：操作系统将CPU的执行时间划分为时间片，依次根据时间片分配给就绪状态的线程执行。
 * 
 * 线程操作
 * 1.获取当前线程ID：GetCurrentThreadId函数。
 * 2.获取当前线程句柄：GetCurrentThread函数。
 * 3.等候单个线程句柄信号：WaitForSingleObject函数。（具备有、无信号的句柄才能被称为可等候句柄）
 * 4.等候多个线程句柄信号：WaitForMultipleObjects函数。
 * 线程执行中无信号，线程执行完毕后有信号，线程被强制结束后有信号。
*/

//CALLBACK回调函数：

DWORD CALLBACK TestProc1(LPVOID lpParam)
{
	LPCWSTR threadName = (LPCWSTR)lpParam;	//将线程参数转换为宽字符类型字符串
	while (true)
	{
		std::wcout << threadName << " is running..." << std::endl;	//输出线程名称
		Sleep(1000);	//线程休眠1秒，模拟线程执行任务的时间
	}
	return 0;
}

DWORD CALLBACK TestProc2(LPVOID lpParam)
{
	LPCWSTR threadName = (LPCWSTR)lpParam;	//将线程参数转换为宽字符类型字符串
	while (true)
	{
		std::wcout << threadName << " is running..." << std::endl;	//输出线程名称
		Sleep(1000);	//线程休眠1秒，模拟线程执行任务的时间
	}
	return 0;
}

int main(void)
{
	DWORD threadId = 0;			//线程ID，系统为每个线程分配一个唯一的ID
	LPCWSTR tparam1 = L"MyThread1"; // 线程名称，必须是宽字符类型字符串
	LPCWSTR tparam2 = L"MyThread2"; // 线程名称，必须是宽字符类型字符串

	HANDLE hThread1 = CreateThread(NULL,	//安全属性，NULL表示默认安全属性
		0,		//堆栈大小，0表示使用默认堆栈大小
		TestProc1,//线程处理函数，使用lambda表达式定义，必须返回一个DWORD值
		(LPVOID)tparam1,	//线程参数，传递给线程处理函数的参数
		0,		//创建标志，0表示立即运行子线程1
		&threadId	//线程ID，传递一个指针来接收线程ID，这里不需要，所以传递NULL
	);

	//等待线程1结束，INFINITE表示无限等待，直到线程1结束才继续执行主线程；所以子线程2无法被创建
	WaitForSingleObject(hThread1, INFINITE);	

	HANDLE hThread2 = CreateThread(NULL,	//安全属性，NULL表示默认安全属性
		0,		//堆栈大小，0表示使用默认堆栈大小
		TestProc2,//线程处理函数，使用lambda表达式定义，必须返回一个DWORD值
		(LPVOID)tparam2,	//线程参数，传递给线程处理函数的参数
		CREATE_SUSPENDED,		//创建标志，CREATE_SUSPENDED表示创建好后挂起子线程2
		&threadId	//线程ID，传递一个指针来接收线程ID，这里不需要，所以传递NULL
	);
	getchar();	//切换到子线程2
	SuspendThread(hThread1);	//挂起线程1，暂停线程的执行
	ResumeThread(hThread2);	//恢复线程2，继续线程的执行
	getchar();	//等待用户输入，防止主线程过早结束导致子线程无法执行
	return 0;
}