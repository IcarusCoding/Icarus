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

BYTE ShellCode86[] = {
		 0x55, 0x8b, 0xec, 0x83, 0xec, 0x1c, 0x83, 0x7d, 0x08, 0x00, 0x75, 0x05, 0xe9, 0x50, 0x01,
		 0x00, 0x00, 0x8b, 0x45, 0x08, 0x8b, 0x48, 0x0c, 0x89, 0x4d, 0xfc, 0x8b, 0x55, 0xfc, 0x8b,
		 0x42, 0x3c, 0x8b, 0x4d, 0xfc, 0x8d, 0x54, 0x01, 0x18, 0x89, 0x55, 0xec, 0xb8, 0x08, 0x00,
		 0x00, 0x00, 0xc1, 0xe0, 0x00, 0x8b, 0x4d, 0xec, 0x83, 0x7c, 0x01, 0x64, 0x00, 0x0f, 0x84,
		 0xb2, 0x00, 0x00, 0x00, 0xba, 0x08, 0x00, 0x00, 0x00, 0xc1, 0xe2, 0x00, 0x8b, 0x45, 0xec,
		 0x8b, 0x4d, 0xfc, 0x03, 0x4c, 0x10, 0x60, 0x89, 0x4d, 0xf4, 0x8b, 0x55, 0xf4, 0x83, 0x7a,
		 0x0c, 0x00, 0x0f, 0x84, 0x90, 0x00, 0x00, 0x00, 0x8b, 0x45, 0xf4, 0x8b, 0x4d, 0xfc, 0x03,
		 0x48, 0x0c, 0x89, 0x4d, 0xe4, 0x8b, 0x55, 0xe4, 0x52, 0x8b, 0x45, 0x08, 0x8b, 0x08, 0xff,
		 0xd1, 0x89, 0x45, 0xe8, 0x8b, 0x55, 0xf4, 0x8b, 0x45, 0xfc, 0x03, 0x42, 0x10, 0x89, 0x45,
		 0xf8, 0x8b, 0x4d, 0xf8, 0x83, 0x39, 0x00, 0x74, 0x54, 0x8b, 0x55, 0xf8, 0x8b, 0x02, 0x25,
		 0x00, 0x00, 0x00, 0x80, 0x74, 0x1f, 0x8b, 0x4d, 0xf8, 0x8b, 0x11, 0x81, 0xe2, 0xff, 0xff,
		 0xff, 0x7f, 0x52, 0x8b, 0x45, 0xe8, 0x50, 0x8b, 0x4d, 0x08, 0x8b, 0x51, 0x04, 0xff, 0xd2,
		 0x8b, 0x4d, 0xf8, 0x89, 0x01, 0xeb, 0x1e, 0x8b, 0x55, 0xf8, 0x8b, 0x02, 0x8b, 0x4d, 0xfc,
		 0x8d, 0x54, 0x01, 0x02, 0x52, 0x8b, 0x45, 0xe8, 0x50, 0x8b, 0x4d, 0x08, 0x8b, 0x51, 0x04,
		 0xff, 0xd2, 0x8b, 0x4d, 0xf8, 0x89, 0x01, 0x8b, 0x55, 0xf8, 0x83, 0xc2, 0x04, 0x89, 0x55,
		 0xf8, 0xeb, 0xa4, 0x8b, 0x45, 0xf4, 0x83, 0xc0, 0x14, 0x89, 0x45, 0xf4, 0xe9, 0x63, 0xff,
		 0xff, 0xff, 0xb9, 0x08, 0x00, 0x00, 0x00, 0x6b, 0xd1, 0x09, 0x8b, 0x45, 0xec, 0x83, 0x7c,
		 0x10, 0x64, 0x00, 0x74, 0x41, 0xb9, 0x08, 0x00, 0x00, 0x00, 0x6b, 0xd1, 0x09, 0x8b, 0x45,
		 0xec, 0x8b, 0x4c, 0x10, 0x60, 0x8b, 0x55, 0xfc, 0x8b, 0x44, 0x0a, 0x0c, 0x89, 0x45, 0xf0,
		 0x83, 0x7d, 0xf0, 0x00, 0x74, 0x22, 0x8b, 0x4d, 0xf0, 0x83, 0x39, 0x00, 0x74, 0x1a, 0x6a,
		 0x00, 0x6a, 0x01, 0x8b, 0x55, 0xfc, 0x52, 0x8b, 0x45, 0xf0, 0x8b, 0x08, 0xff, 0xd1, 0x8b,
		 0x55, 0xf0, 0x83, 0xc2, 0x04, 0x89, 0x55, 0xf0, 0xeb, 0xd8, 0x6a, 0x00, 0x6a, 0x01, 0x8b,
		 0x45, 0xfc, 0x50, 0x8b, 0x4d, 0xec, 0x8b, 0x55, 0xfc, 0x03, 0x51, 0x10, 0xff, 0xd2, 0x8b,
		 0x45, 0x08, 0x8b, 0x4d, 0xfc, 0x89, 0x48, 0x08, 0x8b, 0xe5, 0x5d, 0xc2, 0x04, 0x00,
};

