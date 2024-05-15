#include "Process.hpp"

#ifdef _WIN32
#include <windows.h>
int Process::getPid() {
    return GetCurrentProcessId();
}
#else
#include <unistd.h>
int Process::getPid() {
    return getpid();
}
#endif