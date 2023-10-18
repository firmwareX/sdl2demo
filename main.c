#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "status.h"
#include "sprite.h"

SDL_Window *window;
TTF_Font *font;
SDL_Renderer *renderer;
SDL_Surface *surface;
SDL_Texture *texture;

Status *status;
Sprite *player;

int time = 0;

void update()
{
    time += 1;
    if (time % 60 == 0)
    {
        player->x += player->tox;
        player->y += player->toy;
    }
}

void draw()
{
    SDL_RenderClear(renderer);
    SDL_Color color = {255, 255, 255};
    // 🛸
    surface = TTF_RenderText_Solid(font,
                                   "M", color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);

    // int texW = 0;
    // int texH = 0;
    // SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = {player->x, player->y, player->w, player->h};

    // SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);
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

            if (e.key.keysym.sym == SDLK_a)
            {
                player->tox = player->speed * -1;
            }

            if (e.key.keysym.sym == SDLK_d)
            {
                player->tox = player->speed;
            }

            if (e.key.keysym.sym == SDLK_w)
            {
                player->y = player->speed * -1;
            }

            if (e.key.keysym.sym == SDLK_s)
            {
                player->y = player->speed;
            }

            if (e.key.keysym.sym == SDLK_SPACE)
            {
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
                    player->y = 0;
                }

                if (e.key.keysym.sym == SDLK_s)
                {
                    player->y = 0;
                }
            }
        }

        if (e.type == SDL_KEYUP)
        {
        }
    }
}

int main(int argc, char *argv[])
{
    status = Status_New();
    player = Sprite_New(10, 10, 20, 20);

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
                              SDL_WINDOWPOS_CENTERED, 200, 200,
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

    ///
    /// Section 2: SDL image loader
    ///

    ///
    /// Section 4: SDL ttf and rendering text
    ///

    ///
    /// Section 3: Game Loop and Basic Controls
    ///

    // We add a delay in order to see that our window
    // has successfully popped up.
    // SDL_Delay(3000);

    ///
    /// Section 5: Freeing resources
    ///

    // looping for event with input
    while (!status->quit)
    {
        ProcessEvents();
        update();
        draw();
    }
    // destroy our window

    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);
    SDL_Quit();
}