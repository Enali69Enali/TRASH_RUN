#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "game.h"

const char* title = "Trash run" ;
const int x = SDL_WINDOWPOS_CENTERED ;
const int y = SDL_WINDOWPOS_CENTERED ;

const int w = 830 ;                     // window width
const int h = 790 ;                     // window height
const int Xfield = 700 ;                // playground width
const int Yfield = 700 ;                // playground height
const int paddingX = (w-Xfield)/2 ;
const int paddingY = (h-Yfield)/2 ;

const Color * color_dictionary[11] = {
    &(Color){"Grey", 104, 111, 140},
    &(Color){"Orange", 245, 130, 48},
    &(Color){"White", 255, 255, 255},
    &(Color){"Yellow", 255, 225, 25},
    &(Color){"Lavender", 220, 190, 255},
    &(Color){"Red", 230, 25, 75},
    &(Color){"Green", 60, 180, 75},
    &(Color){"Teal", 0, 128, 128},
    &(Color){"Blue", 0, 130, 200},
    &(Color){"Persan",102, 0, 255},
    &(Color){"Mint", 170, 255, 195}
};

static int game_over[2] = {0,0} ;
static int play = 0 ;
static int menu = 0 ;
/* menu = 10 for main menu
        = 20 for play menu
        = 40 for ranking menu
        = 80 for game over menu
        = 90 for settings menu */

static int rankingMenuNum = 1 ;
static int credits = 0 ;

char music_main_menu[25] = "music/main_menu.wav" ;
char music_to_play[25] = "music/play.wav" ;

raccoon_player * player1 = NULL ;
raccoon_player * player2 = NULL ;
raccoon_player * playerAI = NULL ;

void hold() ;


