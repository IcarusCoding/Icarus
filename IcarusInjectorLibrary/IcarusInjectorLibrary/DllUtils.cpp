#include "DllUtils.h"

DWORD icarus::LoadDll(PCCH szDllPath, PDllRepresentation pDllRepresentation) {
	const DWORD dwAttributes = GetFileAttributes(szDllPath);
	if ((dwAttributes == INVALID_FILE_ATTRIBUTES && GetLastError() == ERROR_FILE_NOT_FOUND) || ((dwAttributes & FILE_ATTRIBUTE_DEVICE) || (dwAttributes & FILE_ATTRIBUTE_DIRECTORY))) {
		return ICARUS_FILE_NOT_FOUND;
	}
	std::ifstream fileIn(szDllPath, std::ios::binary | std::ios::ate);
	if (!fileIn) {
		return ICARUS_FILE_READ_FAILED;
	}
	const std::streampos endPos = fileIn.tellg();
	PBYTE pRawData = new BYTE[static_cast<UINT_PTR>(endPos)];
	fileIn.seekg(std::ios::beg);
	fileIn.read(reinterpret_cast<PCHAR>(pRawData), endPos);
	pDllRepresentation->pRawData = pRawData;
	pDllRepresentation->pDosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(pDllRepresentation->pRawData);
	pDllRepresentation->pNtHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(pDllRepresentation->pRawData + pDllRepresentation->pDosHeader->e_lfanew);
	pDllRepresentation->pCoffHeader = &pDllRepresentation->pNtHeaders->FileHeader;
	pDllRepresentation->pOptHeader = &pDllRepresentation->pNtHeaders->OptionalHeader;
	pDllRepresentation->szDllPath = szDllPath;
	return ICARUS_SUCCESS;
}
