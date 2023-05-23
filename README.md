# Shell Project

This project is a simple shell implementation written in C. It provides a basic command-line interface where users can execute commands and navigate the system.

## Features

- Command execution: The shell can execute external commands.
- Built-in commands: The shell supports built-in commands such as `exit`, `env`, `setenv`, and `unsetenv`.
- Path resolution: The shell can search for executable files in the system's PATH directories.
- Signal handling: The shell handles the SIGINT signal (Ctrl+C) by printing a prompt instead of terminating.

## Getting Started

To compile the shell, run the following command:

```shell
gcc -Wall -Werror -Wextra -pedantic *.c -o shell


To run the shell, execute the compiled binary:

```shell
./shell
