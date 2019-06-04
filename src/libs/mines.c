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

void Spawn_mines(struct game_field *fld, block *obj) {

    /* Initializing variables */
    auto int i = 0, j = 0, k;

    /* Main part */
    for (k = 0; k < mines_l(fld->s); ++k) {
        i = rand() % fld->tiles_y;
        j = rand() % fld->tiles_x;
        if (fld->fld[i][j].type != miny && fld->fld[i] + j != obj) {
            fld->fld[i][j].type = miny;
        } else {
            --k;
            continue;
        }
    }

    Count_near(fld);
}

void Count_near(struct game_field *fld) {

    /* Initializing variables */
    auto int i, j, i1, j1, counter = 0;

    /* Main part */
    for (i = 0; i < fld->tiles_y; ++i) {
        for (j = 0; j < fld->tiles_x; ++j) {
            if (fld->fld[i][j].type == miny) {
                continue;
            }
            for (i1 = ((i - 1 >= 0) ? i - 1 : 0); i1 < ((i + 2 <= fld->tiles_y) ? i + 2 : fld->tiles_y); ++i1) {
                for (j1 = ((j - 1 >= 0) ? j - 1 : 0); j1 < ((j + 2 <= fld->tiles_x) ? j + 2 : fld->tiles_x); ++j1) {
                    if (fld->fld[i1][j1].type == miny) {
                        ++counter;
                    }
                    if (i1 == i && j1 == j) {
                        continue;
                    }
                }
            }
            if (counter) {
                fld->fld[i][j].type = digity;
                fld->fld[i][j].digit = counter;
                counter = 0;
            }
        }
    }
}

void Process_press(struct game_field *fld, block *curr_block) {

    /* Main part */
    switch (curr_block->type) {
        case nothing:
            curr_block->check = pressed;
            Open_near_blank(fld, curr_block);
            break;
        case digity:
            curr_block->check = pressed;
            break;
        case miny:
            curr_block->check = pressed;
            curr_block->type = miny_red;
            Uncover_rest_mines(fld);
            fld->g_state = game_lose;
            break;
        default:
            break;
    }
}

void Open_near_blank(struct game_field *fld, block *curr_block) {

    /* Initializing variables */
    auto int i = 0, j = 0, i1, j1;
    auto __bool is_found = __false;

    /* Main part */
    for (i = 0; i < fld->tiles_y; ++i) {
        for (j = 0; j < fld->tiles_x; ++j) {
            if (fld->fld[i] + j == curr_block) {
                is_found = __true;
                if (curr_block->check != flaggy) {
                    curr_block->check = pressed;
                }
                if (curr_block->type == digity) {
                    return;
                }
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
                if ((fld->fld[i1][j1].type == nothing || fld->fld[i1][j1].type == digity) && fld->fld[i1][j1].check != pressed) {
                    Open_near_blank(fld, fld->fld[i1] + j1);
                }
            }
        }
    }
}

void Uncover_rest_mines(struct game_field *fld) {

    /* Initializing variables */
    auto int i, j;

    /* Main part */
    for (i = 0; i < fld->tiles_y; ++i) {
        for (j = 0; j < fld->tiles_x; ++j) {
            if (fld->fld[i][j].type != miny && fld->fld[i][j].check == flaggy) {
                fld->fld[i][j].check = pressed;
                fld->fld[i][j].type = miny_cross;
            } else if (fld->fld[i][j].type == miny && fld->fld[i][j].check == unchecked) {
                fld->fld[i][j].check = pressed;
            }
        }
    }
}

void Check_for_win(struct game_field *fld, int *minesleft) {

    /* Initializing variables */
    auto int i, j;

    /* Main part */
    for (i = 0; i < fld->tiles_y; ++i) {
        for (j = 0; j < fld->tiles_x; ++j) {
            if ((fld->fld[i][j].check != pressed && fld->fld[i][j].check != flaggy &&
                fld->fld[i][j].type != miny) || fld->fld[i][j].type == miny_red) {
                return;
            }
        }
    }


    for (i = 0; i < fld->tiles_y; ++i) {
        for (j = 0; j < fld->tiles_x; ++j) {
            if (fld->fld[i][j].check != pressed && fld->fld[i][j].check != flaggy && fld->fld[i][j].type == miny) {
                fld->fld[i][j].check = flaggy;
                --*minesleft;
            }
        }
    }
    if (!*minesleft) {
        fld->g_state = game_win;
    }
}

void Mine_searcher(struct game_field *fld, block *obj) {

    /* Initializing variables */
    auto int i = 0, j = 0, i1, j1, counter = 0;
    auto __bool is_found = __false, can_search = __false;

    /* Main part */
    for (i = 0; i < fld->tiles_y; ++i) {
        for (j = 0; j < fld->tiles_x; ++j) {
            if (fld->fld[i] + j == obj) {
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
                if (fld->fld[i1][j1].check == flaggy) {
                    ++counter;
                }
            }
        }

        if (counter >= obj->digit) {
            can_search = __true;
        }

        if (can_search) {
            for (i1 = ((i - 1 >= 0) ? i - 1 : 0); i1 < ((i + 2 <= fld->tiles_y) ? i + 2 : fld->tiles_y); ++i1) {
                for (j1 = ((j - 1 >= 0) ? j - 1 : 0); j1 < ((j + 2 <= fld->tiles_x) ? j + 2 : fld->tiles_x); ++j1) {
                    if (fld->fld[i1][j1].check != pressed && fld->fld[i1][j1].check != flaggy) {
                        Process_press(fld, fld->fld[i1] + j1);
                    }
                }
            }
        }
    }
}