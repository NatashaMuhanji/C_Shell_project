i#include "shell.h"

/**
 * _putchar - writes charcter c to stdout
 * @c: The character to print
 * Return: on success 1, -1 on error
 */

int _putchar(char c)
{
	return (write(1, &c, 1));
}