BYTE ShellCode64[] = {
		 0x48, 0x89, 0x4c, 0x24, 0x08, 0x48, 0x83, 0xec, 0x68, 0x48, 0x83, 0x7c, 0x24, 0x70, 0x00,
		 0x75, 0x05, 0xe9, 0xeb, 0x01, 0x00, 0x00, 0x48, 0x8b, 0x44, 0x24, 0x70, 0x48, 0x8b, 0x40,
		 0x18, 0x48, 0x89, 0x44, 0x24, 0x20, 0x48, 0x8b, 0x44, 0x24, 0x20, 0x48, 0x63, 0x40, 0x3c,
		 0x48, 0x8b, 0x4c, 0x24, 0x20, 0x48, 0x8d, 0x44, 0x01, 0x18, 0x48, 0x89, 0x44, 0x24, 0x40,
		 0xb8, 0x08, 0x00, 0x00, 0x00, 0x48, 0x6b, 0xc0, 0x01, 0x48, 0x8b, 0x4c, 0x24, 0x40, 0x83,
		 0x7c, 0x01, 0x74, 0x00, 0x0f, 0x84, 0x0f, 0x01, 0x00, 0x00, 0xb8, 0x08, 0x00, 0x00, 0x00,
		 0x48, 0x6b, 0xc0, 0x01, 0x48, 0x8b, 0x4c, 0x24, 0x40, 0x8b, 0x44, 0x01, 0x70, 0x48, 0x8b,
		 0x4c, 0x24, 0x20, 0x48, 0x03, 0xc8, 0x48, 0x8b, 0xc1, 0x48, 0x89, 0x44, 0x24, 0x30, 0x48,
		 0x8b, 0x44, 0x24, 0x30, 0x83, 0x78, 0x0c, 0x00, 0x0f, 0x84, 0xde, 0x00, 0x00, 0x00, 0x48,
		 0x8b, 0x44, 0x24, 0x30, 0x8b, 0x40, 0x0c, 0x48, 0x8b, 0x4c, 0x24, 0x20, 0x48, 0x03, 0xc8,
		 0x48, 0x8b, 0xc1, 0x48, 0x89, 0x44, 0x24, 0x50, 0x48, 0x8b, 0x4c, 0x24, 0x50, 0x48, 0x8b,
		 0x44, 0x24, 0x70, 0xff, 0x10, 0x48, 0x89, 0x44, 0x24, 0x48, 0x48, 0x8b, 0x44, 0x24, 0x30,
		 0x8b, 0x40, 0x10, 0x48, 0x8b, 0x4c, 0x24, 0x20, 0x48, 0x03, 0xc8, 0x48, 0x8b, 0xc1, 0x48,
		 0x89, 0x44, 0x24, 0x28, 0x48, 0x8b, 0x44, 0x24, 0x28, 0x48, 0x83, 0x38, 0x00, 0x74, 0x7f,
		 0x48, 0x8b, 0x44, 0x24, 0x28, 0x48, 0xb9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
		 0x48, 0x8b, 0x00, 0x48, 0x23, 0xc1, 0x48, 0x85, 0xc0, 0x74, 0x28, 0x48, 0x8b, 0x44, 0x24,
		 0x28, 0x48, 0x8b, 0x00, 0x48, 0x25, 0xff, 0xff, 0xff, 0x7f, 0x48, 0x8b, 0xd0, 0x48, 0x8b,
		 0x4c, 0x24, 0x48, 0x48, 0x8b, 0x44, 0x24, 0x70, 0xff, 0x50, 0x08, 0x48, 0x8b, 0x4c, 0x24,
		 0x28, 0x48, 0x89, 0x01, 0xeb, 0x2a, 0x48, 0x8b, 0x44, 0x24, 0x28, 0x48, 0x8b, 0x00, 0x48,
		 0x8b, 0x4c, 0x24, 0x20, 0x48, 0x8d, 0x44, 0x01, 0x02, 0x48, 0x8b, 0xd0, 0x48, 0x8b, 0x4c,
		 0x24, 0x48, 0x48, 0x8b, 0x44, 0x24, 0x70, 0xff, 0x50, 0x08, 0x48, 0x8b, 0x4c, 0x24, 0x28,
		 0x48, 0x89, 0x01, 0x48, 0x8b, 0x44, 0x24, 0x28, 0x48, 0x83, 0xc0, 0x08, 0x48, 0x89, 0x44,
		 0x24, 0x28, 0xe9, 0x76, 0xff, 0xff, 0xff, 0x48, 0x8b, 0x44, 0x24, 0x30, 0x48, 0x83, 0xc0,
		 0x14, 0x48, 0x89, 0x44, 0x24, 0x30, 0xe9, 0x13, 0xff, 0xff, 0xff, 0xb8, 0x08, 0x00, 0x00,
		 0x00, 0x48, 0x6b, 0xc0, 0x09, 0x48, 0x8b, 0x4c, 0x24, 0x40, 0x83, 0x7c, 0x01, 0x74, 0x00,
		 0x74, 0x58, 0xb8, 0x08, 0x00, 0x00, 0x00, 0x48, 0x6b, 0xc0, 0x09, 0x48, 0x8b, 0x4c, 0x24,
		 0x40, 0x8b, 0x44, 0x01, 0x70, 0x48, 0x8b, 0x4c, 0x24, 0x20, 0x48, 0x8b, 0x44, 0x01, 0x18,
		 0x48, 0x89, 0x44, 0x24, 0x38, 0x48, 0x83, 0x7c, 0x24, 0x38, 0x00, 0x74, 0x2f, 0x48, 0x8b,
		 0x44, 0x24, 0x38, 0x48, 0x83, 0x38, 0x00, 0x74, 0x24, 0x45, 0x33, 0xc0, 0xba, 0x01, 0x00,
		 0x00, 0x00, 0x48, 0x8b, 0x4c, 0x24, 0x20, 0x48, 0x8b, 0x44, 0x24, 0x38, 0xff, 0x10, 0x48,
		 0x8b, 0x44, 0x24, 0x38, 0x48, 0x83, 0xc0, 0x08, 0x48, 0x89, 0x44, 0x24, 0x38, 0xeb, 0xc9,
		 0x48, 0x8b, 0x44, 0x24, 0x40, 0x8b, 0x40, 0x10, 0x48, 0x8b, 0x4c, 0x24, 0x20, 0x48, 0x03,
		 0xc8, 0x48, 0x8b, 0xc1, 0x45, 0x33, 0xc0, 0xba, 0x01, 0x00, 0x00, 0x00, 0x48, 0x8b, 0x4c,
		 0x24, 0x20, 0xff, 0xd0, 0x48, 0x8b, 0x44, 0x24, 0x70, 0x48, 0x8b, 0x4c, 0x24, 0x20, 0x48,
		 0x89, 0x48, 0x10, 0x48, 0x83, 0xc4, 0x68, 0xc3,
};

