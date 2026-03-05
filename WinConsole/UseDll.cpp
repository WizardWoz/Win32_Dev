#include <iostream>
#include <Windows.h>
#include "../WinDll/ClassDll.h"

using namespace std;

/**
 * 隐式链接动态库
*/

_declspec(dllimport) int WinDLL_add(int add1, int add2);
_declspec(dllimport) int WinDLL_sub(int sub1, int sub2);
_declspec(dllimport) int WinDLL_mul(int mul1, int mul2);

/**
 * 显式链接动态库
*/

typedef int(*DLL_ADD)(int, int);
typedef int(*DLL_SUB)(int, int);
typedef int(*DLL_MUL)(int, int);

//链接器不能直接到动态库文件（dll）中获取函数地址，而是需要到动态库编译链接后生成的.lib文件中获取函数地址
#pragma comment(lib,"../x64/Debug/WinDLL.lib")	//通知链接器到哪个文件获取函数编号与.dll文件名
//.exe可执行文件运行时到.dll文件中获取函数地址，调用函数；.dll文件必须存在且与.exe在同一目录，否则运行时会报错

int main(void)
{
	int sum = WinDLL_add(10, 20);	//存的是函数编号
	int sub = WinDLL_sub(30, 15);
	int mul = WinDLL_mul(5, 6);
	cout << "Sum: " << sum << endl;
	cout << "Subtraction: " << sub << endl;
	cout << "Multiplication: " << mul << endl;

	HINSTANCE hDll = LoadLibrary(L"WinDLL.dll");	//加载动态库，返回一个DLL句柄，只要不为NULL(0)，就说明加载成功
	cout << "DLL Handle: " << hDll << endl;			//得到动态库首地址
	//必须使用C++编译器换名后的函数名得到函数地址
	DLL_ADD pAdd = (DLL_ADD)GetProcAddress(hDll, "WinDLL_add");//加减法使用模块定义方式导出，函数名不变
	DLL_SUB pSub = (DLL_SUB)GetProcAddress(hDll, "WinDLL_sub");
	DLL_MUL pMul = (DLL_MUL)GetProcAddress(hDll, "?WinDLL_mul@@YAHHH@Z");//乘法使用声明导出方式导出，函数名被换名了
	cout << "ADD Function Address: " << pAdd << endl;	//得到函数地址
	cout << "SUB Function Address: " << pSub << endl;
	cout << "MUL Function Address: " << pMul << endl;
	int sum2 = pAdd(10, 20);	//通过函数指针调用函数
	int sub2 = pSub(30, 15);
	int mul2 = pMul(5, 6);
	cout << "Sum: " << sum2 << endl;
	cout << "Subtraction: " << sub2 << endl;
	cout << "Multiplication: " << mul2 << endl;
	FreeLibrary(hDll);	//释放动态库

	CMath math;
	int sum3 = math.Add(10, 20);
	int sub3 = math.Sub(30, 15);
	cout << "Sum: " << sum3 << endl;
	cout << "Subtraction: " << sub3 << endl;

	return 0;
}