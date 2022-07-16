/*
   FreeMine - a free Windows minesweeper clone written on C with SDL2
   Copyright © anton2920, 2019-2022

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

#include "mines.h"


struct SDL_Texture *getTexture(struct SDL_Renderer *renderer, char *path_to_pic)
{
    struct SDL_Surface *Image = IMG_Load(path_to_pic);
    assert(Image != NULL);

    /* SDL_SetColorKey(Image, SDL_TRUE, SDL_MapRGB(Image->format, 255, 255, 255)); */
    struct SDL_Texture *Texture = SDL_CreateTextureFromSurface(renderer, Image);
    assert(Texture != NULL);

    SDL_FreeSurface(Image);

    return Texture;
}


void Draw_frame(struct SDL_Renderer *renderer, struct SDL_Texture *frameTexture, enum field_size size)
{
    struct SDL_Rect frameRect = {0, 0, 0, 0};
    struct SDL_Rect fRect = {0, 0, 0, 0};

    if (size == small) {
        frameRect.w = small_width;
        frameRect.h = small_height;
        fRect.y = WIN_PANEL;
        fRect.w = 161 - 3;
        fRect.h = 236 - fRect.y;
    } else if (size == medium) {
        frameRect.w = medium_width;
        frameRect.h = medium_height;
        fRect.y = LINUX_PANEL;
        fRect.w = 280 - fRect.x;
        fRect.h = 365 - fRect.y;
    } else if (size == large) {
        frameRect.w = large_width;
        frameRect.h = large_height;
        fRect.y = LINUX_PANEL;
        fRect.w = 503 - fRect.x;
        fRect.h = 365 - fRect.y;
    }

    SDL_RenderCopy(renderer, frameTexture, &fRect, &frameRect);
}


void Draw_field(struct SDL_Renderer *renderer, struct SDL_Texture *fieldStuff, struct game_field *fld)
{
    int i, j;
    struct SDL_Rect bTile = {0, 0, tile_w, tile_h};
    struct SDL_Rect currT = {0, 0, tile_w, tile_h};

    for (i = 0; i < fld->tiles_y; ++i) {
        for (j = 0; j < fld->tiles_x; ++j) {
            bTile.x = fld->fld[i][j].rect.x;
            bTile.y = fld->fld[i][j].rect.y;
            if (fld->fld[i][j].check != pressed) {
                currT.x = (fld->fld[i][j].check == unchecked) ? unchecked_x :
                          (fld->fld[i][j].check == hovered) ? pressed_x :
                          (fld->fld[i][j].check == flaggy) ? flaggy_x :
                          (fld->fld[i][j].check == question) ? question_x : hovered_question_x;
                currT.y = (fld->fld[i][j].check == unchecked) ? unchecked_y :
                          (fld->fld[i][j].check == hovered) ? pressed_y :
                          (fld->fld[i][j].check == flaggy) ? flaggy_y :
                          (fld->fld[i][j].check == question) ? question_y : hovered_question_y;
            } else {
                if (fld->fld[i][j].type == miny) {
                    currT.x = mine_black_x;
                    currT.y = mine_y;
                } else if (fld->fld[i][j].type == miny_cross) {
                    currT.x = mine_crossed_x;
                    currT.y = mine_y;
                } else if (fld->fld[i][j].type == miny_red) {
                    currT.x = mine_red_x;
                    currT.y = mine_y;
                } else if (fld->fld[i][j].type == digity) {
                    currT.x = (fld->fld[i][j].digit == 1) ? digit_1_x : (fld->fld[i][j].digit == 2) ? digit_2_x:
                              (fld->fld[i][j].digit == 3) ? digit_3_x : (fld->fld[i][j].digit == 4) ? digit_4_x :
                              (fld->fld[i][j].digit == 5) ? digit_5_x : (fld->fld[i][j].digit == 6) ? digit_6_x :
                              (fld->fld[i][j].digit == 7) ? digit_7_x : (fld->fld[i][j].digit == 8) ? digit_8_x :
                              pressed_x;
                    currT.y = digity_y;
                } else {
                    currT.x = pressed_x;
                    currT.y = pressed_y;
                }
            }
            if (!fld->is_clr_on) {
                currT.x += BW_OFFSET;
            }
            SDL_RenderCopy(renderer, fieldStuff, &currT, &bTile);
        }
    }
}


