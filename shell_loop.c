#include "my_shell.h"

/**
 * my_hsh - Main shell loop.
 * @info: The parameter and return info struct.
 * @av: The argument vector from main().
 *
 * Return: 0 on success, 1 on error, or error code.
 */
int my_hsh(my_info_t *info, char **av)
{
    ssize_t r = 0;
    int builtin_ret = 0;

    while (r != -1 && builtin_ret != -2)
    {
        my_clear_info(info);
        if (my_interactive(info))
            my_puts("$ ");
        my_eputchar(MY_BUF_FLUSH);
        r = my_get_input(info);
        if (r != -1)
        {
            my_set_info(info, av);
            builtin_ret = my_find_builtin(info);
            if (builtin_ret == -1)
                my_find_cmd(info);
        }
        else if (my_interactive(info))
            my_putchar('\n');
        my_free_info(info, 0);
    }
    my_write_history(info);
    my_free_info(info, 1);
    if (!my_interactive(info) && info->status)
        exit(info->status);
    if (builtin_ret == -2)
    {
        if (info->err_num == -1)
            exit(info->status);
        exit(info->err_num);
    }
    return (builtin_ret);
}

/**
 * my_find_builtin - Finds a built-in command.
 * @info: The parameter and return info struct.
 *
 * Return: -1 if built-in not found,
 *         0 if built-in executed successfully,
 *         1 if built-in found but not successful,
 *         -2 if built-in signals exit().
 */
int my_find_builtin(my_info_t *info)
{
    int i, built_in_ret = -1;
    my_builtin_table builtintbl[] = {
        {"exit", my_myexit},
        {"env", my_myenv},
        {"help", my_myhelp},
        {"history", my_myhistory},
        {"setenv", my_mysetenv},
        {"unsetenv", my_myunsetenv},
        {"cd", my_mycd},
        {"alias", my_myalias},
        {NULL, NULL}
    };

    for (i = 0; builtintbl[i].type; i++)
        if (my_strcmp(info->argv[0], builtintbl[i].type) == 0)
        {
            info->line_count++;
            built_in_ret = builtintbl[i].func(info);
            break;
        }
    return built_in_ret;
}

/**
 * my_find_cmd - Finds a command in PATH.
 * @info: The parameter and return info struct.
 *
 * Return: void
 */
void my_find_cmd(my_info_t *info)
{
    char *path = NULL;
    int i, k;

    info->path = info->argv[0];
    if (info->linecount_flag == 1)
    {
        info->line_count++;
        info->linecount_flag = 0;
    }
    for (i = 0, k = 0; info->arg[i]; i++)
        if (!my_is_delim(info->arg[i], " \t\n"))
            k++;
    if (!k)
        return;

    path = my_find_path(info, my_getenv(info, "PATH="), info->argv[0]);
    if (path)
    {
        info->path = path;
        my_fork_cmd(info);
    }
    else
    {
        if ((my_interactive(info) || my_getenv(info, "PATH=") || info->argv[0][0] == '/') && my_is_cmd(info, info->argv[0]))
            my_fork_cmd(info);
        else if (*(info->arg) != '\n')
        {
            info->status = 127;
            my_print_error(info, "not found\n");
        }
    }
}

/**
 * my_fork_cmd - Forks an exec thread to run a command.
 * @info: The parameter and return info struct.
 *
 * Return: void
 */
void my_fork_cmd(my_info_t *info)
{
    pid_t child_pid;

    child_pid = fork();
    if (child_pid == -1)
    {
        perror("Error:");
        return;
    }
    if (child_pid == 0)
    {
        if (execve(info->path, info->argv, my_get_environ(info)) == -1)
        {
            my_free_info(info, 1);
            if (errno == EACCES)
                exit(126);
            exit(1);
        }
    }
    else
    {
        wait(&(info->status));
        if (WIFEXITED(info->status))
        {
            info->status = WEXITSTATUS(info->status);
            if (info->status == 126)
                my_print_error(info, "Permission denied\n");
        }
    }
}

