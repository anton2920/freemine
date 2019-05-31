#include "headers/mines.h"

int main(int argc, const char *argv[], const char *envp[]) {

    /* Initializing variables */
    auto __bool quit = __false;
    auto union SDL_Event event;
    auto struct SDL_Window *window;
    auto struct SDL_Renderer *renderer;

    /* Main part. SDL2 */
    SDL_Init_All();
    if (Init_small_window(&window, &renderer)) {
        while (!quit) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    quit = __true;
                }
            }

            Draw_field(renderer);
            Draw_timerface(renderer);
        }
    } else {
        fprintf(stderr, "Error! Something went wrong: %s\n", SDL_GetError());
    }

    /* Releasing memory */
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();

    /* Returning value */
    return 0;
}