#include "Injector.h"

#include <iostream>

using std::cout;

icarus::Injector::Injector(PInjectionContext ctx) noexcept : ctx(ctx), hTargetProcess(nullptr) {

}

DWORD icarus::Injector::RetrieveHandle() {
	hTargetProcess = OpenProcess(ACCESS_RIGHTS, FALSE, ctx->PID);
	if (!hTargetProcess) {
		return ICARUS_PROCESS_OPEN_FAILED;
	}
	return ICARUS_SUCCESS;
}

icarus::ReflectiveInjector::ReflectiveInjector(PInjectionContext ctx) : Injector(ctx) {}

DWORD icarus::ReflectiveInjector::Inject() noexcept {
	if (hTargetProcess) {
		CloseHandle(hTargetProcess);
		hTargetProcess = nullptr;
	}
	DWORD code = icarus::Injector::RetrieveHandle();
	if (code) {
		return code;
	}
	/* DO PROPERLY */
	std::string dll = ctx->dllVector.at(0);


	return ICARUS_SUCCESS;
}

icarus::Injector* icarus::CreateInjector(icarus::PInjectionContext ctx) {
	DWORD settings = ctx->settings;
	if (settings & LOADLIBRARY) {
		cout << "Not supported\n";
	}	
	if (settings & REFLECTIVE) {
		return new icarus::ReflectiveInjector(ctx);
	}
	if (settings & NATIVE) {
		cout << "Not supported\n";
	}
	return nullptr;
}
