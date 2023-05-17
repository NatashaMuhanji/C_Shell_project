#include "shell.h"

// Function prototypes
void handle_command(info_t *info, char *command);
void execute_command(info_t *info);
void set_environment_variable(info_t *info, char *command);
void unset_environment_variable(info_t *info, char *command);
void change_directory(info_t *info, char *command);

int main(int argc, char *argv[])
{
    info_t info = INFO_INIT;
    char *command;

    if (argc > 2) {
        printf("Usage: %s [filename]\n", argv[0]);
        return 1;
    }

    if (argc == 2) {
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
    } else {
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

            // Skip empty lines and comments
            if (strlen(command) == 0 || command[0] == '#') {
                free(command);
                continue;
            }

            handle_command(&info, command);

            // Free the command memory
            free(command);
        }
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

    // Check if the command is "cd"
    if (strncmp(command, "cd", 2) == 0) {
        change_directory(info, command);
        return;
    }

    // Check if the command is "setenv"
    if (strncmp(command, "setenv", 6) == 0) {
        set_environment_variable(info, command);
        return;
    }

    // Check if the command is "unsetenv"
    if (strncmp(command, "unsetenv", 8) == 0) {
        unset_environment_variable(info, command);
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

// Set environment variable
void set_environment_variable(info_t *info, char *command)
{
    // Extract variable name and value
    char *variable = strtok(command, " \t");
    char *value = strtok(NULL, " \t");

    if (variable != NULL && value != NULL) {
        if (setenv(variable, value, 1) == -1) {
            perror("setenv");
        }
    } else {
        fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
    }
}

// Unset environment variable
void unset_environment_variable(info_t *info, char *command)
{
    // Extract variable name
    char *variable = strtok(command, " \t");

    if (variable != NULL) {
        if (unsetenv(variable) == -1) {
            perror("unsetenv");
        }
    } else {
        fprintf(stderr, "Usage: unsetenv VARIABLE\n");
    }
}

// Change current directory
void change_directory(info_t *info, char *command)
{
    char *dir = strtok(command, " \t");

    // If no argument is given, change to the home directory
    if (dir == NULL || strcmp(dir, "cd") == 0) {
        dir = getenv("HOME");
    } else if (strcmp(dir, "cd") == 0) {
        // If the argument is "-", change to the previous directory
        dir = getenv("OLDPWD");
        printf("%s\n", dir);
    } else {
        // Check if the directory is valid
        if (chdir(dir) == -1) {
            perror("chdir");
            return;
        }
    }

    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        // Update the PWD and OLDPWD environment variables
        setenv("PWD", cwd, 1);
        setenv("OLDPWD", info->pwd, 1);
    } else {
        perror("getcwd");
    }
}

