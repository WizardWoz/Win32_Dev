// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

/*
  动态库程序有入口点，但不能独立运行，不能独立进入内存，在程序运行阶段被其他可执行程序调用
  谁调用了这个动态库，谁就是进程的主体，动态库只是依附于该进程
*/

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

