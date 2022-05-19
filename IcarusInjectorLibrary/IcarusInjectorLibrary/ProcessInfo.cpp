#include "ProcessInfo.h"

#include <iostream>

using std::string;
using std::vector;

string DEFAULT_EXECUTABLE_ICON = "";

ProcessInfo::ProcessInfo(DWORD PID, string name, string icon, Architecture arch, string exePath) noexcept
	: PID(PID), name(name), icon(icon), arch(arch), exePath(exePath) {}

DWORD ProcessInfo::GetPID() const noexcept {
	return this->PID;
}

string ProcessInfo::GetName() const noexcept {
	return this->name;
}

string ProcessInfo::GetIcon() const noexcept {
	return this->icon;
}

Architecture ProcessInfo::GetArchitecture() const noexcept {
	return this->arch;
}

string ProcessInfo::GetExePath() const noexcept {
	return this->exePath;
}

string GetDefaultIcon() noexcept {
	if (DEFAULT_EXECUTABLE_ICON != "") {
		return DEFAULT_EXECUTABLE_ICON;
	}
	const HINSTANCE hImageres = LoadLibraryA("imageres.dll");
	if (hImageres) {
		DEFAULT_EXECUTABLE_ICON = ConvertIconToBase64(LoadIconA(hImageres, MAKEINTRESOURCEA(15)));
	}
	return DEFAULT_EXECUTABLE_ICON;
}

vector<PProcessInfo> GetAllProcesses() noexcept {
    std::vector<PProcessInfo> processes;
    HANDLE hProcessSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnapshot == INVALID_HANDLE_VALUE) {
      //  printError(TEXT("CreateToolhelp32Snapshot (of processes)"));
        return processes;
    }
    PROCESSENTRY32 pe32 = {
        .dwSize = sizeof(PROCESSENTRY32)
    };
    if (!Process32First(hProcessSnapshot, &pe32)) {
      //  printError(TEXT("Process32First"));
        CloseHandle(hProcessSnapshot);
        return processes;
    }
    do {
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID); // TODO PROCESS_QUERY_LIMITED_INFORMATION
        if (hProcess == NULL) {
          //  printError(TEXT("OpenProcess"));
        }
        else {
            DWORD maxPath = MAX_PATH;
            CHAR imageName[MAX_PATH];
            if (!QueryFullProcessImageNameA(hProcess, 0, imageName, &maxPath)) {
            //    printError(TEXT("QueryFullProcessImageNameA"));
            }
            else {
                std::vector<HICON> icons(1);
                UINT nIcons = 0;
                if ((nIcons = ExtractIconExA(imageName, 0, icons.data(), nullptr, 1)) == UINT_MAX) {
                //    printError(TEXT("ExtractIconExA"));
                }
                else {
                    icons.resize(nIcons);
                    std::string icon;
                    if (!icons.empty()) {
                        std::string base64 = ConvertIconToBase64(icons.at(0));
                        icon = base64;
                        DestroyIcon(icons.at(0));
                    }
                    else {
                        icon = GetDefaultIcon();
                    }
                    processes.push_back(new ProcessInfo(pe32.th32ProcessID, pe32.szExeFile, icon, Architecture::ARCH_X64, imageName));
                }
            }
            CloseHandle(hProcess);
        }
    } while (Process32Next(hProcessSnapshot, &pe32));
    CloseHandle(hProcessSnapshot);
    return processes;
}

Architecture GetArchitecture(HANDLE hProcess) noexcept {
	return Architecture();
}
