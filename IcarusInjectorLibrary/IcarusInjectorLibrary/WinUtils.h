#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <stdio.h>

namespace icarus {

	BOOL IsElevated();
	std::string ConvertIconToBase64(HICON hIcon);
	std::string ToBase64(std::vector<BYTE> input);
	VOID StartConsole();
	DWORD ElevatedRestart();
	VOID ExitApplication(DWORD exitCode = 0);
	DWORD GetNextWindowClicked(); // TODO return ProcessInfo

}
