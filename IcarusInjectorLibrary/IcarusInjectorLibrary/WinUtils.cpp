#include "WinUtils.h"

#include "lodepng.h"

#include <iostream>

const static TCHAR encodeLookup[] = TEXT("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
const static TCHAR padCharacter = TEXT('=');

unsigned int decodeBMP(std::vector<unsigned char>& image, unsigned int width, unsigned int height, const std::vector<unsigned char>& bmp) {
    unsigned pixeloffset = bmp[10] + 256 * bmp[11];
    unsigned numChannels = bmp[28] / 8;
    unsigned scanlineBytes = width * numChannels;
    if (scanlineBytes % 4 != 0) {
        scanlineBytes = (scanlineBytes / 4) * 4 + 4;
    }
    unsigned dataSize = scanlineBytes * height;
    image.resize(width * height * 4);
    for (unsigned y = 0; y < height; y++) {
        for (unsigned x = 0; x < width; x++) {
            unsigned bmpos = pixeloffset + (height - y - 1) * scanlineBytes + numChannels * x;
            unsigned newpos = (height - y - 1) * scanlineBytes + numChannels * x;
            image[newpos + 0] = bmp[bmpos + 2];
            image[newpos + 1] = bmp[bmpos + 1];
            image[newpos + 2] = bmp[bmpos + 0];
            image[newpos + 3] = bmp[bmpos + 3];
        }
    }
    return 0;
}

BOOL icarus::IsElevated() {
    BOOL retVal = FALSE;
    HANDLE hToken = NULL;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        TOKEN_ELEVATION tokenElev;
        DWORD cbSize = sizeof(TOKEN_ELEVATION);
        if (GetTokenInformation(hToken, TokenElevation, &tokenElev, sizeof(tokenElev), &cbSize)) {
            retVal = tokenElev.TokenIsElevated;
        }
    }
    CloseHandle(hToken);
    return retVal;
}

std::string icarus::ConvertIconToBase64(HICON hIcon) {
    ICONINFO iconInfo;
    if (!GetIconInfo(hIcon, &iconInfo)) {
        // ERROR
        return "";
    }
    const int width = iconInfo.xHotspot * 2;
    const int height = iconInfo.yHotspot * 2;
    HDC originalHDC = GetDC(0);
    HDC hdc = CreateCompatibleDC(originalHDC);
    ReleaseDC(0, originalHDC);

    BITMAPINFOHEADER bitmapInfo;
    bitmapInfo.biSize = sizeof(BITMAPINFOHEADER);
    bitmapInfo.biWidth = width;
    bitmapInfo.biHeight = height;
    bitmapInfo.biPlanes = 1;
    bitmapInfo.biBitCount = 32;
    bitmapInfo.biCompression = BI_RGB;
    bitmapInfo.biSizeImage = 0;
    bitmapInfo.biXPelsPerMeter = 0;
    bitmapInfo.biYPelsPerMeter = 0;
    bitmapInfo.biClrUsed = 0;
    bitmapInfo.biClrImportant = 0;

    DWORD* bits;

    HBITMAP winBitmap = CreateDIBSection(hdc, (BITMAPINFO*)&bitmapInfo, DIB_RGB_COLORS, (VOID**)&bits, NULL, 0);
    HGDIOBJ prevHDC = (HBITMAP) SelectObject(hdc, winBitmap);
    DrawIconEx(hdc, 0, 0, hIcon, width, height, 0, 0, DI_NORMAL);

    BITMAPINFO bmi;
    memset(&bmi, 0, sizeof(bmi));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = width * height * 4;

    LPBYTE lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, bmi.bmiHeader.biSizeImage);
    GetDIBits(hdc, winBitmap, 0, height, lpBits, &bmi, DIB_RGB_COLORS);

    BITMAPFILEHEADER hdr;

    hdr.bfType = 0x4d42;
    hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) +
        bmi.bmiHeader.biSize + bmi.bmiHeader.biClrUsed
        * sizeof(RGBQUAD) + bmi.bmiHeader.biSizeImage);
    hdr.bfReserved1 = 0;
    hdr.bfReserved2 = 0;
    hdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) +
        bmi.bmiHeader.biSize + bmi.bmiHeader.biClrUsed
        * sizeof(RGBQUAD);

    std::vector<BYTE> dataVec;
    CHAR* hdr_bytes = (CHAR*)&hdr;
    CHAR* bmiHeader_bytes = (CHAR*)&bmi.bmiHeader;

    for (int i = 0; i < sizeof(BITMAPFILEHEADER); i++) {
        dataVec.push_back(hdr_bytes[i]);
    }

    for (int i = 0; i < sizeof(BITMAPINFOHEADER) + bmi.bmiHeader.biClrUsed * sizeof(RGBQUAD); i++) {
        dataVec.push_back(bmiHeader_bytes[i]);
    }

    for (int i = 0; i < bmi.bmiHeader.biSizeImage; i++) {
        dataVec.push_back(lpBits[i]);
    }

    std::vector<unsigned char> image;
    unsigned error = decodeBMP(image, width, height, dataVec);

    std::vector<unsigned char> png;
    error = lodepng::encode(png, image, width, height);

    std::string base64 = icarus::ToBase64(png);

    SelectObject(hdc, prevHDC);
    DeleteObject(winBitmap);
    DeleteDC(hdc);
    DeleteObject(iconInfo.hbmMask);
    DeleteObject(iconInfo.hbmColor);
    GlobalFree(lpBits);
    return base64;
}

