// ConsoleApplication2.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <stdlib.h>  //system()
#include <Windows.h>
#include <direct.h>  
#include <tlhelp32.h> //查询进程

//#include <string>
//using namespace std;

//声明函数
void createprocess(char* commandline);
DWORD GetProcessidFromName(LPCTSTR name);
char * wchar2char(wchar_t * pwszUnicode);

int main()
{
	char buf1[200];  //用于step1
	char buf2[200];  //用于step2
	char buf3[200];  //用于step3
	_getcwd(buf1, sizeof(buf1));   //#include <direct.h> 
	_getcwd(buf2, sizeof(buf2));   //#include <direct.h>
	_getcwd(buf3, sizeof(buf3));   //#include <direct.h>
	printf(buf3);
	//获取当前路径


	printf("%s\n", buf1);
	char step1[] = "\\Procmon.exe /Minimized /NoFilter /BackingFile C:/Users/Public/Desktop/log";
	strcat_s(buf1, step1);
	printf("%s", buf1);
	//整合成语句用于创建进程
	createprocess(buf1);

	//查询程序是否真正启动
	//--------------------
	while (GetProcessidFromName(LPCTSTR("Procmon.exe"))+ GetProcessidFromName(LPCTSTR("Procmon64.exe"))==0) {
		printf("step1 检查进程是否成功创建请等待,3秒后将重试\n");	
		Sleep(3000);
	}



	int closekey;
	//char subPID[10];
	printf("正在记录，请不要关闭此窗口\n");
	printf("--------------------------\n");
	do {
		printf("若安装程序完成请按下！任意键！\n");
		system("pause");
		printf("确定已结束？\n 1 确定  2 否\n");
		scanf_s("%d", &closekey);
	} while (closekey != 1);
	//itoa(pi.dwProcessId,subPID,10);

	//system("c:\\windows\\system32\\taskkill.exe /F /T /PID " + pi.dwProcessId);
	


	char step2[] = "\\Procmon.exe /Terminate";
	strcat_s(buf2, step2);
	printf("%s\n", buf2);
	createprocess(buf2);
	//system("c:\\windows\\system32\\taskkill.exe /F /T /IM procmon.exe");
	//printf("procmon.exe\n");
	//system("c:\\windows\\system32\\taskkill.exe /F /T /IM procmon64.exe");
	//printf("procmon64.exe\n");
	//while (scanf_s("%c", &closekey)&&closekey==13);
	
	
	//查询程序是否真正结束
	//--------------------

	while (GetProcessidFromName(LPCTSTR("Procmon.exe")) + GetProcessidFromName(LPCTSTR("Procmon64.exe"))!=0) {
		printf("检查进程是否成功结束请等待，3秒后将重试\n");
		Sleep(3000);
	}
	printf("程序成功结束!\n");
	system("pause");
	//将保存的pml文件另存为csv格式
	char step3[] = "\\Procmon.exe /OpenLog  C:/Users/Public/Desktop/log.PML /SaveAs  C:/Users/Public/Desktop/log.csv";
	strcat_s(buf3, step3);
	createprocess(buf3);
	system("pause");




	return 0;
}


//创建进程
	
void createprocess(char* commandline){

	//改自
	//#include <Windows.h>
	//https://www.cnblogs.com/vranger/p/3792791.html

	wchar_t* szCommandLine;
	int iSize;
	iSize = MultiByteToWideChar(CP_ACP, 0, commandline, -1, NULL, 0); //iSize =wcslen(pwsUnicode)+1=6
	szCommandLine = (wchar_t *)malloc(iSize * sizeof(wchar_t)); //不需要 pwszUnicode = (wchar_t *)malloc((iSize+1)*sizeof(wchar_t))
	MultiByteToWideChar(CP_ACP, 0, commandline, -1, szCommandLine, iSize);



	//作者：Jeakon
	//来源：CSDN
	//原文：https ://blog.csdn.net/jeakon/article/details/8544964 
	//----------------------------------------------------------
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	BOOL bRet = CreateProcess(
		NULL,	// 不在此指定可执行文件的文件名
		szCommandLine,// 命令行参数
		NULL,	// 默认进程安全性
		NULL,	// 默认进程安全性
		FALSE,	// 指定当前进程内句柄不可以被子进程继承
		CREATE_NEW_CONSOLE,	// 为新进程创建一个新的控制台窗口
		NULL,	// 使用本进程的环境变量
		NULL,	// 使用本进程的驱动器和目录
		&si,
		&pi);
	if (bRet)
	{
		// 不使用的句柄最好关掉
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		printf("新进程的ID号：%d\n", (int)pi.dwProcessId);
		printf("新进程的主线程ID号：%d\n", (int)pi.dwThreadId);
	}
	else {
		printf("进程未成功创建执行\n");
	}
}

//判断进程是否存在
DWORD GetProcessidFromName(LPCTSTR name) {



	PROCESSENTRY32 pe;
	DWORD id = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pe.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hSnapshot, &pe))
		return 0;
	while (1)
	{
		pe.dwSize = sizeof(PROCESSENTRY32);
		if (Process32Next(hSnapshot, &pe) == FALSE)
			break;
		if (strcmp(wchar2char(pe.szExeFile),(const char *)name) == 0)
		{
			id = pe.th32ProcessID;
			break;
		}
	}
	CloseHandle(hSnapshot);
	printf("%d\n", (int)id);
	//printf((const char *)pe.szExeFile);
	//printf((const char *)name);
	return id; //如果存在返回ID，不存在则返回0
}

//wchar_t转char
char * wchar2char(wchar_t * pwszUnicode) {
	int iSize;
	char* pszMultiByte;

	//返回接受字符串所需缓冲区的大小，已经包含字符结尾符'\0'
	iSize = WideCharToMultiByte(CP_ACP, 0, pwszUnicode, -1, NULL, 0, NULL, NULL); //iSize =wcslen(pwsUnicode)+1=6

	pszMultiByte = (char*)malloc(iSize * sizeof(char)); //不需要 pszMultiByte = (char*)malloc(iSize*sizeof(char)+1);

	WideCharToMultiByte(CP_ACP, 0, pwszUnicode, -1, pszMultiByte, iSize, NULL, NULL);
	return pszMultiByte;
}
// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
