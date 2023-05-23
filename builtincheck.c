#include "shell.h"


/**
 * check_build - checks if the command is a built-in
 * @arv: array of arguments
 * Return: pointer to function that takes arv and returns void
 */

void (*check_build(char **arv))(char **arv)
{
	int i, j;
	mybuild T[] = {
		{"exit", exit_shell},
		{"env", print_env},
		{"setenv", set_env},
		{"unsetenv", unset_env},
		{NULL, NULL}
	};

	for (i = 0; T[i].name; i++)
	{
		j = 0;
		if (T[i].name[j] == arv[0][j])
		{
			for (j = 0; arv[0][j]; j++)
			{
				if (T[i].name[j] != arv[0][j])
					break;
			}
			if (!arv[0][j])
				return (T[i].func);
		}
	}
	return (NULL);
}

