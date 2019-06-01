#include "../headers/mines.h"

__bool SDL_Init_All() {

    /* SDL2 */
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    TTF_Init();
    Mix_Init(0);

    /* Returning value */
    return __true;
}

__bool Init_window(struct SDL_Window **window, struct SDL_Renderer **renderer,
        enum field_size s) {

    /* Main part */
    if (s == small) {
        if ((*window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, small_width, small_height, SDL_WINDOW_SHOWN)) < 0) {
            return __false;
        } else if (!(*renderer = SDL_CreateRenderer(*window, -1, 0))) {
            return __false;
        }
    }
}

__bool Field_init(struct game_field *fld, enum field_size s) {

    /* Initializing variables */
    auto int i, j;

    /* Main part */
    if (s == small) {
        fld->s = s;
        if ((fld->fld = (block **) malloc(small_tiles_y * sizeof(block *))) == NULL) {
            return __false;
        }
        for (i = 0; i < small_tiles_y; ++i) {
            fld->fld[i] = (block *) malloc(sizeof(block) * small_tiles_x);
        }

        for (i = 0; i < small_tiles_y; ++i) {
            for (j = 0; j < small_tiles_x; ++j) {
                fld->fld[i][j].type = nothing;
                fld->fld[i][j].check = unchecked;
                fld->fld[i][j].digit = 0;
                fld->fld[i][j].rect.x = j * tile_w + small_tiles_x_offset + j;
                fld->fld[i][j].rect.y = i * tile_h + small_tiles_y_offset + i;
                fld->fld[i][j].rect.w = tile_w;
                fld->fld[i][j].rect.h = tile_h;
            }
        }
    }

    /* Returning value */
    return __true;
}

void Field_destroy(struct game_field *fld, enum field_size s) {

    /* Initializing variables */
    auto int i;

    /* Main part */
    if (s == small) {
        for (i = 0; i < small_tiles_y; ++i) {
            free(fld->fld[i]);
        }
    }

    free(fld->fld);
}

enum field_size Get_Size(const char *argv[]) {

    /* Main part */
    return (**(argv + 1) == '0') ? small : (**(argv + 1) == '1') ? medium : large;
}

int mines_l(enum field_size s) {

    /* Returning value */
    return (s == small) ? small_nmines : (s == medium) ? 0 : 1; /* Rewrite */
}

block **get_clicked_block(struct game_field *fld, int x, int y) {

    /* Initializing variables */
    auto int i, j;

    /* Main part */
    if (fld->s == small) {
        for (i = 0; i < small_tiles_y; ++i) {
            for (j = 0; j < small_tiles_x; ++j) {
                if (x >= fld->fld[i][j].rect.x && x <= fld->fld[i][j].rect.x + fld->fld[i][j].rect.w &&
                        y >= fld->fld[i][j].rect.y && y <= fld->fld[i][j].rect.y + fld->fld[i][j].rect.h) {
                    return fld->fld + i * small_tiles_y + j;
                }
            }
        }
    }

    /* Returning value */
    return NULL;
}

void switch_block_check_type(block **blk, enum mbtn b) {

    /* Main part */
    if (b == mbtn_right) {
        if ((*blk)->check != pressed) {
            (*blk)->check = ((*blk)->check == unchecked) ? flaggy : ((*blk)->check == flaggy) ? question : unchecked;
        }
    } else if (b == mbtn_left) {
        if ((*blk)->check != flaggy && (*blk)->check != pressed) {
            (*blk)->check = pressed;
        }
    }
}