/**
 * 多个线程对同一数据进行原子操作，会产生结果丢失（比如执行++运算），因此需要使用线程同步机制来保证线程安全。
 * 1.原子锁：是一种轻量级的锁机制，使用原子操作来实现线程之间的互斥访问。
 * (1)原子锁可以保证在多线程环境下对共享资源的访问是安全的，避免了死锁和竞争条件等问题。对共享数据所在的进程内存空间加锁
 * (2)每种C++运算符都有对应的原子锁函数，如InterlockedIncrement、InterlockedDecrement、InterlockedExchange等。
*/

#include <iostream>
#include <Windows.h>

using namespace std;

long g_counter = 0; // 全局计数器，多个线程对其进行原子操作

DWORD CALLBACK TestProc3(LPVOID lpParam)
{
	for (int i = 0; i < 1000000; i++)
	{
		//g_counter++; // 对全局计数器进行递增操作，可能会产生结果丢失
		//使用原子锁函数
		InterlockedIncrement(&g_counter); // 使用InterlockedIncrement函数对全局计数器进行原子递增操作，保证线程安全
	}
	return 0;
}

DWORD CALLBACK TestProc4(LPVOID lpParam)
{
	for (int i = 0; i < 1000000; i++)
	{
		//g_counter++; // 对全局计数器进行递增操作，可能会产生结果丢失
		InterlockedIncrement(&g_counter); // 使用InterlockedIncrement函数对全局计数器进行原子递增操作，保证线程安全
	}
	return 0;
}

/*int main(void)
{
	DWORD threadId = 0;			//线程ID，系统为每个线程分配一个唯一的ID
	HANDLE hThread[2];		//线程句柄数组，用于存储创建的线程句柄
	hThread[0] = CreateThread(NULL, 0, TestProc3, NULL, 0, &threadId);	//创建子线程1，传入线程处理函数和线程参数
	hThread[1] = CreateThread(NULL, 0, TestProc4, NULL, 0, &threadId);	//创建子线程2，传入线程处理函数和线程参数
	//等待所有线程执行完毕，参数1为线程数量，参数2为线程句柄数组，参数3为是否等待所有线程，参数4为等待时间
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
	cout << "g_counter = " << g_counter << endl;
	return 0;
}*/