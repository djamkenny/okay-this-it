#include "my_shell.h"

/**
 * my_is_chain - Tests if the current character in the buffer is a chain delimiter.
 * @info: The parameter struct.
 * @buf: The character buffer.
 * @p: Address of the current position in buf.
 *
 * Return: 1 if it's a chain delimiter, 0 otherwise.
 */
int my_is_chain(my_info_t *info, char *buf, size_t *p)
{
    size_t j = *p;

    if (buf[j] == '|' && buf[j + 1] == '|')
    {
        buf[j] = 0;
        j++;
        info->cmd_buf_type = MY_CMD_OR;
    }
    else if (buf[j] == '&' && buf[j + 1] == '&')
    {
        buf[j] = 0;
        j++;
        info->cmd_buf_type = MY_CMD_AND;
    }
    else if (buf[j] == ';') /* Found the end of this command */
    {
        buf[j] = 0; /* Replace semicolon with null */
        info->cmd_buf_type = MY_CMD_CHAIN;
    }
    else
    {
        return 0;
    }
    *p = j;
    return 1;
}

/**
 * my_check_chain - Checks if we should continue chaining based on the last status.
 * @info: The parameter struct.
 * @buf: The character buffer.
 * @p: Address of the current position in buf.
 * @i: Starting position in buf.
 * @len: Length of buf.
 *
 * Return: Void.
 */
void my_check_chain(my_info_t *info, char *buf, size_t *p, size_t i, size_t len)
{
    size_t j = *p;

    if (info->cmd_buf_type == MY_CMD_AND)
    {
        if (info->status)
        {
            buf[i] = 0;
            j = len;
        }
    }
    if (info->cmd_buf_type == MY_CMD_OR)
    {
        if (!info->status)
        {
            buf[i] = 0;
            j = len;
        }
    }

    *p = j;
}

/**
 * my_replace_alias - Replaces an alias in the tokenized string.
 * @info: The parameter struct.
 *
 * Return: 1 if replaced, 0 otherwise.
 */
int my_replace_alias(my_info_t *info)
{
    int i;
    my_list_t *node;
    char *p;

    for (i = 0; i < 10; i++)
    {
        node = my_node_starts_with(info->alias, info->argv[0], '=');
        if (!node)
        {
            return 0;
        }
        free(info->argv[0]);
        p = my_strchr(node->str, '=');
        if (!p)
        {
            return 0;
        }
        p = my_strdup(p + 1);
        if (!p)
        {
            return 0;
        }
        info->argv[0] = p;
    }
    return 1;
}

/**
 * my_replace_vars - Replaces variables in the tokenized string.
 * @info: The parameter struct.
 *
 * Return: 1 if replaced, 0 otherwise.
 */
int my_replace_vars(my_info_t *info)
{
    int i = 0;
    my_list_t *node;

    for (i = 0; info->argv[i]; i++)
    {
        if (info->argv[i][0] != '$' || !info->argv[i][1])
        {
            continue;
        }

        if (!my_strcmp(info->argv[i], "$?"))
        {
            my_replace_string(&(info->argv[i]), my_strdup(my_convert_number(info->status, 10, 0)));
            continue;
        }
        if (!my_strcmp(info->argv[i], "$$"))
        {
            my_replace_string(&(info->argv[i]), my_strdup(my_convert_number(getpid(), 10, 0)));
            continue;
        }
        node = my_node_starts_with(info->env, &info->argv[i][1], '=');
        if (node)
        {
            my_replace_string(&(info->argv[i]), my_strdup(my_strchr(node->str, '=') + 1));
            continue;
        }
        my_replace_string(&info->argv[i], my_strdup(""));
    }
    return 0;
}

/**
 * my_replace_string - Replaces a string.
 * @old: Address of the old string.
 * @new: New string.
 *
 * Return: 1 if replaced, 0 otherwise.
 */
int my_replace_string(char **old, char *new)
{
    free(*old);
    *old = new;
    return 1;
}

