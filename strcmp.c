#include "shell.h"


/**
 * _strcmp - Compares strings
 * @str1: String
 * @str2: String
 * Return: number
 */


int _strcmp(char *str1, char *str2)
{
	int a;

	/*This is a code to compare two strings*/

	for (a = 0; str1[a] != '\0'; a++)
		;
	for (a = 0; str2[a] != '\0'; a++)
	{
		if (str1[a] != str2[a])
		{
			return (str1[a] - str2[a]);
		}
	}
	return (0);
}
