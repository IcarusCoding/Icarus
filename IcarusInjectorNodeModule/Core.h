#pragma once

#include "WinUtils.h"
#include "ProcessInfo.h"

#include "json.hpp"

#include <napi.h>
#include <vector>

class AsyncWorker : public Napi::AsyncWorker {

private:
	DWORD result;

public:
	AsyncWorker(const Napi::Function& callback);
	void Execute() override;
	void OnOK() override;

};

Napi::Value RetrieveProcesses(const Napi::CallbackInfo& info);
Napi::Value IsElevated(const Napi::CallbackInfo& info);
Napi::Value RestartElevated(const Napi::CallbackInfo& info);
VOID ExitApplication(const Napi::CallbackInfo& info);
VOID GetNextClicked(const Napi::CallbackInfo& info);
Napi::Object Init(Napi::Env env, Napi::Object exports);
