#include "status.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

Status *
Status_New()
{
    Status *s = malloc(sizeof(Status));
    // Clear to zero
    memset(s, 0, sizeof(Status));
    s->paused = 0;
    s->over = 0;
    s->quit = 0;
    s->full_screen = 0;
    return s;
}