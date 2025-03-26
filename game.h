#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "SDL.h"
#include <SDL_ttf.h>

#ifndef GAME_H
#define GAME_H

extern const int w ;
extern const int h ;
extern const int Xfield ;
extern const int Yfield ;
extern const int paddingX ;
extern const int paddingY ;


typedef struct Swaste_trail{
    int posX, posY ;
    struct Swaste_trail * next ;
} waste_trail ;

typedef struct{
    int player ;
    char name[5] ;
    unsigned int score;
    //int color[3] ;
    int hat ;
    int actual_posX, actual_posY ;
    int up, down, right, left ;
    int last_move ;
    waste_trail * wt ;
} raccoon_player ;

typedef struct {
    unsigned int speed ;
    int p1[4] ;
    int p2[4] ;
} settings ;

typedef struct {
    char name[20];
    int R;
    int G;
    int B;
} Color;


extern const Color * color_dictionary[11] ;



///*    SDL2 display     *///
void block_rectangle(SDL_Renderer *renderer, int color[3], int posX, int posY, int dimW, int dimH) ;
void block_rectangle_image(SDL_Renderer *renderer, int posX, int posY, int dimW, int dimH, char str[50]) ;
/* To facilitate rectangle creation from images. */
void windows_menu_background(SDL_Renderer *renderer, char str[50], int trans) ;
/* To facilitate windows menu creation from images. */

void display_menu_windows(SDL_Renderer *renderer, TTF_Font * font, int menu, int trans) ;
/* To generate menu windows from images. */
void display_ranking_menu(SDL_Renderer *renderer, TTF_Font * font, int numOFplayer) ;
/* To generate player ranking menu window. */
void display_settings_menu(SDL_Renderer *renderer, TTF_Font * font, settings * s) ;
/* To generate settings menu window. */
void display_gameover_menu(SDL_Renderer *renderer, TTF_Font * font, int trans, raccoon_player *r1, raccoon_player *r2) ;
/* To generate gameover menu window. */

void game_playground_window(SDL_Renderer *renderer) ;
/* To generate game playground window. */
void display_occupied_case(SDL_Renderer *renderer, int mat[70][70] ,int num,int color[3]) ;

void count_down(SDL_Renderer *renderer, TTF_Font * font, raccoon_player *r1, raccoon_player *r2) ; ///here
/* To present a countdown to the release of raccoons in this alleyway. */
void display_pause(SDL_Renderer *renderer, TTF_Font * font, SDL_Event e, raccoon_player *r1, raccoon_player *r2) ;


///*    ABOUT THE PLAYER     *///
/// Create player
raccoon_player * create_raccoon_player(char n[5], int p, int h, int initial_posX, int initial_posY, int key_up, int key_down, int key_right, int key_left) ;
/* To add each player's information to the structure of his/her/their raccoon. */
void free_raccoon_player (raccoon_player * r);
raccoon_player * replay_raccoon_player (raccoon_player * r, int initial_posX, int initial_posY, int last_move) ;
waste_trail * create_waste_trail(int initial_posX, int initial_posY) ;
/* To create a track trash. */
waste_trail * append_waste_trail(waste_trail *wt, int actual_posX, int actual_posY) ;
/* To add a track trash in waste structure. */
char* get_hat_image_name(int hat) ;
/* To get the hat image skin name in folder "/image." */

/// Display
raccoon_player * choose_player_window(SDL_Renderer *renderer, SDL_Event e, TTF_Font * font, raccoon_player *r, int player,int key_up, int key_down, int key_right, int key_left) ;
/* To generate all windows for customizing the player's raccoon before starting the game (name, skin, ... ). */
raccoon_player * choose_player_name(SDL_Renderer *renderer, SDL_Event e, TTF_Font * font, raccoon_player *p, int player, int key_up, int key_down, int key_right, int key_left) ;
/* To generate window to choose your raccoon name. */
int choose_player_skin(SDL_Renderer *renderer, SDL_Event e, SDL_Rect skinbox, SDL_Surface * raccoon_player_hat_image, SDL_Texture * raccoon_player_hat_image_texture, raccoon_player *r) ;
/* To return the number of the hat chosen by the player, it's funnier with funny hats ! */

void display_name(SDL_Renderer *renderer, TTF_Font * font, char name[6], int sizefont, int x, int y,int space) ;
/* Using lib SDL2 TTF to display the raccoon's name. */

void display_player(SDL_Renderer *renderer, int Xfield, int Yfield, raccoon_player * player) ;
/* To place the player on the game grid. */
void display_track(SDL_Renderer *renderer, raccoon_player * player) ;
/* To display the raccoon player track. */

/// Play
int play_a_raccoon_game(SDL_Renderer *renderer, TTF_Font * font, SDL_Event e, raccoon_player *r1, raccoon_player *r2, settings * s, int menu, int ai) ;
/* To launch a game according to the arguments play the game 1 player or with 2. */
raccoon_player * raccoon_move(raccoon_player * player, int direction_move) ;
/* To modify the player raccoon's position according to its choice of movement. */
int get_opposite_move(raccoon_player *p, int direction_move) ;
/* To get opposite move to prevent the player from following in his footsteps. */
int get_case_statement(int mat[70][70], int actual_posX, int actual_posY) ;
/* To get the case number :
        0 -> free
        1 -> player 1 track
        2 -> player 2 track
        4 -> trash fence */
int player_gameOver(int mat[70][70], raccoon_player * player) ;
/* To return a positive integer if the game is over. */
int player_gameOver_score(raccoon_player * player, int game_over) ;
/* To return score after a game over :
        + 50 -> if you win
        - 20 -> if you touched your own track
        - 50 -> if you hit your opponent's track
        - 10 -> out of the field
        - 30 -> -> you've tripped over a piece of garbage. */

int get_key_move(SDL_Renderer *renderer, SDL_Event e, settings * s, int color[3], int posX, int posY, int dimW, int dimH) ;
/* To get key move of a player. */
int change_key_move(int new_move, settings * s) ;
/* To change key move in settings. */

int AImove(int mat[70][70], int actual_posX, int actual_posY, int intelligence, raccoon_player * player) ;
/* To give the AI robot raccoon's next move. */



void save_csv(raccoon_player *p, int game) ;
/* To save your game with the form : "player_number : name ; hat_number : score". */

settings * create_settings() ;
/* To set player keypad parameters and to select raccoon speed. */



int digit_number(int num) ;



SDL_AudioDeviceID play_music(SDL_AudioDeviceID deviceId, const char* music_path) ;
/* To play a tune with its name in the "music/" directory. */
SDL_AudioDeviceID pause_music(SDL_AudioDeviceID deviceId) ;
/* To pause current music. */

#endif
