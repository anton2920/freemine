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

#ifndef SRC_MINES_H
#define SRC_MINES_H

/* Include standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <STL.h>
#include <assert.h>
#include <time.h>

/* SDL2 */
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

/* Macros */
#ifndef TITLE
    #define TITLE ("FreeMine")
#endif
#define DELAY_TIME (60)
#define WIN_PANEL (20)
#define LYAGUSHA (6585) /* For Samara's Voenkomat */

#define FRAME_PATH ("../files/winmine.gif")
#define MID_FRAME_PATH ("../files/mid_frame1.png")
#define LARGE_FRAME_PATH ("../files/large_frame1.png")
#define TILES_PATH ("../files/sprite.png")

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
    small_face_y_offset = 35
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
    medium_face_y_offset = 35
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
    large_face_y_offset = 35
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
    mine_black_x = 64
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
};

/* Function declarations */

/* help_routines.c */
__bool SDL_Init_All();
__bool Init_window(struct SDL_Window **window, struct SDL_Renderer **renderer, enum field_size);
__bool Field_init(struct game_field *, enum field_size);
void Field_destroy(struct game_field *, enum field_size);
enum field_size Get_Size(const char *argv[]);
int mines_l(enum field_size);
block *get_clicked_block(struct game_field *, int, int);
enum check_type switch_block_check_type(block *, enum mbtn);
void Block_untoggle_hovered(struct game_field *);
__bool is_hit_face(enum field_size, int, int);
void two_btns(struct game_field *, int, int);

/* draw.c */
struct SDL_Texture *getTexture(struct SDL_Renderer *, char *name);
void Draw_frame(struct SDL_Renderer *, struct SDL_Texture *, enum field_size);
void Draw_field(struct SDL_Renderer *, struct SDL_Texture *, struct game_field *);
void Draw_timerface(struct SDL_Renderer *, struct SDL_Texture *, enum field_size, size_t, int, enum face_state);

/* mines.c */
void Spawn_mines(struct game_field *, block *);
void Count_near(struct game_field *);
void Process_press(struct game_field *, block *);
void Open_near_blank(struct game_field *, block *);
void Uncover_rest_mines(struct game_field *);
void Check_for_win(struct game_field *, int *);
void Mine_searcher(struct game_field *, block *);

#endif
