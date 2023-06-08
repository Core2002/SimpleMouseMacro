#include <Windows.h> 
#include <winsvc.h> 
#include <conio.h>  
#include <stdio.h>

namespace MyNTDriverLoader {
	BOOL __cdecl LoadNTDriver(char* lpszDriverName, char* lpszDriverPath);
	BOOL __cdecl UnloadNTDriver(char* szSvrName);
}