void Draw_timerface(struct SDL_Renderer *renderer, struct game_field *fld, struct SDL_Texture *timerText,
                    enum field_size size, size_t currtime, int minesleft, enum face_state fc)
{
    struct SDL_Rect bTile = {0, 0, digit_w, digit_h};
    struct SDL_Rect currT = {0, 0, digit_w, digit_h};
    int digit, i, timer_x_o = 0, timer_y_o = 0, mines_x_o = 0, mines_y_o = 0, face_x_o = 0, face_y_o = 0;

    if (size == small) {
        timer_x_o = small_timer_last_x_offset;
        timer_y_o = small_timer_last_y_offset;
        mines_x_o = small_mines_last_x_offset;
        mines_y_o = small_mines_last_y_offset;
        face_x_o = small_face_x_offset;
        face_y_o = small_face_y_offset;
    } else if (size == medium) {
        timer_x_o = medium_timer_last_x_offset;
        timer_y_o = medium_timer_last_y_offset;
        mines_x_o = medium_mines_last_x_offset;
        mines_y_o = medium_mines_last_y_offset;
        face_x_o = medium_face_x_offset;
        face_y_o = medium_face_y_offset;
    } else if (size == large) {
        timer_x_o = large_timer_last_x_offset;
        timer_y_o = large_timer_last_y_offset;
        mines_x_o = large_mines_last_x_offset;
        mines_y_o = large_mines_last_y_offset;
        face_x_o = large_face_x_offset;
        face_y_o = large_face_y_offset;
    }

    bTile.x = timer_x_o;
    bTile.y = timer_y_o;
    for (i = 3; i > 0; --i, currtime /= 10, bTile.x -= digit_w) {
        digit = currtime % 10;
        currT.x = digit * digit_w;
        if (!fld->is_clr_on) {
            currT.x += BW_OFFSET;
        }
        SDL_RenderCopy(renderer, timerText, &currT, &bTile);
    }

    bTile.x = mines_x_o;
    bTile.y = mines_y_o;
    if (minesleft >= 0) {
        for (i = 3; i > 0; --i, minesleft /= 10, bTile.x -= digit_w) {
            digit = minesleft % 10;
            currT.x = digit * digit_w;
            if (!fld->is_clr_on) {
                currT.x += BW_OFFSET;
            }
            SDL_RenderCopy(renderer, timerText, &currT, &bTile);
        }
    } else {
        minesleft = abs(minesleft);
        for (i = 2; i > 0; --i, minesleft /= 10, bTile.x -= digit_w) {
            digit = minesleft % 10;
            currT.x = digit * digit_w;
            if (!fld->is_clr_on) {
                currT.x += BW_OFFSET;
            }
            SDL_RenderCopy(renderer, timerText, &currT, &bTile);
        }
        currT.x = 10 * digit_w;
        if (!fld->is_clr_on) {
            currT.x += BW_OFFSET;
        }
        SDL_RenderCopy(renderer, timerText, &currT, &bTile);
    }

    bTile.x = face_x_o;
    bTile.y = face_y_o;
    bTile.w = face_w;
    bTile.h = face_h;
    currT.y = face_y_offset;
    currT.x = (fc == face_normal) ? face_normal_x_offset :
              (fc == face_pressed) ? face_pressed_x_offset :
              (fc == face_o) ? face_o_x_offset :
              (fc == face_dead) ? face_dead_x_offset :
              face_cool_x_offset;
    currT.w = bTile.w;
    currT.h = bTile.h;
    if (!fld->is_clr_on) {
        currT.x += BW_OFFSET;
    }
    SDL_RenderCopy(renderer, timerText, &currT, &bTile);
}


void Draw_menu(struct SDL_Renderer *renderer, struct game_field *fld, struct menu_state *st,
               struct SDL_Texture *menu_game, struct SDL_Texture *select, struct SDL_Texture *tiles)
{
    struct SDL_Rect drawRect = {0, 0, menu_panel_w, menu_panel_h};
    struct SDL_Rect tickRect = {tick_x_offset, tick_black_y_offset, tick_w, tick_h};
    struct SDL_Rect selRect = {0, 0, menu_itm_w, menu_itm_h};

