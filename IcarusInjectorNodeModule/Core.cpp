#include "WinUtils.h"
#include "ProcessInfo.h"

#include "json.hpp"

#include <napi.h>
#include <vector>
#include <iostream>
#include <tlhelp32.h>
#include <tchar.h>

void printError(const char* msg) {
    DWORD eNum;
    TCHAR sysMsg[256];
    TCHAR* p;

    eNum = GetLastError();
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, eNum,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_ENGLISH_US),
        sysMsg, 256, NULL);

    std::cout << "WARNING: " << msg << " failed with error " << eNum << " (" << sysMsg << ")\n";
}

Napi::Value RetrieveProcesses(const Napi::CallbackInfo &info) {
	Napi::Env env = info.Env();
    std::vector<PProcessInfo> processes = GetAllProcesses();
    Napi::Array processesNapi = Napi::Array::New(env, processes.size());
    int i = 0;
	for (auto const& processInfo : processes) {
		nlohmann::json j;
		j["pid"] = processInfo->GetPID();
		j["name"] = processInfo->GetName();
		j["icon"] = processInfo->GetIcon();
		j["exePath"] = processInfo->GetExePath();
		processesNapi[i++] = Napi::String::New(env, j.dump());
        delete processInfo;
	}
	return processesNapi;
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
	exports.Set(Napi::String::New(env, "retrieveProcesses"), Napi::Function::New<RetrieveProcesses>(env));
	return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