#ifdef _WIN64
#define ShellCode ShellCode64
#else
#define ShellCode ShellCode86
#endif

icarus::ReflectiveInjector::ReflectiveInjector(PInjectionContext ctx) : Injector(ctx) {}

//TODO free memory in called subroutines
ICARUS_ERROR_CODE icarus::ReflectiveInjector::_Inject(PDllRepresentation pDllRepresentation) noexcept {
	PBYTE pLocalImageBase, pExtImageBase;
	ICARUS_ERROR_CODE code = Allocate(pDllRepresentation, &pLocalImageBase, &pExtImageBase);
	std::cout << "LOCAL IMAGE BASE -> " << static_cast<PVOID>(pLocalImageBase) << "\n";
	std::cout << "EXT IMAGE BASE -> " << static_cast<PVOID>(pExtImageBase) << "\n";
	if (code) {
		return code;
	}
	if (code = CopyHeadersAndSections(pDllRepresentation, pLocalImageBase)) {
		VirtualFree(pLocalImageBase, 0, MEM_RELEASE);
		VirtualFreeEx(hTargetProcess, pExtImageBase, 0, MEM_RELEASE);
		return code;
	}
	if (pExtImageBase - pDllRepresentation->pOptHeader->ImageBase) {
		if (code = Relocate(pDllRepresentation, pLocalImageBase, pExtImageBase)) {
			VirtualFree(pLocalImageBase, 0, MEM_RELEASE);
			VirtualFreeEx(hTargetProcess, pExtImageBase, 0, MEM_RELEASE);
			return code;
		}
	}
	if (code = CopyToProcess(pDllRepresentation, pLocalImageBase, pExtImageBase)) {
		VirtualFree(pLocalImageBase, 0, MEM_RELEASE);
		return code;
	}
	VirtualFree(pLocalImageBase, 0, MEM_RELEASE);
	if (code = ExecuteRemoteShellCode(pDllRepresentation, pExtImageBase)) {
		return code;
	}
	return code;
}

