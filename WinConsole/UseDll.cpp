#include <iostream>
using namespace std;

_declspec(dllimport) int WinDLL_add(int add1, int add2);
_declspec(dllimport) int WinDLL_sub(int sub1, int sub2);
_declspec(dllimport) int WinDLL_mul(int mul1, int mul2);

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
	return 0;
}