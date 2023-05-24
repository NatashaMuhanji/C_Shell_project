#include "shell.h"

/**
 * _strdup - returns a pointer to a newly allocated space in memory, which
 * contains a copy of the string given as a parameter
 * @str: pointer to a string
 * Return: pointer to a string
 */
char *_strdup(char *str)
{
	int a, l;
	char *new;

	if (!str)
	{
		return (NULL);
	}
	for (b = 0; str[l] != '\0';)
	{
		b++;
	}
	new = malloc(sizeof(char) * b + 1);
	if (!new)
	{
		return (NULL);
	}
	for (a = 0; a < b; a++)
	{
		new[a] = str[a];
	}
	new[b] = str[b];
	return (new);
}

/**
 * concat_all - concats 3 strings in a newly allocated memory
 * @name: first string
 * @sep: second string
 * @value: Third string
 * Return: pointer to the new string
 */
char *concat_all(char *name, char *sep, char *value)
{
	char *result;
	int l1, l2, l3, a, k;

	l1 = _strlen(name);
	l2 = _strlen(sep);
	l3 = _strlen(value);

	result = malloc(l1 + l2 + l3 + 1);
	if (!result)
		return (NULL);

	for (a = 0; name[a]; a++)
		result[a] = name[a];
	k = a;

	for (a = 0; sep[a]; a++)
		result[k + a] = sep[a];
	k = k + a;

	for (a = 0; value[a]; a++)
		result[k + a] = value[a];
	k = k + a;

	result[k] = '\0';

	return (result);
}

/**
 * _strlen - it gives the length of a string
 * @s: pointer to the string
 * Return: the length of string
 */
int _strlen(char *s)
{
	int a = 0;

	while (*(s + a) != '\0')
	{
		a++;
	}
	return (a);
}

/**
 * _putchar - writes the character c to stdout
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int _putchar(char c)
{
	return (write(1, &c, 1));
}

/**
 * _puts - prints a string
 * @str: pointer to string
 */

void _puts(char *str)
{
	int a = 0;

	while (str[a])
	{
		_putchar(str[a]);
		a++;
	}
}