int main(int argc, char *argv[]){
    /* Initializing window*/
    SDL_Window *window = NULL ;
    SDL_PumpEvents() ;

    /* Initializing font */
    TTF_Init() ;
    TTF_Font * font = TTF_OpenFont("Pixellari.ttf", 100) ; // font choice

    /* Initializing audio */
    SDL_AudioDeviceID deviceId ;
    SDL_Init(SDL_INIT_AUDIO) ;

    deviceId = play_music(deviceId, music_main_menu) ;


    if(0 != SDL_Init(SDL_INIT_VIDEO)){
        fprintf(stderr, "Error SDL_Init : %s", SDL_GetError()) ;
        return EXIT_FAILURE ;
    }

    window = SDL_CreateWindow(title, x, y, w, h, SDL_WINDOW_SHOWN ) ;

    if(NULL == window){
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError()) ;
        return EXIT_FAILURE ;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED) ;
    SDL_Event e ;

    settings * settings = create_settings() ;

    menu = 10 ;

    while(menu != 0)
    { /* Leave the game and close the window */
        switch(menu/10)
        {
            case 1:            // MAIN MENU
                printf("~~~ Main menu ~~~\n") ;
                display_menu_windows(renderer, font,10,255) ;
                while(menu == 10)
                {
                    while(SDL_PollEvent(&e) != 0)
                    {
                        if  (e.type == SDL_MOUSEBUTTONUP)
                        { // react with mouse
                            if(e.button.x > 80 && e.button.x < 380 && e.button.y > 200 && e.button.y < 300)
                                menu = 20 ;  // go to the play menu
                            else if(e.button.x > 80 && e.button.x < 380 && e.button.y > 310 && e.button.y < 410)
                                menu = 40 ;  // go to the ranking menu
                            else if(e.button.x > w-250 && e.button.x < w-50 && e.button.y > h-250 && e.button.y < h-50)
                                menu = 90 ;  // go to the settings menu
                        }
                    }
                }
                break ;
            case 2:            // PLAY MENU
                printf("~~~ Play menu ~~~\n") ;
                display_menu_windows(renderer, font,20,100) ;
                while(menu == 20)
                {
                    while(SDL_PollEvent(&e) != 0 && play == 0)
                    {
                        if  (e.type == SDL_MOUSEBUTTONUP)
                        {
                            if (e.button.x > 80 && e.button.x < 380 && e.button.y > 200 && e.button.y < 300)
                            { // GAME SOLO
                                menu = 21 ;
                                player1 = choose_player_window(renderer, e, font, player1, 1, settings->p1[0], settings->p1[1], settings->p1[2], settings->p1[3]) ;
                                play = 1 ;
                            }
                            else if (e.button.x > 80 && e.button.x < 380 && e.button.y > 310 && e.button.y < 410)
                            { // GAME DUO
                                menu = 22 ;
                                player1 = choose_player_window(renderer, e, font, player1, 1, settings->p1[0], settings->p1[1], settings->p1[2], settings->p1[3]) ;
                                player2 = choose_player_window(renderer, e, font, player2, 2, settings->p2[0], settings->p2[1], settings->p2[2], settings->p2[3]) ;
                                play = 2 ;
                            }
                            else if (e.button.x > 80 && e.button.x < 380 && e.button.y > 420 && e.button.y < 520)
                            { // GAME YOU VS IA
                                menu = 23 ;
                                player1 = choose_player_window(renderer, e, font, player1, 1, settings->p1[0], settings->p1[1], settings->p1[2], settings->p1[3]) ;
                                playerAI = create_raccoon_player("Robot",2,10,68,68,1,2,3,4) ;
                                play = 3 ;
                            }
                            else if(e.button.x > w-250 && e.button.x < w-50 && e.button.y > h-250 && e.button.y < h-50)
                            { // go to main menu
                                menu = 10 ;
                            }
                        }
                    }
                }
                    if(play==1)
                    { // GAME SOLO
                        printf(" => GAME 1 PLAYER \n") ;

                        deviceId = pause_music(deviceId) ;
                        deviceId = play_music(deviceId, music_to_play) ;
                        menu = play_a_raccoon_game(renderer, font, e, player1, NULL, settings, 21, 0) ;
                        deviceId = pause_music(deviceId) ;
                        deviceId = play_music(deviceId, music_main_menu) ;
                        play = 0 ;
                        free_raccoon_player(player1);
                        player1 = NULL ;
                    }
                    else if(play==2){
                        printf(" => GAME 2 PLAYER \n") ;
                        deviceId = pause_music(deviceId) ;
                        deviceId = play_music(deviceId, music_to_play) ;
                        menu = play_a_raccoon_game(renderer, font, e, player1, player2, settings,22, 0) ;
                        deviceId = pause_music(deviceId) ;
                        deviceId = play_music(deviceId, music_main_menu) ;
                        play = 0 ;
                        free_raccoon_player(player1); free_raccoon_player(player2);
                        player1 = NULL ; player2 = NULL ;
                    }
                    else if(play==3){
                            // TEST IA
                            printf(" => GAME YOU VS IA \n") ;
                            int intelligence = 20 ;
                            deviceId = pause_music(deviceId) ;
                            deviceId = play_music(deviceId, music_to_play) ;
                            menu = play_a_raccoon_game(renderer, font, e, player1, playerAI, settings,23, 30) ;
                            deviceId = pause_music(deviceId) ;
                            deviceId = play_music(deviceId, music_main_menu) ;
                            play = 0 ;
                            free_raccoon_player(player1); free_raccoon_player(playerAI);
                            player1 = NULL ; playerAI = NULL ;
                        }

                break ;
            case 4:            // RANKING DISPLAY
                printf("~~~ Ranking display ~~~\n") ;
                display_ranking_menu(renderer,font,rankingMenuNum) ;
                while(menu/10==4)
                {
                    while(SDL_PollEvent(&e) != 0)
                    {
                        if  (e.type == SDL_MOUSEBUTTONUP)
                        {
                            if (e.button.x > 50 && e.button.x < 250 && e.button.y > h-250 && e.button.y < h-50)
                                menu = 10 ; // go to main menu
                            else if (e.button.x > 100 && e.button.x < 400 && e.button.y > 15 && e.button.y < 115 && rankingMenuNum==2){
                                rankingMenuNum = 1 ;
                                display_ranking_menu(renderer,font,rankingMenuNum) ;
                            }
                            else if (e.button.x > w-100-300 && e.button.x < w-100 && e.button.y > 15 && e.button.y < 115 && rankingMenuNum==1){
                                rankingMenuNum = 2 ;
                                display_ranking_menu(renderer,font,rankingMenuNum) ;
                            }
                        }
                    }
                }
                break ;
            case 9:            // SETTINGS MENU
                printf("~~~ Settings menu ~~~\n") ;
                display_settings_menu(renderer, font, settings) ;
                while(menu/10==9)
                {
                    while (SDL_PollEvent(&e) != 0) {
                        if (e.type == SDL_MOUSEBUTTONUP) {
                            //printf("x:[%d]   y:[%d]\n", e.button.x, e.button.y);

                            if (e.button.x > 50 && e.button.x < 250 && e.button.y > h - 250 && e.button.y < h - 50)
                                menu = 10;
                            else if (e.button.x > 716 && e.button.x < 716 + 75 && e.button.y > 172 && e.button.y < 172 + 75 && settings->speed < 5) {
                                settings->speed++;
                                display_settings_menu(renderer, font, settings);
                            }
                            else if (e.button.x > 320 && e.button.x < 320 + 75 && e.button.y > 172 && e.button.y < 172 + 75 && settings->speed > 1) {
                                settings->speed--;
                                display_settings_menu(renderer, font, settings);
                            }
                            else if(e.button.x > w-100 && e.button.x < w-25 && e.button.y > 25 && e.button.y < 100) {
                                printf("credits") ;
                                windows_menu_background(renderer, "image/credits.bmp",255);
                                SDL_RenderPresent(renderer);
                                while(!credits){
                                    if(SDL_PollEvent(&e) !=0){
                                        credits = 1 ;
                                    }
                                }
                                credits = 0 ;
                                display_settings_menu(renderer, font, settings);
                            }
                            else{
                                int x_positions[4] = {85, 220, 355, 490};
                                int y_positions[2] = {328, 472};
                                int key_indexes[4] = {0, 2, 1, 3};
                                int colorW[3] = {255,255,255} ; int colorR[3] = {255,0,0} ;
                                const char* key_names[4] = {"up","right", "down", "left"};

                                for (int i = 0; i < 4; i++) {
                                    for (int j = 0; j < 2; j++) {
                                        int x = x_positions[i];
                                        int y = y_positions[j];

                                        if (e.button.x > x && e.button.x < x + 110 && e.button.y > y && e.button.y < y + 40) {
                                            printf("Change %s key for player %d. \n", key_names[i], j + 1);
                                            block_rectangle(renderer,colorW, x, y, 110, 40);

                                            if (j == 0) {

                                                settings->p1[key_indexes[i]] = -1 ;
                                                settings->p1[key_indexes[i]] = change_key_move(get_key_move(renderer, e, settings, colorR, x, y, 110, 40), settings) ;
                                            } else if (j == 1) {
                                                settings->p2[key_indexes[i]] = -1 ;
                                                settings->p2[key_indexes[i]] = change_key_move(get_key_move(renderer, e, settings, colorR, x, y, 110, 40), settings) ;
                                            }
                                            display_settings_menu(renderer, font, settings);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                break ;
            default :
                printf("menu int = %d  ",menu) ;
        }
    }
    free(player1) ; free(player2) ; free(playerAI) ;

    SDL_RenderClear(renderer) ;
    SDL_CloseAudioDevice(deviceId);
    SDL_DestroyWindow(window) ;
    TTF_CloseFont(font);
    TTF_Quit() ;
    SDL_Quit() ;

    return EXIT_SUCCESS ;
}


void hold(){
    int go = 0 ;
     SDL_Event event;
    while(!go){
        SDL_WaitEvent(&event);
        switch(event.type){
            case SDL_QUIT:
                go = 1;
        }
    }
}
