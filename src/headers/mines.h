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
#define DELAY_TIME (30)
#define WIN_PANEL (20)
#define LYAGUSHA (6585) /* For Samara's Voenkomat */

#define FRAME_PATH ("../files/winmine.gif")
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

enum fill_type {
    miny,
    digity,
    nothing
};

enum check_type {
    unchecked,
    pressed,
    flaggy,
    question
};

enum tile_s {
    tile_w = 15,
    tile_h = 15
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
    face1_x_offset = 0,
    face1_y_offset = 56
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
    mbtn_left,
    mbtn_right,
    mbtn_mid
};

typedef struct __block {
    struct SDL_Rect rect;
    enum fill_type type;
    enum check_type check;
    char digit;
} block;

struct game_field {
    block **fld;
    enum field_size s;
};

/* Function declarations */

/* help_routines.c */
__bool SDL_Init_All();
__bool Init_window(struct SDL_Window **window, struct SDL_Renderer **renderer, enum field_size);
__bool Field_init(struct game_field *, enum field_size);
void Field_destroy(struct game_field *, enum field_size);
enum field_size Get_Size(const char *argv[]);
int mines_l(enum field_size);
block **get_clicked_block(struct game_field *, int, int);
void switch_block_check_type(block **, enum mbtn);

/* draw.c */
struct SDL_Texture *getTexture(struct SDL_Renderer *, char *name);
void Draw_frame(struct SDL_Renderer *, struct SDL_Texture *, enum field_size);
void Draw_field(struct SDL_Renderer *, struct SDL_Texture *, struct game_field *);
void Draw_timerface(struct SDL_Renderer *, struct SDL_Texture *, enum field_size, size_t, int);


#endif
