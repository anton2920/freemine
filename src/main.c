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

#include "headers/mines.h"

int main(int argc, const char *argv[], const char *envp[]) {

    /* Initializing variables */
    assert(argc == 2);

    auto __bool quit = __false;
    auto union SDL_Event event, new_event;
    auto int get_ev = 0;

    auto struct SDL_Window *window;
    auto struct SDL_Renderer *renderer;

    auto struct SDL_Texture *frameTexture = NULL;
    auto struct SDL_Texture *tilesTexture = NULL;
    auto struct SDL_Texture *menu_game_texture;
    auto struct SDL_Texture *menu_help_texture;

    auto struct game_field field;
    auto enum field_size s = Get_Size(argv);

    auto size_t starttime = 0, currtime = 0;
    auto int minesleft;
    auto __bool is_start = __false;
    auto enum face_state fc = face_normal;
    auto enum face_state beg_fc = face_normal;

    auto block *curr_block;
    auto enum check_type ch = unchecked;
    auto enum mbtn btn = mbtn_no_btn;

    /* Main part. SDL2 */
    srand(time(NULL) / 2);
    SDL_Init_All();
    if (Init_window(&window, &renderer, s)) {
        if (Field_init(&field, s) == __false) {
            exit(0);
        }
        minesleft = mines_l(field.s);
        frameTexture = getTexture(renderer, (field.s == small) ? FRAME_PATH : (field.s == medium) ? MID_FRAME_PATH : LARGE_FRAME_PATH);
        tilesTexture = getTexture(renderer, TILES_PATH);
        /*menu_game_texture = getTexture(renderer, MENU_GAME_PATH);
        menu_help_texture = getTexture(renderer, MENU_HELP_PATH);*/
        while (!quit) {
            switch (field.g_state) {
                case game_off: case game_start:
                    break;
                case game_lose:
                    is_start = __false;
                    if (fc != face_dead && fc != face_pressed) {
                        fc = face_dead;
                        beg_fc = face_dead;
                    }
                    break;
                case game_win:
                    is_start = __false;
                    if (fc != face_cool && fc != face_pressed) {
                        fc = face_cool;
                        beg_fc = face_cool;
                    }
                    break;
                default:
                    break;
            }
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    quit = __true;
                }

                if (field.g_state != game_lose && field.g_state != game_win) {
                    Block_untoggle_hovered(&field);
                    if (event.type == SDL_MOUSEBUTTONDOWN) {
                        if ((get_ev = SDL_PollEvent(&new_event))) {
                            if (new_event.type == SDL_MOUSEBUTTONDOWN) {
                                if ((event.button.button == SDL_BUTTON_LEFT &&
                                     new_event.button.button == SDL_BUTTON_RIGHT) ||
                                    (event.button.button == SDL_BUTTON_RIGHT &&
                                     new_event.button.button == SDL_BUTTON_LEFT)) {
                                    btn = mbtn_mid;
                                    two_btns(&field, event.button.x, event.button.y);
                                    fc = face_o;
                                    beg_fc = fc;
                                }
                            }
                        } else if (event.button.button == SDL_BUTTON_RIGHT) {
                            if (btn == mbtn_no_btn) {
                                btn = mbtn_right;
                                curr_block = get_clicked_block(&field, event.button.x, event.button.y);
                                if (curr_block != NULL) {
                                    if ((ch = switch_block_check_type(curr_block, mbtn_right)) == flaggy) {
                                        --minesleft;
                                    } else if (ch == question) {
                                        ++minesleft;
                                    }
                                }
                            } else if (btn == mbtn_left) {
                                btn = mbtn_mid;
                                two_btns(&field, event.button.x, event.button.y);
                                fc = face_o;
                                beg_fc = fc;
                            }
                        } else if (event.button.button == SDL_BUTTON_LEFT) {
                            if (btn == mbtn_no_btn) {
                                btn = mbtn_left;
                                curr_block = get_clicked_block(&field, event.button.x, event.button.y);
                                fc = face_o;
                                if (curr_block != NULL) {
                                    if (curr_block->check != flaggy && curr_block->check != question &&
                                        curr_block->check != pressed) {
                                        curr_block->check = hovered;
                                    } else if (curr_block->check == question) {
                                        curr_block->check = hovered_question;
                                    }
                                } else if (is_hit_face(field.s, event.button.x, event.button.y)) {
                                    fc = face_pressed;
                                }
                                /* field.m_state = Is_menu_pressed(&field, event.button.x, event.button.y); */
                                beg_fc = fc;
                            } else if (btn == mbtn_right) {
                                btn = mbtn_mid;
                                two_btns(&field, event.button.x, event.button.y);
                                fc = face_o;
                                beg_fc = fc;
                            }
                        } else if (event.button.button == SDL_BUTTON_MIDDLE) {
                            btn = mbtn_mid;
                            two_btns(&field, event.button.x, event.button.y);
                            fc = face_o;
                            beg_fc = fc;
                        }
                    } else if (event.type == SDL_MOUSEBUTTONUP) {
                        if (event.button.button == SDL_BUTTON_LEFT && !get_ev && btn == mbtn_left) {
                            curr_block = get_clicked_block(&field, event.button.x, event.button.y);
                            if (curr_block != NULL && beg_fc == face_o) {
                                if (curr_block->check != flaggy && curr_block->check != pressed) {
                                    if (field.g_state == game_off) {
                                        field.g_state = game_start;
                                        Spawn_mines(&field, curr_block);
                                    }
                                    Process_press(&field, curr_block);
                                }
                                if (!is_start) {
                                    starttime = SDL_GetTicks();
                                    is_start = __true;
                                }
                            } else if (is_hit_face(field.s, event.button.x, event.button.y) && fc == face_pressed) {
                                Field_init(&field, s);
                                is_start = __false;
                                starttime = 0;
                                currtime = 0;
                                minesleft = mines_l(field.s);
                            }
                        } else if ((event.button.button == SDL_BUTTON_MIDDLE || get_ev) && btn == mbtn_mid) {
                            curr_block = get_clicked_block(&field, event.button.x, event.button.y);
                            if (curr_block != NULL) {
                                if (field.g_state == game_start) {
                                    Mine_searcher(&field, curr_block);
                                }
                                if (!is_start) {
                                    starttime = SDL_GetTicks();
                                    is_start = __true;
                                }
                            }
                        }
                        if (btn == mbtn_mid && !is_start &&
                            get_clicked_block(&field, event.button.x, event.button.y) != NULL) {
                            starttime = SDL_GetTicks();
                            is_start = __true;
                        }
                        fc = face_normal;
                        btn = mbtn_no_btn;
                    } else if (event.type == SDL_MOUSEMOTION) {
                        switch (btn) {
                            case mbtn_left:
                                btn = mbtn_left;
                                curr_block = get_clicked_block(&field, event.motion.x, event.motion.y);
                                if (curr_block != NULL && beg_fc == face_o) {
                                    if (curr_block->check != flaggy && curr_block->check != question &&
                                        curr_block->check != pressed) {
                                        curr_block->check = hovered;
                                    } else if (curr_block->check == question) {
                                        curr_block->check = hovered_question;
                                    }
                                } else if (is_hit_face(field.s, event.motion.x, event.motion.y) && beg_fc != face_o) {
                                    fc = face_pressed;
                                } else if (beg_fc != face_o) {
                                    fc = face_normal;
                                }
                                break;
                            case mbtn_mid:
                                btn = mbtn_mid;
                                two_btns(&field, event.motion.x, event.motion.y);
                                fc = face_o;
                                break;
                            case mbtn_right:
                                btn = mbtn_no_btn;
                            default:
                                break;
                        }
                    }
                } else {
                    if (event.type == SDL_MOUSEBUTTONDOWN) {
                        if (event.button.button == SDL_BUTTON_LEFT) {
                            if (is_hit_face(field.s, event.button.x, event.button.y)) {
                                fc = face_pressed;
                                btn = mbtn_left;
                            }
                        }
                    } else if (event.type == SDL_MOUSEBUTTONUP) {
                        if (event.button.button == SDL_BUTTON_LEFT) {
                            if (is_hit_face(field.s, event.button.x, event.button.y)) {
                                Field_init(&field, s);
                                is_start = __false;
                                starttime = 0;
                                currtime = 0;
                                minesleft = mines_l(field.s);
                                fc = face_normal;
                                beg_fc = fc;
                            }
                        }
                        btn = mbtn_no_btn;
                    } else if (event.type == SDL_MOUSEMOTION) {
                        if (btn == mbtn_left) {
                            if (is_hit_face(field.s, event.motion.x, event.motion.y)) {
                                fc = face_pressed;
                            } else {
                                fc = beg_fc;
                            }
                        }
                    }
                }
            }
            if (is_start) {
                currtime = SDL_GetTicks() + 1000 - starttime;
            }
            if (currtime / 1000 == 999) {
                is_start = __false;
            }
            SDL_RenderClear(renderer);
            Check_for_win(&field, &minesleft);
            Draw_frame(renderer, frameTexture, field.s);
            Draw_field(renderer, tilesTexture, &field);
            Draw_timerface(renderer, tilesTexture, s, (currtime / 1000), minesleft, fc);
            /*if (field.m_state != menu_off) {
                Show_menu();
            }*/
            SDL_RenderPresent(renderer);
            SDL_Delay(DELAY_TIME);
        }
    } else {
        fprintf(stderr, "Error! Something went wrong: %s\n", SDL_GetError());
    }

    /* Releasing memory */
    Field_destroy(&field, s);
    SDL_DestroyTexture(frameTexture);
    SDL_DestroyTexture(tilesTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();

    /* Returning value */
    return 0;
}