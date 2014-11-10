// showinfo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <mmsystem.h.>
int main(int argc, char* argv[])
{
	int count=0;
	char tempitoa[1000];
	for(int i=0; ; i++)
	{
		itoa(i, tempitoa, 10);
		OutputDebugString(tempitoa);

//		int count=0;
		srand(time(NULL));
		for(int j=0; j<10; j++)
		{
			
			count = rand() % 1000;
		}
		Sleep(count);
		printf("%d\n", i);
	}
	return 0;
}

