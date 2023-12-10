#include "shell.h"

/**
 * ff_clear_info - initializes info_t struct
 * @info: struct address
 */
void ff_clear_info(info_t *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}

/**
 * ff_set_info - initializes info_t struct
 * @info: struct address
 * @av: argument vector
 */
void ff_set_info(info_t *info, char **av)
{
	int i = 0;

	info->fname = av[0];
	if (info->arg)
	{
		info->argv = ff_strtow(info->arg, " \t");
		if (!info->argv)
		{

			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = ff_strdup(info->arg);
				info->argv[1] = NULL;
			}
		}
		for (i = 0; info->argv && info->argv[i]; i++)
			;
		info->argc = i;

		ff_replace_alias(info);
		ff_replace_vars(info);
	}
}

/**
 * ff_free_info - frees info_t struct fields
 * @info: struct address
 * @all: true if freeing all fields
 */
void ff_free_info(info_t *info, int all)
{
	ff_ffree(info->argv);
	info->argv = NULL;
	info->path = NULL;
	if (all)
	{
		if (!info->cmd_buf)
			free(info->arg);
		if (info->env)
			ff_free_list(&(info->env));
		if (info->history)
			ff_free_list(&(info->history));
		if (info->alias)
			ff_free_list(&(info->alias));
		ff_ffree(info->environ);
			info->environ = NULL;
		ff_bfree((void **)info->cmd_buf);
		if (info->readfd > 2)
			close(info->readfd);
		ff_putchar(BUF_FLUSH);
	}
}
