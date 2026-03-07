/**
 * 事件：程序之间的通知
 * 1.创建事件：使用CreateEvent函数创建一个事件对象，返回一个句柄。
 * 2.等待事件：使用WaitForSingleObject/WaitForMultipleObjects函数等待事件对象的信号，参数为事件对象的句柄和等待时间。
 * 3.设置事件：使用SetEvent函数设置事件对象的状态为有信号，参数为事件对象的句柄。
 * 4.重置事件：使用ResetEvent函数重置事件对象的状态为无信号，参数为事件对象的句柄。
 * 5.关闭事件：使用CloseHandle函数关闭事件对象的句柄，释放系统资源。
 * 事件对象的状态：事件对象的状态可以是有信号（已被设置）或无信号（未被设置）。
 * 当一个线程成功等待事件对象时，事件对象变为有信号状态，其他线程等待该事件对象时会被阻塞。当持有事件对象的线程释放它时，事件对象变为无信号状态，等待的线程可以继续执行。
*/

#include <iostream>
#include <Windows.h>

HANDLE g_hEvent = 0;	//接收事件句柄
DWORD CALLBACK PrintProc(LPVOID pParam)
{
	while (true)
	{
		WaitForSingleObject(g_hEvent, INFINITE);//一旦执行被阻塞于此，直到事件对象被设置为有信号状态，线程才会继续执行
		ResetEvent(g_hEvent);	//重置事件对象的状态为无信号，参数为事件对象的句柄
		std::cout << "Event is signaled!" << std::endl;
	}
	return 0;
}

DWORD CALLBACK CtrlProc(LPVOID pParam)
{
	while (true)
	{
		Sleep(1000);	//线程休眠1秒，模拟线程执行任务的时间
		SetEvent(g_hEvent);		//解救子线程1
	}
	return 0;
}

int main(void)
{
	g_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	DWORD dwWaitResult = 0;	//等待结果
	HANDLE hThread[2] = { 0 };	//线程句柄数组
	hThread[0] = CreateThread(NULL, 0, PrintProc, NULL, 0, &dwWaitResult);	//创建线程，参数为线程属性、堆栈大小、线程处理函数、线程参数、创建标志和线程ID
	hThread[1] = CreateThread(NULL, 0, CtrlProc, NULL, 0, &dwWaitResult);	//创建线程，参数为线程属性、堆栈大小、线程处理函数、线程参数、创建标志和线程ID
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);

	return 0;
}