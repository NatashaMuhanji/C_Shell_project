#include "shell.h"

/*
 * int line_number = 1, status = 0;
 * char *sname = NULL;
 * main - main is used to execute commands
 * @ac: these are the number of inputs
 * array of input
 * Return: 0, or anothe rnumber
{
<<<<<<< HEAD
	int read_bytes, separated = FALSE, a;
	size_t buffer_size = 1;
	char *buffer = NULL, *buffer_ptr, *buffer_tmp;
	char **arguments = NULL;
=======
	if (sig_num == SIGINT)
	{
		_puts("\n#NatashaAnncarl$ ");
	}
}
>>>>>>> 45b61f670ea4a90594807eb394a7b0fe2d9f1eb1

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

<<<<<<< HEAD
	return (status % 256);
=======
void _isatty(void)
{
	if (isatty(STDIN_FILENO))
		_puts("#NatashaAnncarl$ ");
>>>>>>> 45b61f670ea4a90594807eb394a7b0fe2d9f1eb1
}
#include "shell.h"
*/
/**
 * main - This is the shell.c file for our project
 * Return:Return 0 if success
 */
int main(void)
{
<<<<<<< HEAD
	ssize_t bytes_rd = 0;
	size_t bf_size = 0;
	char *entry = NULL, *arguments[20];
	int counter = 1, vf_stat = 0, exist_stat = 0, exit_stat = 0, blt_stat = 0;
=======
	ssize_t len = 0;
	char *buff = NULL, *value, *pathname, **arv;
	size_t size = 0;
	_listpath *head = '\0';
	void (*f)(char **);
>>>>>>> 45b61f670ea4a90594807eb394a7b0fe2d9f1eb1

	_printp("$ ", 2);
	bytes_rd = getline(&entry, &bf_size, stdin);
	while (bytes_rd != -1)
	{
<<<<<<< HEAD
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
=======
		_isatty();
		len = getline(&buff, &size, stdin);
		_EOF(len, buff);
		arv = splitstring(buff, " \n");
		if (!arv || !arv[0])
			_execute_(arv);
		else
		{
			value = _getenv("PATH");
			head = linkpath(value);
			pathname = _which(arv[0], head);
			f = checkbuild(arv);
			if (f)
			{
				free(buff);
				f(arv);
			}
			else if (!pathname)
				_execute_(arv);
			else if (pathname)
			{
				free(arv[0]);
				arv[0] = pathname;
				_execute_(arv);
>>>>>>> 45b61f670ea4a90594807eb394a7b0fe2d9f1eb1
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

