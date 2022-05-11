#include <nan.h>
#include <iostream>
#include <Windows.h>
#include "Injector.h"

using namespace std;
using namespace Nan;
using namespace v8;

NAN_METHOD(PassNumber) {
    HINSTANCE hInst = LoadLibraryA("IcarusInjectorLibrary.dll");
    std::cout << hInst << "\n";
    Injector *inj = new Injector();
    std::cout << (inj == nullptr) << "\n";
    Nan::Maybe<double> value = Nan::To<double>(info[0]);
    Local<Number> retval = Nan::New(value.FromJust() + 100);
    info.GetReturnValue().Set(retval);
}

NAN_MODULE_INIT(Init) {
    Nan::Set(target, New<String>("test").ToLocalChecked(),
        GetFunction(New<FunctionTemplate>(PassNumber)).ToLocalChecked());
}

NODE_MODULE(icarus_injector, Init)
