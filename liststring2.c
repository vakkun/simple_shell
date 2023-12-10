#include "shell.h"

/**
 * ff_list_len - determines length of linked list
 * @h: pointer to first node
 *
 * Return: size of list
 */
size_t ff_list_len(const list_t *h)
{
	size_t i = 0;

	while (h)
	{
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * ff_list_to_strings - returns an array of strings of the list->str
 * @head: pointer to first node
 *
 * Return: array of strings
 */
char **ff_list_to_strings(list_t *head)
{
	list_t *node = head;
	size_t i = ff_list_len(head), j;
	char **strs;
	char *str;

	if (!head || !i)
		return (NULL);
	strs = malloc(sizeof(char *) * (i + 1));
	if (!strs)
		return (NULL);
	for (i = 0; node; node = node->next, i++)
	{
		str = malloc(ff_strlen(node->str) + 1);
		if (!str)
		{
			for (j = 0; j < i; j++)
				free(strs[j]);
			free(strs);
			return (NULL);
		}

		str = ff_strcpy(str, node->str);
		strs[i] = str;
	}
	strs[i] = NULL;
	return (strs);
}


/**
 * ff_print_list - prints all elements of a list_t
 * @h: pointer to first node
 *
 * Return: size of list
 */
size_t ff_print_list(const list_t *h)
{
	size_t i = 0;

	while (h)
	{
		ff_puts(ff_convert_number(h->num, 10, 0));
		ff_putchar(':');
		ff_putchar(' ');
		ff_puts(h->str ? h->str : "(nil)");
		ff_puts("\n");
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * ff_node_starts_with - returns node whose string starts with prefix
 * @node: pointer to list head
 * @prefix: string to match
 * @c: the next character after prefix to match
 *
 * Return: match node or null
 */
list_t *ff_node_starts_with(list_t *node, char *prefix, char c)
{
	char *p = NULL;

	while (node)
	{
		p = ff_starts_with(node->str, prefix);
		if (p && ((c == -1) || (*p == c)))
			return (node);
		node = node->next;
	}
	return (NULL);
}

/**
 * ff_get_node_index - gets the index of a node
 * @head: pointer to list head
 * @node: pointer to the node
 *
 * Return: index of node or -1
 */
ssize_t ff_get_node_index(list_t *head, list_t *node)
{
	size_t i = 0;

	while (head)
	{
		if (head == node)
			return (i);
		head = head->next;
		i++;
	}
	return (-1);
}
