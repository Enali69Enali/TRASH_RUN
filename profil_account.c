#include <stddef.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "game.h"


/*Profil * create (char * name, int score){
    / To create the first element of a chained list representing the users of the game.
        name : player name
        score : player score /
	Profil *ret = (Profil *) malloc (sizeof(Profil)) ;
	ret->name = name ;
	ret->score = score ;
	ret->next = NULL ;
	return ret ;
}*/
/*
Profil * append (char * name, int score, Profil *p){
    / To add a user to a chained list
        name : player name
        score : player score
        p : the chained list /
	if(p == NULL){
		return create(name, score) ;
	}
	else{
		p->next = append(name, score, p->next) ;
	}
	return p ;
}*/
/*
int length(Profil *p){
    /To return the number of players from a chained list
        p : the chained list /
	assert(p != NULL) ;
	int counter = 1 ;
	while(p->next != NULL){
		counter++;
		p = p->next ;
	}
	return counter ;
}

void printProfilList(Profil *p){
    /To print all of the players
        p : the chained list /
	assert(p != NULL) ;
	printf("profils :\n") ;
	while(p->next != NULL){
		printf("%s : %d pts\n", p->name, p->score) ;
		p = p->next ;
	}
	printf("%s : %d pts\n", p->name, p->score) ;
}

Profil * insert_in_list(char * name, int score, int pos, Profil * p){
    /To insert a user with a particular position
        name : player name
        score : player score
        p : the chained list /
	assert(p != NULL) ;
	assert(pos <= length(p)) ;
	Profil * user ;
	if(pos == 0) {
		user = create(name,score) ;
		user->next = p ;
	}
	else if(pos == length(p)){
			user = append(name, score, p) ;
	}
	else {
		user = create(p->name, p->score) ;
		/probleme ici/
		for(int i=0 ; i<length(p)+1 ; i++){
			if(i == pos-1){
				user = append(name, score, user) ;
			}
				p = p->next ;
				user = append(p->name, p->score, user) ;
		}
	}
	return user ;
}*/

/*
Profil * score_tri(Profil *p){ //pb car il echange les scores
	assert(p != NULL) ;
	Profil *tmp1, *tmp2, *min ;
	unsigned int s ;
	char * n ;
	for (tmp1=p; tmp1->next != NULL; tmp1=tmp1->next){
		min = tmp1 ;
		for (tmp2=tmp1->next; tmp2 != NULL; tmp2=tmp2->next){
			if (tmp2->score < min->score){
				min = tmp2 ;
			}
			if (min != tmp1) {
				s = min->score ; n = min->name ;
				min->score = tmp1->score ; min->name = tmp1->name ;
				tmp1->score = s ; tmp1->name = n ;
			}
		}
	}
	return tmp1 ;
}

Profil * reverse_list(Profil *p){
    Profil *previous = NULL;   // le pointeur précédent
    Profil *current = p;   // le pointeur principal

    // parcourt la liste
    while (current != NULL)
    {
        // délicat : notez le nœud suivant
        Profil *next = current->next;

        current->next = previous;    // corrige le noeud courant

        // avance les deux pointeurs
        previous = current;
        current = next;
    }

    // fixe le pointeur principal pour pointer vers le nouveau front
    // p = previous;
    return previous;
}*/



/*
Profil * get_profils_from_file(){
	FILE* players_file ;

	char c;
	char str[80];
	players_file = fopen("sudoku_player_file.csv", "r");
	while(fgets(str,80,players_file)){
		//printf("--%s--\n",str) ;
		printf("%s",str) ;
        }
}
*/
