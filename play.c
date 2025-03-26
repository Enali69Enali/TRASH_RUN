#include "game.h"
FILE* players_file ;

///* Create player *///
raccoon_player * create_raccoon_player (char n[5], int p, int h, int initial_posX, int initial_posY, int key_up, int key_down, int key_right, int key_left){
    raccoon_player *ret = (raccoon_player *) malloc (sizeof(raccoon_player)) ;
	ret->player = p ;
    strcpy(ret->name, n);
	ret->score = 0 ;
	ret->hat = h ;
	ret->actual_posX = initial_posX ;
	ret->actual_posY = initial_posY ;
	ret->up = key_up ;
	ret->down = key_down ;
	ret->right = key_right ;
	ret->left = key_left ;
	ret->last_move = 0 ;
	ret->wt = create_waste_trail(initial_posX, initial_posY) ;
	return ret ;
}

void free_raccoon_player (raccoon_player * r) {
    free(r->wt);
    free(r);
}

raccoon_player * replay_raccoon_player (raccoon_player * r, int initial_posX, int initial_posY, int last_move){
	r->score = 0 ;
	r->actual_posX = initial_posX ;
	r->actual_posY = initial_posY ;
	r->last_move = last_move;
	r->wt = create_waste_trail(initial_posX, initial_posY) ;
	return r ;
}

waste_trail * create_waste_trail(int initial_posX, int initial_posY){
    waste_trail *ret = (waste_trail *) malloc (sizeof(waste_trail)) ;
    ret->posX = initial_posX ;
    ret->posY = initial_posY ;
    ret->next = NULL ;
    return ret ;
}

waste_trail * append_waste_trail (waste_trail *wt, int actual_posX, int actual_posY){
	if(wt == NULL){
		return create_waste_trail(actual_posX, actual_posY) ;
	}
	else{
		wt->next = append_waste_trail( wt->next, actual_posX, actual_posY) ;
	}
	return wt ;
}

char* get_hat_image_name(int hat) {
    char* hat_buffer = malloc(24 * sizeof(char)); // Allouer de l'espace mémoire pour la chaîne
    if (hat_buffer == NULL) {
        printf("Erreur d'allocation de mémoire.\n");
        return NULL ;
    }
    const char* raccoon_player_image = "image/raccoon_player_hat" ; const char* bmp_image = ".bmp" ; char *hat_num ;
    if (asprintf(&hat_num, "%d", hat) == -1) {
        perror("asprintf");
    } else {
        strcat(strcpy(hat_buffer, raccoon_player_image), hat_num) ;
        strcat(strcpy(hat_buffer, hat_buffer), bmp_image) ;
        return hat_buffer ;
    }
}


///* Raccoon move *///
raccoon_player * raccoon_move(raccoon_player * player, int direction_move) {
    int key_up = player->up ;
    int key_down = player->down ;
    int key_right = player->right ;
    int key_left = player -> left ;

    if (direction_move == key_up) {
        player->actual_posY--;
    } else if (direction_move == key_down) {
        player->actual_posY++;
    } else if (direction_move == key_right) {
        player->actual_posX++;
    } else if (direction_move == key_left) {
        player->actual_posX--;
    }
    return player ;
}

int get_opposite_move(raccoon_player *player, int direction_move){
    int opp = 0 ;
    int key_up = player->up ;
    int key_down = player->down ;
    int key_right = player->right ;
    int key_left = player -> left ;

    if (direction_move == key_up) {
        opp = key_down ;
    } else if (direction_move == key_down) {
         opp = key_up ;
    } else if (direction_move == key_right) {
          opp = key_left ;
    } else if (direction_move == key_left) {
          opp = key_right ;
    }

    return opp ;
}

int get_case_statement(int mat[70][70], int actual_posX, int actual_posY){
    return mat[actual_posX][actual_posY] ;
}

int player_gameOver(int mat[70][70], raccoon_player * player){
        int posX = player->actual_posX;
    int posY = player->actual_posY;

    if (posX < 0 || posX > 69 || posY < 0 || posY > 69) {
        return 3;
    }
    else {
        int caseValue = get_case_statement(mat, posX, posY);
        if (caseValue != 0) {
            printf("%d -> %d (%d,%d)\n", player->player, caseValue, posX, posY);
        }
        return caseValue;
    }
}

