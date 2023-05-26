#include "shell.h"

/**
 * exist - This checks if a file exists
 * @pathname: path
 * Return: retuns 0 if file exists
 */
int exist(char *pathname)
{
	int exist_stat;

	exist_stat = (open(pathname, O_RDONLY));

	/*if statement to check if file exists*/
	if (exist_stat != -1)
	{
		close(exist_stat);
		return (0);
	}
	else
		return (-1);
}
