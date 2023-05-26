#include "shell.h"


/**
 *free_grid - Free the allocated memory
 *@grid: pointer
 *@height: Height of the grid
 */

void free_grid(char **grid, int height)
{
	int i = 0;

	/*This code frees the allocated memory*/


	if (grid == NULL)
	{
		return;
	}

	while (i < height)
	{
		free(grid[i]);
		i++;
	}
}
