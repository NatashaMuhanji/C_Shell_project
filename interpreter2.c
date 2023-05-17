#include "shell.h"

// Function prototypes
void handle_command(info_t *info, char *command);
void execute_command(info_t *info);

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

        handle_command(&info, command);

        // Free the command memory
        free(command);
    }

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

