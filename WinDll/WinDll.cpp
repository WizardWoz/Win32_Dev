/**
 * 动态库
 * 1.运行时独立存在（必须依附调用程序）
 * 2.源代码不会链接到执行程序
 * 3.使用时加载（调用程序使用动态库必须主动使动态库执行）
 *
 * 与静态库比较
 * 1.静态库将代码嵌入到调用程序，多个程序使用则存在多份相同代码；动态库多个使用程序通过函数地址调用同一份源代码
 * 2.静态库发生变化后，新代码需要重新链接到调用程序；动态库发生变化，若库中函数定义（地址）未变化，则无需重新链接
*/

/**
 * 创建动态库
 * 1.新建动态库项目
 * 2.添加库程序
 * 3.库程序导出（提供给调用程序的库中函数信息）
 * (1)声明导出：使用_declspec(dllexport)；动态库编译链接后也存在.lib文件，存放的是C++编译器换名后的函数名以及标号
 * (2)模块定义文件.def：例如LIBRARY DLLFunc（库）；EXPORTS（库导出表）；DLL_MUL @1（导出的函数）
*/

/**
 * 使用动态库
 * 1.隐式链接（操作系统负责使动态库执行）
 * (1)头文件和函数原型：在函数原型的生命前增加__declspec(dllimport)，告诉编译器这个函数在动态库中定义了，编译器就不会报错了
 * (2)导入动态库的.lib文件：使用#pragma comment(lib,"../x64/Debug/WinDLL.lib")，链接器会自动链接这个库文件
 * (3)运行时加载：调用程序使用动态库必须主动使动态库执行；例如：WinDLL_add(10, 20);
 * (4)隐式链接的情况，dll文件可存放的路径：与执行文件同一个目录（推荐）、当前工作目录、Windows目录、Windows\System32目录、Windows\System目录、环境变量PATH指定的目录
 * 
 * 2.显式链接（调用程序负责使动态库执行）
 * (1)定义函数指针类型：typedef int(*DLL_ADD)(int, int);
 * (2)加载动态库：使用LoadLibrary函数加载动态库，返回一个模块句柄
 * (3)获取函数地址：使用GetProcAddress函数获取函数地址
 * (4)调用函数：通过函数指针调用动态库中的函数
 * (5)释放动态库：使用FreeLibrary函数释放动态库
*/

_declspec(dllexport) int WinDLL_add(int add1, int add2)
{
	return add1 + add2;
}

_declspec(dllexport) int WinDLL_sub(int sub1, int sub2)
{
	return sub1 - sub2;
}

_declspec(dllexport) int WinDLL_mul(int mul1, int mul2)
{
	return mul1 * mul2;
}