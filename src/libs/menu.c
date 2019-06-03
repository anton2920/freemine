#include "../headers/mines.h"

enum menu_panel Is_menu_pressed(struct game_field *fld, int x, int y, int pr_u, enum menu_panel press_change) {

    /* Initializing variables */
    auto enum menu_panel mp = menu_off, local_test = menu_off;

    /* Main part */
    if (fld->s == small) {
        mp = (x >= small_menu_game_x_offset && x <= small_menu_game_x_offset + menu_btn_w &&
            y >= small_menu_game_y_offset && y <= small_menu_game_y_offset + menu_btn_h) ? menu_game_pressed : menu_off;
    } else if (fld->s == medium) {
        mp = (x >= medium_menu_game_x_offset && x <= medium_menu_game_x_offset + menu_btn_w &&
                y >= medium_menu_game_y_offset && y <= medium_menu_game_y_offset + menu_btn_h) ? menu_game_pressed : menu_off;
    } else if (fld->s == large) {
        mp = (x >= large_menu_game_x_offset && x <= large_menu_game_x_offset + menu_btn_w &&
                y >= large_menu_game_y_offset && y <= large_menu_game_y_offset + menu_btn_h) ? menu_game_pressed : menu_off;
    }



    if (mp == menu_game_pressed && pr_u == UNPRESS) {
        return mp;
    }

    /* Returning value */
    return (mp == menu_game_pressed && fld->m_state != menu_game_pressed && pr_u == PRESS) ? mp : menu_off;
}

__bool Check_hover(enum field_size s, struct menu_state *st, int x, int y) {

    /* Initializing variables */
    auto __bool is_changed = __false;
    auto int x_o = (s == small) ? small_menu_game_x_offset : (s == medium) ? medium_menu_game_x_offset : large_menu_game_x_offset;

    /* Main part */
    if (x >= x_o && x <= x_o + menu_itm_w) {
        if (y >= menu_new_y_offset && y <= menu_new_y_offset + menu_itm_h) {
            st->is_hovered = 0;
            is_changed = __true;
        } else if (y >= menu_beg_y_offset && y <= menu_beg_y_offset + menu_itm_h) {
            st->is_hovered = 1;
            is_changed = __true;
        } else if (y >= menu_interm_y_offset && y <= menu_interm_y_offset + menu_itm_h) {
            st->is_hovered = 2;
            is_changed = __true;
        } else if (y >= menu_adv_y_offset && y <= menu_adv_y_offset + menu_itm_h) {
            st->is_hovered = 3;
            is_changed = __true;
        } else if (y >= menu_mks_y_offset && y <= menu_mks_y_offset + menu_itm_h) {
            st->is_hovered = 4;
            is_changed = __true;
        } else if (y >= menu_clr_y_offset && y <= menu_clr_y_offset + menu_itm_h) {
            st->is_hovered = 5;
            is_changed = __true;
        } else if (y >= menu_snd_y_offset && y <= menu_snd_y_offset + menu_itm_h) {
            st->is_hovered = 6;
            is_changed = __true;
        } else if (y >= menu_tabl_y_offset && y <= menu_tabl_y_offset + menu_itm_h) {
            st->is_hovered = 7;
            is_changed = __true;
        } else if (y >= menu_exit_y_offset && y <= menu_exit_y_offset + menu_itm_h) {
            st->is_hovered = 8;
            is_changed = __true;
        } else {
            st->is_hovered = NOT_HOVERED;
        }
    } else {
        st->is_hovered = NOT_HOVERED;
    }

    /* Returning value */
    return is_changed;
}

int Process_menu_press(struct menu_state *st) {

    /* Main part */
    switch (st->is_hovered) {
        case 0: case 7: case 8:
            break;
        case 1:
            st->menu_i_begginer = __true;
            st->menu_i_intermediate = __false;
            st->menu_i_advanced = __false;
            break;
        case 2:
            st->menu_i_intermediate = __true;
            st->menu_i_begginer = __false;
            st->menu_i_advanced = __false;
            break;
        case 3:
            st->menu_i_advanced = __true;
            st->menu_i_begginer = __false;
            st->menu_i_intermediate = __false;
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

    /* Returning value */
    return st->is_hovered;
}