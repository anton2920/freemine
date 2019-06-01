#include "../headers/mines.h"

struct SDL_Texture *getTexture(struct SDL_Renderer *renderer, char *path_to_pic) {

    /* Initializing variables */
    auto struct SDL_Surface *Image = IMG_Load(path_to_pic);
    assert(Image != NULL); /* Debugging sh*t */
    /* SDL_SetColorKey(Image, SDL_TRUE, SDL_MapRGB(Image->format, 255, 255, 255)); */
    auto struct SDL_Texture *Texture = SDL_CreateTextureFromSurface(renderer, Image);
    assert(Texture != NULL);
    SDL_FreeSurface(Image);


    /* Returning value */
    return Texture;
}

void Draw_frame(struct SDL_Renderer *renderer, struct SDL_Texture *frameTexture, enum field_size size) {

    /* Initializing variables */
    auto struct SDL_Rect frameRect = {0, 0, 0, 0};
    auto struct SDL_Rect fRect = {0, 0, 0, 0};

    /* Main part */
    if (size == small) {
        frameRect.w = small_width;
        frameRect.h = small_height;
        fRect.y = WIN_PANEL;
        fRect.w = 161 - 3;
        fRect.h = 236 - fRect.y;
    }

    SDL_RenderCopy(renderer, frameTexture, &fRect, &frameRect);
}

void Draw_field(struct SDL_Renderer *renderer, struct SDL_Texture *fieldStuff, struct game_field *fld) {

    /* Initializing variables */
    auto int i, j;
    auto struct SDL_Rect bTile = {0, 0, tile_w, tile_h};
    auto struct SDL_Rect currT = {0, 0, tile_w, tile_h};

    /* Main part */
    if (fld->s == small) {
        for (i = 0; i < small_tiles_y; ++i) {
            for (j = 0; j < small_tiles_x; ++j) {
                bTile.x = fld->fld[i][j].rect.x;
                bTile.y = fld->fld[i][j].rect.y;
                currT.x = (fld->fld[i][j].check == unchecked) ? unchecked_x :
                          (fld->fld[i][j].check == pressed) ? pressed_x :
                          (fld->fld[i][j].check == flaggy) ? flaggy_x : question_x;
                currT.y = (fld->fld[i][j].check == unchecked) ? unchecked_y :
                          (fld->fld[i][j].check == pressed) ? pressed_y :
                          (fld->fld[i][j].check == flaggy) ? flaggy_y : question_y;
                SDL_RenderCopy(renderer, fieldStuff, &currT, &bTile);
            }
        }
    }
}

void Draw_timerface(struct SDL_Renderer *renderer, struct SDL_Texture *timerText,
        enum field_size size, size_t currtime, int minesleft) {

    /* Initializing variables */
    auto struct SDL_Rect bTile = {0, 0, digit_w, digit_h};
    auto struct SDL_Rect currT = {0, 0, digit_w, digit_h};
    auto int digit, i;

    /* Main part */
    bTile.x = small_timer_last_x_offset;
    bTile.y = small_timer_last_y_offset;
    for (i = 3; i > 0; --i, currtime /= 10, bTile.x -= digit_w) {
        digit = currtime % 10;
        currT.x = digit * digit_w;
        SDL_RenderCopy(renderer, timerText, &currT, &bTile);
    }

    bTile.x = small_mines_last_x_offset;
    bTile.y = small_mines_last_y_offset;
    if (minesleft >= 0) {
        for (i = 3; i > 0; --i, minesleft /= 10, bTile.x -= digit_w) {
            digit = minesleft % 10;
            currT.x = digit * digit_w;
            SDL_RenderCopy(renderer, timerText, &currT, &bTile);
        }
    } else {
        minesleft = abs(minesleft);
        for (i = 2; i > 0; --i, minesleft /= 10, bTile.x -= digit_w) {
            digit = minesleft % 10;
            currT.x = digit * digit_w;
            SDL_RenderCopy(renderer, timerText, &currT, &bTile);
        }
        currT.x = 10 * digit_w;
        SDL_RenderCopy(renderer, timerText, &currT, &bTile);
    }

    bTile.x = small_face_x_offset;
    bTile.y = small_face_y_offset;
    bTile.w = face_w;
    bTile.h = face_h;
    currT.x = face1_x_offset;
    currT.y = face1_y_offset;
    currT.w = bTile.w;
    currT.h = bTile.h;
    SDL_RenderCopy(renderer, timerText, &currT, &bTile);
}