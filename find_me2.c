#include "shell.h"

/**
 * ff_bfree - frees a pointe
 * @p: address of the pointer to free
 *
 * Return: 1 if freed, otherwise 0.
 */
int ff_bfree(void **p)
{
	if (p && *p)
	{
		free(*p);
		*p = NULL;
		return (1);
	}
	return (0);
}
