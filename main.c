#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>
#include "status.h"
#include "sprite.h"

#define BULLETSTOTAL 100
#define ENEMYSTOTAL 10
#define WIDTH 640
#define HEIGHT 480

SDL_Window *window;
TTF_Font *font;
TTF_Font *bigfont;
SDL_Renderer *renderer;
SDL_Surface *surface;
SDL_Texture *texture;

Status *status;
Sprite *player;

Sprite *bullets[BULLETSTOTAL];
Sprite *enemys[ENEMYSTOTAL];

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
    if (status->over)
    {
        return;
    }

    if (status->paused)
    {
        return;
    }

    status->time += 1;

    if (status->time % 30 == 0)
    {
        make_enemy();
    }

    player->x += player->tox * player->speed;
    player->y += player->toy * player->speed;

    for (size_t i = 0; i < ENEMYSTOTAL; i++)
    {
        if (enemys[i]->life > 0)
        {
            enemys[i]->y += enemys[i]->toy * enemys[i]->speed;

            if (enemys[i]->y > HEIGHT)
            {
                enemys[i]->life -= 1;
            }
        }
    }

    for (size_t i = 0; i < BULLETSTOTAL; i++)
    {
        if (bullets[i]->life > 0)
        {
            bullets[i]->y += bullets[i]->toy * bullets[i]->speed;
            if (bullets[i]->y <= bullets[i]->h * -1)
            {
                bullets[i]->life -= 1;
            }
        }
    }

    for (size_t i = 0; i < ENEMYSTOTAL; i++)
    {
        if (enemys[i]->life > 0)
        {
            if (enemys[i]->x < player->x + player->w * 0.75 &&
                enemys[i]->x + enemys[i]->w * 0.75 > player->x &&
                enemys[i]->y < player->y + player->h * 0.75 &&
                enemys[i]->y + enemys[i]->h * 0.75 > player->y)
            {
                player->life -= enemys[i]->attack;
                if (player->life < 1)
                {
                    status->over = 1;
                    break;
                }
                else
                {
                    enemys[i]->life -= player->attack;
                }
            }

            for (size_t j = 0; j < BULLETSTOTAL; j++)
            {
                if (bullets[j]->life > 0)
                {
                    if (enemys[i]->x < bullets[j]->x + bullets[j]->w * 0.75 &&
                        enemys[i]->x + enemys[i]->w * 0.75 > bullets[j]->x &&
                        enemys[i]->y < bullets[j]->y + bullets[j]->h * 0.75 &&
                        enemys[i]->y + enemys[i]->h * 0.75 > bullets[i]->y)
                    {
                        bullets[j]->life -= enemys[i]->attack;
                        enemys[i]->life -= bullets[j]->attack;
                        player->score += 1;
                    }
                }
            }
        }
    }
}

