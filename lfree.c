#include "shell.h"


/**
 * last_free - Free the memory
 * @entry: Input typed by the user
 */
void last_free(char *entry)

	/*This code frees the memory*/
{
	if (isatty(STDIN_FILENO))
	{
		_putchar('\n');
		free(entry);
	}
	if (!isatty(STDIN_FILENO))
		free(entry);
}
