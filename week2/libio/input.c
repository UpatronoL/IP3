#include "libio.h"
#include <stdio.h>

char *input(void)
{
    static char buffer[1024];
    return fgets(buffer, sizeof(buffer), stdin);
}
