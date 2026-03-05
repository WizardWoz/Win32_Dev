#ifndef _CLASSDLL_H
#define _CLASSDLL_H

/**
 * 动态库中封装类
 * 1.类名称前加上__declspec(dllexport)，其实也可使用模块定义文件.def导出类，但不推荐，
 * 因为模块定义文件.def只能导出类的成员函数（相对地址），无法导出成员变量（编译器视作变量类型）
 * 2.通常使用预编译开关切换类的导入导出定义
*/

#ifdef DLLCLASS_EXPORTS
#define EXT_CLASS _declspec(dllexport)
#else 
#define EXT_CLASS _declspec(dllimport)
#endif // DLLCLASS_EXPORTS

class EXT_CLASS CMath
{
public:
	int Add(int add1,int add2);
	int Sub(int sub1,int sub2);
};

#endif // !_CLASSDLL_H
