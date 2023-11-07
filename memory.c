#include "my_shell.h"

/**
 * my_bfree - Frees a pointer and sets it to NULL
 * @ptr: Address of the pointer to free
 *
 * Return: 1 if freed, otherwise 0.
 */
int my_bfree(void **ptr)
{
    if (ptr && *ptr)
    {
        free(*ptr);
        *ptr = NULL;
        return 1;
    }
    return 0;
}

