#include <windows.h>
#include <amem.h>

HANDLE used_memory_mutex = CreateMutex(NULL, FALSE, NULL);
