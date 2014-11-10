// ReactDebugview.cpp : Defines the entry point for the console application.
//

#include "StdAfx.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
using namespace std;
#define PAGE_SIZE   4096

#define DBWIN_MAP   "DBWIN_BUFFER"
#define DBWIN_WRITEVENT  "DBWIN_BUFFER_READY"
#define DBWIN_READEVENT  "DBWIN_DATA_READY"

LARGE_INTEGER  litmp ;
LONGLONG  QPart1,QPart2 ;
double  dfMinus, dfFreq, dfTim ;

int row;

typedef struct _tag_DBMap
{
 DWORD dwProcessId;
 CHAR szString[PAGE_SIZE - sizeof(DWORD)];
} DB_MAP, *PDB_MAP;

int main(int argc, char* argv[])
{
/*******************************\
控制台界面
	\*******************************/
	HANDLE hIn;			///标准输入
    HANDLE hOut;		///标准输出
    HANDLE hError;		///标准错误
	hIn = GetStdHandle(STD_INPUT_HANDLE);
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);		
    hError = GetStdHandle(STD_ERROR_HANDLE);
	
	/*	控制台标题	*/
	SetConsoleTitle("control debugview");	
	
	COORD set_goneng_Position;
	COORD max_postion;
	COORD set_client_postion;
	DWORD Written;
	max_postion = GetLargestConsoleWindowSize(hOut);
	
	
	set_goneng_Position.X = 0;
	set_goneng_Position.Y = 0;
	SetConsoleCursorPosition(hOut, set_goneng_Position);
	FillConsoleOutputCharacter(hOut, '=', max_postion.X/2, set_goneng_Position, &Written);
	
	set_client_postion.X = 1;
	set_client_postion.Y = 1;
	SetConsoleTextAttribute(hOut, FOREGROUND_GREEN);
	
	HANDLE hMap;
	PDB_MAP pDBMap;
	
	HANDLE hEventRead;
	HANDLE hEventWrite;
	HANDLE hMutex;
	
	row = 0;
	
	hMutex = CreateMutex(NULL, TRUE, "DBWinMutex");
	hEventRead = CreateEvent(NULL, FALSE, FALSE, DBWIN_READEVENT);
	hEventWrite = CreateEvent(NULL, FALSE, FALSE, DBWIN_WRITEVENT);
	if(!hEventRead || !hEventWrite)
	{
		return 0;
	}
	
	hMap = CreateFileMapping(INVALID_HANDLE_VALUE,
        NULL,
        PAGE_READWRITE,
        0,
        PAGE_SIZE,
        DBWIN_MAP);
	if(!hMap)
		return 0;
	
	pDBMap = (PDB_MAP)MapViewOfFile(hMap,
        FILE_MAP_ALL_ACCESS,
        0,
        0,
        0);
	if(!pDBMap)
	{
		CloseHandle(hMap);
		return 0;
	}
	
	SetEvent(hEventWrite);
	ResetEvent(hEventRead);
	dfTim = 0;
	
	while(TRUE)
	{
		QueryPerformanceFrequency(&litmp) ;
		// 获得计数器的时钟频率
		dfFreq = (double)litmp.QuadPart ;
		QueryPerformanceCounter(&litmp) ;
		// 获得初始值
		QPart1 = litmp.QuadPart ;
		
		WaitForSingleObject(hEventRead, INFINITE);
		
		QueryPerformanceCounter(&litmp) ;
		
		// 获得中止值
		QPart2 = litmp.QuadPart ;
		dfMinus = (double)(QPart2 - QPart1) ;
		dfTim = dfTim + (dfMinus / dfFreq) ;
		
		
		set_client_postion.Y = row + 1;
		SetConsoleCursorPosition(hOut, set_client_postion);
		
		printf("%-5d       %10.15f         [%d]%s\n", row, dfTim, pDBMap->dwProcessId, pDBMap->szString);
		row++;	 
		
		SetEvent(hEventWrite);
		ResetEvent(hEventRead);		 
	}
	
	return 0;
}




