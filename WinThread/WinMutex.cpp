/**
 * 互斥：是一种同步机制，用于保护共享资源的访问，确保在同一时刻只有一个线程可以访问该资源。
 * 互斥锁：是一种重量级的锁机制，使用操作系统提供的互斥对象来实现线程之间的互斥访问。
 * 互斥锁可以保证在多线程环境下对共享资源的访问是安全的，避免了死锁和竞争条件等问题。
 * 1.创建互斥锁：使用CreateMutex函数创建一个互斥对象，返回一个句柄。
 * 2.等待互斥锁：使用WaitForSingleObject函数等待互斥对象的信号，参数为互斥对象的句柄和等待时间。（谁先等候谁先获取）
 * 3.释放互斥锁：使用ReleaseMutex函数释放互斥对象，参数为互斥对象的句柄。（2，3之间的代码被成功加锁）
 * 4.关闭互斥锁：使用CloseHandle函数关闭互斥对象的句柄，释放系统资源。
 * 互斥句柄的状态：互斥对象的状态可以是有信号（未被占用）或无信号（被占用）。
 * 当一个线程成功获取互斥锁时，互斥对象变为无信号状态，其他线程等待该互斥锁时会被阻塞。当持有互斥锁的线程释放它时，互斥对象变为有信号状态，等待的线程可以继续执行。
*/

#include <iostream>
#include <Windows.h>

HANDLE g_hMutex=0;	//互斥对象句柄

DWORD CALLBACK TestProc5(LPVOID lpParam)
{
	LPCWSTR threadName = (LPCWSTR)lpParam;	//将线程参数转换为宽字符类型字符串
	while (true)
	{
		//std::wcout << threadName << " is running..." << std::endl;	//输出线程名称
		//Sleep(1000);	//线程休眠1秒，模拟线程执行任务的时间

		WaitForSingleObject(g_hMutex, INFINITE);
		for (int i = 0; i < lstrlenW(threadName); i++)
		{
			std::wcout << threadName[i];
			Sleep(125);	//线程休眠125毫秒，模拟线程执行任务的时间
		}
		std::wcout << std::endl;
		ReleaseMutex(g_hMutex);
	}
	return 0;
}

DWORD CALLBACK TestProc6(LPVOID lpParam)
{
	LPCWSTR threadName = (LPCWSTR)lpParam;	//将线程参数转换为宽字符类型字符串
	while (true)
	{
		//std::wcout << threadName << " is running..." << std::endl;	//输出线程名称
		//Sleep(1000);	//线程休眠1秒，模拟线程执行任务的时间

		WaitForSingleObject(g_hMutex, INFINITE);
		for (int i = 0; i < lstrlenW(threadName); i++)
		{
			std::wcout << threadName[i];
			Sleep(125);	//线程休眠125毫秒，模拟线程执行任务的时间
		}
		std::wcout << std::endl;
		ReleaseMutex(g_hMutex);
	}
	return 0;
}

/*int main(void)
{
	g_hMutex = CreateMutex(NULL, FALSE, NULL);	//创建一个互斥对象，初始状态为未拥有（有信号），名称为NULL表示匿名互斥对象
	DWORD threadId = 0;			//线程ID，系统为每个线程分配一个唯一的ID
	LPCWSTR tparam1 = L"********"; // 线程名称，必须是宽字符类型字符串
	LPCWSTR tparam2 = L"--------"; // 线程名称，必须是宽字符类型字符串

	HANDLE hThread1 = CreateThread(NULL,	//安全属性，NULL表示默认安全属性
		0,		//堆栈大小，0表示使用默认堆栈大小
		TestProc5,//线程处理函数，使用lambda表达式定义，必须返回一个DWORD值
		(LPVOID)tparam1,	//线程参数，传递给线程处理函数的参数
		0,		//创建标志，0表示立即运行子线程1
		&threadId	//线程ID，传递一个指针来接收线程ID，这里不需要，所以传递NULL
	);

	//等待线程1结束，INFINITE表示无限等待，直到线程1结束才继续执行主线程；所以子线程2无法被创建
	//WaitForSingleObject(hThread1, INFINITE);

	HANDLE hThread2 = CreateThread(NULL,	//安全属性，NULL表示默认安全属性
		0,		//堆栈大小，0表示使用默认堆栈大小
		TestProc6,//线程处理函数，使用lambda表达式定义，必须返回一个DWORD值
		(LPVOID)tparam2,	//线程参数，传递给线程处理函数的参数
		0,		//创建标志，CREATE_SUSPENDED表示创建好后挂起子线程2
		&threadId	//线程ID，传递一个指针来接收线程ID，这里不需要，所以传递NULL
	);
	getchar();	//切换到子线程2
	//SuspendThread(hThread1);	//挂起线程1，暂停线程的执行
	//ResumeThread(hThread2);	//恢复线程2，继续线程的执行
	//getchar();	//等待用户输入，防止主线程过早结束导致子线程无法执行
	CloseHandle(g_hMutex);	//关闭互斥对象句柄，释放系统资源
	return 0;
}*/