int player_gameOver_score(raccoon_player * player, int game_over){
    int s = player->score ;
    printf("Score : %d\n",s);
    switch (game_over) {
        case 0:
            printf("    -> you win.\n") ;
            s += 50 ;
        case 1:
            if(player->player == 1){
                printf("    -> you touched your own track.\n") ;
                s -= 20 ;
            }
            else {
                printf("    -> you hit your opponent's track.\n") ;
                s -= 50 ;
            }
            break;

        case 2:
            if(player->player == 2){
                printf("    -> you touched your own track.\n") ;
                s -= 20 ;
            }
            else {
                printf("    -> you hit your opponent's track.\n") ;
                s -= 50 ;
            }
            break;

        case 3:
            printf("    -> out of the field.\n") ;
            s -= 10 ;
            break;

        case 4:
            printf("    -> you've tripped over a piece of garbage .\n") ;
            s -= 30 ;

        default:
            printf("ERROR : [%d].\n",game_over) ;
            break;
    }
    return s ;
}

int get_key_move(SDL_Renderer *renderer, SDL_Event e, settings * s, int color[3], int posX, int posY, int dimW, int dimH){
    while(1){
        while(SDL_PollEvent(&e) != 0)
            {
                if  (e.type == SDL_KEYDOWN)
                {
                    //printf("sdl keydown \n") ;
                    if((e.key.keysym.sym>=SDLK_a && e.key.keysym.sym<=SDLK_z) || e.key.keysym.sym==SDLK_UP || e.key.keysym.sym==SDLK_DOWN || e.key.keysym.sym==SDLK_RIGHT || e.key.keysym.sym==SDLK_LEFT){
                        if (e.key.keysym.sym != s->p1[0] && e.key.keysym.sym != s->p1[1] && e.key.keysym.sym != s->p1[2] && e.key.keysym.sym != s->p1[3] && e.key.keysym.sym != s->p2[0] && e.key.keysym.sym != s->p2[1] && e.key.keysym.sym != s->p2[2] && e.key.keysym.sym != s->p2[3]) {
                            printf("%d -> %c \n",e.key.keysym.sym,e.key.keysym.sym) ;
                            return e.key.keysym.sym ;
                        } else {
                            printf("error keydown \n") ;
                            block_rectangle(renderer,color, posX, posY, dimW, dimH);

                        }
                    }
                    else {
                        printf("error keydown \n") ;
                        block_rectangle(renderer,color, posX, posY, dimW, dimH);
                    }

                }
            }
    }
}

int change_key_move(int new_move,  settings * s){
    if (new_move!=s->p1[0] && new_move!=s->p1[1] && new_move!=s->p1[2] && new_move!=s->p1[3] && new_move!=s->p2[0] && new_move!=s->p2[1] && new_move!=s->p2[2] && new_move!=s->p2[3]){
        return new_move ;
    }
}

