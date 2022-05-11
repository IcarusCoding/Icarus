#pragma once

#include <Windows.h>
#include <vector>
#include <string>

#define DLLEXPORT __declspec(dllexport)

class DLLEXPORT Injector {

public:
	explicit Injector() noexcept;

};