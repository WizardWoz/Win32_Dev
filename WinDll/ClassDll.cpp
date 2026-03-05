#define DLLCLASS_EXPORTS	//切换类的导出定义
#include "ClassDll.h"

int CMath::Add(int add1, int add2)
{
	return add1 + add2;
}

int CMath::Sub(int sub1, int sub2)
{
	return sub1 - sub2;
}