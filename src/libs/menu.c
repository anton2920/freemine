#include "../headers/mines.h"

enum menu_state Is_menu_pressed(struct game_field *fld, int x, int y) {

    /* Main part */
    if (fld->s == small) {
        return (x >= small_menu_game_x_offset && x <= small_menu_game_x_offset + menu_w &&
            y >= small_menu_game_y_offset && y <= small_menu_game_y_offset + menu_h) ? menu_game_pressed : menu_off;
    } else if (fld->s == medium) {
        return (x >= medium_menu_game_x_offset && x <= medium_menu_game_x_offset + menu_w &&
                y >= medium_menu_game_y_offset && y <= medium_menu_game_y_offset + menu_h) ? menu_game_pressed : menu_off;
    } else if (fld->s == large) {
        return (x >= large_menu_game_x_offset && x <= large_menu_game_x_offset + menu_w &&
                y >= large_menu_game_y_offset && y <= large_menu_game_y_offset + menu_h) ? menu_game_pressed : menu_off;
    }
}

void Show_menu(struct game_field *fld, struct SDL_Texture *menu_game,
        struct SDL_Texture *menu_help, struct SDL_Texture *tiles) {

    /* Initializing variables */
    auto struct SDL_Rect drawRect;
    auto struct SDL_Rect currRect;
    auto int x, y, w, h;

    /* Main part */
    if (fld->s == small) {
        if (fld->m_state == menu_game_pressed) {
            x = small_menu_game_x_offset;
            y = small_menu_game_y_offset;
        } else if (fld->m_state == menu_help_pressed) {
            x = small_menu_help_x_offset;
            y = small_menu_help_y_offset;
        }
    }
}