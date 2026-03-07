/**
 * 信号量：类似事件，解决通知的相关问题，区别在于：事件只能通知一个线程，而信号量可以通知多个线程
 * 1.创建信号量：CreateSemaphore函数
 * 2.等候信号量：WaitForSingleObject/WaitForMultipleObjects函数
 * 3.给信号量指定计数值：ReleaseSemaphore
 * 4.关闭信号量：CloseHandle
*/
#include <iostream>
#include <Windows.h>

HANDLE g_hSema = 0;		//信号量句柄

DWORD CALLBACK TestProc(LPVOID pParam)
{
	while (true)
	{
		WaitForSingleObject(g_hSema, INFINITE);
		std::cout << "********" << std::endl;
	}
}

int main(void)
{
	g_hSema = CreateSemaphore(NULL, 3, 10, NULL);
	DWORD hSemaID = 0;
	HANDLE hThread = CreateThread(NULL, 0, TestProc, NULL, 0, &hSemaID);
	getchar();
	ReleaseSemaphore(g_hSema, 5, NULL);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(g_hSema);
	return 0;
}