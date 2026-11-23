#include <stdio.h>

#ifdef _WIN32
#include <windows.h>

#define BUFFER_SIZE 25

/*
 * Figure 3.23 & 3.24 (Operating System Concepts, 10th Ed.)
 * Windows anonymous pipe demonstration: Parent process side.
 */
int main(void)
{
    HANDLE hRead, hWrite;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    SECURITY_ATTRIBUTES sa;
    char message[BUFFER_SIZE] = "Greetings";
    DWORD bytesWritten;

    // Set up security attributes so pipe handles are inherited
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    // Create anonymous pipe
    if (!CreatePipe(&hRead, &hWrite, &sa, 0)) {
        fprintf(stderr, "CreatePipe failed.\n");
        return 1;
    }

    // Set up STARTUPINFO structure for child process
    GetStartupInfo(&si);
    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    si.hStdInput = hRead;
    si.dwFlags |= STARTF_USESTDHANDLES;

    // Prevent child from inheriting the write end
    SetHandleInformation(hWrite, HANDLE_FLAG_INHERIT, 0);

    // Create child process
    if (!CreateProcess(NULL,
                       "16_windows_pipe_child.exe",
                       NULL,
                       NULL,
                       TRUE, // inherit handles
                       0,
                       NULL,
                       NULL,
                       &si,
                       &pi))
    {
        fprintf(stderr, "CreateProcess failed.\n");
        return 1;
    }

    // Close the read handle so child sees EOF when parent finishes writing
    CloseHandle(hRead);

    // Write to anonymous pipe
    if (!WriteFile(hWrite, message, BUFFER_SIZE, &bytesWritten, NULL)) {
        fprintf(stderr, "Error writing to pipe.\n");
    }

    // Close the write handle
    CloseHandle(hWrite);

    // Wait for child process to complete
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Close process and thread handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
#else
int main(void)
{
    printf("================ Windows Anonymous Pipe (Parent) ================\n");
    printf("This program demonstrates the Windows CreatePipe() API (Figure 3.23 & 3.24).\n");
    printf("You are running on Linux (POSIX). On Linux, ordinary pipes are created\n");
    printf("using pipe(), fork(), and read()/write() (see 14_unix_pipe.c).\n");
    printf("To run this code natively, compile on Windows with MSVC or MinGW.\n");
    printf("==================================================================\n");
    return 0;
}
#endif
