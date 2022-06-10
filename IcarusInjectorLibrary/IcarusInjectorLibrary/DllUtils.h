#pragma once

#include "Errors.h"

#include <Windows.h>
#include <fstream>

namespace icarus {

	typedef struct DllRepresentation {
		PBYTE pRawData;
		PIMAGE_DOS_HEADER pDosHeader;
		PIMAGE_NT_HEADERS pNtHeaders;
		PIMAGE_FILE_HEADER pCoffHeader;
		PIMAGE_OPTIONAL_HEADER pOptHeader;
		PCCH szDllPath;
	} DllRepresentation, *PDllRepresentation;

	DWORD LoadDll(PCCH szDllPath, PDllRepresentation pDllRepresentation);

}