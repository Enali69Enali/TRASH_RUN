#include "game.h"

///*    SDL2 display     *///
void block_rectangle(SDL_Renderer *renderer, int color[3], int posX, int posY, int dimW, int dimH){
    SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], 255);
    int strokeWidth = 5; // Epaisseur du trait

    // To make the rectangle thicker
    for (int i = 0; i < strokeWidth; i++) {
        SDL_Rect rect = { posX - i, posY - i, dimW + i * 2, dimH + i * 2 };
        SDL_RenderDrawRect(renderer, &rect);
    }
    SDL_RenderPresent(renderer);
}


void block_rectangle_image(SDL_Renderer *renderer, int posX, int posY, int dimW, int dimH, char str[50]){
    SDL_Surface * block = SDL_LoadBMP(str) ;
    SDL_Texture * block_texture = SDL_CreateTextureFromSurface(renderer, block) ;
    SDL_Rect b ; b.x = posX ; b.y = posY ; b.w = dimW ; b.h = dimH ;
    SDL_RenderCopy(renderer,block_texture,NULL,&b) ;
}

void windows_menu_background(SDL_Renderer *renderer, char str[50], int trans){
    /*  To generate the main background with *.bmp image and choose transparency. */
    SDL_RenderClear(renderer) ;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255) ;
    SDL_Surface * display_menu_windows = SDL_LoadBMP(str) ;
    SDL_Texture * main_menu_texture = SDL_CreateTextureFromSurface(renderer, display_menu_windows) ;
    SDL_SetTextureAlphaMod(main_menu_texture, trans) ;
    SDL_RenderCopy(renderer,main_menu_texture,NULL,NULL) ;
}


void display_menu_windows(SDL_Renderer *renderer, TTF_Font * font, int menu, int trans) {
    /*  To generate the different menu window with .bmp image in "image/" folder. */
    char *b1 ; char *b2 ; char *b3 ; char *br ; char *word;

    windows_menu_background(renderer,"image/background.bmp",trans) ;

    switch (menu) {
        case 10:    // MAIN MENU
            b1 = "image/menu_play_button.bmp" ;
            b2 = "image/ranking_button.bmp" ;
            b3 = "image/settings_button.bmp" ;
            block_rectangle_image(renderer, 80, 200, 300, 100, b1) ;
            block_rectangle_image(renderer, 80, 200+100+10, 300, 100, b2) ;
            block_rectangle_image(renderer, w-250, h-250, 200, 200, b3) ;
            break ;

        case 20:    // PLAY MENU
            b1 = "image/bouton_1.bmp" ;
            b2 = "image/bouton_2.bmp" ;
            b3 = "image/bouton_3.bmp" ;
            block_rectangle_image(renderer, 80, 200, 300, 100, b1) ;
            block_rectangle_image(renderer, 80, 200+100+10, 300, 100, b2) ;
            block_rectangle_image(renderer, 80, 200+2*(100+10), 300, 100, b3) ;
            br = "image/go_back_button.bmp" ;
            break ;
        case 80:    // GAME OVER
            b1 = "image/replay_button.bmp" ;
            b2 = "image/quit_button.bmp" ;
            word = "GAME OVER" ;
            SDL_Color color = {255, 255, 255} ;
            SDL_Rect word_space ; word_space.x = (w-(9*20))/2 ; word_space.y = 130 ; word_space.h = 50 ;
            word_space.w = strlen(word)*20 ;
            SDL_Surface * texte_word = TTF_RenderText_Blended(font, word, color );
            SDL_Texture * texte_word_texture = SDL_CreateTextureFromSurface(renderer, texte_word) ;
            SDL_RenderCopy(renderer,texte_word_texture,NULL,&word_space) ;
        default :
            printf("error") ;
    }

    if (menu != 10){
        block_rectangle_image(renderer, w-250, h-250, 200, 200, br) ;
    }

    SDL_RenderPresent(renderer) ;
}

