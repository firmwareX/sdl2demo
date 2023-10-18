#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

typedef struct Status
{
    int time;
    int over;
    int paused;
    int quit;
    int full_screen;
} Status;

Status *Status_New();