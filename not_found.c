#include "shell.h"


/**
 * print_not_found - prints error
 * @arguments: array of args
 * @counter: Times
 * Return: Exit status
 */


int print_not_found(char **arguments, int counter)
{
	char *imode_shell_name = "hsh";
	char *non_i_mode_shell_name = "./hsh";

	/*This is a code to write the error message when command is not found*/

	if (isatty(STDIN_FILENO))
		write(2, imode_shell_name, 3);
	else
	{
		write(2, non_i_mode_shell_name, 5);
	}
	write(2, ": ", 2);
	print_numbers(counter);
	write(2, ": ", 2);
	write(2, arguments[0], _strlen(arguments[0]));
	write(2, ": not found\n", 12);
	return (127);
}
