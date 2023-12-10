#include "shell.h"

/**
 * ff_strlen - returns the length of a string
 * @str: the string whose length to check
 *
 * Return: integer length of string
 */
int ff_strlen(char *str)
{
	int i = 0;

	if (!str)
		return (0);

	while (*str)
	{
		i++;
		str++;
	}
	return (i);
}

/**
 * ff_strcmp - performs lexicogarphic comparison of two strangs.
 * @st1: the first strang
 * @st2: the second strang
 *
 * Return: negative if s1 < s2, positive if s1 > s2, zero if s1 == s2
 */
int ff_strcmp(char *st1, char *st2)
{
	while (*st1 && *st2)
	{
		if (*st1 != *st2)
			return (*st1 - *st2);
		st1++;
		st2++;
	}
	if (*st1 == *st2)
		return (0);
	else
		return (*st1 < *st2 ? -1 : 1);
}

/**
 * ff_starts_with - checks if needle starts with haystack
 * @hay: string to search
 * @need: the substring to find
 *
 * Return: address of next char of haystack or NULL
 */
char *ff_starts_with(const char *hay, const char *need)
{
	while (*need)
		if (*need++ != *hay++)
			return (NULL);
	return ((char *)hay);
}

/**
 * ff_strcat - concatenates two strings
 * @d: the destination buffer
 * @src: the source buffer
 *
 * Return: pointer to destination buffer
 */
char *ff_strcat(char *d, char *src)
{
	char *ret = d;

	while (*d)
		d++;
	while (*src)
		*d++ = *src++;
	*d = *src;
	return (ret);
}
