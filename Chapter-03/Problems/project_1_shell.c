#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 * Programming Project 1: UNIX Shell and History Feature (Figure 3.36 Outline)
 * (Operating System Concepts, 10th Ed., pp. 204–207)
 *
 * Implemented Features:
 * 1. Command execution via fork() and execvp().
 * 2. Background process execution with trailing '&'.
 * 3. History feature: "!!" executes the most recent command.
 * 4. Input redirection with '<' (e.g., sort < input.txt).
 * 5. Output redirection with '>' (e.g., ls > output.txt).
 * 6. IPC Pipe connecting two commands with '|' (e.g., ls -l | grep txt).
 * 7. Built-in "exit" command.
 */

#define MAX_LINE 80       // 80 chars per line, per command
#define MAX_ARGS (MAX_LINE / 2 + 1)

static char last_command[MAX_LINE] = "";
static int has_history = 0;

// Parse line into arguments array, return token count
int parse_command(char *line, char *args[])
{
    int count = 0;
    char *token = strtok(line, " \t\r\n");

    while (token != NULL && count < MAX_ARGS - 1) {
        args[count++] = token;
        token = strtok(NULL, " \t\r\n");
    }
    args[count] = NULL;
    return count;
}

// Execute command with redirection (< and >)
void execute_single_command(char *args[], int in_background)
{
    // Check for redirection operators
    char *input_file = NULL;
    char *output_file = NULL;
    char *clean_args[MAX_ARGS];
    int clean_count = 0;

    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "<") == 0) {
            if (args[i + 1] != NULL) {
                input_file = args[i + 1];
                i++; // Skip filename
            }
        } else if (strcmp(args[i], ">") == 0) {
            if (args[i + 1] != NULL) {
                output_file = args[i + 1];
                i++; // Skip filename
            }
        } else {
            clean_args[clean_count++] = args[i];
        }
    }
    clean_args[clean_count] = NULL;

    if (clean_count == 0) return;

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return;
    }

    if (pid == 0) {
        // Child process
        if (input_file) {
            int in_fd = open(input_file, O_RDONLY);
            if (in_fd < 0) {
                perror("Cannot open input file");
                exit(1);
            }
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);
        }

        if (output_file) {
            int out_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (out_fd < 0) {
                perror("Cannot open output file");
                exit(1);
            }
            dup2(out_fd, STDOUT_FILENO);
            close(out_fd);
        }

        execvp(clean_args[0], clean_args);
        perror("execvp failed");
        exit(1);
    }
    else {
        // Parent process
        if (!in_background) {
            waitpid(pid, NULL, 0);
        } else {
            printf("[Background process started: PID %d]\n", pid);
        }
    }
}

// Execute two commands connected via a pipe (cmd1 | cmd2)
void execute_piped_command(char *args[], int pipe_pos, int in_background)
{
    char *cmd1_args[MAX_ARGS];
    char *cmd2_args[MAX_ARGS];

    for (int i = 0; i < pipe_pos; i++) {
        cmd1_args[i] = args[i];
    }
    cmd1_args[pipe_pos] = NULL;

    int idx = 0;
    for (int i = pipe_pos + 1; args[i] != NULL; i++) {
        cmd2_args[idx++] = args[i];
    }
    cmd2_args[idx] = NULL;

    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe failed");
        return;
    }

    pid_t pid1 = fork();
    if (pid1 == 0) {
        // Child 1 (cmd1): stdout redirected to pipe write end
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);

        execvp(cmd1_args[0], cmd1_args);
        perror("pipe command 1 failed");
        exit(1);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        // Child 2 (cmd2): stdin redirected to pipe read end
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);

        execvp(cmd2_args[0], cmd2_args);
        perror("pipe command 2 failed");
        exit(1);
    }

    // Parent closes both pipe ends
    close(fd[0]);
    close(fd[1]);

    if (!in_background) {
        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);
    }
}

int main(void)
{
    char line[MAX_LINE];
    char line_copy[MAX_LINE];
    char *args[MAX_ARGS];
    int should_run = 1;

    printf("====================================================\n");
    printf("   OS Shell (osh) - Chapter 3 Programming Project   \n");
    printf("   Supports: commands, '&', '!!', '<', '>', '|'     \n");
    printf("   Type 'exit' to quit.                             \n");
    printf("====================================================\n");

    while (should_run) {
        printf("osh> ");
        fflush(stdout);

        if (!fgets(line, MAX_LINE, stdin)) {
            break;
        }

        // Remove trailing newline
        line[strcspn(line, "\r\n")] = '\0';

        // Ignore empty lines
        if (strlen(line) == 0) {
            continue;
        }

        // Handle history command "!!"
        if (strcmp(line, "!!") == 0) {
            if (!has_history) {
                printf("No commands in history.\n");
                continue;
            }
            // Recall last command
            strcpy(line, last_command);
            printf("%s\n", line);
        } else {
            // Save to history
            strcpy(last_command, line);
            has_history = 1;
        }

        strcpy(line_copy, line);
        int arg_count = parse_command(line_copy, args);
        if (arg_count == 0) continue;

        // Built-in exit
        if (strcmp(args[0], "exit") == 0) {
            should_run = 0;
            break;
        }

        // Check if command should run in background
        int in_background = 0;
        if (strcmp(args[arg_count - 1], "&") == 0) {
            in_background = 1;
            args[arg_count - 1] = NULL;
            arg_count--;
        }

        // Check if there is a pipe '|'
        int pipe_pos = -1;
        for (int i = 0; i < arg_count; i++) {
            if (strcmp(args[i], "|") == 0) {
                pipe_pos = i;
                break;
            }
        }

        if (pipe_pos != -1) {
            execute_piped_command(args, pipe_pos, in_background);
        } else {
            execute_single_command(args, in_background);
        }
    }

    return 0;
}
