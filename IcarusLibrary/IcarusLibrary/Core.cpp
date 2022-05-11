#include <Windows.h>
#include <stdio.h>
#include <iostream>

using std::cout;

void Init() {
	AllocConsole();
	FILE* file;
	freopen_s(&file, "CONOUT$", "w", stdout);
	cout << "INJECTED!\n";
}

BOOL WINAPI DllMain(HINSTANCE hInstDll, DWORD fdwReason, LPVOID lpReserved) {
	switch (fdwReason) {
		case DLL_PROCESS_ATTACH:
			Init();
	}
	return TRUE;
}