void display_ranking_menu(SDL_Renderer *renderer, TTF_Font * font, int numOFplayer){
    /*  To generate the rancking menu with *.bmp image and the list of the best player  . */
    windows_menu_background(renderer, "image/hanking_menu.bmp", 255) ;
    block_rectangle_image(renderer, 50, h-250, 200, 200, "image/go_back_button.bmp") ;
    block_rectangle_image(renderer, 100, 15, 300, 100, "image/bouton_1.bmp") ;
    block_rectangle_image(renderer, w-100-300, 15, 300, 100, "image/bouton_2.bmp") ;

    if (numOFplayer==1)
        block_rectangle_image(renderer, w-100-300, 15, 300, 100, "image/dark_layer.bmp") ;
    else
        block_rectangle_image(renderer, 100, 15, 300, 100, "image/dark_layer.bmp") ;
    //char *hat_buffer ;

    int game ;
    unsigned int score ;
    unsigned int ranking1score = 0 ;
    unsigned int ranking2score = 0 ;
    unsigned int ranking3score = 0 ;
    unsigned int ranking4score = 0 ;
    unsigned int ranking5score = 0 ;
    unsigned int hat = 0 ;
    unsigned int ranking1hat = 0 ;
    unsigned int ranking2hat = 0 ;
    unsigned int ranking3hat = 0 ;
    unsigned int ranking4hat = 0 ;
    unsigned int ranking5hat = 0 ;

    char name[6] ;
    char ranking1name[6] = "" ;
    char ranking2name[6] = "" ;
    char ranking3name[6] = "" ;
    char ranking4name[6] = "" ;
    char ranking5name[6] = "" ;

    FILE * players_file = fopen("TRASHRUN_player_file.csv", "r") ;

    if ( players_file == NULL ) {
        fprintf( stderr, "Cannot open file for writing\n" );
        //exit( -1 );
    }

    while (fscanf(players_file, "%d : %s ; %d : %d\n", &game, name, &hat, &score)==4) {   //nombre de string
        if (game == numOFplayer){
            if (score > ranking1score) {
                strcpy(ranking5name,ranking4name) ;
                strcpy(ranking4name,ranking3name) ;
                strcpy(ranking3name,ranking2name) ;
                strcpy(ranking2name,ranking1name) ;
                strcpy(ranking1name,name) ;
                ranking5score = ranking4score ;
                ranking4score = ranking3score ;
                ranking3score = ranking2score ;
                ranking2score = ranking1score ;
                ranking1score = score ;
                ranking5hat = ranking4hat ;
                ranking4hat = ranking3hat ;
                ranking3hat = ranking2hat ;
                ranking2hat = ranking1hat ;
                ranking1hat = hat ;
            }
            if (score < ranking1score && score > ranking2score) {
                strcpy(ranking5name,ranking4name) ;
                strcpy(ranking4name,ranking3name) ;
                strcpy(ranking3name,ranking2name) ;
                strcpy(ranking2name,name) ;
                ranking5score = ranking4score ;
                ranking4score = ranking3score ;
                ranking3score = ranking2score ;
                ranking2score = score ;
                ranking5hat = ranking4hat ;
                ranking4hat = ranking3hat ;
                ranking3hat = ranking2hat ;
                ranking2hat = hat ;
            }
            if (score < ranking2score && score > ranking3score){
                strcpy(ranking5name,ranking4name) ;
                strcpy(ranking4name,ranking3name) ;
                strcpy(ranking3name,name) ;
                ranking5score = ranking4score ;
                ranking4score = ranking3score ;
                ranking3score = score ;
                ranking5hat = ranking4hat ;
                ranking4hat = ranking3hat ;
                ranking3hat = hat ;
            }
            if (score < ranking3score && score > ranking4score){
                strcpy(ranking5name,ranking4name) ;
                strcpy(ranking4name,name) ;
                ranking5score = ranking4score ;
                ranking4score = score ;
                ranking5hat = ranking4hat ;
                ranking4hat = hat ;
            }
            if (score < ranking4score && score > ranking5score){
                strcpy(ranking5name,name) ;
                ranking5score = score ;
                ranking5hat = hat ;
            }
        }
    }

   	fclose(players_file) ;
    int decalage = 54 ;
    char scorestr[20];

    char* ranking_names[] = {ranking1name, ranking2name, ranking3name, ranking4name, ranking5name};
    int ranking_scores[] = {ranking1score, ranking2score, ranking3score, ranking4score, ranking5score};
    int ranking_hats[] = {ranking1hat, ranking2hat, ranking3hat, ranking4hat, ranking5hat};

    for (int i = 0; i < 5; i++) {
        int y = 190 + decalage * i;
        display_name(renderer, font, ranking_names[i], 1, 550, y, -1);
        sprintf(scorestr, "%d", ranking_scores[i]);
        display_name(renderer, font, scorestr, 1, 210 - 20 * (digit_number(ranking_scores[i]) - 7), y, -1);
        block_rectangle_image(renderer, 740, 170 + decalage * i, 70, 70, get_hat_image_name(ranking_hats[i]));
    }

    SDL_RenderPresent(renderer) ;
}

