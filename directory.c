#include "shell.h"
#include <unistd.h>
#include <limits.h>

// Function prototypes
void handle_command(info_t *info, char *command);
void execute_command(info_t *info);
void handle_setenv(info_t *info, char *command);
void handle_unsetenv(info_t *info, char *command);
void handle_cd(info_t *info, char *command);

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

    // Check if the command is "setenv"
    if (strncmp(command, "setenv", 6) == 0) {
        handle_setenv(info, command);
        return;
    }

    // Check if the command is "unsetenv"
    if (strncmp(command, "unsetenv", 8) == 0) {
        handle_unsetenv(info, command);
        return;
    }

    // Check if the command is "cd"
    if (strncmp(command, "cd", 2) == 0) {
        handle_cd(info, command);
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

// Handle the "setenv" command
void handle_setenv(info_t *info, char *command)
{
    // Tokenize the command to get variable and value
    strtok(command, " "); // Discard "setenv"
    char *variable = strtok(NULL, " ");
    char *value = strtok(NULL, " ");

    // Check if variable and value are provided
    if (variable == NULL || value == NULL) {
        fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
        return;
    }

    // Set the environment variable
    if (setenv(variable, value, 1) == -1) {
        perror("setenv");
        return;
    }
}

// Handle the "unsetenv" command
void handle_unsetenv(info_t *info, char *command)
{
    // Tokenize the command to get variable
    strtok(command, " "); // Discard "unsetenv"
    char *variable = strtok(NULL, " ");

    // Check if variable is provided
    if (variable == NULL) {
        fprintf(stderr, "Usage: unsetenv VARIABLE\n");
        return;
    }

    // Unset the environment variable
    if (unsetenv(variable) == -1) {
        perror("unsetenv");
        return;
    }
}

// Handle the "cd" command
void handle_cd(info_t *info, char *command)
{
    // Tokenize the command to get the directory
    strtok(command, " "); // Discard "cd"
    char *directory = strtok(NULL, " ");

    // Check if directory is provided, otherwise use $HOME
    if (directory == NULL) {
        directory = getenv("HOME");
        if (directory == NULL) {
            fprintf(stderr, "cd: $HOME not set\n");
            return;
        }
    }

    // Handle "cd -" to go back to the previous directory
    if (strcmp(directory, "-") == 0) {
        directory = getenv("OLDPWD");
        if (directory == NULL) {
            fprintf(stderr, "cd: OLDPWD not set\n");
            return;
        }
        printf("%s\n", directory);
    }

    // Change the directory
    if (chdir(directory) == -1) {
        perror("chdir");
        return;
    }

    // Update the PWD environment variable
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
        return;
    }
    if (setenv("PWD", cwd, 1) == -1) {
        perror("setenv");
        return;
    }
}

