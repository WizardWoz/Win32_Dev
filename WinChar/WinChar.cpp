#include <Windows.h>
#include <stdio.h>

void C_char()
{
	const char* pszText = "Hello Char";
	int len = strlen(pszText);
	printf("%s %d\n", pszText, len);
}

void W_char()
{
	const wchar_t* pszText = L"Hello WChar";
	int len = wcslen(pszText);
	wprintf(L"%s %d\n", pszText, len);
}

void T_char()
{
	const TCHAR* pszText = TEXT("Hello TChar");
#ifdef UNICODE
	int len = wcslen(pszText);
	wprintf(L"%s %d\n", pszText, len);
#else
	int len = strlen(pszText);
	printf("%s %d\n", pszText, len);
#endif // !UNICODE
}

void PrintUnicode()
{
	wchar_t unicodeStr[] = L"你好，世界"; // "Hello, World" in Chinese
	//可以验证wprintf()函数对Unicode中文字符输出的支持不完善
	wprintf(L"%ls\n", unicodeStr);		//中文输出变为?????
	//Windows下推荐使用WriteConsole宏函数输出Unicode字符
	//WriteConsole(标准输出句柄,unicodeStr,wcslen(unicodeStr),NULL);
	//GetStdHandle(STD_INPUT_HANDLE)获取标准输入句柄；GetStdHandle(STD_OUTPUT_HANDLE)获取标准输出句柄；GetStdHandle(STD_ERROR_HANDLE)获取标准错误句柄
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	WriteConsole(hConsole, unicodeStr, wcslen(unicodeStr), NULL, NULL);
}

int main()
{
	C_char();
	W_char();
	T_char();
	PrintUnicode();
	return 0;
}