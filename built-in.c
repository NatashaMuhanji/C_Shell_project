#include "shell.h"

int main(void)
{
    info_t info = INFO_INIT;
    char *command;

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

        // Execute the command using fork and exec
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            continue;
        } else if (pid == 0) {
            // Child process
            if (execlp(command, command, NULL) == -1) {
                perror("execlp");
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

