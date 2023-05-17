#include "shell.h"

#define MAX_ARGS 10

int main(void)
{
    info_t info = INFO_INIT;
    char *command;
    char *args[MAX_ARGS + 2]; // +2 to account for command and NULL terminator

    while (1) {
        printf("$ ");  // Display the prompt
        command = NULL;

        // Read the command line
        ssize_t read = getline(&command, &(size_t){0}, stdin);
        if (read == -1) {
            if (feof(stdin)) {
                printf("\n");  // Print a newline for Ctrl+D (EOF)
                break;  // Exit the shell if end of file is reached
            }
            perror("getline");
            continue;
        }

        // Remove the newline character from the command
        if (command[read - 1] == '\n')
            command[read - 1] = '\0';

        // Parse the command and arguments
        int argc = 0;
        char *token = command;
        while (*token != '\0' && argc < MAX_ARGS) {
            while (*token == ' ') // Skip leading spaces
                token++;

            if (*token == '\0') // End of command
                break;

            args[argc++] = token;

            while (*token != '\0' && *token != ' ') // Find argument boundary
                token++;

            if (*token != '\0') {
                *token = '\0'; // Null-terminate the argument
                token++;
            }
        }

        args[argc] = NULL; // Null-terminate the argument list

        // Execute the command using fork and exec
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            continue;
        } else if (pid == 0) {
            // Child process
            if (execvp(command, args) == -1) {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        } else {
            // Parent process
            int status;
            if (wait(&status) == -1) {
                perror("wait");
                continue;
            }
        }

        // Free the command memory
        free(command);
    }

    return 0;
}