ICARUS_ERROR_CODE icarus::ReflectiveInjector::Allocate(PDllRepresentation pDllRepresentation, PBYTE* ppLocalImageBase, PBYTE* ppExtImageBase) noexcept {
	if (!pDllRepresentation || !ppLocalImageBase || !ppExtImageBase) {
		return ICARUS_ARGUMENT_INVALID;
	}
	PBYTE pLocalImageBase = reinterpret_cast<PBYTE>(VirtualAlloc(nullptr, pDllRepresentation->pOptHeader->SizeOfImage, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE));
	if (!pLocalImageBase) {
		return ICARUS_LOCAL_MEMORY_ALLOCATION_FAILED;
	}
	DWORD dwExAllocationSize = pDllRepresentation->pOptHeader->SizeOfImage + sizeof(ReflectiveContext) + sizeof(ShellCode);
	PBYTE pExtImageBase = reinterpret_cast<PBYTE>(VirtualAllocEx(hTargetProcess, reinterpret_cast<PVOID>(pDllRepresentation->pOptHeader->ImageBase), dwExAllocationSize, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE));
	if (!pExtImageBase) {
		pExtImageBase = reinterpret_cast<PBYTE>(VirtualAllocEx(hTargetProcess, nullptr, dwExAllocationSize, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE));
		if (!pExtImageBase) {
			VirtualFree(pLocalImageBase, 0, MEM_RELEASE);
			return ICARUS_EXT_MEMORY_ALLOCATION_FAILED;
		}
	}
	*ppLocalImageBase = pLocalImageBase;
	*ppExtImageBase = pExtImageBase;
	return ICARUS_SUCCESS;
}

ICARUS_ERROR_CODE icarus::ReflectiveInjector::CopyHeadersAndSections(PDllRepresentation pDllRepresentation, PBYTE pLocalImageBase) noexcept {
	if (!pDllRepresentation || !pLocalImageBase) {
		return ICARUS_ARGUMENT_INVALID;
	}
	memcpy(pLocalImageBase, pDllRepresentation->pRawData, pDllRepresentation->pOptHeader->SizeOfHeaders);
	PIMAGE_SECTION_HEADER pCurrSectionHeader = IMAGE_FIRST_SECTION(pDllRepresentation->pNtHeaders);
	for (UINT i = 0; i < pDllRepresentation->pCoffHeader->NumberOfSections; i++) {
		if (pCurrSectionHeader->SizeOfRawData) {
			memcpy(pLocalImageBase + pCurrSectionHeader->VirtualAddress, pDllRepresentation->pRawData + pCurrSectionHeader->PointerToRawData, pCurrSectionHeader->SizeOfRawData);
		}
		pCurrSectionHeader++;
	}
	return ICARUS_SUCCESS;
}

