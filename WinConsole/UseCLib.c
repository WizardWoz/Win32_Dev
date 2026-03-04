/*
 * C语言使用函数前不需要声明，只编译程序不会报错；C++语言使用函数前必须声明，否则编译器会报错。
*/
#include <stdio.h>
//通知链接器到哪里去寻找WinCLib.lib库文件，链接器会自动链接这个库文件
#pragma comment(lib,"../x64/Debug/WinCLib.lib")	//静态库相对路径，相对于本项目的目录

//int main(void)
//{
//	int add1 = 10, add2 = 20;
//	int sub1 = 30, sub2 = 15;
//	int addResult = WinCLib_add(add1, add2);
//	int subResult = WinCLib_sub(sub1, sub2);
//	printf("add result: %d\n", addResult);
//	printf("sub result: %d\n", subResult);
//	return 0;
//}