void draw()
{
    SDL_RenderClear(renderer);
    SDL_Color color = {255, 255, 255};
    // 🛸
    surface = TTF_RenderText_Solid(font,
                                   player->data, color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);

    // int texW = 0;
    // int texH = 0;
    // SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = {player->x, player->y, player->w, player->h};

    // SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);

    for (size_t i = 0; i < sizeof(enemys) / sizeof(enemys[0]); i++)
    {
        if (enemys[i]->life > 0)
        {
            surface = TTF_RenderText_Solid(font,
                                           enemys[i]->data, color);
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect dstrect = {enemys[i]->x, enemys[i]->y, enemys[i]->w, enemys[i]->h};
            SDL_RenderCopy(renderer, texture, NULL, &dstrect);
        }
    }

    for (size_t i = 0; i < sizeof(bullets) / sizeof(bullets[0]); i++)
    {
        if (bullets[i]->life > 0)
        {
            surface = TTF_RenderText_Solid(font,
                                           bullets[i]->data, color);
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect dstrect = {bullets[i]->x, bullets[i]->y, bullets[i]->w, bullets[i]->h};
            SDL_RenderCopy(renderer, texture, NULL, &dstrect);
        }
    }
    char score[1000];
    sprintf(score, "SCORE:%d", player->score);
    surface = TTF_RenderText_Solid(font,
                                   score, color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);

    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect score_dstrect = {10, 10, texW, texH};

    // SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderCopy(renderer, texture, NULL, &score_dstrect);

    if (status->over)
    {
        surface = TTF_RenderText_Solid(bigfont,
                                       "GAME OVER", color);
        texture = SDL_CreateTextureFromSurface(renderer, surface);

        int texW = 0;
        int texH = 0;
        SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
        SDL_Rect dstrect = {WIDTH / 2 - texW / 2, HEIGHT / 2 - texH / 2, texW, texH};
        // SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderCopy(renderer, texture, NULL, &dstrect);
    }

    if (status->paused)
    {
        surface = TTF_RenderText_Solid(bigfont,
                                       "PAUSED", color);
        texture = SDL_CreateTextureFromSurface(renderer, surface);

        int texW = 0;
        int texH = 0;
        SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
        SDL_Rect dstrect = {WIDTH / 2 - texW / 2, HEIGHT / 2 - texH / 2, texW, texH};
        // SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderCopy(renderer, texture, NULL, &dstrect);
    }

    SDL_RenderPresent(renderer);
}

void ProcessEvents()
{
    SDL_Event e;

    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            status->quit = 1;
            return;
        }

        if (e.type == SDL_KEYDOWN)
        {
            if (e.key.keysym.sym == SDLK_ESCAPE)
            {
                status->quit = 1;
            }

            if (status->over)
            {
                return;
            }

            if (e.key.keysym.sym == SDLK_a)
            {
                player->tox = -1;
            }

            if (e.key.keysym.sym == SDLK_d)
            {
                player->tox = 1;
            }

            if (e.key.keysym.sym == SDLK_w)
            {
                player->toy = -1;
            }

            if (e.key.keysym.sym == SDLK_s)
            {
                player->toy = 1;
            }

            if (e.key.keysym.sym == SDLK_SPACE)
            {
            }
        }

        if (e.type == SDL_KEYUP)
        {
            if (e.key.keysym.sym == SDLK_r)
            {
                if (status->over)
                {
                    init_player();
                    init_enemys();
                    init_bullets();
                    status->over = 0;
                    status->time = 0;
                }
            }

            if (status->over)
            {
                return;
            }

            if (e.key.keysym.sym == SDLK_p)
            {
                status->paused = !status->paused;
            }

            if (e.key.keysym.sym == SDLK_a)
            {
                player->tox = 0;
            }

            if (e.key.keysym.sym == SDLK_d)
            {
                player->tox = 0;
            }

            if (e.key.keysym.sym == SDLK_w)
            {
                player->toy = 0;
            }

            if (e.key.keysym.sym == SDLK_s)
            {
                player->toy = 0;
            }

            if (e.key.keysym.sym == SDLK_SPACE)
            {
                make_bullet();
            }
        }
    }
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

    // triggers the program that controls
    // your graphics hardware and sets flags

    // creates a renderer to render our images
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    renderer = SDL_CreateRenderer(window, -1, render_flags);
    // SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);

    // TTF_Font *font = TTF_OpenFont("./fonts/white-rabbit.TTF", 25);
    font = TTF_OpenFont("./fonts/white-rabbit.TTF", 25);
    bigfont = TTF_OpenFont("./fonts/white-rabbit.TTF", 50);

    // SDL_Delay(3000);

    // looping for event with input
    while (!status->quit)
    {
        ProcessEvents();
        update();
        draw();
        SDL_Delay(1000 / 60);
    }

    /// Freeing resources
    TTF_CloseFont(font);
    TTF_CloseFont(bigfont);
    TTF_Quit();
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}