///* Raccoon game *///
int play_a_raccoon_game(SDL_Renderer *renderer, TTF_Font * font, SDL_Event e, raccoon_player *r1, raccoon_player *r2, settings * s ,int menu, int ai){
    int field_case_statement[70][70] ;
    int color_trash_fence[3] = {240, 195, 0} ;
    int replay ; int pause = 0;
    do {

        replay = 0 ;
        r1 = replay_raccoon_player (r1, 1, 1, 0) ;
        if (r2 != NULL)
            r2 = replay_raccoon_player (r2, 68, 68, 0) ;

        int game_over[2] = {0,0} ;
        int trash_fence = 0 ;
        int Lastevents1 = r1->down ;
        int Lastevents2 ;
        if (r2!=NULL){
            Lastevents2 = r2->up ;
        }

        int trashX = 0 ; int trashY = 0 ;
        count_down(renderer, font, r1, r2) ;

        for (int i=0;i<70;i++){
            for (int j=0;j<70;j++){
                field_case_statement[j][i] = 0 ;
            }
        }

        while (!game_over[0] && !game_over[1])
        {
            SDL_FlushEvents(SDL_QUIT, SDL_LASTEVENT); //To clean SDL_LASTEVENT
            SDL_PumpEvents(); // To take all the keydown

            SDL_RenderClear(renderer) ;
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255) ;
            game_playground_window(renderer) ;

            if (r2 == NULL){
                display_occupied_case(renderer, field_case_statement,4, color_trash_fence);
            } else {
                display_track(renderer,r2) ;
                display_player(renderer, Xfield, Yfield, r2) ;
                r2->last_move = Lastevents2 ;
            }
            display_track(renderer,r1) ;
            display_player(renderer, Xfield, Yfield, r1) ;
            r1->last_move = Lastevents1 ;

            SDL_RenderPresent(renderer) ;
            SDL_Delay((100*(6-s->speed)/5-(trash_fence/(15*s->speed)))-10) ;    // player speed management
            SDL_PollEvent(&e) ;


            if ((r1->up == e.key.keysym.sym || r1->down == e.key.keysym.sym || r1->right == e.key.keysym.sym || r1->left == e.key.keysym.sym) && get_opposite_move(r1,r1->last_move) != e.key.keysym.sym) {
                Lastevents1 = e.key.keysym.sym ;
                pause = 0 ;
            }
            if (r2!=NULL && ai == 0 && (r2->up == e.key.keysym.sym || r2->down == e.key.keysym.sym || r2->right == e.key.keysym.sym || r2->left == e.key.keysym.sym) && get_opposite_move(r2,r2->last_move) != e.key.keysym.sym) {
                Lastevents2 = e.key.keysym.sym ;
                pause = 0 ;
            }else if(r2!=NULL && ai != 0){
                Lastevents2 = AImove(field_case_statement, r2->actual_posX, r2->actual_posY,ai,r2) ;
                if (pause>0)
                    pause -- ;
            }

            if (SDLK_SPACE == e.key.keysym.sym && !pause){
                display_pause(renderer,font,e,r1,r2) ;
                pause = 5 ;
                e.key.keysym.sym = 0 ;
            }

            r1->wt = append_waste_trail(r1->wt, r1->actual_posX, r1->actual_posY) ;
            r1 = raccoon_move(r1, Lastevents1) ;
            game_over[0] = player_gameOver(field_case_statement,r1) ;
            if (!game_over[0]){
                field_case_statement[r1->actual_posX][r1->actual_posY] = 1 ;
                r1->score += 10*(s->speed) ;
            }

            if (r2 != NULL){
                r2->wt = append_waste_trail(r2->wt, r2->actual_posX, r2->actual_posY) ;
                r2 = raccoon_move(r2, Lastevents2) ;
                game_over[1] = player_gameOver(field_case_statement,r2) ;
                if (!game_over[1]){
                    field_case_statement[r2->actual_posX][r2->actual_posY] = 2 ;
                    r2->score += 10*(s->speed) ;
                }
            }

            if (trash_fence%15 == 0 && r2 == NULL) {    // random waste to challenge player 1
                do {
                    trashX = rand()%70 ;
                    trashY = rand()%70 ;
                } while(field_case_statement[trashX][trashY]!=0) ;
                field_case_statement[trashX][trashY] = 4 ;
            }
            trash_fence++ ;

        }

        printf("-> save score in file TRASHRUN_player_file.csv\n") ;
        r1->score = player_gameOver_score(r1, game_over[0]) ;
        if(r2==NULL){
            save_csv(r1,1) ;
        } else {
            r2->score = player_gameOver_score(r2, game_over[1]) ;
            save_csv(r1,2) ;
            save_csv(r2,2) ;
        }
        printf("XXX GAME OVER XXX\n\n") ;
        display_gameover_menu(renderer, font, 100, r1, r2) ;

        while((SDL_PollEvent(&e) !=0 || menu/10==2) && replay == 0)
                        {
                            if  (e.type == SDL_MOUSEBUTTONUP)
                            {
                                if (e.button.x > 80 && e.button.x < 380 && e.button.y > 250 && e.button.y < 350)
                                {
                                    printf("-> main menu\n") ;
                                    menu = 10 ;
                                }
                                else if (e.button.x > 80 && e.button.x < 380 && e.button.y > 250+100+10 && e.button.y < 350+100+10)
                                {
                                    printf("-> quit\n") ;
                                    menu = 0 ;
                                }
                                else if (e.button.x > 450 && e.button.x < 750 && e.button.y > 550 && e.button.y < 650)
                                {
                                    printf("-> replay\n") ;
                                    replay = 1 ;
                                }
                            }
                            //printf("vv\n") ;
                        }
        printf("here\n") ;

    } while(replay == 1) ;
    return menu ;
}

