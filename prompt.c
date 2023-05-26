#include "shell.h"


/**
 * _printp - Prints the prompt
 * @prompt: Pointer
 * @size: Length
 * Return: 0 or 1
 */


int _printp(const char *prompt, unsigned int size)
{
	int written;

	/*This code prints the prompt in the standard input*/

	if (isatty(STDIN_FILENO))
	{
		written = write(1, prompt, size);
		if (written == -1)
			return (-1);
	}
		return (0);
}
