#include <stdio.h>

#ifdef _WIN32
#include <windows.h>

int main(void)
{
    STARTUPINFO si;   //Contains startup configuration for the new process
    PROCESS_INFORMATION pi;  // Receives informations

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si); // Contains the size of the structure
    ZeroMemory(&pi, sizeof(pi));

    printf("Parent Process Started\n");

    // Start child process
    if (!CreateProcess(
        NULL,   //use the command line
        "mspaint.exe",
        NULL,   //Security attributes
        NULL,
        FALSE,  //inherit handles
        0,      //default creation flags
        NULL,   //use the parent's environment
        NULL,   //use the parent's current directory
        &si,
        &pi))
    {
        printf("CreateProcess Failed\n");
        return 1;
    }

    printf("Child Process Created Successfully\n");

    // Wait for Child Process
    WaitForSingleObject(pi.hProcess, INFINITE); 
    //Infinite = wait without timeout 

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
