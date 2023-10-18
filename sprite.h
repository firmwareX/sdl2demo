#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

typedef struct Sprite
{
    int x;
    int y;
    int w;
    int h;
    int attack; // 攻击力，对方 blood -= 己方的 attack
    int life;
    int blood;
    int value; // 对方增加的分数
    int score;
    int tox;
    int toy;
    int speed;
} Sprite;

Sprite *Sprite_New();