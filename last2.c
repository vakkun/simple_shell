#include "shell.h"

/**
 * ff_strcpy - copies a string
 * @d: the destination
 * @src: the source
 *
 * Return: pointer to destination
 */
char *ff_strcpy(char *d, char *src)
{
	int i = 0;

	if (d == src || src == 0)
		return (d);
	while (src[i])
	{
		d[i] = src[i];
		i++;
	}
	d[i] = 0;
	return (d);
}

/**
 * ff_strdup - duplicates a string
 * @s: the string to duplicate
 *
 * Return: pointer to the duplicated string
 */
char *ff_strdup(const char *s)
{
	int length = 0;
	char *ret;

	if (s == NULL)
		return (NULL);
	while (*s++)
		length++;
	ret = malloc(sizeof(char) * (length + 1));
	if (!ret)
		return (NULL);
	for (length++; length--;)
		ret[length] = *--s;
	return (ret);
}

/**
 *ff_puts - prints an input string
 *@s: the string to be printed
 *
 * Return: Nothing
 */
void ff_puts(char *s)
{
	int i = 0;

	if (!s)
		return;
	while (s[i] != '\0')
	{
		ff_putchar(s[i]);
		i++;
	}
}

/**
 * ff_putchar - writes the character c to stdout
 * @ch: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int ff_putchar(char ch)
{
	static int i;
	static char buf[WRITE_BUF_SIZE];

	if (ch == BUF_FLUSH || i >= WRITE_BUF_SIZE)
	{
		write(1, buf, i);
		i = 0;
	}
	if (ch != BUF_FLUSH)
		buf[i++] = ch;
	return (1);
}