void display_settings_menu(SDL_Renderer *renderer, TTF_Font * font, settings * s){
    /*  To generate the rancking menu with *.bmp image and the list of the best player  . */
    SDL_RenderClear(renderer) ;
    SDL_Surface * menu = SDL_LoadBMP("image/settings_menu.bmp") ;
    SDL_Texture * menu_texture = SDL_CreateTextureFromSurface(renderer, menu) ;
    SDL_RenderCopy(renderer,menu_texture,NULL,NULL) ;

    block_rectangle_image(renderer,50,h-250,200,200,"image/go_back_button.bmp") ;
    block_rectangle_image(renderer,w-100,25,75,75,"image/credits_logo.bmp") ;
    block_rectangle_image(renderer,320,172,75,75,"image/minus_button.bmp") ;
    block_rectangle_image(renderer,716,172,75,75,"image/plus_button.bmp") ;
    block_rectangle_image(renderer,518+(75*(s->speed-3)),172,75,75,"image/pointer.bmp") ;

    int player_keys[2][4];
    memcpy(player_keys[0], s->p1, sizeof(s->p1));
    memcpy(player_keys[1], s->p2, sizeof(s->p2));
    char keyName[2] ;
    for(int k=0 ; k<2; k++){
        for(int a = 0 ; a< 4; a++){
            printf("%d mmm\n",player_keys[k][a]) ;
            if (player_keys[k][a]== SDLK_UP || player_keys[k][a]== SDLK_DOWN || player_keys[k][a]== SDLK_RIGHT || player_keys[k][a]== SDLK_LEFT ) {
                //keyName[0] = s->p1[a];
                switch(player_keys[k][a]){
                    case SDLK_UP :
                        block_rectangle_image(renderer,155+(a%2)*270+(a/2)*135,330+140*k,35,35,"image/arrow1.bmp") ;
                        break ;
                    case SDLK_DOWN :
                        block_rectangle_image(renderer,155+(a%2)*270+(a/2)*135,330,35,35,"image/arrow2.bmp") ;
                        break ;
                    case SDLK_RIGHT :
                        block_rectangle_image(renderer,155+(a%2)*270+(a/2)*135,330,35,35,"image/arrow3.bmp") ;
                        break ;
                    case SDLK_LEFT :
                        block_rectangle_image(renderer,155+(a%2)*270+(a/2)*135,330,35,35,"image/arrow4.bmp") ;
                        break ;
                }
            } else {
                keyName[0] = player_keys[k][a];
                display_name(renderer,font,keyName,1,155+(a%2)*270+(a/2)*135,330+140*k,1) ;
            }

        }

    }

    SDL_RenderPresent(renderer) ;
}

