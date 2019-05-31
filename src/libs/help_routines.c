#include "../headers/mines.h"

__bool SDL_Init_All() {

    /* SDL2 */
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    TTF_Init();
    Mix_Init(0);

    /* Returning value */
    return __true;
}

__bool Init_custom_window(struct SDL_Window **window, struct SDL_Renderer **renderer,
        size_t width, size_t height) {

    /* Main part */
    if ((*window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN)) < 0) {
        return __false;
    } else if (!(*renderer = SDL_CreateRenderer(*window, -1, 0))) {
        return __false;
    }
}

__bool Init_small_window(struct SDL_Window **window, struct SDL_Renderer **renderer) {

    /* Main part */
    return Init_custom_window(window, renderer, small_width, small_height);
}