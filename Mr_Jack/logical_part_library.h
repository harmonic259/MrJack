
#ifndef MR_JACK_LOGICAL_PART_H
#define MR_JACK_LOGICAL_PART_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int position[9][2];
void initial_position();

struct cell
{
    //0: empty/ 1: home/ 2: opened well/ 3: closed well/ 4: on light/ 5: off light/ 7: unavailable
    int attitude;
    //0: empty/ 1: SH/ 2: JW/ 3: JS/ 4: IL/ 5: MS/ 6: SG/ 7: WG/ 8: JB
    int character;
};

//array of position of each cell for finding them
struct cell map[13][9];

//struct of characters
struct character_node
{
    int character;
    struct character_node* next;
};

//linked list of characters
struct character_node* cards=NULL;
struct character_node* suspects_cards=NULL;


//breaking list of cards in two lists of 4 cards, one for odd turns and other for even turns
struct character_node* odd_cards=NULL;
struct character_node* even_cards=NULL;

//push
void push(struct character_node** list, int new_data);


//find by index in list of cards
struct character_node* find(struct character_node* head, int x)
{
    struct character_node *current=head;
    int i=0;
    while(i<x)
    {
        current=current->next;
        i++;
    }
    return current;
}

//make list of suspects cards
void make_list_of_suspects_cards();


//make two list of length 4 one for odd turns and other for even turns
void make_two_pack_of_cards();


//initial situation at the beginning of the game
void init_map();

//this is an important function which applies any action
void change();

//check if an action is true or not (returns 0 or 1)
int check(int chosen_character, int init_c, int init_r, int c, int r);


//this function is for actions of characters
void character_action(int chosen_character);

#endif //MR_JACK_LOGICAL_PART_H