void display_gameover_menu(SDL_Renderer *renderer, TTF_Font * font, int trans, raccoon_player *r1, raccoon_player *r2) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255) ;
    windows_menu_background(renderer,"image/background.bmp",trans)  ;

    block_rectangle_image(renderer, 80, 250, 300, 100, "image/menu_button.bmp") ;
    block_rectangle_image(renderer, 80, 250+100+10, 300, 100, "image/quit_button.bmp") ;
    block_rectangle_image(renderer, 450, 550, 300, 100, "image/replay_button.bmp") ;

    char scorestr[33] ;
    if (r1 != NULL) {
        sprintf(scorestr, "Player%d score : %d", r1->player, r1->score);
        display_name(renderer, font, scorestr, 1, 100, 100+50*(r1->player-1),-1) ;
    }
    if (r2 != NULL) {
        sprintf(scorestr, "Player%d score : %d", r2->player, r2->score);
        display_name(renderer, font, scorestr, 1, 100, 100+50*(r2->player-1),-1) ;
    }
    if (r1 != NULL && r2 != NULL) {
        char winner[50];
        if (r1->score > r2->score)
            strcpy(winner, "Player 1 win!");
        else if (r1->score < r2->score)
            strcpy(winner, "Player 2 win!");
        else
            strcpy(winner, "There's a tie.");
        display_name(renderer, font, winner, 1, 450, 350, -1);
    }

    SDL_RenderPresent(renderer) ;
}

void display_pause(SDL_Renderer *renderer, TTF_Font * font, SDL_Event e, raccoon_player *r1, raccoon_player *r2){
    //windows_menu_background(renderer, "image/pause_menu.bmp",255);
    int pause = 1 ;
    SDL_Surface * display_menu_windows = SDL_LoadBMP( "image/pause_menu.bmp") ;
    SDL_Texture * main_menu_texture = SDL_CreateTextureFromSurface(renderer, display_menu_windows) ;
    SDL_SetTextureAlphaMod(main_menu_texture, 255) ;
    SDL_RenderCopy(renderer,main_menu_texture,NULL,NULL) ;

    SDL_RenderPresent(renderer) ;
    printf("-> pause\n") ;
    //SDL_Delay(5000) ;
    while(pause){
        while(SDL_PollEvent(&e) !=0 && e.type == SDL_KEYDOWN){
            if (SDLK_SPACE == e.key.keysym.sym){
                pause = 0 ;
                printf("-> play\n") ;
            }
        }
    }
    SDL_RenderClear(renderer) ;
    //count_down(renderer,font,r1,r2) ;
}

void game_playground_window(SDL_Renderer *renderer) {
    SDL_RenderClear(renderer) ;
    SDL_Rect window ; window.x = 0 ; window.y = 0 ; window.w = w ; window.h = h ;
    SDL_SetRenderDrawColor(renderer, 109, 127, 113, 255) ;
    SDL_RenderDrawRect(renderer, &window) ;
    SDL_Rect background_game ; background_game.x = (w-Xfield)/2 ; background_game.y = (h-Yfield)/2 ; background_game.w = Xfield ; background_game.h = Yfield ;
    SDL_RenderDrawRect(renderer, &background_game) ;
    SDL_Rect game_field ; game_field.x = (w-Xfield)/2 ; game_field.y = (h-Yfield)/2 ; game_field.w = Xfield ; game_field.h = Yfield ;
    SDL_SetRenderDrawColor(renderer, 166, 172, 171, 255) ;
    SDL_RenderDrawRect(renderer, &game_field) ;
    int n = (Xfield/10)*(Yfield/10) ; int posX, posY ;
    for (int compteur=0 ; compteur< n ; compteur++) {
        posX = (compteur%(Xfield/10))*10 ;
        posY = (compteur/(Yfield/10))*10 ;
        SDL_Rect c ; c.x = ((w-Xfield)/2)+posX ; c.y = ((h-Yfield)/2)+posY ; c.w = 10 ; c.h = 10 ;
        SDL_SetRenderDrawColor(renderer, 166, 172, 171, 255);
        SDL_RenderDrawRect(renderer, &c) ;
    }
}

