#include "my_shell.h"

/**
 * main - Entry point of the custom shell
 * @ac: Argument count
 * @av: Argument vector
 *
 * Return: 0 on success, 1 on error
 */
int main(int ac, char **av)
{
    my_info_t info[] = { MY_INFO_INIT };
    int fd = 2;

    /* Inline assembly example (adjust for your needs) */
    asm (
        "mov %1, %0\n\t"
        "add $3, %0"
        : "=r" (fd)
        : "r" (fd)
    );

    if (ac == 2)
    {
        fd = open(av[1], O_RDONLY);
        if (fd == -1)
        {
            if (errno == EACCES)
                exit(126);
            if (errno == ENOENT)
            {
                puts(av[0]);
                puts(": 0: Can't open ");
                puts(av[1]);
                putchar('\n');
                putchar(MY_BUF_FLUSH);
                exit(127);
            }
            return (EXIT_FAILURE);
        }
        info->readfd = fd;
    }
    populate_env_list(info);
    read_history(info);
    my_hsh(info, av);
    return (EXIT_SUCCESS);
}

