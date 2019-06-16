/*
FreeMine — a free Windows minesweeper clone written on C with SDL2
Copyright © Pavlovsky Anton, 2019

This file is part of FreeMine.

FreeMine is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

FreeMine is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with FreeMine. If not, see <https://www.gnu.org/licenses/>.
*/

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
        if ((*window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, small_width, small_height, SDL_WINDOW_SHOWN)) == NULL) {
            return __false;
        } else if ((*renderer = SDL_CreateRenderer(*window, -1, 0)) == NULL) {
            return __false;
        }
    } else if (s == medium) {
        if ((*window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, medium_width, medium_height, SDL_WINDOW_SHOWN)) == NULL) {
            return __false;
        } else if ((*renderer = SDL_CreateRenderer(*window, -1, 0)) == NULL) {
            return __false;
        }
    } else if (s == large) {
        if ((*window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, large_width, large_height, SDL_WINDOW_SHOWN)) == NULL) {
            return __false;
        } else if ((*renderer = SDL_CreateRenderer(*window, -1, 0)) == NULL) {
            return __false;
        }
    }

	/* Returning value */
	return __false;
}

__bool Field_init(struct game_field *fld, enum field_size s) {

    /* Initializing variables */
    auto int i, j;

    /* Main part */
    fld->s = s;
    if (s == small) {
        fld->tiles_x = small_tiles_x;
        fld->tiles_y = small_tiles_y;
    } else if (s == medium) {
        fld->tiles_x = medium_tiles_x;
        fld->tiles_y = medium_tiles_y;
    } else if (s == large) {
        fld->tiles_x = large_tiles_x;
        fld->tiles_y = large_tiles_y;
    }

    fld->g_state = game_off;
    fld->m_state = menu_off;

    if ((fld->fld = (block **) malloc(fld->tiles_y * sizeof(block *))) == NULL) {
        return __false;
    }
    for (i = 0; i < fld->tiles_y; ++i) {
        if ((fld->fld[i] = (block *) malloc(sizeof(block) * fld->tiles_x)) == NULL) {
            return __false;
        }
    }
    for (i = 0; i < fld->tiles_y; ++i) {
        for (j = 0; j < fld->tiles_x; ++j) {
            fld->fld[i][j].type = nothing;
            fld->fld[i][j].check = unchecked;
            fld->fld[i][j].digit = 0;
            fld->fld[i][j].rect.x = j * tile_w + j + ((s == small) ? small_tiles_x_offset : (s == medium) ? medium_tiles_x_offset : large_tiles_x_offset);
            fld->fld[i][j].rect.y = i * tile_h + i + ((s == small) ? small_tiles_y_offset : (s == medium) ? medium_tiles_y_offset : large_tiles_y_offset);
            fld->fld[i][j].rect.w = tile_w;
            fld->fld[i][j].rect.h = tile_h;
        }
    }

    /* Returning value */
    return __true;
}

void Field_destroy(struct game_field *fld, enum field_size s) {

    /* Initializing variables */
    auto int i;

    /* Main part */
    for (i = 0; i < fld->tiles_y; ++i) {
        free(fld->fld[i]);
    }

    free(fld->fld);
}

enum field_size Get_Size(int argc, const char *argv[]) {

    /* Main part */
    return (argc != 2) ? small : (**(argv + 1) == '0') ? small : (**(argv + 1) == '1') ? medium : (**(argv + 1) == '2') ? large : -1;
}

int mines_l(enum field_size s) {

    /* Returning value */
    return (s == small) ? small_nmines : (s == medium) ? medium_nmines : large_nmines;
}

block *get_clicked_block(struct game_field *fld, int x, int y) {

    /* Initializing variables */
    auto int i, j;

    /* Main part */
    for (i = 0; i < fld->tiles_y; ++i) {
        for (j = 0; j < fld->tiles_x; ++j) {
            if (x >= fld->fld[i][j].rect.x - tile_space && x <= fld->fld[i][j].rect.x + fld->fld[i][j].rect.w &&
                    y >= fld->fld[i][j].rect.y - tile_space && y <= fld->fld[i][j].rect.y + fld->fld[i][j].rect.h) {
                return fld->fld[i] + j;
            }
        }
    }

    /* Returning value */
    return NULL;
}

enum check_type switch_block_check_type(block *blk, enum mbtn b, __bool is_mks) {

    /* Main part */
    if (b == mbtn_right) {
        if (is_mks) {
            if (blk->check != pressed) {
                blk->check = (blk->check == unchecked) ? flaggy : (blk->check == flaggy) ? question : (blk->check == question) ? unchecked : flaggy;
            }
        } else {
            if (blk->check != pressed) {
                blk->check = (blk->check == unchecked) ? flaggy : (blk->check == flaggy) ? unchecked : flaggy;
            }
        }
    } else if (b == mbtn_left) {
        if (blk->check != flaggy && blk->check != pressed) {
            blk->check = pressed;
        }
    }