void count_down(SDL_Renderer *renderer, TTF_Font * font, raccoon_player *r1, raccoon_player *r2) {
    SDL_RenderClear(renderer) ;
    SDL_Rect window ; window.x = 0 ; window.y = 0 ; window.w = w ; window.h = h ;
    SDL_SetRenderDrawColor(renderer, 109, 127, 113, 255) ;
    char scorestr[4]; char num_of_player1[9] = "1 PLAYER" ; char num_of_player2[9] ;
    printf("Count-down 3 secondes.\n") ;
        /// probleme
    if(r2 != NULL){
        strcpy(num_of_player2, "2 PLAYER") ;
    }

    for(int c = 3; c>0 ; c--){
        SDL_RenderClear(renderer) ;
        SDL_RenderDrawRect(renderer, &window) ;
        sprintf(scorestr, "%d", c) ;
        display_name(renderer, font, scorestr, 3, 395-10, 200, -1) ;
        display_name(renderer, font, num_of_player1, 2, 70, 400, -1) ;
        display_name(renderer, font, r1->name, 2, 70, 500, -1) ;
        if(r2 != NULL){
            display_name(renderer, font, num_of_player2, 2, 450, 400, -1) ;
        display_name(renderer, font, r2->name, 2, 450, 500, -1) ;
        }
        SDL_RenderPresent(renderer) ;
        SDL_Delay(1000) ;
    }
}


