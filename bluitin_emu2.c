#include "shell.h"

/**
 * ff_myhistory - displays the history list, one command by line, preceded
 *              with line numbers, starting at 0.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int ff_myhistory(info_t *info)
{
	ff_print_list(info->history);
	return (0);
}

/**
 * ff_unset_alias - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int ff_unset_alias(info_t *info, char *str)
{
	char *p, c;
	int ret;

	p = ff_strchr(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = ff_delete_node_at_index(&(info->alias),
		ff_get_node_index(info->alias, ff_node_starts_with(info->alias, str, -1)));
	*p = c;
	return (ret);
}

/**
 * ff_set_alias - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int ff_set_alias(info_t *info, char *str)
{
	char *p;

	p = ff_strchr(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (ff_unset_alias(info, str));

	ff_unset_alias(info, str);
	return (ff_add_node_end(&(info->alias), str, 0) == NULL);
}

/**
 * ff_print_alias - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int ff_print_alias(list_t *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = ff_strchr(node->str, '=');
		for (a = node->str; a <= p; a++)
			ff_putchar(*a);
		ff_putchar('\'');
		ff_puts(p + 1);
		ff_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * ff_myalias - mimics the alias builtin (man alias)
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int ff_myalias(info_t *info)
{
	int i = 0;
	char *p = NULL;
	list_t *node = NULL;

	if (info->argc == 1)
	{
		node = info->alias;
		while (node)
		{
			ff_print_alias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; info->argv[i]; i++)
	{
		p = ff_strchr(info->argv[i], '=');
		if (p)
			ff_set_alias(info, info->argv[i]);
		else
			ff_print_alias(ff_node_starts_with(info->alias, info->argv[i], '='));
	}

	return (0);
}
