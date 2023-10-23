#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "sprite.h"

TTF_Font *font;
TTF_Font *bigfont;
SDL_Renderer *renderer;
SDL_Surface *surface;
SDL_Texture *texture;

void draw_init(SDL_Window *window)
{
    font = TTF_OpenFont("./fonts/white-rabbit.TTF", 25);
    bigfont = TTF_OpenFont("./fonts/white-rabbit.TTF", 50);
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    renderer = SDL_CreateRenderer(window, -1, render_flags);
}

void draw_clear()
{
    SDL_RenderClear(renderer);
}

void draw_paused(int WIDTH, int HEIGHT)
{
    SDL_Color color = {255, 0, 0};
    surface = TTF_RenderText_Solid(bigfont,
                                   "PAUSED", color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);

    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = {WIDTH / 2 - texW / 2, HEIGHT / 2 - texH / 2, texW, texH};
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);
};

void draw_gameover(int WIDTH, int HEIGHT)
{
    SDL_Color color = {255, 0, 0};
    surface = TTF_RenderText_Solid(bigfont,
                                   "GAME OVER", color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);

    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = {WIDTH / 2 - texW / 2, HEIGHT / 2 - texH / 2, texW, texH};
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);
};

void draw_score(int _score)
{
    SDL_Color color = {255, 255, 255};
    char score[1000];
    sprintf(score, "SCORE:%d", _score);
    surface = TTF_RenderText_Solid(font,
                                   score, color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);

    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect score_dstrect = {10, 10, texW, texH};

    SDL_RenderCopy(renderer, texture, NULL, &score_dstrect);
};

void draw_sprite(Sprite *s)
{
    SDL_Color color = {255, 255, 255};
    // 🛸
    surface = TTF_RenderText_Solid(font,
                                   s->data, color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    // int texW = 0;
    // int texH = 0;
    // SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = {s->x, s->y, s->w, s->h};
    // SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);
};

void draw_show()
{
    SDL_RenderPresent(renderer);
}

void draw_destroy()
{
    TTF_CloseFont(font);
    TTF_CloseFont(bigfont);
    TTF_Quit();
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    SDL_DestroyRenderer(renderer);
}