///*    ABOUT THE PLAYER     *///
raccoon_player * choose_player_window(SDL_Renderer *renderer, SDL_Event e, TTF_Font * font, raccoon_player *r, int player,int key_up, int key_down, int key_right, int key_left){
    /* To generate the player menu and see/choose their name, their score, their raccoon skin. */
    const char* hat_image = "image/hat" ; const char* bmp_image = ".bmp" ; char *hat_num ; char hat_buffer[15] ;
    char * name ;
    int sideX ; sideX = 350 ;
    int hatboxX = 265 ; int hatboxY = 445 ;
    char playerstr[9] ;
    sprintf(playerstr, "PLAYER %d", player);

    SDL_Surface * raccoon_player_image ; SDL_Texture * raccoon_player_image_texture ;
    SDL_Surface * raccoon_hat_image ; SDL_Texture * raccoon_hat_image_texture ;
    SDL_Surface * play_button_image ; SDL_Texture * play_button_image_texture ;

    SDL_Rect player_skin ; player_skin.x = w-sideX-50+20 ; player_skin.y = 50+40 ; player_skin.w = 300 ; player_skin.h = 267 ;
    SDL_Rect hatbox ; hatbox.w = 70 ; hatbox.h = 70 ;
    SDL_Rect play_button ; play_button.x = 550 ; play_button.y = 655 ; play_button.w = 250 ; play_button.h = 125 ;

    SDL_Rect square = { 0, 0, 10, 10};
    int first = 1 ;

    r = choose_player_name(renderer, e, font, r, player, key_up, key_down, key_right, key_left) ;
    name = r->name ;


    do {
        SDL_RenderClear(renderer) ;
        windows_menu_background(renderer,"image/choose_profil_raccoon.bmp",255) ;
        display_name(renderer, font, playerstr, 1, 100, 50,-1) ;
        for (int hat_counter = 0 ; hat_counter<=8 ; hat_counter++){
            hatbox.x = hatboxX + (hat_counter%4)*121 ; hatbox.y = hatboxY + (hat_counter/4)*115 ;
            if (asprintf(&hat_num, "%d", hat_counter) == -1) {
                perror("asprintf");
            } else {
                strcat(strcpy(hat_buffer, hat_image), hat_num) ;
                strcat(strcpy(hat_buffer, hat_buffer), bmp_image) ;
                raccoon_hat_image = SDL_LoadBMP(hat_buffer) ;
                raccoon_hat_image_texture = SDL_CreateTextureFromSurface(renderer, raccoon_hat_image) ;
                SDL_RenderCopy(renderer,raccoon_hat_image_texture,NULL,&hatbox) ;
            }
            square.x = hatboxX + (hat_counter%4)*121 + 55 ;
            square.y = hatboxY + (hat_counter/4)*115 + 55 ;
            SDL_SetRenderDrawColor(renderer, color_dictionary[hat_counter]->R, color_dictionary[hat_counter]->G, color_dictionary[hat_counter]->B, 255); // Rouge (RVBA)
            SDL_RenderFillRect(renderer, &square);
        }
        free(hat_num) ;
        play_button_image = SDL_LoadBMP("image/play_button.bmp") ;

        if (first) {
            raccoon_player_image = SDL_LoadBMP("image/raccoon_player_hat0.bmp") ;
            raccoon_player_image_texture = SDL_CreateTextureFromSurface(renderer, raccoon_player_image) ;
            SDL_RenderCopy(renderer,raccoon_player_image_texture,NULL,&player_skin) ;

            SDL_RenderPresent(renderer) ;

            display_name(renderer, font, name, 1, 70, 120,-1) ;

            play_button_image_texture = SDL_CreateTextureFromSurface(renderer, play_button_image) ;
            SDL_RenderCopy(renderer,play_button_image_texture,NULL,&play_button) ;
            SDL_RenderPresent(renderer) ;
            first = 0 ;
        }

        play_button_image_texture = SDL_CreateTextureFromSurface(renderer, play_button_image) ;
        SDL_RenderCopy(renderer,play_button_image_texture,NULL,&play_button) ;
        display_name(renderer, font, name, 1, 70, 120,-1) ;
    } while ( first || (!choose_player_skin(renderer, e, player_skin, raccoon_player_image,raccoon_player_image_texture, r))) ;
    printf("You have finished configuring your character.\n") ;
    return r ;
}

raccoon_player * choose_player_name(SDL_Renderer *renderer, SDL_Event e, TTF_Font * font, raccoon_player *r, int player, int key_up, int key_down, int key_right, int key_left){
    int find_name = 0 ;
    int name_lenght = 0 ;
    char name[6];

    printf("Give your raccoon's name : \n") ;
    windows_menu_background(renderer,"image/choose_name_raccoon.bmp",255) ;
    SDL_RenderPresent(renderer) ;

    while(! find_name){
        while(SDL_PollEvent(&e) !=0 && e.type == SDL_KEYDOWN){

            if(e.key.keysym.sym == SDLK_RETURN){
                if (name_lenght > 0 && name_lenght <= 5)
                {
                    find_name = 1 ;
                }
                else
                    printf("\nPlease provide a name with 1 letter min and 10 max.\n") ;
            }
            else {
                if(e.key.keysym.sym == SDLK_BACKSPACE && name_lenght > 0){
                        name_lenght -- ;
                        name[name_lenght] = '\0' ; //NULL ;
                }
                else if(e.key.keysym.sym>=SDLK_a && e.key.keysym.sym<=SDLK_z && name_lenght < 5){
                    //printf("%d -> %c \n", e.key.keysym.sym,e.key.keysym.sym) ;
                    name[name_lenght] = e.key.keysym.sym ;
                    name_lenght ++ ;
                }

            }
            SDL_RenderClear(renderer) ;
                windows_menu_background(renderer,"image/choose_name_raccoon.bmp",255) ;

                display_name(renderer,font, name,2,180,340,108) ;
                SDL_RenderPresent(renderer) ;
        }
    }
    return create_raccoon_player(name,player,0,1+67*(player-1),1+67*(player-1),key_up, key_down, key_right, key_left) ;  //create(name,0) ;
}

