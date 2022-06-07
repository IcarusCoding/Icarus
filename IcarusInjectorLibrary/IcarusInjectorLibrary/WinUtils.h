#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <stdio.h>

namespace icarus {

	class WindowSelector {

	private:
		static WindowSelector* pWindowSelector;
		static HHOOK hWinHook;
		bool unhooked;
		DWORD PID;
		static LRESULT CALLBACK HookProc(INT code, WPARAM wParam, LPARAM lParam);

	public:
		explicit WindowSelector();
		~WindowSelector();
		DWORD SelectNextWindow();
		static void AbortActiveSelection();

	};

	BOOL IsElevated();
	std::string ConvertIconToBase64(HICON hIcon);
	std::string ToBase64(std::vector<BYTE> input);
	VOID StartConsole();
	DWORD ElevatedRestart();
	VOID ExitApplication(DWORD exitCode = 0);

}
