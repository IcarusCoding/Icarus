#pragma once

#include "Functions.h"
#include "WinUtils.h"

#include <Windows.h>
#include <string>
#include <vector>
#include <tlhelp32.h>

class ProcessInfo;

typedef enum class Architecture {
	ARCH_X86,
	ARCH_X64
} Architecture;

typedef ProcessInfo* PProcessInfo;

class ProcessInfo {

private:
	DWORD PID;
	std::string name;
	std::string icon;
	Architecture arch;
	std::string exePath;

public:
	explicit ProcessInfo() = default;
	ProcessInfo(DWORD PID, std::string name, std::string icon, Architecture arch, std::string exePath) noexcept;
	DWORD GetPID() const noexcept;
	std::string GetName() const noexcept;
	std::string GetIcon() const noexcept;
	Architecture GetArchitecture() const noexcept;
	std::string GetExePath() const noexcept;

};

std::string GetDefaultIcon() noexcept;
std::vector<PProcessInfo> GetAllProcesses() noexcept;
