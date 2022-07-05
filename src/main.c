/*
FreeMine — a free Windows minesweeper clone written on C with SDL2
Copyright © Pavlovsky Anton, 2019-2022

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

#ifdef _WIN32
	#undef main
#endif

int main(int argc, const char *argv[], const char *envp[]) {

    /* Initializing variables */
#ifdef _WIN32
	#define KIB (100)
	auto char hp[KIB] = {0}, hp1[KIB] = {0};
	strcpy(hp, "");
	strcat(hp, "pics\\");
	strcpy(hp1, hp);
#endif
#ifdef __unix__
	#define KIB (1024)
    auto char hp[KIB] = {}, hp1[KIB] = {};
    strcat(hp, getenv("HOME"));
    strcat(hp, "/.local/share/FreeMine/");
    strcpy(hp1, hp);
#endif

    auto __bool quit = __false;
    auto union SDL_Event event, new_event;
    auto int get_ev = 0;

    auto struct SDL_Window *window;
    auto struct SDL_Renderer *renderer;

    auto struct SDL_Texture *frameTexture = NULL;
    auto struct SDL_Texture *tilesTexture = NULL;
    auto struct SDL_Texture *menu_texture = NULL;
    auto struct SDL_Texture *selectTexture = NULL;

    auto struct game_field field = {NULL, 0, 0, small, game_off, menu_off, __true, __true, __false};
    auto enum field_size s = Get_Size(argc, argv);
    auto struct menu_state m_state;
    auto int menu_press_state;

    auto size_t starttime = 0, currtime = 0;
    auto int minesleft;
    auto __bool is_start = __false;
    auto enum face_state fc = face_normal;
    auto enum face_state beg_fc = face_normal;

    auto block *curr_block;
    auto enum check_type ch;
    auto enum mbtn btn = mbtn_no_btn;

    auto struct Mix_Chunk *click = NULL;
    auto Mix_Music *mus = NULL;

    auto struct person records[3] = {0};

    /* Main part. SDL2 */
    srand(time(NULL) / 2);
    SDL_Init_All();
    if (Init_window(&window, &renderer, s)) {
        if (Field_init(&field, s) == __false) {
            exit(BAD_EXIT_CODE);
        }
        Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
        Menu_state_init(&field, &m_state);
        minesleft = mines_l(field.s);
        frameTexture = getTexture(renderer, (field.s == small) ? strcat(hp1, FRAME_PATH) : (field.s == medium) ? strcat(hp1, MID_FRAME_PATH) : strcat(hp1, LARGE_FRAME_PATH));
        strcpy(hp1, hp);
        tilesTexture = getTexture(renderer, strcat(hp1, TILES_PATH));
        strcpy(hp1, hp);
        menu_texture = getTexture(renderer, strcat(hp1, MENU_GAME_PATH));
        strcpy(hp1, hp);
        selectTexture = getTexture(renderer, strcat(hp1, SELECTED_MENU_PATH));
        strcpy(hp1, hp);
        click = Mix_LoadWAV(strcat(hp1, CLICK_SND_PATH));
        strcpy(hp1, hp);
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
                                    if ((ch = switch_block_check_type(curr_block, mbtn_right, field.is_mks_on)) == flaggy) {
                                        --minesleft;
                                    } else if ((ch == question && field.is_mks_on) || (ch == unchecked && !field.is_mks_on)) {
                                        ++minesleft;
                                    }
                                    if (field.is_snd_on) {
                                        Play_click_sound(click);
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
                                if (field.m_state == menu_off || !Check_hover(field.s, &m_state, event.button.x, event.button.y)) {
                                    field.m_state = Is_menu_pressed(&field, event.button.x, event.button.y, PRESS);
                                }
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
                            if (field.m_state == menu_game_pressed && Check_hover(field.s, &m_state, event.button.x, event.button.y)) {
                                if (!(menu_press_state = Process_menu_press(&m_state))) {
                                    Field_init(&field, s);
                                    is_start = __false;
                                    starttime = 0;
                                    currtime = 0;
                                    minesleft = mines_l(field.s);
                                    fc = face_normal;
                                    beg_fc = fc;
                                } else if (menu_press_state >= 1 && menu_press_state <= 3) {
                                    if ((s == small && menu_press_state == 1) ||
                                        (s == medium && menu_press_state == 2) ||
                                        (s == large && menu_press_state == 3)) {
                                        continue;
                                    }
                                    SDL_DestroyTexture(frameTexture);
                                    SDL_DestroyTexture(tilesTexture);
                                    SDL_DestroyTexture(menu_texture);
                                    SDL_DestroyTexture(selectTexture);
                                    SDL_DestroyRenderer(renderer);
                                    SDL_DestroyWindow(window);
                                    s = (menu_press_state == 1) ? small : (menu_press_state == 2) ? medium : large;
                                    if (Init_window(&window, &renderer, s)) {
                                        if (Field_init(&field, s) == __false) {
                                            exit(BAD_EXIT_CODE);
                                        }

                                        is_start = __false;
                                        starttime = 0;
                                        currtime = 0;
                                        minesleft = mines_l(field.s);
                                        fc = face_normal;
                                        beg_fc = fc;

                                        Menu_state_init(&field, &m_state);
                                        minesleft = mines_l(field.s);
                                        frameTexture = getTexture(renderer, (field.s == small) ? strcat(hp1, FRAME_PATH) : (field.s == medium) ? strcat(hp1, MID_FRAME_PATH) : strcat(hp1, LARGE_FRAME_PATH));
                                        strcpy(hp1, hp);
                                        tilesTexture = getTexture(renderer, strcat(hp1, TILES_PATH));
                                        strcpy(hp1, hp);
                                        menu_texture = getTexture(renderer, strcat(hp1, MENU_GAME_PATH));
                                        strcpy(hp1, hp);
                                        selectTexture = getTexture(renderer, strcat(hp1, SELECTED_MENU_PATH));
                                        strcpy(hp1, hp);
                                        continue;
                                    } else {
                                        fprintf(stderr, "Error! Something went wrong: %s\n", SDL_GetError());
                                    }
                                } else if (menu_press_state == 4) {
                                    field.is_mks_on = m_state.menu_i_marks;
                                    if (field.is_mks_on == __false) {
                                        Remove_questions(&field);
                                    }
                                } else if (menu_press_state == 5) {
                                    field.is_clr_on = m_state.menu_i_color;
                                } else if (menu_press_state == 6) {
                                    field.is_snd_on = m_state.menu_i_sound;
                                    if (field.is_snd_on) {
                                        Play_music(&mus, strcat(hp1, MUS_PATH));
                                        strcpy(hp1, hp);
                                    } else {
                                        Mix_FreeMusic(mus);
                                    }
                                } else if (menu_press_state == 7) {
                                    Print_Records(records);
                                } else if (menu_press_state == 8) {
                                    quit = __true;
                                    break;
                                }
                                field.m_state = Is_menu_pressed(&field, event.button.x, event.button.y, UNPRESS);
                            } else if (curr_block != NULL && beg_fc == face_o) {
                                if (curr_block->check != flaggy && curr_block->check != pressed) {
                                    if (field.g_state == game_off) {
                                        field.g_state = game_start;
                                        Spawn_mines(&field, curr_block);
                                    }
                                    Process_press(&field, curr_block);
                                    if (field.is_snd_on) {
                                        Play_click_sound(click);
                                    }
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
                                if (field.is_snd_on) {
                                    Play_click_sound(click);
                                }
                            }
                        } else if ((event.button.button == SDL_BUTTON_MIDDLE || get_ev) && btn == mbtn_mid) {
                            curr_block = get_clicked_block(&field, event.button.x, event.button.y);
                            if (curr_block != NULL) {
                                if (field.g_state == game_start) {
                                    Mine_searcher(&field, curr_block);
                                    if (field.is_snd_on) {
                                        Play_click_sound(click);
                                    }
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
                        Check_hover(field.s, &m_state, event.motion.x, event.motion.y);
                    }
                } else {
                    if (event.type == SDL_MOUSEBUTTONDOWN) {
                        if (event.button.button == SDL_BUTTON_LEFT) {
                            if (is_hit_face(field.s, event.button.x, event.button.y)) {
                                fc = face_pressed;
                                btn = mbtn_left;
                            }
                            if (field.m_state == menu_off || !Check_hover(field.s, &m_state, event.button.x, event.button.y)) {
                                field.m_state = Is_menu_pressed(&field, event.button.x, event.button.y, PRESS);
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
                                if (field.is_snd_on) {
                                    Play_click_sound(click);
                                }
                            } else if (field.m_state == menu_game_pressed && Check_hover(field.s, &m_state, event.button.x, event.button.y)) {
                                if (!(menu_press_state = Process_menu_press(&m_state))) {
                                    Field_init(&field, s);
                                    is_start = __false;
                                    starttime = 0;
                                    currtime = 0;
                                    minesleft = mines_l(field.s);
                                    fc = face_normal;
                                    beg_fc = fc;
                                } else if (menu_press_state >= 1 && menu_press_state <= 3) {
                                    if ((s == small && menu_press_state == 1) ||
                                        (s == medium && menu_press_state == 2) ||
                                        (s == large && menu_press_state == 3)) {
                                        continue;
                                    }
                                    SDL_DestroyTexture(frameTexture);
                                    SDL_DestroyTexture(tilesTexture);
                                    SDL_DestroyTexture(menu_texture);
                                    SDL_DestroyTexture(selectTexture);
                                    SDL_DestroyRenderer(renderer);
                                    SDL_DestroyWindow(window);
                                    s = (menu_press_state == 1) ? small : (menu_press_state == 2) ? medium : large;
                                    if (Init_window(&window, &renderer, s)) {
                                        if (Field_init(&field, s) == __false) {
                                            exit(BAD_EXIT_CODE);
                                        }

                                        is_start = __false;
                                        starttime = 0;
                                        currtime = 0;
                                        minesleft = mines_l(field.s);
                                        fc = face_normal;
                                        beg_fc = fc;

                                        Menu_state_init(&field, &m_state);
                                        minesleft = mines_l(field.s);
                                        frameTexture = getTexture(renderer, (field.s == small) ? strcat(hp1, FRAME_PATH) : (field.s == medium) ? strcat(hp1, MID_FRAME_PATH) : strcat(hp1, LARGE_FRAME_PATH));
                                        strcpy(hp1, hp);
                                        tilesTexture = getTexture(renderer, strcat(hp1, TILES_PATH));
                                        strcpy(hp1, hp);
                                        menu_texture = getTexture(renderer, strcat(hp1, MENU_GAME_PATH));
                                        strcpy(hp1, hp);
                                        selectTexture = getTexture(renderer, strcat(hp1, SELECTED_MENU_PATH));
                                        strcpy(hp1, hp);
                                        continue;
                                    } else {
                                        fprintf(stderr, "Error! Something went wrong: %s\n", SDL_GetError());
                                    }
                                } else if (menu_press_state == 4) {
                                    field.is_mks_on = m_state.menu_i_marks;
                                    if (field.is_mks_on == __false) {
                                        Remove_questions(&field);
                                    }
                                } else if (menu_press_state == 5) {
                                    field.is_clr_on = m_state.menu_i_color;
                                } else if (menu_press_state == 6) {
                                    field.is_snd_on = m_state.menu_i_sound;
                                    if (field.is_snd_on) {
                                        Play_music(&mus, strcat(hp1, MUS_PATH));
                                        strcpy(hp1, hp);
                                    } else {
                                        Mix_FreeMusic(mus);
                                    }
                                } else if (menu_press_state == 7) {
                                    Print_Records(records);
                                } else if (menu_press_state == 8) {
                                    quit = __true;
                                    break;
                                }
                                field.m_state = Is_menu_pressed(&field, event.button.x, event.button.y, UNPRESS);
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
                        Check_hover(field.s, &m_state, event.motion.x, event.motion.y);
                    }
                }

                if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_F2) {
                        Field_init(&field, s);
                        is_start = __false;
                        starttime = 0;
                        currtime = 0;
                        minesleft = mines_l(field.s);
                        fc = face_normal;
                        beg_fc = fc;
                    } else if (event.key.keysym.sym == SDLK_w) {
                        if (R_u_s() && field.g_state == game_start) {
                            Open_field(&field, &minesleft);
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
            Draw_timerface(renderer, &field, tilesTexture, s, (currtime / 1000), minesleft, fc);
            if (field.m_state != menu_off) {
                Draw_menu(renderer, &field, &m_state, menu_texture, selectTexture, tilesTexture);
            }
            SDL_RenderPresent(renderer);
            if (field.g_state == game_win) {
                if (Check_Records(records, field.s, currtime / 1000)) {
                    Print_Records(records);
                }
            }
            SDL_Delay(DELAY_TIME);
        }
    } else {
        fprintf(stderr, "Error! Something went wrong: %s\n", SDL_GetError());
    }

    /* Releasing memory */
    Field_destroy(&field, s);
    SDL_DestroyTexture(frameTexture);
    SDL_DestroyTexture(tilesTexture);
    SDL_DestroyTexture(menu_texture);
    SDL_DestroyTexture(selectTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_FreeChunk(click);
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();

    /* Returning value */
    return 0;
}