    /* Returning value */
    return blk->check;
}

void Block_untoggle_hovered(struct game_field *fld) {

    /* Initializing variables */
    auto int i, j;

    /* Main part */
    for (i = 0; i < fld->tiles_y; ++i) {
        for (j = 0; j < fld->tiles_x; ++j) {
            if (fld->fld[i][j].check == hovered) {
                fld->fld[i][j].check = unchecked;
            } else if (fld->fld[i][j].check == hovered_question) {
                fld->fld[i][j].check = question;
            }
        }
    }
}

__bool is_hit_face(enum field_size s, int x, int y) {

    /* Main part */
    if (s == small) {
        return (x >= small_face_x_offset && x <= small_face_x_offset + face_w &&
                y >= small_face_y_offset && y <= small_face_y_offset + face_h) ? __true : __false;
    } else if (s == medium) {
        return (x >= medium_face_x_offset && x <= medium_face_x_offset + face_w &&
                y >= medium_face_y_offset && y <= medium_face_y_offset + face_h) ? __true : __false;
    } else if (s == large) {
        return (x >= large_face_x_offset && x <= large_face_x_offset + face_w &&
                y >= large_face_y_offset && y <= large_face_y_offset + face_h) ? __true : __false;
    }

	/* Returning value */
	return __false;
}

void two_btns(struct game_field *fld, int x, int y) {

    /* Initializing variables */
    auto int i = 0, j = 0, i1, j1;
    auto __bool is_found = __false;

    /* Main part */
    for (i = 0; i < fld->tiles_y; ++i) {
        for (j = 0; j < fld->tiles_x; ++j) {
            if (x >= fld->fld[i][j].rect.x && x <= fld->fld[i][j].rect.x + fld->fld[i][j].rect.w &&
                y >= fld->fld[i][j].rect.y && y <= fld->fld[i][j].rect.y + fld->fld[i][j].rect.h) {
                is_found = __true;
                break;
            }
        }
        if (is_found) {
            break;
        }
    }
    if (is_found) {
        for (i1 = ((i - 1 >= 0) ? i - 1 : 0); i1 < ((i + 2 <= fld->tiles_y) ? i + 2 : fld->tiles_y); ++i1) {
            for (j1 = ((j - 1 >= 0) ? j - 1 : 0); j1 < ((j + 2 <= fld->tiles_x) ? j + 2 : fld->tiles_x); ++j1) {
                if (fld->fld[i1][j1].check != flaggy && fld->fld[i1][j1].check != question && fld->fld[i1][j1].check != pressed) {
                    fld->fld[i1][j1].check = hovered;
                } else if (fld->fld[i1][j1].check == question) {
                    fld->fld[i1][j1].check = hovered_question;
                }
            }
        }
    }

}

void Menu_state_init(struct game_field *fld, struct menu_state *st) {

    /* Main part */
    st->menu_i_begginer = (fld->s == small) ? __true : __false;
    st->menu_i_intermediate = (fld->s == medium) ? __true : __false;
    st->menu_i_advanced = (fld->s == large) ? __true : __false;

    st->menu_i_marks = fld->is_mks_on;
    st->menu_i_color = fld->is_clr_on;
    st->menu_i_sound = fld->is_snd_on;

    st->is_hovered = NOT_HOVERED;
}

void Play_music(Mix_Music **fon, char *name) {

    /* Initializing variables */
    *fon = Mix_LoadMUS(name);
    assert(fon != NULL);

    /* Main part */
    Mix_PlayMusic(*fon, -1);

}

void Play_click_sound(struct Mix_Chunk *Sound) {

    /* Main part */
    Mix_PlayChannel(-1, Sound, 0);
}

void Remove_questions(struct game_field *fld) {

    /* Initializing variables */
    auto int i, j;

    /* Main part */
    for (i = 0; i < fld->tiles_y; ++i) {
        for (j = 0; j < fld->tiles_x; ++j) {
            if (fld->fld[i][j].check == question) {
                fld->fld[i][j].check = unchecked;
            }
        }
    }
}

void Open_field(struct game_field *fld, int *currm) {

    /* Initializing variables */
    auto int i, j;

    /* Main part */
    for (i = 0; i < fld->tiles_y; ++i) {
        for (j = 0; j < fld->tiles_x; ++j) {
            if (fld->fld[i][j].type != miny) {
                fld->fld[i][j].check = pressed;
            }
        }
    }

    Check_for_win(fld, currm);
}

__bool R_u_s(void) {

    /* Initializing variables */
    auto int func;

    /* I/O flow */
    printf("Are you sure you want to cheat and win? [Y/n]: ");
    func = getchar();

    /* Main part */
    if (func == 'Y' || func == 'y' || func == '\n') {
        if (func != '\n') {
            while (getchar() != '\n')
                ;
        }
    } else {
        while (getchar() != '\n')
            ;
        return __false;
    }

    /* Returning value */
    return __true;
}