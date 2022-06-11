#include "Injector.h"

#include <iostream>

icarus::Injector::Injector(PInjectionContext ctx) noexcept : ctx(ctx), hTargetProcess(nullptr) {}

ICARUS_ERROR_CODE icarus::Injector::Inject() noexcept {
	if (hTargetProcess) {
		CloseHandle(hTargetProcess);
		hTargetProcess = nullptr;
	}
	ICARUS_ERROR_CODE code = icarus::Injector::RetrieveHandle();
	if (code) {
		return code;
	}
	for (auto &dllPath : ctx->dllVector) {
		DllRepresentation dll = { 0 };
		if (code = LoadDll(dllPath.c_str(), &dll)) {
			delete dll.pRawData;
			break;
		}
		if (code = ValidateDll(&dll)) {
			delete dll.pRawData;
			break;
		}
		if (code = _Inject(&dll)) {
			delete dll.pRawData;
			break;
		}
		delete dll.pRawData;
	}
	if (hTargetProcess) {
		CloseHandle(hTargetProcess);
		hTargetProcess = nullptr;
	}
	return code;
}

ICARUS_ERROR_CODE icarus::Injector::RetrieveHandle() {
	hTargetProcess = OpenProcess(ACCESS_RIGHTS, FALSE, ctx->PID);
	if (!hTargetProcess) {
		return ICARUS_PROCESS_OPEN_FAILED;
	}
	return ICARUS_SUCCESS;
}

icarus::ReflectiveInjector::ReflectiveInjector(PInjectionContext ctx) : Injector(ctx) {}

ICARUS_ERROR_CODE icarus::ReflectiveInjector::_Inject(PDllRepresentation pDllRepresentation) noexcept {
	return 0;
}

icarus::Injector* icarus::CreateInjector(icarus::PInjectionContext ctx) {
	DWORD settings = ctx->settings;
	if (settings & LOADLIBRARY) {
		std::cout << "Not supported\n";
	}	
	if (settings & REFLECTIVE) {
		return new icarus::ReflectiveInjector(ctx);
	}
	if (settings & NATIVE) {
		std::cout << "Not supported\n";
	}
	return nullptr;
}
