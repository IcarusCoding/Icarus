#pragma once

#include <Windows.h>
#include <string>
#include <vector>

BOOL IsElevated();
std::string ConvertIconToBase64(HICON hIcon);
std::string ToBase64(std::vector<BYTE> input);
