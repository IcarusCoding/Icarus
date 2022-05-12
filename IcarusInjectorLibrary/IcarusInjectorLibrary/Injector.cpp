#include "Injector.h"

#include <iostream>

using std::cout;

Injector::Injector(PInjectionContext ctx) noexcept : ctx(ctx) {
	cout << "NICO STINKT NACH MAGGI\n";
}

ReflectiveInjector::ReflectiveInjector(PInjectionContext ctx) : Injector(ctx) {}

DWORD ReflectiveInjector::Inject() noexcept {
	return 42;
}

Injector* CreateInjector(PInjectionContext ctx) {
	return new ReflectiveInjector(ctx);
}
