#include <stdio.h>

#ifdef _WIN32
#include <windows.h>

#define BUFFER_SIZE 25

/*
 * Figure 3.25 (Operating System Concepts, 10th Ed.)
 * Windows anonymous pipe demonstration: Child process side.
 */
int main(void)
{
    HANDLE hStdIn;
    char buffer[BUFFER_SIZE];
    DWORD bytesRead;

    // Get the standard input handle (redirected by parent to pipe read end)
    hStdIn = GetStdHandle(STD_INPUT_HANDLE);

    // Read from the anonymous pipe
    if (ReadFile(hStdIn, buffer, BUFFER_SIZE, &bytesRead, NULL)) {
        printf("Child Process: Read from pipe: %s\n", buffer);
    }
    else {
        fprintf(stderr, "Child Process: ReadFile failed.\n");
        return 1;
    }

    return 0;
}
#else
int main(void)
{
    printf("================ Windows Anonymous Pipe (Child) ================\n");
    printf("This program demonstrates the Windows anonymous pipe child (Figure 3.25).\n");
    printf("It reads from GetStdHandle(STD_INPUT_HANDLE), redirected from the parent's pipe.\n");
    printf("On Linux (POSIX), pipes are read using read(fd[0], ...) (see 14_unix_pipe.c).\n");
    printf("=================================================================\n");
    return 0;
}
#endif
