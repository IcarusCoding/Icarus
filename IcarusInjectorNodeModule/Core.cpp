#include "Core.h"

#include <thread>
#include <chrono>

#include <iostream>

AsyncWorker::AsyncWorker(const Napi::Function& callback) : Napi::AsyncWorker(callback) {}

void AsyncWorker::Execute() {
	icarus::WindowSelector selector;
	result = selector.SelectNextWindow();
}

/* TODO error handling */
void AsyncWorker::OnOK() {
	Napi::Env env = Env();
	Callback().MakeCallback(Receiver().Value(), { Napi::Number::New(env, result) });
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
		j["arch"] = processInfo->GetArchitecture() == Architecture::ARCH_X64 ? "x64" : "x86";
		j["exePath"] = processInfo->GetExePath();
		processesNapi[i++] = Napi::String::New(env, j.dump());
        delete processInfo;
	}
	return processesNapi;
}

Napi::Value IsElevated(const Napi::CallbackInfo& info) {
	return Napi::Boolean::New(info.Env(), icarus::IsElevated());
}

Napi::Value RestartElevated(const Napi::CallbackInfo& info) {
	return Napi::Number::New(info.Env(), icarus::ElevatedRestart());
}

VOID ExitApplication(const Napi::CallbackInfo& info) {
	icarus::ExitApplication(info.Length() != 0 ? info[0].ToNumber().Int32Value() : 0);
}

VOID GetNextClicked(const Napi::CallbackInfo& info) {
	icarus::WindowSelector::AbortActiveSelection();
	(new AsyncWorker(info[0].As<Napi::Function>()))->Queue();
}

VOID AbortSelection(const Napi::CallbackInfo& info) {
	icarus::WindowSelector::AbortActiveSelection();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
	icarus::StartConsole();
	exports.Set(Napi::String::New(env, "retrieveProcesses"), Napi::Function::New<RetrieveProcesses>(env));
	exports.Set(Napi::String::New(env, "isElevated"), Napi::Function::New<IsElevated>(env));
	exports.Set(Napi::String::New(env, "restartElevated"), Napi::Function::New<RestartElevated>(env));
	exports.Set(Napi::String::New(env, "exitApplication"), Napi::Function::New<ExitApplication>(env));
	exports.Set(Napi::String::New(env, "getNextClicked"), Napi::Function::New<GetNextClicked>(env));
	exports.Set(Napi::String::New(env, "abortSelection"), Napi::Function::New<AbortSelection>(env));
	return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
