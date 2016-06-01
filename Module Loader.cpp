#include <string>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <Windows.h>

using namespace std;

string localPath;
string modulesPath;
vector<string> params;
vector<string> modules;
vector<string> description;

typedef char *(__stdcall *ModuleDescription)();
ModuleDescription ModuleDescriptionA = nullptr;

typedef char *(__stdcall *ModuleParams)();
ModuleParams ModuleParamsA = nullptr;

void init()
{
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA ffd;
	char temp[MAX_PATH];
	UINT prevErrorMode;
	HINSTANCE hLib;

	prevErrorMode = GetErrorMode();
	SetErrorMode(prevErrorMode | SEM_FAILCRITICALERRORS);

	if (GetCurrentDirectoryA(MAX_PATH, temp) == 0)
	{
		printf("Error code: -1\nFailed to init...\n");
		
		exit(-1);
	}

	localPath = temp;
	modulesPath = localPath + "\\modules\\";

	hFind = FindFirstFile((modulesPath + "*").c_str(), &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		printf("Error code: -2\nFailed to init...\n");

		exit(-2);
	}

	do
	{
		if (ffd.dwFileAttributes ^ FILE_ATTRIBUTE_DIRECTORY)
		{
			hLib = LoadLibrary((modulesPath + ffd.cFileName).c_str());

			if (hLib)
			{
				ModuleDescriptionA = reinterpret_cast<ModuleDescription>(GetProcAddress(hLib, "ModuleDescription"));
				ModuleParamsA = reinterpret_cast<ModuleParams>(GetProcAddress(hLib, "ModuleParams"));

				if (ModuleDescriptionA && ModuleParamsA)
				{
					description.push_back(ModuleDescriptionA());
					params.push_back(ModuleParamsA());
					modules.push_back(ffd.cFileName);
				}

				FreeLibrary(hLib);
			}
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	FindClose(hFind);
	SetErrorMode(prevErrorMode);
}

void selectModule(int id)
{
	string in;

	printf("\nSyntax for module %s:\n\n%s\n", modules[id].c_str(), params[id].c_str());
	printf("Your arguments: ");
	getline(cin, in);

	printf("args: %s\n", in.c_str());
}

int main(int argc, char* argv[])
{
	int selectedMod = 0;
	init();

	printf("Loacting modules: %s\n", modulesPath.c_str());

	printf("Modules loaded: %d\n\n", modules.size());

	printf("0) Exit\n");
	for (int i = 0; i < (int)modules.size(); i++)
	{
		printf("%d) %s - %s\n", i+1, modules[i].c_str(), description[i].c_str());
	}

	printf("\nSelect the module you want to use: ");
	cin >> selectedMod;
	getchar();
	selectedMod--;

	if (selectedMod == -1)
		return 1;

	selectModule(selectedMod);

	system("pause");
	return 0;
}