int choose_player_skin(SDL_Renderer *renderer, SDL_Event e, SDL_Rect skinbox, SDL_Surface * raccoon_player_hat_image, SDL_Texture * raccoon_player_hat_image_texture, raccoon_player *r){         //pb
    int sideX, sideY ;
    /*const char* raccoon_player_image = "image/raccoon_player_hat" ; const char* bmp_image = ".bmp" ; char *hat_num ;*/ char *hat_buffer ;
    while (1) {
        while(SDL_PollEvent(&e) !=0){
            if  (e.type == SDL_MOUSEBUTTONUP){
                if (e.button.x > 550 && e.button.x < 550+250 && e.button.y > 655 && e.button.y < 655+125){
                    printf("tu as appuye sur play.\n") ;
                    return 1 ;
                } else {
                    for(int hat_counter = 0 ; hat_counter<=8 ; hat_counter++){
                        sideX = 265 + (hat_counter%4)*121 ;
                        sideY = 445 + (hat_counter/4)*115 ;
                        if(e.button.x > sideX && e.button.x < sideX+70 && e.button.y > sideY && e.button.y < sideY+70) {
                            /*if (asprintf(&hat_num, "%d", hat_counter) == -1) {
                                perror("asprintf");
                            } else {*/

                                //strcat(strcpy(hat_buffer, raccoon_player_image), hat_num) ;
                                //strcat(strcpy(hat_buffer, hat_buffer), bmp_image) ;
                                hat_buffer = get_hat_image_name(hat_counter) ;
                                printf("hat : %d ---> %s\n", hat_counter,hat_buffer) ;
                                raccoon_player_hat_image = SDL_LoadBMP(hat_buffer) ;
                                raccoon_player_hat_image_texture = SDL_CreateTextureFromSurface(renderer, raccoon_player_hat_image) ;
                                SDL_RenderCopy(renderer,raccoon_player_hat_image_texture,NULL,&skinbox) ;
                                SDL_RenderPresent(renderer) ;
                                r->hat = hat_counter ;
                            //}
                            SDL_RenderPresent(renderer) ;
                            return 0 ;
                        }
                    }
                }
            }
        }
    }
}

void display_name(SDL_Renderer *renderer, TTF_Font * font, char name[6], int sizefont, int x, int y,int space){
    int blockW = 20*sizefont ; int blockH = 50*sizefont ;
    SDL_Color color = {255, 255, 255} ;
    SDL_Rect name_space ; name_space.x = x ; name_space.y = y ; name_space.h = blockH ;
    SDL_Surface* texte_name = NULL ; SDL_Texture * texte_name_texture = NULL ;


    if (space == -1 ){
        name_space.w = strlen(name)*blockW ;
        texte_name = TTF_RenderText_Blended(font, name, color );
        texte_name_texture = SDL_CreateTextureFromSurface(renderer, texte_name) ;
        SDL_RenderCopy(renderer,texte_name_texture,NULL,&name_space) ;
        SDL_FreeSurface(texte_name);
        SDL_DestroyTexture(texte_name_texture);
    }
    else {
        char letter[2];
        letter[1] = NULL ;
        for (int a = 0 ; a<strlen(name) ; a++){
            letter[0] = name[a] ;
            name_space.w = blockW ;
            name_space.x = x + space*a ;

            texte_name = TTF_RenderText_Blended(font, letter, color );
            texte_name_texture = SDL_CreateTextureFromSurface(renderer, texte_name) ;
            SDL_RenderCopy(renderer,texte_name_texture,NULL,&name_space) ;
            SDL_FreeSurface(texte_name);
            SDL_DestroyTexture(texte_name_texture);
        }
    }

}

