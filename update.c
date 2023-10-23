#include "status.h"
#include "sprite.h"

void update_bullet(Sprite *bullet)
{
    bullet->y += bullet->toy * bullet->speed;
    if (bullet->y <= bullet->h * -1)
    {
        bullet->life -= 1;
    }
}

void update_enemy(Sprite *enemy, int HEIGHT)
{
    enemy->y += enemy->toy * enemy->speed;
    if (enemy->y >= HEIGHT)
    {
        enemy->life -= 1;
    }
}

void update_player(Sprite *player, int WIDTH, int HEIGHT)
{
    player->x += player->tox * player->speed;
    player->y += player->toy * player->speed;

    if (player->x < 0)
    {
        player->x = 0;
        player->tox = 0;
    }
    if (player->x + player->w > WIDTH)
    {
        player->x = WIDTH - player->w;
        player->tox = 0;
    }
    if (player->y < 0)
    {
        player->y = 0;
        player->toy = 0;
    }
    if (player->y + player->h > HEIGHT)
    {
        player->y = HEIGHT - player->h;
        player->toy = 0;
    }
}