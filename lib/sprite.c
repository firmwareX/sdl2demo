#include "sprite.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

Sprite *
Sprite_New(int x, int y, int w, int h, char *data, int max_w, int max_h)
{
    Sprite *s = malloc(sizeof(Sprite));
    // Clear to zero
    memset(s, 0, sizeof(Sprite));
    s->x = x;
    s->y = y;
    s->w = w;
    s->h = h;
    s->score = 0;
    s->life = 1;
    s->blood = 1;
    s->attack = 1;
    s->value = 1;
    s->tox = 0;
    s->toy = 0;
    s->speed = 1;
    s->data = data;
    s->can_out_screen = 1;
    s->max_w = max_w;
    s->max_h = max_h;
    return s;
}