void display_player(SDL_Renderer *renderer, int Xfield, int Yfield, raccoon_player * player) {
    SDL_Rect p ;
    p.x = ((w-Xfield)/2)+(player->actual_posX%(Xfield/10))*10-20 ; p.y = ((h-Yfield)/2)+(player->actual_posY%(Yfield/10))*10-20 ; p.w = 50 ; p.h = 50 ;
    //printf("%d , %d, %d\n",player->hat) ;
    /*SDL_SetRenderDrawColor(renderer, color_dictionary[player->hat]->R, color_dictionary[player->hat]->G, color_dictionary[player->hat]->B, 255) ;  // keep just the color
    SDL_RenderFillRect(renderer, &p) ;*/
    block_rectangle_image(renderer, p.x, p.y, p.w, p.h,  get_hat_image_name(player->hat)) ;
}

void display_track(SDL_Renderer *renderer, raccoon_player * player){
    SDL_Rect track ;
    track.w = 10 ; track.h = 10 ;
    waste_trail * wt = player -> wt ;
    int track_color[3] = {color_dictionary[player->hat]->R, color_dictionary[player->hat]->G, color_dictionary[player->hat]->B } ;
    for(int c=0 ; c<3 ; c++){
        if (track_color[c]>40){
            track_color[c] -= 40 ;
        }
    }
    while(wt!=NULL) {
        track.x = ((w-Xfield)/2)+(wt->posX %(Xfield/10))*10 ;
        track.y = ((h-Yfield)/2)+(wt->posY %(Yfield/10))*10 ;

        SDL_SetRenderDrawColor(renderer, track_color[0], track_color[1] ,track_color[2], 255) ;
        SDL_RenderFillRect(renderer, &track) ;
        wt = wt->next ;
    }
}

void display_occupied_case(SDL_Renderer *renderer, int mat[70][70] ,int num,int color[3]) {
    SDL_Rect case_playground ;
    case_playground.w = 30 ; case_playground.h = 30 ;
    for(int x=0;x<70;x++) {
        for(int y=0;y<70;y++) {
            //printf("%d\n",mat[x][y]) ;
            if(mat[x][y]==num){
                //printf(" ===>%d,%d\n",x,y) ;
                case_playground.x = ((w-Xfield)/2)+(x %(Xfield/10))*10-10 ;
                case_playground.y = ((h-Yfield)/2)+(y %(Yfield/10))*10-10 ;
                /*SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], 100) ;     // keep just the color
                SDL_RenderFillRect(renderer, &case_playground) ;*/
                block_rectangle_image(renderer, case_playground.x, case_playground.y, case_playground.w, case_playground.h,"image/hat3.bmp") ;
            }
        }
    }
}







SDL_AudioDeviceID play_music(SDL_AudioDeviceID deviceId, const char* music_path) {
    SDL_AudioSpec wavSpec;
    Uint8* wavBuffer;
    Uint32 wavLength;

    // Charger le fichier audio
    if (SDL_LoadWAV(music_path, &wavSpec, &wavBuffer, &wavLength) == NULL) {
        fprintf(stderr, "Impossible de charger le fichier audio : %s\n", SDL_GetError());
        return NULL ;
    }

    // Ouvrir le périphérique audio
    deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
    if (deviceId == 0) {
        fprintf(stderr, "Impossible d'ouvrir le périphérique audio : %s\n", SDL_GetError());
        SDL_FreeWAV(wavBuffer);
        return NULL ;
    }

    // Jouer l'audio
    int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
    SDL_PauseAudioDevice(deviceId, 0);
    //SDL_Delay(5000) ;

    SDL_FreeWAV(wavBuffer);
    return deviceId ;
}

SDL_AudioDeviceID pause_music(SDL_AudioDeviceID deviceId) {
    SDL_PauseAudioDevice(deviceId, 1);
    return deviceId ;
}

