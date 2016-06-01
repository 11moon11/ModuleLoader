// 
// Email Checker Module
// 

#include <Windows.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <thread>

using namespace std;

#define DLL_EXPORT

extern "C"
{
	__declspec(dllexport) char *ModuleDescription()
	{
		return "Simple Module (just an example)";
	}

	__declspec(dllexport) char *ModuleParams()
	{
		return	"-db=<str> path to the list (email:pass or email;pass)\n"
				"-t=<int> ammount of threads (by default 10)\n"
				"-s=<str> what to search for (in quotation marks if more than 1 word)\n"
				"-o=<str> where to save output, without this parameter this module will just display the results\n";
	}

}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	return TRUE;
}