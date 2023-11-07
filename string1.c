#include "my_shell.h"

/**
 * my_strcpy - Copies a string.
 * @dest: The destination.
 * @src: The source.
 *
 * Return: Pointer to destination.
 */
char *my_strcpy(char *dest, char *src)
{
    int i = 0;

    if (dest == src || src == NULL)
        return dest;
    while (src[i])
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return dest;
}

/**
 * my_strdup - Duplicates a string.
 * @str: The string to duplicate.
 *
 * Return: Pointer to the duplicated string.
 */
char *my_strdup(const char *str)
{
    int length = 0;
    char *ret;

    if (str == NULL)
        return NULL;
    while (*str++)
        length++;
    ret = malloc(sizeof(char) * (length + 1));
    if (!ret)
        return NULL;
    for (length++; length--;)
        ret[length] = *--str;
    return ret;
}

/**
 * my_puts - Prints an input string.
 * @str: The string to be printed.
 *
 * Return: Nothing.
 */
void my_puts(char *str)
{
    int i = 0;

    if (!str)
        return;
    while (str[i] != '\0')
    {
        my_putchar(str[i]);
        i++;
    }
}

/**
 * my_putchar - Writes the character c to stdout.
 * @c: The character to print.
 *
 * Return: On success, 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int my_putchar(char c)
{
    static int i;
    static char buf[MY_WRITE_BUF_SIZE];

    if (c == MY_BUF_FLUSH || i >= MY_WRITE_BUF_SIZE)
    {
        write(1, buf, i);
        i = 0;
    }
    if (c != MY_BUF_FLUSH)
        buf[i++] = c;
    return 1;
}

