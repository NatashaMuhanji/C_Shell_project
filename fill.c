#include "shell.h"


/**
 * fill_args - Fill an array
 * @entry: String
 * @arguments: Array
 * Return: Length of array
 */

int fill_args(char *entry, char **arguments)
{
	int i = 0;
	char *options, *aux = entry, *command;

	/*This code fills an array with every input typed by the user*/

	command = strtok(entry, "\n\t\r ");
	arguments[i] = command;
	while (aux != NULL)
	{
		++i;
		options = strtok(NULL, "\n\t\r ");
		aux = options;
		arguments[i] = options;
	}
	arguments[i] = NULL;

	return (i);
}
