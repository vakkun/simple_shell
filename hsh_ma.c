#include "shell.h"

/**
 * ff_hsh - main shell loop
 * @info: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int ff_hsh(info_t *info, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		ff_clear_info(info);
		if (ff_interactive(info))
			ff_puts("$ ");
		ff_eputchar(BUF_FLUSH);
		r = ff_get_input(info);
		if (r != -1)
		{
			ff_set_info(info, av);
			builtin_ret = ff_find_builtin(info);
			if (builtin_ret == -1)
				ff_find_cmd(info);
		}
		else if (ff_interactive(info))
			ff_putchar('\n');
		ff_free_info(info, 0);
	}
	ff_write_history(info);
	ff_free_info(info, 1);
	if (!ff_interactive(info) && info->status)
		exit(info->status);
	if (builtin_ret == -2)
	{
		if (info->err_num == -1)
			exit(info->status);
		exit(info->err_num);
	}
	return (builtin_ret);
}

/**
 * ff_find_builtin - finds a builtin command
 * @info: the parameter & return info struct
 *
 * Return: -1
 */
int ff_find_builtin(info_t *info)
{
	int i, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", ff_myexit},
		{"env", ff_myenv},
		{"help", ff_myhelp},
		{"history", ff_myhistory},
		{"setenv", ff_mysetenv},
		{"unsetenv", ff_myunsetenv},
		{"cd", ff_mycd},
		{"alias", ff_myalias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (ff_strcmp(info->argv[0], builtintbl[i].type) == 0)
		{
			info->line_count++;
			built_in_ret = builtintbl[i].func(info);
			break;
		}
	return (built_in_ret);
}

/**
 * ff_find_cmd - finds a command in PATH
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void ff_find_cmd(info_t *info)
{
	char *path = NULL;
	int i, k;

	info->path = info->argv[0];
	if (info->linecount_flag == 1)
	{
		info->line_count++;
		info->linecount_flag = 0;
	}
	for (i = 0, k = 0; info->arg[i]; i++)
		if (!ff_is_delim(info->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = ff_find_path(info, ff_getenv(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		ff_fork_cmd(info);
	}
	else
	{
		if ((ff_interactive(info) || ff_getenv(info, "PATH=")
			|| info->argv[0][0] == '/') && ff_is_cmd(info, info->argv[0]))
			ff_fork_cmd(info);
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			ff_print_error(info, "not found\n");
		}
	}
}

/**
 * ff_fork_cmd - forks a an exec thread to run cmd
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void ff_fork_cmd(info_t *info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(info->path, info->argv, ff_get_environ(info)) == -1)
		{
			ff_free_info(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
	}
	else
	{
		wait(&(info->status));
		if (WIFEXITED(info->status))
		{
			info->status = WEXITSTATUS(info->status);
			if (info->status == 126)
				ff_print_error(info, "Permission denied\n");
		}
	}
}
