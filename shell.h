#include "shell.h"

/*
 * int line_number = 1, status = 0;
 * char *sname = NULL;
 * main - main is used to execute commands
 * @ac: these are the number of inputs
 * array of input
 * Return: 0, or anothe rnumber
{
	int read_bytes, separated = FALSE, a;
	size_t buffer_size = 1;
	char *buffer = NULL, *buffer_ptr, *buffer_tmp;
	char **arguments = NULL;

	sname = strdup(*av);

	environ = array_cpy(environ, list_len(environ, NULL));

	signal(SIGINT, SIG_IGN);

	buffer = malloc(1);
	if (buffer == NULL)
		exit(EXIT_FAILURE);

	while (1)
	{
		if (separated == FALSE)
		{
			if (isatty(STDIN_FILENO) == 1)
				write(STDOUT_FILENO, "my_shell$ ", 10);

			read_bytes = getline(&buffer, &buffer_size, stdin);

			if (read_bytes == -1)
				break;
			if (read_bytes == 1)
			{
				line_number++;
				continue;
			}
			buffer[read_bytes - 1] = '\0';
			buffer = input_san(buffer, &buffer_size);
			if (buffer_size == 0)
			{
				line_number++;
				continue;
			}
			buffer_ptr = buffer;
		}
		else
			buffer_ptr = buffer_tmp;

		buffer_tmp = NULL;
		arguments = make_array(buffer_ptr, ' ', &buffer_tmp);
		if (buffer_tmp != NULL)
			separated = TRUE;
		else
			separated = FALSE;

		a = command_manager(arguments);

		free(arguments);

		if (separated == FALSE)
			line_number++;

		if (a == EXIT_SHELL)
			break;
	}
	free(buffer);
	alias_func(NULL, TRUE);
	free_array(environ);
	free(sname);

	return (status % 256);
}
#include "shell.h"
*/
/**
 * main - This is the shell.c file for our project
 * Return:Return 0 if success
 */
int main(void)
{
	ssize_t bytes_rd = 0;
	size_t bf_size = 0;
	char *entry = NULL, *arguments[20];
	int counter = 1, vf_stat = 0, exist_stat = 0, exit_stat = 0, blt_stat = 0;

	_printp("$ ", 2);
	bytes_rd = getline(&entry, &bf_size, stdin);
	while (bytes_rd != -1)
	{
		if (*entry != '\n')
		{
			fill_args(entry, arguments);
			if (arguments[0] != NULL)
			{
				exist_stat = exist(arguments[0]);
				if (exist_stat != 0)
				{
					vf_stat = verify_path(arguments);
					if (vf_stat == 0)
						exit_stat = exec(arguments), free(entry), free(*arguments);
					else
					{
					blt_stat = verify_blt(arguments, exit_stat);
					if (blt_stat != 0)
						exit_stat = print_not_found(arguments, counter), free(entry);
					}
				}
				else
					exit_stat = exec(arguments), free(entry);
			}
			else
				free(entry);
		}
		else if (*entry == '\n')
			free(entry);
		entry = NULL, counter++;
		_printp("$ ", 2), bytes_rd = getline(&entry, &bf_size, stdin);
	}
	last_free(entry);
	return (exit_stat);
}
