#include "shell.h"

/**
 * ff_input_buf - buffers chained commands
 * @info: parameter struct
 * @buf: address of buffer
 * @len: address of len var
 *
 * Return: bytes read
 */
ssize_t ff_input_buf(info_t *info, char **buf, size_t *len)
{
	ssize_t r = 0;
	size_t len_p = 0;

	if (!*len)
	{
		free(*buf);
		*buf = NULL;
		signal(SIGINT, ff_sigintHandler);
#if USE_GETLINE
		r = getline(buf, &len_p, stdin);
#else
		r = ff_getline(info, buf, &len_p);
#endif
		if (r > 0)
		{
			if ((*buf)[r - 1] == '\n')
			{
				(*buf)[r - 1] = '\0';
				r--;
			}
			info->linecount_flag = 1;
			ff_remove_comments(*buf);
			ff_build_history_list(info, *buf, info->histcount++);
			{
				*len = r;
				info->cmd_buf = buf;
			}
		}
	}
	return (r);
}

/**
 * ff_get_input - gets a line minus the newline
 * @info: parameter struct
 *
 * Return: bytes read
 */
ssize_t ff_get_input(info_t *info)
{
	static char *buf;
	static size_t i, j, len;
	ssize_t r = 0;
	char **buf_p = &(info->arg), *p;

	ff_putchar(BUF_FLUSH);
	r = ff_input_buf(info, &buf, &len);
	if (r == -1)
		return (-1);
	if (len)
	{
		j = i;
		p = buf + i;
		ff_check_chain(info, buf, &j, i, len);
		while (j < len)
		{
			if (ff_is_chain(info, buf, &j))
				break;
			j++;
		}

		i = j + 1;
		if (i >= len)
		{
			i = len = 0;
			info->cmd_buf_type = CMD_NORM;
		}

		*buf_p = p;
		return (ff_strlen(p)); /* return length of current command */
	}

	*buf_p = buf;
	return (r);
}

/**
 * ff_read_buf - reads a buffer
 * @info: parameter struct
 * @buf: buffer
 * @i: size
 *
 * Return: r
 */
ssize_t ff_read_buf(info_t *info, char *buf, size_t *i)
{
	ssize_t n = 0;

	if (*i)
		return (0);
	n = read(info->readfd, buf, READ_BUF_SIZE);
	if (n >= 0)
		*i = n;
	return (n);
}

/**
 * ff_getline - gets the next line of input from STDIN
 * @info: parameter struct
 * @ptr: address of pointer
 * @length: len
 * Return: s
 */
int ff_getline(info_t *info, char **ptr, size_t *length)
{
	static char buf[READ_BUF_SIZE];
	static size_t i, len;
	size_t k;
	ssize_t r = 0, s = 0;
	char *ptr2 = NULL, *new_p = NULL, *c;

	ptr2 = *ptr;
	if (ptr2 && length)
		s = *length;
	if (i == len)
		i = len = 0;

	r = ff_read_buf(info, buf, &len);
	if (r == -1 || (r == 0 && len == 0))
		return (-1);

	c = ff_strchr(buf + i, '\n');
	k = c ? 1 + (unsigned int)(c - buf) : len;
	new_p = ff_realloc(ptr2, s, s ? s + k : k + 1);
	if (!new_p)
		return (ptr2 ? free(ptr2), -1 : -1);

	if (s)
		ff_strncat(new_p, buf + i, k - i);
	else
		ff_strncpy(new_p, buf + i, k - i + 1);

	s += k - i;
	i = k;
	ptr2 = new_p;

	if (length)
		*length = s;
	*ptr = ptr2;
	return (s);
}

/**
 * ff_sigintHandler - blocks ctrl-C
 * @sig_num: the signal number
 *
 * Return: void
 */
void ff_sigintHandler(__attribute__((unused))int sig_num)
{
	ff_puts("\n");
	ff_puts("$ ");
	ff_putchar(BUF_FLUSH);
}
