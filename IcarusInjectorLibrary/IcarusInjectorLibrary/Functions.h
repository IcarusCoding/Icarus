#pragma once

#include <Windows.h>

typedef BOOL(__stdcall* func_IsWow64Process) (HANDLE, PBOOL);

static func_IsWow64Process I_IsWow64Process = (func_IsWow64Process)GetProcAddress(GetModuleHandleA("kernel32"), "IsWow64Process");
