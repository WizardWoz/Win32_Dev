/*
 * C++静态库的创建
 * 1.创建一个静态库项目
 * 2.添加.cpp库文件源代码，使用鼠标右键在项目名称上选择生成，来进行编译连接
 * C++静态库的使用
 * 1.库路径设置：使用pragma关键字， #pragma comment(lib,"../lib/CLib.lib")
*/
int WinCppLib_add(int add1, int add2)
{
	return add1 + add2;
}
int WinCppLib_sub(int sub1, int sub2)
{
	return sub1 - sub2;
}