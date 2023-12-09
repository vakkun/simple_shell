#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;


/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *					allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histcount: the history line number count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;


/* hsh.c */
int ff_hsh(info_t *, char **);
int ff_find_builtin(info_t *);
void ff_find_cmd(info_t *);
void ff_fork_cmd(info_t *);

/* path.c */
int ff_is_cmd(info_t *, char *);
char *ff_dup_chars(char *, int, int);
char *ff_find_path(info_t *, char *, char *);

/* loophsh.c */
int ff_loophsh(char **);

/* err_string_functions.c */
void ff_eputs(char *);
int ff_eputchar(char);
int ff_putfd(char c, int fd);
int ff_putsfd(char *str, int fd);

/* string_functions.c */
int ff_strlen(char *);
int ff_strcmp(char *, char *);
char *ff_starts_with(const char *, const char *);
char *ff_strcat(char *, char *);

/* string_functions2.c */
char *ff_strcpy(char *, char *);
char *ff_strdup(const char *);
void ff_puts(char *);
int ff_putchar(char);

/* string_functions3.c */
char *ff_strncpy(char *, char *, int);
char *ff_strncat(char *, char *, int);
char *ff_strchr(char *, char);

/* string_functions4.c */
char **ff_strtow(char *, char *);
char **ff_strtow2(char *, char);

/* memory_functions */
char *ff_memset(char *, char, unsigned int);
void ff_ffree(char **);
void *ff_realloc(void *, unsigned int, unsigned int);

/* memory_functions2.c */
int ff_bfree(void **);

/* more_functions.c */
int ff_interactive(info_t *);
int ff_is_delim(char, char *);
int ff_isalpha(int);
int ff_atoi(char *);

/* more_functions2.c */
int ff_erratoi(char *);
void ff_print_error(info_t *, char *);
int ff_print_d(int, int);
char *ff_convert_number(long int, int, int);
void ff_remove_comments(char *);

/* builtin_emulators.c */
int ff_myexit(info_t *);
int ff_mycd(info_t *);
int ff_myhelp(info_t *);

/* builtin_emulators2.c */
int ff_myhistory(info_t *);
int ff_myalias(info_t *);

/* getline.c module */
ssize_t ff_get_input(info_t *);
int ff_getline(info_t *, char **, size_t *);
void ff_sigintHandler(int);

/* info.c module */
void ff_clear_info(info_t *);
void ff_set_info(info_t *, char **);
void ff_free_info(info_t *, int);

/* env.c module */
char *ff_getenv(info_t *, const char *);
int ff_myenv(info_t *);
int ff_mysetenv(info_t *);
int ff_myunsetenv(info_t *);
int ff_populate_env_list(info_t *);

/* env2.c module */
char **ff_get_environ(info_t *);
int ff_unsetenv(info_t *, char *);
int ff_setenv(info_t *, char *, char *);

/* file_io_functions.c */
char *ff_get_history_file(info_t *info);
int ff_write_history(info_t *info);
int ff_read_history(info_t *info);
int ff_build_history_list(info_t *info, char *buf, int linecount);
int ff_renumber_history(info_t *info);

/* liststr.c module */
list_t *ff_add_node(list_t **, const char *, int);
list_t *ff_add_node_end(list_t **, const char *, int);
size_t ff_print_list_str(const list_t *);
int ff_delete_node_at_index(list_t **, unsigned int);
void ff_free_list(list_t **);

/* liststr2.c module */
size_t ff_list_len(const list_t *);
char **ff_list_to_strings(list_t *);
size_t ff_print_list(const list_t *);
list_t *ff_node_starts_with(list_t *, char *, char);
ssize_t ff_get_node_index(list_t *, list_t *);

/* chain.c */
int ff_is_chain(info_t *, char *, size_t *);
void ff_check_chain(info_t *, char *, size_t *, size_t, size_t);
int ff_replace_alias(info_t *);
int ff_replace_vars(info_t *);
int ff_replace_string(char **, char *);

#endif
