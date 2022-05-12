#pragma once

#include <Windows.h>
#include <vector>
#include <string>

// Injection Method
#define LOADLIBRARY 0x40
#define REFLECTIVE 0x80
#define NATIVE 0x100

class ReflectiveInjector;

typedef std::vector<std::string> stringvector;

typedef struct InjectionContext {
	DWORD PID;
	stringvector dllVector;
	DWORD settings;
} InjectionContext, * PInjectionContext;

typedef struct DllStructure {
	PBYTE pDllData;
	PIMAGE_DOS_HEADER pDosHeader;
	PIMAGE_NT_HEADERS pNtHeaders;
	PIMAGE_FILE_HEADER pCoffHeader;
	PIMAGE_OPTIONAL_HEADER pOptHeader;
	std::string pDllPath;
} DllStructure, * PDllStructure;

class Injector {

protected:
	HANDLE hTargetProcess;
	PInjectionContext ctx;
	explicit Injector(PInjectionContext ctx) noexcept;

public:
	virtual DWORD Inject() noexcept = 0;
	DWORD GetHandle();
	DWORD UnlinkFromPEB(HINSTANCE hModule) noexcept;

private:
	DWORD HijackHandle(DWORD access) noexcept;

};

class ReflectiveInjector : public Injector {

private:
	ReflectiveInjector(PInjectionContext ctx);

public:
	friend Injector* CreateInjector(PInjectionContext ctx);
	DWORD Inject() noexcept;

private:
	DWORD _Inject(PDllStructure pDllStructure) noexcept;

};

Injector* CreateInjector(PInjectionContext ctx);