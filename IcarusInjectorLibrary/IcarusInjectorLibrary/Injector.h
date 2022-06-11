#pragma once

#include "Errors.h"
#include "DllUtils.h"

#include <Windows.h>
#include <vector>
#include <string>

// Injection Method
#define LOADLIBRARY 0x40
#define REFLECTIVE 0x80
#define NATIVE 0x100

namespace icarus {

	class ReflectiveInjector;

	constexpr DWORD ACCESS_RIGHTS = PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_QUERY_INFORMATION;

	typedef std::vector<std::string> stringvector;

	typedef struct InjectionContext {
		DWORD PID;
		stringvector dllVector;
		DWORD settings;
	} InjectionContext, * PInjectionContext;

	class Injector {

	protected:
		HANDLE hTargetProcess;
		PInjectionContext ctx;
		explicit Injector(PInjectionContext ctx) noexcept;
		virtual ICARUS_ERROR_CODE _Inject(PDllRepresentation pDllRepresentation) noexcept = 0;

	public:
		virtual ICARUS_ERROR_CODE Inject() noexcept;
		ICARUS_ERROR_CODE RetrieveHandle();
		ICARUS_ERROR_CODE UnlinkFromPEB(HINSTANCE hModule) noexcept;

	private:
		ICARUS_ERROR_CODE HijackHandle(DWORD access) noexcept;

	};

	class ReflectiveInjector : public Injector {

	private:
		ReflectiveInjector(PInjectionContext ctx);

	public:
		friend Injector* CreateInjector(PInjectionContext ctx);

	private:
		ICARUS_ERROR_CODE _Inject(PDllRepresentation pDllRepresentation) noexcept override;

	};

	Injector* CreateInjector(PInjectionContext ctx);

}