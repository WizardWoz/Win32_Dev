/*
 * C语言使用函数前不需要声明，只编译程序不会报错；C++语言使用函数前必须声明，否则编译器会报错。
*/
#include <iostream>

using namespace std;

//函数声明，告诉编译器这个函数在别的文件中定义了，编译器就不会报错了
int WinCppLib_add(int add1, int add2);
int WinCppLib_sub(int sub1, int sub2);
//extern "C"：告诉C++编译器应该按照C语言规则来编译；这也是C++语言使用函数前必须声明的原因，不然不知道是否使用C语言风格
extern "C" int WinCLib_add(int add1, int add2);
extern "C" int WinCLib_sub(int sub1, int sub2);

//通知链接器到哪里去寻找WinCppLib.lib库文件，链接器会自动链接这个库文件
#pragma comment(lib,"../x64/Debug/WinCppLib.lib")	//静态库相对路径，相对于本项目的目录，C++编译器会给函数换名
#pragma comment(lib,"../x64/Debug/WinCLib.lib")	//静态库相对路径，相对于本项目的目录，C编译器不会给函数换名

int main(void)
{
	int add1 = 10, add2 = 20;
	int sub1 = 30, sub2 = 15;
	int addResult = WinCppLib_add(add1, add2);
	int subResult = WinCppLib_sub(sub1, sub2);
	cout << "add result: " << addResult << endl;
	cout << "sub result: " << subResult << endl;
	//报错原因
	int CaddResult = WinCLib_add(add1, add2);	//C++编译器会给函数换名? WinCLib_add@@YAHHH@Z)
	int CsubResult = WinCLib_sub(sub1, sub2);	//C++编译器会给函数换名? WinCLib_sub@@YAHHH@Z)
	cout << "C add result: " << CaddResult << endl;
	cout << "C sub result: " << CsubResult << endl;
	return 0;
}