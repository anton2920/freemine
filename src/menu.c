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

#include <stdbool.h>

#include "mines.h"


enum menu_panel Is_menu_pressed(struct game_field *fld, int x, int y, int pr_u)
{
    enum menu_panel mp = menu_off;

    if (fld->s == small) {
        mp = (x >= small_menu_game_x_offset && x <= small_menu_game_x_offset + menu_btn_w &&
              y >= small_menu_game_y_offset && y <= small_menu_game_y_offset + menu_btn_h) ? menu_game_pressed
                                                                                           : menu_off;
    } else if (fld->s == medium) {
        mp = (x >= medium_menu_game_x_offset && x <= medium_menu_game_x_offset + menu_btn_w &&
              y >= medium_menu_game_y_offset && y <= medium_menu_game_y_offset + menu_btn_h) ? menu_game_pressed
                                                                                             : menu_off;
    } else if (fld->s == large) {
        mp = (x >= large_menu_game_x_offset && x <= large_menu_game_x_offset + menu_btn_w &&
              y >= large_menu_game_y_offset && y <= large_menu_game_y_offset + menu_btn_h) ? menu_game_pressed
                                                                                           : menu_off;
    }

    if (mp == menu_game_pressed && pr_u == UNPRESS) {
        return mp;
    }

    return (mp == menu_game_pressed && fld->m_state != menu_game_pressed && pr_u == PRESS) ? mp : menu_off;
}


bool Check_hover(enum field_size s, struct menu_state *st, int x, int y)
{
    bool is_changed = false;
    int x_o = (s == small) ? small_menu_game_x_offset : (s == medium) ? medium_menu_game_x_offset
                                                                      : large_menu_game_x_offset;

    /* Main part */
    if (x >= x_o && x <= x_o + menu_itm_w) {
        if (y >= menu_new_y_offset && y <= menu_new_y_offset + menu_itm_h) {
            st->is_hovered = 0;
            is_changed = true;
        } else if (y >= menu_beg_y_offset && y <= menu_beg_y_offset + menu_itm_h) {
            st->is_hovered = 1;
            is_changed = true;
        } else if (y >= menu_interm_y_offset && y <= menu_interm_y_offset + menu_itm_h) {
            st->is_hovered = 2;
            is_changed = true;
        } else if (y >= menu_adv_y_offset && y <= menu_adv_y_offset + menu_itm_h) {
            st->is_hovered = 3;
            is_changed = true;
        } else if (y >= menu_mks_y_offset && y <= menu_mks_y_offset + menu_itm_h) {
            st->is_hovered = 4;
            is_changed = true;
        } else if (y >= menu_clr_y_offset && y <= menu_clr_y_offset + menu_itm_h) {
            st->is_hovered = 5;
            is_changed = true;
        } else if (y >= menu_snd_y_offset && y <= menu_snd_y_offset + menu_itm_h) {
            st->is_hovered = 6;
            is_changed = true;
        } else if (y >= menu_tabl_y_offset && y <= menu_tabl_y_offset + menu_itm_h) {
            st->is_hovered = 7;
            is_changed = true;
        } else if (y >= menu_exit_y_offset && y <= menu_exit_y_offset + menu_itm_h) {
            st->is_hovered = 8;
            is_changed = true;
        } else {
            st->is_hovered = NOT_HOVERED;
        }
    } else {
        st->is_hovered = NOT_HOVERED;
    }

    return is_changed;
}


int Process_menu_press(struct menu_state *st)
{
    switch (st->is_hovered) {
        case 0:
        case 7:
        case 8:
            break;
        case 1:
            st->menu_i_begginer = true;
            st->menu_i_intermediate = false;
            st->menu_i_advanced = false;
            break;
        case 2:
            st->menu_i_intermediate = true;
            st->menu_i_begginer = false;
            st->menu_i_advanced = false;
            break;
        case 3:
            st->menu_i_advanced = true;
            st->menu_i_begginer = false;
            st->menu_i_intermediate = false;
            break;
        case 4:
            st->menu_i_marks = REVERSE_BOOL(st->menu_i_marks);
            break;
        case 5:
            st->menu_i_color = REVERSE_BOOL(st->menu_i_color);
            break;
        case 6:
            st->menu_i_sound = REVERSE_BOOL(st->menu_i_sound);
            break;
        default:
            break;
    }

    return st->is_hovered;
}
