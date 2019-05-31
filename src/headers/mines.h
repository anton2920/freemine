#ifndef SRC_MINES_H
#define SRC_MINES_H

/* Include standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <STL.h>

/* SDL2 */
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

/* Macros */
#ifndef TITLE
    #define TITLE ("FreeMine")
#endif

/* Data types */
enum small_field {
    small_width = 30,
    small_height = 60,
    small_nmines = 10,
    small_tiles_x = 8,
    small_tiles_y = 8
};

/* Function declarations */

/* help_routines.c */
__bool SDL_Init_All();
__bool Init_custom_window(struct SDL_Window **window, struct SDL_Renderer **renderer, size_t width, size_t height);
__bool Init_small_window(struct SDL_Window **window, struct SDL_Renderer **renderer);

#endif
