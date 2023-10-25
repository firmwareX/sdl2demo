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

Status status;
Sprite player;

Sprite bullets[BULLETSTOTAL];
Sprite enemys[ENEMYSTOTAL];

SDL_Window *window;

void init_player()
{
    player.attack = 1;
    player.life = 1;
    player.x = WIDTH / 2 - 20 / 2;
    player.y = HEIGHT - 20 - 10;
    player.tox = 0;
    player.toy = 0;
    player.w = 20;
    player.h = 30;
    player.score = 0;
    player.data = "x";
    player.speed = 2;
}

void init_enemys()
{
    for (size_t i = 0; i < ENEMYSTOTAL; i++)
    {
        enemys[i].x = 0;
        enemys[i].y = -20;
        enemys[i].w = 20;
        enemys[i].h = 20;
        enemys[i].data = "A";
        enemys[i].value = 1;
        enemys[i].attack = 1;
        enemys[i].life = 0;
        enemys[i].speed = 1;
        enemys[i].toy = 1;
    }
}

void init_bullets()
{
    for (size_t i = 0; i < BULLETSTOTAL; i++)
    {
        bullets[i].w = 15;
        bullets[i].h = 10;
        bullets[i].data = "^";
        bullets[i].value = 1;
        bullets[i].attack = 1;
        bullets[i].life = 0;
        bullets[i].speed = 4;
        bullets[i].toy = -1;
        bullets[i].attack = 1;
    }
}

void make_enemy()
{
    for (size_t i = 0; i < ENEMYSTOTAL; i++)
    {
        if (enemys[i].life < 1)
        {
            srand(time(NULL));
            int x = rand() % (WIDTH - enemys[i].w);
            enemys[i].x = x;
            enemys[i].y = -1 * enemys[i].h;
            enemys[i].life = 1;
            break;
        }
    }
}

void make_bullet()
{
    for (size_t i = 0; i < BULLETSTOTAL; i++)
    {
        if (bullets[i].life < 1)
        {
            bullets[i].x = player.x + player.w / 2 - bullets[i].w / 2;
            bullets[i].y = player.y;
            bullets[i].life = 1;
            break;
        }
    }
}

void update()
{
    update_player(&player, WIDTH, HEIGHT);

    for (size_t i = 0; i < BULLETSTOTAL; i++)
    {
        if (bullets[i].life > 0)
        {
            update_bullet(&bullets[i]);
        }
    }

    for (size_t i = 0; i < ENEMYSTOTAL; i++)
    {
        if (enemys[i].life > 0)
        {
            update_enemy(&enemys[i], HEIGHT);
        }
    }

    for (size_t i = 0; i < ENEMYSTOTAL; i++)
    {
        if (enemys[i].life > 0)
        {
            if (collision_detection(enemys[i], player))
            {
                status.over = 1;
            }
        }
    }

    for (size_t i = 0; i < ENEMYSTOTAL; i++)
    {
        if (enemys[i].life > 0)
        {
            for (size_t j = 0; j < BULLETSTOTAL; j++)
            {
                if (bullets[j].life > 0)
                {
                    if (collision_detection(enemys[i], bullets[j]))
                    {
                        enemys[i].life -= bullets[j].attack;
                        bullets[j].life -= enemys[i].attack;
                        player.score += enemys[i].value;
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
    for (size_t i = 0; i < ENEMYSTOTAL; i++)
    {
        if (enemys[i].life > 0)
        {
            draw_sprite(enemys[i]);
        }
    }
    for (size_t i = 0; i < BULLETSTOTAL; i++)
    {
        if (bullets[i].life > 0)
        {
            draw_sprite(bullets[i]);
        }
    }
    draw_score(player.score);
    if (status.over)
    {
        draw_gameover(WIDTH, HEIGHT);
    }

    if (status.paused)
    {
        draw_paused(WIDTH, HEIGHT);
    }
    draw_show();
}

int main(int argc, char *argv[])
{
    status = init_status();

    init_player();
    init_enemys();
    init_bullets();

    // printf("%d %s",argc,argv[1]);

    if (argc == 2)
    {
        if (strcmp(argv[1], "full") == 0)
        {
            status.full_screen = 1;
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

    if (status.full_screen)
    {
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    }

    draw_init(window);

    // looping for event with input
    while (!status.quit)
    {
        int is_restart = 0;
        int is_make_bullet = 0;

        ProcessEvents(&status, &player, &is_restart, &is_make_bullet);

        draw();

        if (is_restart == 1)
        {
            init_player();
            init_enemys();
            init_bullets();

            status.over = 0;
            status.time = 0;

            is_restart = 0;
        }

        if (status.over)
        {
            continue;
        }

        if (status.paused)
        {
            continue;
        }

        status.time += 1;

        if (status.time % 30 == 0)
        {
            make_enemy();
        }

        if (is_make_bullet == 1)
        {
            make_bullet();
            is_make_bullet = 0;
        }

        update();

        SDL_Delay(1000 / 60);
    }

    /// Freeing resources
    draw_destroy();
    SDL_DestroyWindow(window);
    SDL_Quit();
}