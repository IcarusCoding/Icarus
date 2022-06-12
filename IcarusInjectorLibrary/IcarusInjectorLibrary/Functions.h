#pragma once

#include <Windows.h>

typedef BOOL(__stdcall* func_IsWow64Process) (HANDLE, PBOOL);
typedef HMODULE(__stdcall* func_LoadLibraryA) (PCCH lpLibFileName);
typedef UINT_PTR(__stdcall* func_GetProcAddress) (HINSTANCE hModule, PCCH lpProcName);
typedef BOOL(__stdcall* func_DllMain) (PVOID hInst, DWORD fdwReason, LPVOID lpReserved);

static func_IsWow64Process I_IsWow64Process = (func_IsWow64Process)GetProcAddress(GetModuleHandleA("kernel32"), "IsWow64Process");
