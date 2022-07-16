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

#ifndef MINES_H
#define MINES_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* SDL2 */
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

/* Macros */
#define TITLE ("FreeMine")

#define DELAY_TIME (30)
#define WIN_PANEL (20)
#define LINUX_PANEL (WIN_PANEL + 3)
#define NAME_SIZE (20)
#define PRESS (1)
#define UNPRESS (2)
#define NOT_HOVERED (-1)
#define LYAGUSHA (6585) /* For Samara's Voenkomat */

#define FRAME_PATH ("winmine1.gif")
#define MID_FRAME_PATH ("mid_frame1.png")
#define LARGE_FRAME_PATH ("large_frame1.png")
#define TILES_PATH ("sprite1.png")
#define MENU_GAME_PATH ("menu_game.png")
#define SELECTED_MENU_PATH ("selected_menu.png")
#define MUS_PATH ("mus.mp3")
#define CLICK_SND_PATH ("click.wav")


/* Data types */
enum field_size {
    small,
    medium,
    large
};


enum small_field {
    small_width = 158,
    small_height = 216,
    small_nmines = 10,
    small_tiles_x = 8,
    small_tiles_y = 8,
    small_tiles_x_offset = 15,
    small_tiles_y_offset = 75,
    small_timer_last_x_offset = 123,
    small_timer_last_y_offset = 36,
    small_mines_last_x_offset = 46,
    small_mines_last_y_offset = 36,
    small_face_x_offset = 65,
    small_face_y_offset = 35,
    small_menu_game_x_offset = 3,
    small_menu_game_y_offset = 0,
    small_tick_x_offset = 11
};


enum medium_field {
    medium_width = 280,
    medium_height = 342,
    medium_nmines = 40,
    medium_tiles_x = 16,
    medium_tiles_y = 16,
    medium_tiles_x_offset = 14,
    medium_tiles_y_offset = 74,
    medium_timer_last_x_offset = 250,
    medium_timer_last_y_offset = 35,
    medium_mines_last_x_offset = 45,
    medium_mines_last_y_offset = 35,
    medium_face_x_offset = 129,
    medium_face_y_offset = 35,
    medium_menu_game_x_offset = 2,
    medium_menu_game_y_offset = 0,
    medium_tick_x_offset = 10
};


enum large_field {
    large_width = 503,
    large_height = 342,
    large_nmines = 99,
    large_tiles_x = 30,
    large_tiles_y = 16,
    large_tiles_x_offset = 13,
    large_tiles_y_offset = 74,
    large_timer_last_x_offset = 473,
    large_timer_last_y_offset = 35,
    large_mines_last_x_offset = 44,
    large_mines_last_y_offset = 35,
    large_face_x_offset = 240,
    large_face_y_offset = 35,
    large_menu_game_x_offset = 1,
    large_menu_game_y_offset = 0,
    large_tick_x_offset = 9
};


enum fill_type {
    miny,
    miny_red,
    miny_cross,
    digity,
    nothing
};


enum check_type {
    unchecked,
    hovered,
    pressed,
    flaggy,
    question,
    hovered_question
};


enum tile_s {
    tile_w = 15,
    tile_h = 15,
    tile_space = 1,
};


enum png_offset {
    unchecked_x = 0,
    unchecked_y = 39,
    pressed_x = 0,
    pressed_y = 23,
    flaggy_x = 16,
    flaggy_y = 39,
    question_x = 80,
    question_y = 39,
    hovered_question_x = 96,
    hovered_question_y = 39,
    face_normal_x_offset = 0,
    face_pressed_x_offset = 26,
    face_o_x_offset = 52,
    face_dead_x_offset = 78,
    face_cool_x_offset = 104,
    face_y_offset = 56,
    digity_y = 23,
    digit_1_x = 16,
    digit_2_x = 32,
    digit_3_x = 48,
    digit_4_x = 64,
    digit_5_x = 80,
    digit_6_x = 96,
    digit_7_x = 112,
    digit_8_x = 128,
    mine_y = 39,
    mine_red_x = 32,
    mine_crossed_x = 48,
    mine_black_x = 64,
    tick_x_offset = 140,
    tick_black_y_offset = 49,
    tick_white_y_offset = 77,
    BW_OFFSET = 160
};


enum digit_s {
    digit_w = 13,
    digit_h = 23
};


enum face_s {
    face_w = 26,
    face_h = 26
};


