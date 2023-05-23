#include "shell.h"

/**
 * _getenv - gets the value of the global variable
 * @name: name of the global variable
 * Return: string of value
 */
char *_getenv(const char *name)
{
	int i, j;
	char *value;

	if (!name)
		return (NULL);
	for (i = 0; environ[i]; i++)
	{
		j = 0;
		if (name[j] == environ[i][j])
		{
			while (name[j])
			{
				if (name[j] != environ[i][j])
					break;

				j++;
			}
			if (name[j] == '\0')
			{
				value = (environ[i] + j + 1);
				return (value);
			}
		}
	}
	return (NULL);
}

/**
 * _which - finds the pathname of a filename
 * @filename: name of file or command
 * Return: pathname of filename or NULL if no match
 */
char *_which(char *filename)
{
	struct stat st;
	char *string;
	char *path = _getenv("PATH");
	char *token;

	if (!path)
		return (NULL);

	token = strtok(path, ":");
	while (token)
	{
		string = concat_all(token, "/", filename);
		if (stat(string, &st) == 0)
			return (string);
		free(string);
		token = strtok(NULL, ":");
	}

	return (NULL);
}

/**
 * _strlen - returns the length of a string
 * @s: pointer to the string
 * Return: the length of the string
 */
int _strlen(char *s)
{
	int len = 0;

	while (s && s[len])
		len++;

	return (len);
}

/**
 * _strdup - returns a pointer to a newly allocated space in memory, which
 * contains a copy of the string given as a parameter
 * @str: pointer to a string
 * Return: pointer to a string
 */
char *_strdup(char *str)
{
	char *new;
	int i, len;

	if (!str)
		return (NULL);

	len = _strlen(str);
	new = malloc(sizeof(char) * (len + 1));
	if (!new)
		return (NULL);

	for (i = 0; i <= len; i++)
		new[i] = str[i];

	return (new);
}

/**
 * concat_all - concatenates three strings into a newly allocated memory
 * @name: first string
 * @sep: second string
 * @value: third string
 * Return: pointer to the new string
 */
char *concat_all(char *name, char *sep, char *value)
{
	char *result;
	int len1, len2, len3, i, j;

	len1 = _strlen(name);
	len2 = _strlen(sep);
	len3 = _strlen(value);

	result = malloc(sizeof(char) * (len1 + len2 + len3 + 1));
	if (!result)
		return (NULL);

	for (i = 0; i < len1; i++)
		result[i] = name[i];

	for (j = 0; j < len2; j++)
		result[i + j] = sep[j];

	for (j = 0; j <= len3; j++)
		result[i + j + len2] = value[j];

	return (result);
}

