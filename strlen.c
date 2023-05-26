#include "shell.h"
/**
 * _strlen - Sizes
 * @string: String
 * Return: length
 */

int _strlen(const char *string)
{
	int length = 0;

	/**This code sizes the length of the string*/

	if (string == NULL)
		return (0);

	while (string[length] != '\0')
		length++;
	return (length);
}