enum mbtn {
    mbtn_no_btn,
    mbtn_left,
    mbtn_right,
    mbtn_mid
};


enum face_state {
    face_normal,
    face_pressed,
    face_o,
    face_dead,
    face_cool
};


enum game_state {
    game_off,
    game_start,
    game_lose,
    game_win
};


enum tick_size {
    tick_w = 7,
    tick_h = 7
};


enum menu_panel {
    menu_off,
    menu_game_pressed,

    menu_btn_w = 42,
    menu_btn_h = 20,

    menu_itm_w = 120,
    menu_itm_h = 17,
    menu_itm_add_y_offset = 3,

    menu_panel_w = 166,
    menu_panel_h = 214,

    tick_y_beg = 54,
    tick_y_interm = 71,
    tick_y_adv = 88,

    tick_y_mks = 114,
    tick_y_clr = 131,
    tick_y_snd = 148,

    menu_new_y_offset = 22,

    menu_beg_y_offset = 48,
    menu_interm_y_offset = 65,
    menu_adv_y_offset = 82,

    menu_mks_y_offset = 108,
    menu_clr_y_offset = 125,
    menu_snd_y_offset = 142,

    menu_tabl_y_offset = 168,

    menu_exit_y_offset = 194
};


struct menu_state {
    __bool menu_i_begginer;
    __bool menu_i_intermediate;
    __bool menu_i_advanced;

    __bool menu_i_marks;
    __bool menu_i_color;
    __bool menu_i_sound;

    int is_hovered; /* One to eight, -1 if not */
};


typedef struct __block {
    struct SDL_Rect rect;
    enum fill_type type;
    enum check_type check;
    int digit;
} block;


struct game_field {
    block **fld;
    int tiles_x;
    int tiles_y;
    enum field_size s;
    enum game_state g_state;
    enum menu_panel m_state;
    __bool is_mks_on;
    __bool is_clr_on;
    __bool is_snd_on;
};


struct person {
    char name[NAME_SIZE];
    int time;
};


/* help_routines.c */
__bool SDL_init_all();
__bool Init_window(struct SDL_Window **window, struct SDL_Renderer **renderer, enum field_size);
__bool Field_init(struct game_field *, enum field_size);
void Field_destroy(struct game_field *, enum field_size);
enum field_size Get_Size(int, const char *argv[]);
int mines_l(enum field_size);
block *get_clicked_block(struct game_field *, int, int);
enum check_type switch_block_check_type(block *, enum mbtn, __bool);
void Block_untoggle_hovered(struct game_field *);
__bool is_hit_face(enum field_size, int, int);
void two_btns(struct game_field *, int, int);
void Menu_state_init(struct game_field *, struct menu_state *);
void Play_music(Mix_Music **fon, char *name);
void Play_click_sound(struct Mix_Chunk *Sound);
void Remove_questions(struct game_field *);
void Open_field(struct game_field *, int *);
__bool R_u_s(void);


/* draw.c */
struct SDL_Texture *getTexture(struct SDL_Renderer *, char *name);
void Draw_frame(struct SDL_Renderer *, struct SDL_Texture *, enum field_size);
void Draw_field(struct SDL_Renderer *, struct SDL_Texture *, struct game_field *);
void Draw_timerface(struct SDL_Renderer *, struct game_field *, struct SDL_Texture *, enum field_size, size_t, int,
                    enum face_state);
void Draw_menu(struct SDL_Renderer *, struct game_field *, struct menu_state *,
               struct SDL_Texture *menu_game, struct SDL_Texture *select, struct SDL_Texture *tiles);


/* mines.c */
void Spawn_mines(struct game_field *, block *);
void Count_near(struct game_field *);
void Process_press(struct game_field *, block *);
void Open_near_blank(struct game_field *, block *);
void Uncover_rest_mines(struct game_field *);
void Check_for_win(struct game_field *, int *);
void Mine_searcher(struct game_field *, block *);


/* menu.c */
enum menu_panel Is_menu_pressed(struct game_field *fld, int x, int y, int pr_u);
__bool Check_hover(enum field_size, struct menu_state *, int x, int y);
int Process_menu_press(struct menu_state *);


/* lead.c */
void Write_Records(struct person *p);
void Read_Records(struct person *p);
__bool Check_Records(struct person *p, enum field_size s, int time);
void Print_Records(struct person *p);
void New_Record(struct person *p, enum field_size s, int time);

#endif
