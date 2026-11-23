#include <stdio.h>

#ifdef _WIN32
#include <windows.h>

int main(void)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    printf("Parent Process Started\n");

    // Start child process
    if (!CreateProcess(
        NULL,
        "mspaint.exe",
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi))
    {
        printf("CreateProcess Failed\n");
        return 1;
    }

    printf("Child Process Created Successfully\n");

    // Wait for Child Process
    WaitForSingleObject(pi.hProcess, INFINITE);

    printf("Child Complete\n");

    // Close Handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
#else
int main(void)
{
    printf("================ Windows Process Creation ================\n");
    printf("This program demonstrates the Windows CreateProcess() API.\n");
    printf("You are currently running on Linux (POSIX), where process creation\n");
    printf("is performed using fork() and exec() (see 02_linux_process_creation.c).\n");
    printf("To run this Windows binary, compile on Windows using MSVC or MinGW.\n");
    printf("==========================================================\n");
    return 0;
}
#endif
