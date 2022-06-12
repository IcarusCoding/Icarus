#pragma once

#ifndef ICARUS_ERROR_DEF
typedef unsigned long ICARUS_ERROR_CODE;
#endif

#define ICARUS_SUCCESS 0x00;
#define ICARUS_PROCESS_OPEN_FAILED 0x01;
#define ICARUS_FILE_NOT_FOUND 0x02;
#define ICARUS_FILE_READ_FAILED 0x03;
#define ICARUS_FILE_INVALID_DLL 0x04;
#define ICARUS_ARCH_INVALID_PLATFORM 0x05;
#define ICARUS_ARGUMENT_INVALID 0x06;
#define ICARUS_LOCAL_MEMORY_ALLOCATION_FAILED 0x07;
#define ICARUS_EXT_MEMORY_ALLOCATION_FAILED 0x08;
#define ICARUS_EXT_MEMORY_WRITE_FAILED 0x09;
#define ICARUS_EXT_THREAD_CREATION_FAILED 0x0A;
