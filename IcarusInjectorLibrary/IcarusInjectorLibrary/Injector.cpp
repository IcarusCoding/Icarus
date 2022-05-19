#include "Injector.h"

#include <iostream>

using std::cout;

Injector::Injector(PInjectionContext ctx) noexcept : ctx(ctx) {

}

ReflectiveInjector::ReflectiveInjector(PInjectionContext ctx) : Injector(ctx) {}

DWORD ReflectiveInjector::Inject() noexcept {
	return 42;
}

Injector* CreateInjector(PInjectionContext ctx) {
	DWORD settings = ctx->settings;
	if (settings & LOADLIBRARY) {
		cout << "Not supported\n";
	}	
	if (settings & REFLECTIVE) {
		return new ReflectiveInjector(ctx);
	}
	if (settings & NATIVE) {
		cout << "Not supported\n";
	}
	return nullptr;
}