std::string icarus::ToBase64(std::vector<BYTE> input) {
    std::basic_string<TCHAR> encodedString;
    encodedString.reserve(((input.size() / 3) + (input.size() % 3 > 0)) * 4);
    DWORD temp;
    std::vector<BYTE>::iterator cursor = input.begin();
    for (size_t idx = 0; idx < input.size() / 3; idx++)
    {
        temp = (*cursor++) << 16; //Convert to big endian
        temp += (*cursor++) << 8;
        temp += (*cursor++);
        encodedString.append(1, encodeLookup[(temp & 0x00FC0000) >> 18]);
        encodedString.append(1, encodeLookup[(temp & 0x0003F000) >> 12]);
        encodedString.append(1, encodeLookup[(temp & 0x00000FC0) >> 6]);
        encodedString.append(1, encodeLookup[(temp & 0x0000003F)]);
    }
    switch (input.size() % 3)
    {
    case 1:
        temp = (*cursor++) << 16; //Convert to big endian
        encodedString.append(1, encodeLookup[(temp & 0x00FC0000) >> 18]);
        encodedString.append(1, encodeLookup[(temp & 0x0003F000) >> 12]);
        encodedString.append(2, padCharacter);
        break;
    case 2:
        temp = (*cursor++) << 16; //Convert to big endian
        temp += (*cursor++) << 8;
        encodedString.append(1, encodeLookup[(temp & 0x00FC0000) >> 18]);
        encodedString.append(1, encodeLookup[(temp & 0x0003F000) >> 12]);
        encodedString.append(1, encodeLookup[(temp & 0x00000FC0) >> 6]);
        encodedString.append(1, padCharacter);
        break;
    }
    return encodedString;
}

VOID icarus::StartConsole() {
    AllocConsole();
    FILE* file;
    freopen_s(&file, "CONOUT$", "w", stdout);
}

DWORD icarus::ElevatedRestart() {
    CHAR szModule[MAX_PATH];
    GetModuleFileNameA(NULL, szModule, MAX_PATH);
    CHAR szDirectory[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, szDirectory);
    char* cmd = GetCommandLineA();
    if (*cmd == '"') {
        ++cmd;
        while (*cmd)
            if (*cmd++ == '"')
                break;
    } else {
        while (*cmd && *cmd != ' ' && *cmd != '\t')
            ++cmd;
    }
    while (*cmd == ' ' || *cmd == '\t')
        cmd++;
    SHELLEXECUTEINFOA shellExecuteInfo = {
        .cbSize = sizeof(SHELLEXECUTEINFOA),
        .fMask = NULL,
        .hwnd = NULL,
        .lpVerb = "runas",
        .lpFile = szModule,
        .lpParameters = cmd,
        .lpDirectory = szDirectory,
        .nShow = SW_SHOW,
        .hInstApp = NULL
    };
    if (!ShellExecuteExA(&shellExecuteInfo)) {
        return GetLastError();
    }
    return 0;
}

VOID icarus::ExitApplication(DWORD exitCode) {
    ExitProcess(exitCode);
}

icarus::WindowSelector* icarus::WindowSelector::pWindowSelector;
HHOOK icarus::WindowSelector::hWinHook;

icarus::WindowSelector::WindowSelector() : unhooked(false), PID(-1) {}

icarus::WindowSelector::~WindowSelector() {
    if (hWinHook) {
        UnhookWindowsHookEx(pWindowSelector->hWinHook);
    }
}

LRESULT icarus::WindowSelector::HookProc(INT code, WPARAM wParam, LPARAM lParam) {
    if (wParam == WM_LBUTTONDOWN) {
        POINT p;
        GetCursorPos(&p);
        HWND hwnd = WindowFromPoint(p);
        RECT rect;
        GetWindowRect(hwnd, &rect);
        DWORD PID;
        GetWindowThreadProcessId(hwnd, &PID);
        if (PID != GetCurrentProcessId()) {
            pWindowSelector->PID = PID;
            UnhookWindowsHookEx(pWindowSelector->hWinHook);
            pWindowSelector->unhooked = true;
        }
    }
    return CallNextHookEx(pWindowSelector->hWinHook, code, wParam, lParam);
}

DWORD icarus::WindowSelector::SelectNextWindow() {
    if (pWindowSelector) {
        // ERROR -> already running
        return -1;
    }
    PID = -1;
    pWindowSelector = this;
    hWinHook = SetWindowsHookEx(WH_MOUSE_LL, HookProc, NULL, 0);
    MSG msg;
    while (!unhooked) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    unhooked = FALSE;
    pWindowSelector = nullptr;
    return PID;
}

void icarus::WindowSelector::AbortActiveSelection() {
    if (pWindowSelector) {
        UnhookWindowsHookEx(pWindowSelector->hWinHook);
        pWindowSelector->unhooked = true;
        pWindowSelector = nullptr;
    }
}
