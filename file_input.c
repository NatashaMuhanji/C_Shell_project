#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

// Function prototypes
void handle_command(info_t *info, char *command);
void execute_command(info_t *info);

int main(int argc, char *argv[])
{
    info_t info = INFO_INIT;
    char *command;

    if (argc != 2) {
        printf("Usage: %s [filename]\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("fopen");
        return 1;
    }

    while (getline(&command, &(size_t){0}, file) != -1) {
        // Remove the newline character from the command
        size_t len = strlen(command);
        if (len > 0 && command[len - 1] == '\n')
            command[len - 1] = '\0';

        // Skip empty lines and comments
        if (strlen(command) == 0 || command[0] == '#')
            continue;

        // Execute the command using fork and exec
        handle_command(&info, command);
    }

    fclose(file);
    free(command);

    return 0;
}

// Handle command line with arguments
void handle_command(info_t *info, char *command)
{
    // Check if the command is "exit"
    if (strcmp(command, "exit") == 0) {
        exit(EXIT_SUCCESS);
    }

    // Check if the command is "env"
    if (strcmp(command, "env") == 0) {
        char **env = info->environ;
        while (*env != NULL) {
            printf("%s\n", *env);
            env++;
        }
        return;
    }

    // Execute the command
    execute_command(info);
}

// Execute the command using fork and exec
void execute_command(info_t *info)
{
    // Fork a new process
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    } else if (pid == 0) {
        // Child process
        char *command = info->argv[0];
        char *path = find_path(info, command, "/");
        if (path != NULL) {
            // Execute the command with arguments
            if (execve(path, info->argv, info->environ) == -1) {
                perror("execve");
                exit(EXIT_FAILURE);
            }
        } else {
            // Command not found
            printf("Command not found: %s\n", command);
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process
        int status;
        if (wait(&status) == -1) {
            perror("wait");
            return;
        }
    }
}

