#include "shell.h"
#include <string.h>

// Function prototypes
void handle_command(info_t *info, char *command);
void execute_command(info_t *info);
void execute_commands_separated(info_t *info, char *commands);
int execute_logical_operator(info_t *info, char *command);
void handle_alias_command(info_t *info, char *command);
void print_aliases(info_t *info);
void print_alias(info_t *info, char *alias);
void define_alias(info_t *info, char *alias, char *value);

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

        // Execute the command(s)
        execute_commands_separated(&info, command);

        // Free the command memory
        free(command);
    }

    return 0;
}

// Handle command line with commands separated by ";"
void execute_commands_separated(info_t *info, char *commands)
{
    char *command = strtok(commands, ";");

    while (command != NULL) {
        if (execute_logical_operator(info, command) == 0) {
            // If logical operator returns 0, break the execution chain
            break;
        }
        command = strtok(NULL, ";");
    }
}

// Execute command with logical operator
int execute_logical_operator(info_t *info, char *command)
{
    char *logical_operator = NULL;
    char *next_command = NULL;

    // Check for logical operator "&&"
    logical_operator = strstr(command, "&&");
    if (logical_operator != NULL) {
        *logical_operator = '\0';
        next_command = logical_operator + 2;
        trim(command);
        trim(next_command);

        handle_command(info, command);
        if (info->last_status == EXIT_SUCCESS) {
            handle_command(info, next_command);
        }

        return info->last_status;
    }

    // Check for logical operator "||"
    logical_operator = strstr(command, "||");
    if (logical_operator != NULL) {
        *logical_operator = '\0';
        next_command = logical_operator + 2;
        trim(command);
        trim(next_command);

        handle_command(info, command);
        if (info->last_status != EXIT_SUCCESS) {
            handle_command(info, next_command);
        }

        return info->last_status;
    }

    // No logical operator found, execute single command
    handle_command(info, command);
    return info->last_status;
}

// Handle single command
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
        info->last_status = EXIT_SUCCESS;
        return;
    }

    // Check if the command is "cd"
    if (strncmp(command, "cd", 2) == 0) {
        char *arg = strtok(command, " ");
        arg = strtok(NULL, " ");
        char *path = arg;
        if (path == NULL || strcmp(path, "-") == 0) {
            path = getenv("HOME");
        }
        if (chdir(path) == -1) {
            perror("chdir");
            info->last_status = EXIT_FAILURE;
        } else {
            char cwd[PATH_MAX];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                setenv("PWD", cwd, 1);
            } else {
                perror("getcwd");
                info->last_status = EXIT_FAILURE;
            }
        }
        return;
    }

    // Check if the command is "alias"
    if (strncmp(command, "alias", 5) == 0) {
        handle_alias_command(info, command);
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
        info->last_status = WIFEXITED(status) ? WEXITSTATUS(status) : EXIT_FAILURE;
    }
}

// Handle the alias command
void handle_alias_command(info_t *info, char *command)
{
    char *arg = strtok(command, " ");
    arg = strtok(NULL, " ");

    if (arg == NULL) {
        // Print all aliases
        print_aliases(info);
    } else {
        // Print or define specific aliases
        while (arg != NULL) {
            if (strchr(arg, '=') != NULL) {
                // Define alias with value
                char *alias = strtok(arg, "=");
                char *value = strtok(NULL, "=");
                define_alias(info, alias, value);
            } else {
                // Print alias
                print_alias(info, arg);
            }
            arg = strtok(NULL, " ");
        }
    }
}

// Print all aliases
void print_aliases(info_t *info)
{
    for (int i = 0; i < info->alias_count; i++) {
        printf("%s='%s'\n", info->aliases[i].name, info->aliases[i].value);
    }
    info->last_status = EXIT_SUCCESS;
}

// Print alias
void print_alias(info_t *info, char *alias)
{
    for (int i = 0; i < info->alias_count; i++) {
        if (strcmp(info->aliases[i].name, alias) == 0) {
            printf("%s='%s'\n", info->aliases[i].name, info->aliases[i].value);
            info->last_status = EXIT_SUCCESS;
            return;
        }
    }
    info->last_status = EXIT_FAILURE;
}

// Define alias
void define_alias(info_t *info, char *alias, char *value)
{
    // Check if the alias already exists
    for (int i = 0; i < info->alias_count; i++) {
        if (strcmp(info->aliases[i].name, alias) == 0) {
            // Replace the existing alias value
            free(info->aliases[i].value);
            info->aliases[i].value = strdup(value);
            info->last_status = EXIT_SUCCESS;
            return;
        }
    }

    // Create a new alias
    if (info->alias_count < MAX_ALIASES) {
        info->aliases[info->alias_count].name = strdup(alias);
        info->aliases[info->alias_count].value = strdup(value);
        info->alias_count++;
        info->last_status = EXIT_SUCCESS;
    } else {
        // Maximum number of aliases reached
        fprintf(stderr, "Maximum number of aliases reached\n");
        info->last_status = EXIT_FAILURE;
    }
}

