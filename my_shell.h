/* my_shell.h */

#ifndef _MY_SHELL_H_
#define _MY_SHELL_H_

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

/* Custom constants */
#define MY_READ_BUF_SIZE 1024
#define MY_WRITE_BUF_SIZE 1024
#define MY_BUF_FLUSH -1

/* Custom command chaining definitions */
#define MY_CMD_NORM 0
#define MY_CMD_OR 1
#define MY_CMD_AND 2
#define MY_CMD_CHAIN 3

/* Custom conversion options */
#define MY_CONVERT_LOWERCASE 1
#define MY_CONVERT_UNSIGNED 2

/* Custom history file and limit */
#define MY_HIST_FILE ".my_shell_history"
#define MY_HIST_MAX 4096

extern char **environ;

/* Custom struct for linked list */
typedef struct my_liststr {
    int num;
    char *str;
    struct my_liststr *next;
} my_list_t;

/* Custom struct for passing information */
typedef struct my_passinfo {
    char *arg;
    char **argv;
    char *path;
    int argc;
    unsigned int line_count;
    int err_num;
    int linecount_flag;
    char *fname;
    my_list_t *env;
    my_list_t *history;
    my_list_t *alias;
    char **environ;
    int env_changed;
    int status;

    char **cmd_buf;
    int cmd_buf_type;
    int readfd;
    int histcount;
} my_info_t;

/* Initialize the custom info struct */
#define MY_INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
0, 0, 0}

/* Custom struct for built-in commands and their functions */
typedef struct my_builtin {
    char *type;
    int (*func)(my_info_t *);
} my_builtin_table;

/* Custom function declarations for the shell */
int my_hsh(my_info_t *, char **);
int my_find_builtin(my_info_t *);
void my_find_cmd(my_info_t *);
void my_fork_cmd(my_info_t *);
void populate_env_list(my_info_t *info);
void read_history(my_info_t *info);


int my_strlen(char *);
int my_strcmp(char *, char *);
char *my_starts_with(const char *, const char *);
char *my_strcat(char *, char *);

void my_clear_info(my_info_t *);
int my_interactive(my_info_t *);

void my_puts(char *);
int my_eputchar(char);
int my_putfd(char c, int fd);
int my_putsfd(char *str, int fd);

ssize_t my_get_input(my_info_t *);
int my_getline(my_info_t *, char **, size_t *);
void my_sigintHandler(int);

char *my_strcpy(char *, char *);
char *my_strdup(const char *);
int my_putchar(char);

char *my_get_history_file(my_info_t *info);
int my_write_history(my_info_t *info);
int my_read_history(my_info_t *info);
int my_build_history_list(my_info_t *info, char *buf, int linecount);
int my_renumber_history(my_info_t *info);

int my_is_chain(my_info_t *, char *, size_t *);
void my_check_chain(my_info_t *, char *, size_t *, size_t, size_t);
int my_replace_alias(my_info_t *);
int my_replace_vars(my_info_t *);
int my_replace_string(char **, char *);
/*not yet edited*/

size_t my_list_len(const my_list_t *);
char **my_list_to_strings(my_list_t *);
size_t my_print_list(const my_list_t *);
my_list_t *my_node_starts_with(my_list_t *, char *, char);
ssize_t my_get_node_index(my_list_t *, my_list_t *);

my_list_t *my_add_node(my_list_t **, const char *, int);
my_list_t *my_add_node_end(my_list_t **, const char *, int);
size_t my_print_list_str(const my_list_t *);
int my_delete_node_at_index(my_list_t **, unsigned int);
void my_free_list(my_list_t **);

char **my_get_environ(my_info_t *);
int my_unsetenv(my_info_t *, char *);
int my_setenv(my_info_t *, char *, char *);

char *my_getenv(my_info_t *, const char *);
int my_myenv(my_info_t *);
int my_mysetenv(my_info_t *);
int my_myunsetenv(my_info_t *);
int my_populate_env_list(my_info_t *);

void my_clear_info(my_info_t *);
void my_set_info(my_info_t *, char **);
void my_free_info(my_info_t *, int);

/* toem_getinfo.c */
void my_clear_info(my_info_t *);
void my_set_info(my_info_t *, char **);
void my_free_info(my_info_t *, int);

/* toem_errors1.c */
int my_erratoi(char *);
void my_print_error(my_info_t *, char *);
int my_print_d(int, int);
char *my_convert_number(long int, int, int);
void my_remove_comments(char *);

/* toem_builtin.c */
int my_myexit(my_info_t *);
int my_mycd(my_info_t *);
int my_myhelp(my_info_t *);

/* toem_builtin1.c */
int my_myhistory(my_info_t *);
int my_myalias(my_info_t *);

/* toem_string.c */
int my_strlen(char *);
int my_strcmp(char *, char *);
char *my_starts_with(const char *, const char *);
char *my_strcat(char *, char *);

/* toem_string1.c */
char *my_strcpy(char *, char *);
char *my_strdup(const char *);
void my_puts(char *);
int my_putchar(char);

/* toem_exits.c */
char *my_strncpy(char *, char *, int);
char *my_strncat(char *, char *, int);
char *my_strchr(char *, char);

/* toem_tokenizer.c */
char **my_strtow(char *, char *);
char **my_strtow2(char *, char);

/* toem_realloc.c */
char *my_memset(char *, char, unsigned int);
void my_ffree(char **);
void *my_realloc(void *, unsigned int, unsigned int);

/* toem_memory.c */
int my_bfree(void **);

/* toem_atoi.c */
int my_interactive(my_info_t *);
int my_is_delim(char, char *);
int my_isalpha(int);
int my_atoi(char *);

/* toem_shloop.c */
int my_hsh(my_info_t *, char **);
int my_find_builtin(my_info_t *);
void my_find_cmd(my_info_t *);
void my_fork_cmd(my_info_t *);

/* toem_parser.c */
int my_is_cmd(my_info_t *, char *);
char *my_dup_chars(char *, int, int);
char *my_find_path(my_info_t *, char *, char *);

/* loophsh.c */
int my_loophsh(char **);

/* Define your custom functions for other parts of the code */

#endif