    if (fld->m_state == menu_game_pressed) {
        if (fld->s == small) {
            drawRect.x = small_menu_game_x_offset;
            drawRect.y = small_menu_game_y_offset;
        } else if (fld->s == medium) {
            drawRect.x = medium_menu_game_x_offset;
            drawRect.y = medium_menu_game_y_offset;
        } else if (fld->s == large) {
            drawRect.x = large_menu_game_x_offset;
            drawRect.y = large_menu_game_y_offset;
        }

        SDL_RenderCopy(renderer, menu_game, NULL, &drawRect);

        drawRect.w = tick_w;
        drawRect.h = tick_h;
        drawRect.x = (fld->s == small) ? small_tick_x_offset : (fld->s == medium) ? medium_tick_x_offset
                                                                                  : large_tick_x_offset;
        drawRect.y = (st->menu_i_begginer) ? tick_y_beg : (st->menu_i_intermediate) ? tick_y_interm : tick_y_adv;
        SDL_RenderCopy(renderer, tiles, &tickRect, &drawRect);

        if (st->menu_i_marks) {
            drawRect.y = tick_y_mks;
            SDL_RenderCopy(renderer, tiles, &tickRect, &drawRect);
        }
        if (st->menu_i_color) {
            drawRect.y = tick_y_clr;
            SDL_RenderCopy(renderer, tiles, &tickRect, &drawRect);
        }
        if (st->menu_i_sound) {
            drawRect.y = tick_y_snd;
            SDL_RenderCopy(renderer, tiles, &tickRect, &drawRect);
        }

        if (st->is_hovered != NOT_HOVERED) {
            if (fld->s == small) {
                drawRect.x = small_menu_game_x_offset;
                drawRect.y = small_menu_game_y_offset;
            } else if (fld->s == medium) {
                drawRect.x = medium_menu_game_x_offset;
                drawRect.y = medium_menu_game_y_offset;
            } else if (fld->s == large) {
                drawRect.x = large_menu_game_x_offset;
                drawRect.y = large_menu_game_y_offset;
            }

            drawRect.x += menu_itm_add_y_offset;

            tickRect.y = tick_white_y_offset;
            drawRect.w = menu_itm_w;
            drawRect.h = menu_itm_h;

            drawRect.y += (!st->is_hovered) ? (menu_new_y_offset) :
                          (st->is_hovered == 1) ? (menu_beg_y_offset) :
                          (st->is_hovered == 2) ? (menu_interm_y_offset) :
                          (st->is_hovered == 3) ? (menu_adv_y_offset) :
                          (st->is_hovered == 4) ? (menu_mks_y_offset) :
                          (st->is_hovered == 5) ? (menu_clr_y_offset) :
                          (st->is_hovered == 6) ? (menu_snd_y_offset) :
                          (st->is_hovered == 7) ? (menu_tabl_y_offset) :
                          (menu_exit_y_offset);
            selRect.y = st->is_hovered * menu_itm_h;
            SDL_RenderCopy(renderer, select, &selRect, &drawRect);

            drawRect.w = tick_w;
            drawRect.h = tick_h;
            drawRect.x = (fld->s == small) ? small_tick_x_offset : (fld->s == medium) ? medium_tick_x_offset
                                                                                      : large_tick_x_offset;
            drawRect.y = (st->menu_i_begginer && st->is_hovered == 1) ? tick_y_beg :
                         (st->menu_i_intermediate && st->is_hovered == 2) ? tick_y_interm :
                         (st->menu_i_advanced && st->is_hovered == 3) ? tick_y_adv : 0;
            if (st->is_hovered >= 1 && st->is_hovered <= 3 && drawRect.y) {
                SDL_RenderCopy(renderer, tiles, &tickRect, &drawRect);
            }

            if (st->menu_i_marks && st->is_hovered == 4) {
                drawRect.y = tick_y_mks;
                SDL_RenderCopy(renderer, tiles, &tickRect, &drawRect);
            }
            if (st->menu_i_color && st->is_hovered == 5) {
                drawRect.y = tick_y_clr;
                SDL_RenderCopy(renderer, tiles, &tickRect, &drawRect);
            }
            if (st->menu_i_sound && st->is_hovered == 6) {
                drawRect.y = tick_y_snd;
                SDL_RenderCopy(renderer, tiles, &tickRect, &drawRect);
            }
        }
    }
}
