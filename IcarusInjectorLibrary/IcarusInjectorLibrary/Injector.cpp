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
	if (pLocalImageBase - pDllRepresentation->pOptHeader->ImageBase) {
		if (code = Relocate(pDllRepresentation, pLocalImageBase)) {
			VirtualFree(pLocalImageBase, 0, MEM_RELEASE);
			VirtualFreeEx(hTargetProcess, pExtImageBase, 0, MEM_RELEASE);
			return code;
		}
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
	DWORD dwExAllocationSize = pDllRepresentation->pOptHeader->SizeOfImage + sizeof(ReflectiveContext) + 1024; //TODO dont use hardcoded shellcode size
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

ICARUS_ERROR_CODE icarus::ReflectiveInjector::Relocate(PDllRepresentation pDllRepresentation, PBYTE pLocalImageBase) noexcept {
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
					*reinterpret_cast<PULONG_PTR>(pLocalImageBase + pBaseRelocBlock->VirtualAddress + (*pRelocEntry & 0xFFF)) += reinterpret_cast<ULONG_PTR>(pLocalImageBase - pDllRepresentation->pOptHeader->ImageBase);
				}
				pRelocEntry++;
			}
			pBaseRelocBlock = reinterpret_cast<PIMAGE_BASE_RELOCATION>(reinterpret_cast<PBYTE>(pBaseRelocBlock) + pBaseRelocBlock->SizeOfBlock);
		}
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
