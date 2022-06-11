#pragma once

#include "Errors.h"
#include "DllUtils.h"
#include "Functions.h"

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

	public:
		virtual ICARUS_ERROR_CODE Inject() noexcept;
		ICARUS_ERROR_CODE RetrieveHandle();
		ICARUS_ERROR_CODE UnlinkFromPEB(HINSTANCE hModule) noexcept;

	protected:
		HANDLE hTargetProcess;
		PInjectionContext ctx;
		explicit Injector(PInjectionContext ctx) noexcept;
		virtual ICARUS_ERROR_CODE _Inject(PDllRepresentation pDllRepresentation) noexcept = 0;

	private:
		ICARUS_ERROR_CODE HijackHandle(DWORD access) noexcept;

	};

	class ReflectiveInjector : public Injector {

		typedef struct ReflectiveContext {
			func_LoadLibraryA funcLoadLibraryA;
			func_GetProcAddress funcGetProcAddress;
			HINSTANCE hModule;
			PBYTE pImageBase;
			DWORD settings;
		} ReflectiveContext, *PReflectiveContext;

	public:
		friend Injector* CreateInjector(PInjectionContext ctx);

	private:
		ReflectiveInjector(PInjectionContext ctx);
		ICARUS_ERROR_CODE _Inject(PDllRepresentation pDllRepresentation) noexcept override;
		ICARUS_ERROR_CODE Allocate(PDllRepresentation pDllRepresentation, PBYTE* ppLocalImageBase, PBYTE* ppExtImageBase) noexcept;
		ICARUS_ERROR_CODE CopyHeadersAndSections(PDllRepresentation pDllRepresentation, PBYTE pLocalImageBase) noexcept;
		ICARUS_ERROR_CODE Relocate(PDllRepresentation pDllRepresentation, PBYTE pLocalImageBase) noexcept;

	};

	Injector* CreateInjector(PInjectionContext ctx);

}