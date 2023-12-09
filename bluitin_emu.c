#include "shell.h"

/**
 * ff_myexit - exits the shell
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: exits with a given exit status
 *         (0) if info.argv[0] != "exit"
 */
int ff_myexit(info_t *info)
{
	int exitcheck;

	if (info->argv[1])
	{
		exitcheck = ff_erratoi(info->argv[1]);
		if (exitcheck == -1)
		{
			info->status = 2;
			ff_print_error(info, "Illegal number: ");
			ff_eputs(info->argv[1]);
			ff_eputchar('\n');
			return (1);
		}
		info->err_num = ff_erratoi(info->argv[1]);
		return (-2);
	}
	info->err_num = -1;
	return (-2);
}

/**
 * ff_mycd - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int ff_mycd(info_t *info)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
		ff_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!info->argv[1])
	{
		dir = ff_getenv(info, "HOME=");
		if (!dir)
			chdir_ret =
				chdir((dir = ff_getenv(info, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (ff_strcmp(info->argv[1], "-") == 0)
	{
		if (!ff_getenv(info, "OLDPWD="))
		{
			ff_puts(s);
			ff_putchar('\n');
			return (1);
		}
		ff_puts(ff_getenv(info, "OLDPWD=")), ff_putchar('\n');
		chdir_ret =
			chdir((dir = ff_getenv(info, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(info->argv[1]);
	if (chdir_ret == -1)
	{
		ff_print_error(info, "can't cd to ");
		ff_eputs(info->argv[1]), ff_eputchar('\n');
	}
	else
	{
		ff_setenv(info, "OLDPWD", ff_getenv(info, "PWD="));
		ff_setenv(info, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * ff_myhelp - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int ff_myhelp(info_t *info)
{
	char **arg_array;

	arg_array = info->argv;
	ff_puts("help call works. Function not yet implemented \n");
	if (0)
		ff_puts(*arg_array);
	return (0);
}
