#include "headers/mines.h"

int main(int argc, const char *argv[], const char *envp[]) {

    /* Initializing variables */
    assert(argc == 2);

    auto __bool quit = __false;
    auto union SDL_Event event;

    auto struct SDL_Window *window;
    auto struct SDL_Renderer *renderer;

    auto struct SDL_Texture *frameTexture;
    auto struct SDL_Texture *tilesTexture;

    auto struct game_field field;
    auto enum field_size s = Get_Size(argv);

    auto size_t starttime = time(NULL), currtime = starttime;
    auto int minesleft;

    auto block **curr_block;


    /* Main part. SDL2 */
    SDL_Init_All();
    if (Init_window(&window, &renderer, s)) {
        Field_init(&field, s);
        minesleft = mines_l(s);
        frameTexture = getTexture(renderer, FRAME_PATH);
        tilesTexture = getTexture(renderer, TILES_PATH);
        while (!quit) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    quit = __true;
                }
                if (event.type == SDL_MOUSEBUTTONDOWN) {
                    if (event.button.button == SDL_BUTTON_RIGHT) {
                        curr_block = get_clicked_block(&field, event.button.x, event.button.y);
                        if (curr_block != NULL) {
                            switch_block_check_type(curr_block, mbtn_right);
                        }
                    }
                }
            }
            currtime = time(NULL);
            SDL_RenderClear(renderer);
            Draw_frame(renderer, frameTexture, field.s);
            Draw_field(renderer, tilesTexture, &field);
            Draw_timerface(renderer, tilesTexture, s, currtime - starttime, minesleft);
            SDL_RenderPresent(renderer);
            SDL_Delay(DELAY_TIME);
        }
    } else {
        fprintf(stderr, "Error! Something went wrong: %s\n", SDL_GetError());
    }

    /* Releasing memory */
    Field_destroy(&field, s);
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();

    /* Returning value */
    return 0;
}