ICARUS_ERROR_CODE icarus::ReflectiveInjector::Relocate(PDllRepresentation pDllRepresentation, PBYTE pLocalImageBase, PBYTE pExtImageBase) noexcept {
	if (!pDllRepresentation || !pLocalImageBase) {
		return ICARUS_ARGUMENT_INVALID;
	}
	if (pDllRepresentation->pOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size) {
		PIMAGE_BASE_RELOCATION pBaseRelocBlock = reinterpret_cast<PIMAGE_BASE_RELOCATION>(pLocalImageBase + pDllRepresentation->pOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
		while (pBaseRelocBlock->VirtualAddress && pBaseRelocBlock->SizeOfBlock) {
			PWORD pRelocEntry = reinterpret_cast<PWORD>(pBaseRelocBlock + 1); // first offset entry is the first word after the IMAGE_BASE_RELOCATION structure
			for (UINT i = 0; i < (pBaseRelocBlock->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD); i++) {
#ifdef _WIN64
				if (*pRelocEntry >> 0xC == IMAGE_REL_BASED_DIR64) {
#else
				if (*pRelocEntry >> 0xC == IMAGE_REL_BASED_HIGHLOW) {
#endif
					// apply fixup at base + blockVA + reloc offset
					*reinterpret_cast<PULONG_PTR>(pLocalImageBase + pBaseRelocBlock->VirtualAddress + (*pRelocEntry & 0xFFF)) += reinterpret_cast<ULONG_PTR>(pExtImageBase - pDllRepresentation->pOptHeader->ImageBase);
				}
				pRelocEntry++;
			}
			pBaseRelocBlock = reinterpret_cast<PIMAGE_BASE_RELOCATION>(reinterpret_cast<PBYTE>(pBaseRelocBlock) + pBaseRelocBlock->SizeOfBlock);
		}
	}
	return ICARUS_SUCCESS;
}

ICARUS_ERROR_CODE icarus::ReflectiveInjector::CopyToProcess(PDllRepresentation pDllRepresentation, PBYTE pLocalImageBase, PBYTE pExtImageBase) noexcept {
	ReflectiveContext ctx = {
		.funcLoadLibraryA = LoadLibraryA,
		.funcGetProcAddress = reinterpret_cast<func_GetProcAddress>(GetProcAddress),
		.hModule = nullptr,
		.pImageBase = pExtImageBase,
		.settings = this->ctx->settings
	};
	// write image to remote process
	if (!WriteProcessMemory(hTargetProcess, pExtImageBase, pLocalImageBase, pDllRepresentation->pOptHeader->SizeOfImage, nullptr)) {
		return ICARUS_EXT_MEMORY_WRITE_FAILED;
	}
	// write shellcode parameter to remote process
	if (!WriteProcessMemory(hTargetProcess, pExtImageBase + pDllRepresentation->pOptHeader->SizeOfImage, &ctx, sizeof(ctx), nullptr)) {
		VirtualFreeEx(hTargetProcess, pExtImageBase, 0, MEM_RELEASE);
		return ICARUS_EXT_MEMORY_WRITE_FAILED;
	}
	// write shellcode to remote process
	if (!WriteProcessMemory(hTargetProcess, pExtImageBase + pDllRepresentation->pOptHeader->SizeOfImage + sizeof(ctx), ShellCode, sizeof(ShellCode), nullptr)) {
		VirtualFreeEx(hTargetProcess, pExtImageBase + pDllRepresentation->pOptHeader->SizeOfImage, 0, MEM_RELEASE);
		VirtualFreeEx(hTargetProcess, pExtImageBase, 0, MEM_RELEASE);
		return ICARUS_EXT_MEMORY_WRITE_FAILED;
	}
	return ICARUS_SUCCESS;
}

ICARUS_ERROR_CODE icarus::ReflectiveInjector::ExecuteRemoteShellCode(PDllRepresentation pDllRepresentation, PBYTE pExtImageBase) noexcept {
	HANDLE hRemoteThread = CreateRemoteThread(hTargetProcess, nullptr, 0, 
		reinterpret_cast<LPTHREAD_START_ROUTINE>(pExtImageBase + pDllRepresentation->pOptHeader->SizeOfImage + sizeof(ReflectiveContext)),
		pExtImageBase + pDllRepresentation->pOptHeader->SizeOfImage, 0, nullptr);
	if (!hRemoteThread) {
		VirtualFreeEx(hTargetProcess, pExtImageBase + pDllRepresentation->pOptHeader->SizeOfImage + sizeof(ReflectiveContext), 0, MEM_RELEASE);
		VirtualFreeEx(hTargetProcess, pExtImageBase + pDllRepresentation->pOptHeader->SizeOfImage, 0, MEM_RELEASE);
		VirtualFreeEx(hTargetProcess, pExtImageBase, 0, MEM_RELEASE);
		return ICARUS_EXT_THREAD_CREATION_FAILED;
	}
	WaitForSingleObject(hRemoteThread, INFINITE);
	CloseHandle(hRemoteThread);
	HMODULE hModule = nullptr;
	while (!hModule) {
		ReflectiveContext targetCtx = { 0 };
		ReadProcessMemory(hTargetProcess, pExtImageBase + pDllRepresentation->pOptHeader->SizeOfImage, &targetCtx, sizeof(targetCtx), nullptr);
		hModule = targetCtx.hModule;
		Sleep(100);
	}
	return ICARUS_SUCCESS;
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
