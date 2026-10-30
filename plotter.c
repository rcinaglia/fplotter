#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>

#define DENSITY 5
#define GRID_DENSITY (DENSITY * 10)

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

void draw_grid(SDL_Renderer *renderer)
{
    int cols = WINDOW_WIDTH / GRID_DENSITY;
    int rows = WINDOW_HEIGHT / GRID_DENSITY;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 25);

    for (int j = 1; j <= cols; j++)
    {
        SDL_RenderDrawLine(renderer, GRID_DENSITY * j, 0, (GRID_DENSITY * j) + 1, WINDOW_HEIGHT);
    }

    for (int i = 1; i <= rows; i++)
    {
        SDL_RenderDrawLine(renderer, 0, GRID_DENSITY * i, WINDOW_WIDTH, (GRID_DENSITY * i) + 1);
    }
}

void draw_text(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, const char *text, int x, int y)
{
    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text, color);

    if (surface == NULL)
        return;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect dest = {x - (surface->w / 2), y - (surface->h / 2), surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dest);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void draw_xy_axis(SDL_Renderer *renderer)
{
    TTF_Font *font = TTF_OpenFont("tinos-italic.ttf", 24);
    SDL_Color color = (SDL_Color){255, 255, 255, 180};

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 180);

    // x
    SDL_RenderDrawLine(renderer, WINDOW_WIDTH / 2, 0, (WINDOW_WIDTH / 2), WINDOW_HEIGHT);
    draw_text(renderer, font, color, "x", WINDOW_WIDTH - 50, WINDOW_HEIGHT / 2 - 50);
    // y
    SDL_RenderDrawLine(renderer, 0, WINDOW_HEIGHT / 2, WINDOW_WIDTH, (WINDOW_HEIGHT / 2));
    draw_text(renderer, font, color, "y", WINDOW_WIDTH / 2 + 48, 12);
}

void draw_xy_scale(SDL_Renderer *renderer)
{
    TTF_Font *font = TTF_OpenFont("tinos-italic.ttf", 18);
    SDL_Color color = (SDL_Color){255, 255, 255, 180};
    double max_x = ((WINDOW_WIDTH / 2) / GRID_DENSITY);
    int wx;
    for (int i = -max_x; i <= max_x; i++)
    {
        const int n = i + max_x;
        char str[20];
        snprintf(str, sizeof(int), "%d", i);
        if (i != 0)
        {

            draw_text(renderer, font, color, str, n * (GRID_DENSITY), (WINDOW_HEIGHT / 2) + 24);
        }
        SDL_RenderDrawLine(renderer, n * GRID_DENSITY, (WINDOW_HEIGHT / 2) - 5, (n * GRID_DENSITY), (WINDOW_HEIGHT / 2) + 5);
    }

    double max_y = ((WINDOW_HEIGHT / 2) / GRID_DENSITY);
    for (int i = max_y; i >= -max_y; i--)
    {
        const int n = max_y - i;
        char str[20];
        snprintf(str, sizeof(int), "%d", i);
        if (i != 0)
        {
            draw_text(renderer, font, color, str, (WINDOW_WIDTH / 2) + 24, n * (GRID_DENSITY));
        }
        SDL_RenderDrawLine(renderer, WINDOW_WIDTH / 2 - 5, n * GRID_DENSITY, WINDOW_WIDTH / 2 + 5, n * GRID_DENSITY);
    }
}

// function to plot
double f(double x)
{
    return sin(1 / x);
}

void draw_function_plot(SDL_Renderer *renderer)
{
    double x1, x2, y1, y2;
    double origin_x = WINDOW_WIDTH / 2;
    double origin_y = WINDOW_HEIGHT / 2;
    for (int px = 0; px < WINDOW_WIDTH; px++)
    {
        x1 = (px - origin_x) / GRID_DENSITY;
        x2 = ((px + 1) - origin_x) / GRID_DENSITY;

        y1 = f(x1);
        y2 = f(x2);

        // non renderizzare comportamenti strani come NaN, INFINITY o -INFINITY
        if (!isfinite(y1) || !isfinite(y2))
            continue;

        int py1 = origin_y - (y1 * GRID_DENSITY);
        int py2 = origin_y - (y2 * GRID_DENSITY);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(renderer, px, py1, px + 1, py2);
    }
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_Window *pwindow = SDL_CreateWindow("Math function plotter in C", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    SDL_Event event;



    SDL_Renderer *renderer = SDL_CreateRenderer(pwindow, -1, 0);

    draw_grid(renderer);
    draw_xy_axis(renderer);
    draw_xy_scale(renderer);
    draw_function_plot(renderer);

    SDL_RenderPresent(renderer);

    bool is_app_running = true;
    while (is_app_running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                is_app_running = false;
        }
    }
}
