#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>
#include "status.h"
#include "sprite.h"
#include "process_events.h"
#include "update.h"
#include "draw.h"
#include "collision_detection.h"

#define BULLETSTOTAL 10
#define ENEMYSTOTAL 10
#define WIDTH 640
#define HEIGHT 480

Status *status;
Sprite *player;

Sprite *bullets[BULLETSTOTAL];
Sprite *enemys[ENEMYSTOTAL];

SDL_Window *window;

void init_player()
{
    player = Sprite_New(WIDTH / 2 - 20 / 2, HEIGHT - 20 - 10, 20, 20, "X");
    player->speed = 2;
}

void init_enemys()
{
    // for (size_t i = 0; i < sizeof(enemys) / sizeof(enemys[0]); i++)
    for (size_t i = 0; i < ENEMYSTOTAL; i++)
    {
        enemys[i] = Sprite_New(0, -20, 20, 20, "A");
        enemys[i]->life = 0;
    }
}

void init_bullets()
{
    for (size_t i = 0; i < BULLETSTOTAL; i++)
    {
        bullets[i] = Sprite_New(10, 10, 10, 5, "^");
        bullets[i]->life = 0;
    }
}

void make_enemy()
{
    for (size_t i = 0; i < ENEMYSTOTAL; i++)
    {
        if (enemys[i]->life < 1)
        {
            srand(time(NULL));
            int x = rand() % (WIDTH - enemys[i]->w);
            enemys[i]->x = x;
            enemys[i]->y = -1 * enemys[i]->h;
            enemys[i]->life = 1;
            enemys[i]->speed = 1;
            enemys[i]->toy = 1;
            break;
        }
    }
}

void make_bullet()
{
    for (size_t i = 0; i < BULLETSTOTAL; i++)
    {
        if (bullets[i]->life < 1)
        {
            bullets[i]->x = player->x + player->w / 2 - bullets[i]->w / 2;
            bullets[i]->y = player->y;
            bullets[i]->life = 1;
            bullets[i]->speed = 4;
            bullets[i]->toy = -1;
            break;
        }
    }
}

void update()
{
    update_player(player, WIDTH, HEIGHT);

    for (size_t i = 0; i < BULLETSTOTAL; i++)
    {
        if (bullets[i]->life > 0)
        {
            update_bullet(bullets[i]);
        }
    }

    for (size_t i = 0; i < ENEMYSTOTAL; i++)
    {
        if (enemys[i]->life > 0)
        {
            update_enemy(enemys[i], HEIGHT);
        }
    }

    for (size_t i = 0; i < ENEMYSTOTAL; i++)
    {
        if (enemys[i]->life > 0)
        {
            if (collision_detection(enemys[i], player))
            {
                status->over = 1;
            }
        }
    }

    for (size_t i = 0; i < ENEMYSTOTAL; i++)
    {
        if (enemys[i]->life > 0)
        {
            for (size_t j = 0; j < BULLETSTOTAL; j++)
            {
                if (bullets[j]->life > 0)
                {
                    if (collision_detection(enemys[i], bullets[j]))
                    {
                        enemys[i]->life -= bullets[j]->attack;
                        bullets[j]->life -= enemys[i]->attack;
                        player->score += enemys[i]->value;
                    }
                }
            }
        }
    }
}

void draw()
{
    draw_clear();
    draw_sprite(player);
    for (size_t i = 0; i < sizeof(enemys) / sizeof(enemys[0]); i++)
    {
        if (enemys[i]->life > 0)
        {
            draw_sprite(enemys[i]);
        }
    }
    for (size_t i = 0; i < sizeof(bullets) / sizeof(bullets[0]); i++)
    {
        if (bullets[i]->life > 0)
        {
            draw_sprite(bullets[i]);
        }
    }
    draw_score(player->score);
    if (status->over)
    {
        draw_gameover(WIDTH, HEIGHT);
    }

    if (status->paused)
    {
        draw_paused(WIDTH, HEIGHT);
    }
    draw_show();
}

int main(int argc, char *argv[])
{
    status = Status_New();

    init_player();
    init_enemys();
    init_bullets();

    // printf("%d %s",argc,argv[1]);

    if (argc == 2)
    {
        if (strcmp(argv[1], "full") == 0)
        {
            status->full_screen = 1;
        }
    }

    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() != 0)
    {
        printf("error initializing SDL_TTF: %s\n", SDL_GetError());
        return 1;
    }

    // creates a window
    window = SDL_CreateWindow("demo",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT,
                              SDL_WINDOW_SHOWN);

    if (window == NULL)
    {
        printf("error: %s\n", SDL_GetError());
        exit(1);
    }

    if (status->full_screen)
    {
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    }

    draw_init(window);

    // looping for event with input
    while (!status->quit)
    {
        ProcessEvents(status, player, init_player, init_enemys, init_bullets, make_bullet);

        draw();

        if (status->over)
        {
            continue;
        }

        if (status->paused)
        {
            continue;
        }

        status->time += 1;

        if (status->time % 30 == 0)
        {
            make_enemy();
        }

        update();

        SDL_Delay(1000 / 60);
    }

    /// Freeing resources
    draw_destroy();
    SDL_DestroyWindow(window);
    SDL_Quit();
}