int AImove( int mat[70][70], int actual_posX, int actual_posY, int intelligence, raccoon_player * player){
    int valid_moves[4] = { 0, 0, 0, 0 }; //up, down, right, left
    int opposite_move = get_opposite_move(player,player->last_move);

    // To check valid movements
    if (actual_posY > 0 && get_case_statement(mat, actual_posX, actual_posY - 1) == 0) {
        valid_moves[0] = 1 ; // up
    }
    if (actual_posY < 69 && get_case_statement(mat, actual_posX, actual_posY + 1) == 0) {
        valid_moves[1] = 1 ; // down
    }
    if (actual_posX < 69 && get_case_statement(mat, actual_posX + 1, actual_posY) == 0) {
        valid_moves[2] = 1 ; // right
    }
    if (actual_posX > 0 && get_case_statement(mat, actual_posX - 1, actual_posY) == 0) {
        valid_moves[3] = 1 ; // left
    }

    // To avoid touching yourself
    if (opposite_move > 0 && valid_moves[opposite_move - 1]) {
        valid_moves[opposite_move - 1] = 0 ;
    }

    // To avoid surrounding yourself
    for (int i = 1; i <= intelligence; i++) {
        if (actual_posY - i >= 0 && get_case_statement(mat, actual_posX, actual_posY - i) != 0) {
            valid_moves[0] = 0 ;
        }
        if (actual_posY + i < 69 && get_case_statement(mat, actual_posX, actual_posY + i) != 0) {
            valid_moves[1] = 0 ;
        }
        if (actual_posX + i < 69 && get_case_statement(mat, actual_posX + i, actual_posY) != 0) {
            valid_moves[2] = 0 ;
        }
        if (actual_posX - i >= 0 && get_case_statement(mat, actual_posX - i, actual_posY) != 0) {
            valid_moves[3] = 0 ;
        }
    }

    int total_moves = 0;
    for (int i = 0; i < 4; i++) {
        total_moves += valid_moves[i];
    }

    if (total_moves > 0) {
        // Choose random right moves
        int choice = rand() % total_moves;
        for (int i = 0; i < 4; i++) {
            if (valid_moves[i]) {
                if (choice == 0) {
                    return i + 1;
                }
                choice--;
            }
        }
    }
    return 0;

    /*int choice_dir_ia ;
    int proba[4] = {0,0,0,0} ; //up, down, right, left

    for (int i = 1; i<=intelligence ; i++){
        if (actual_posY-i > 0) {
        if (get_case_statment(mat,actual_posX,actual_posY-i) != 0 && actual_posY-i-1 > 0){
            i = intelligence ;
            //printf("y-%d\n",i) ;
        } else {
            proba[0] ++ ;
        }
        }
    }
    for (int i = 1; i<=intelligence ; i++){
        if (actual_posY+i < 70) {
        if (get_case_statment(mat,actual_posX,actual_posY+i) != 0 && actual_posY+i < 70){
            i = intelligence ;
            //printf("y+%d\n",i) ;
        } else {
            proba[1] ++ ;
        }
        }
    }
    for (int i = 1; i<=intelligence ; i++){
        if (actual_posX+i < 70) {
        if (get_case_statment(mat,actual_posX+i,actual_posY) != 0 && actual_posX+i < 70){
            i = intelligence ;
            //printf("x+%d\n",i) ;
        } else {
            proba[2] ++ ;
        }
        }
    }
    for (int i = 1; i<=intelligence ; i++){
        if (actual_posX-i > 0) {
        //printf("\n\n%d\n",actual_posX-i) ;
        if (get_case_statment(mat,actual_posX-i,actual_posY) != 0 && actual_posX-i >= 0){
            i = intelligence ;
            //printf("x-%d\n",i) ;
        } else {
            proba[3] ++ ;
        }
        }
    }
    int P = 0 ;
    for(int p=0; p<4; p++){
        //printf("%d        => %d\n",p,proba[p]) ;
        if (proba[p]==player->last_move){
            proba[p] = proba[p]*3 ;
        }
        P += proba[p] ;
    }

    if (P != 0) {
        choice_dir_ia = rand()%P +1 ;
        //printf("\n->test D ->%d",choice_dir_ia) ;
        for(int p=3; p>=0; p--){
            //printf("    %d ?\n", P-proba[p]) ;
            P = P-proba[p] ;
            if (choice_dir_ia > P-proba[p]){
                //printf("\n*********************%d",p) ;
                return p+1 ;
            }
        }
        //printf("\n->test E\n\n") ;
    }
    return 0 ; //garde la meme direction et meurt dignement*/
}



int digit_number(int num){
    int digit ;
    int quotient = num;

    while (quotient != 0) {
        quotient = quotient / 10 ;
        digit ++ ;
    }

    return digit ;
}


settings * create_settings (){
    settings *ret = (settings *) malloc (sizeof(settings)) ;
	ret->speed = 3 ;

    ret->p1[0] = SDLK_UP;
    ret->p1[1] = SDLK_DOWN;
    ret->p1[2] = SDLK_RIGHT;
    ret->p1[3] = SDLK_LEFT;

    ret->p2[0] = SDLK_z;
    ret->p2[1] = SDLK_s;
    ret->p2[2] = SDLK_d;
    ret->p2[3] = SDLK_q;

	return ret ;
}

void save_csv(raccoon_player *p, int game){
	FILE * players_file = fopen("TRASHRUN_player_file.csv", "a") ;
    fprintf(players_file,"%d : %s ; %d : %d\n",game,p->name,p->hat,p->score) ;
   	fclose(players_file) ;
}









