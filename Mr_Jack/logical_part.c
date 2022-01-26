#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "visual_part.h"

/////0: empty/ 1: SH/ 2: JW/ 3: JS/ 4: IL/ 5: MS/ 6: SG/ 7: WG/ 8: JB
//real position of  a cell in map
int real_position[13][9][2];

//position of each character
int position[9][2];

//this variable implies turn
int turn=1;

void initial_position()
{
    position[1][0]=6,position[1][1]=5;
    position[2][0]=0,position[2][1]=4;
    position[3][0]=6,position[3][1]=2;
    position[4][0]=4,position[4][1]=4;
    position[5][0]=8,position[5][1]=7;
    position[6][0]=12,position[6][1]=3;
    position[7][0]=4,position[7][1]=0;
    position[8][0]=8,position[8][1]=3;
}

//struct contains attributes of each cell
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
void push(struct character_node** list, int new_data)
{
    struct character_node* new_node = (struct character_node*) malloc(sizeof(struct character_node));
    new_node->character = new_data;
    new_node->next = (*list);
    (*list)    = new_node;
}

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

//pop
void pop()
{

}

//make list of suspects cards
void make_list_of_suspects_cards()
{
    //making initial linked list of cards
    for (int i=1; i<=8; i++)
    {
        push(&suspects_cards,i);
    }
    //a function to generate a random permutation of list
    srand ( time(NULL) );

    for (int i = 7; i > 0; i--)
    {
        int j = rand() % (i+1);
        int temp=find(suspects_cards,i)->character;
        find(suspects_cards,i)->character=find(suspects_cards,j)->character;
        find(suspects_cards,j)->character=temp;
    }
}

//make two list of length 4 one for odd turns and other for even turns
void make_two_pack_of_cards()
{
    //making initial linked list of cards
    for (int i=1; i<=8; i++)
    {
        push(&cards,i);
    }
    //a function to generate a random permutation of list
    srand ( time(NULL) );

    for (int i = 7; i > 0; i--)
    {
        int j = rand() % (i+1);
        int temp=find(cards,i)->character;
        find(cards,i)->character=find(cards,j)->character;
        find(cards,j)->character=temp;
    }
    for(int i=0; i<4; i++)
    {
        push(&odd_cards,find(cards,i)->character);
        push(&even_cards, find(cards,i+4)->character);
    }
}

//initial situation at the beginning of the game
void init_map()
{
    //put characters in their initial positions
    initial_position();

    //shuffle cards and break the list in two list of length 4
    make_two_pack_of_cards();

    make_list_of_suspects_cards();

    //initial situation of attitudes in map
    FILE* fpin1 = fopen("C:\\Users\\ASUS-pc Ishragh\\Documents\\GitHub\\MrJack\\Mr_Jack\\initial_map_attitude.txt","r");
    for(int j = 0; j < 9; j++)
    {
        for(int i=0; i<13; i++)
        {
            fscanf(fpin1, "%d",&(map[i][j].attitude));
        }
    }
    fclose(fpin1);

    //initial situation of characters in map
    FILE* fpin2 = fopen("C:\\Users\\ASUS-pc Ishragh\\Documents\\GitHub\\MrJack\\Mr_Jack\\initial_map_character.txt","r");
    for(int j = 0; j < 9; j++)
    {
        for(int i=0; i<13; i++)
        {
            fscanf(fpin2, "%d",&(map[i][j].character));
        }
    }
    fclose(fpin2);

}


//check if an action is true or not (returns 0 or 1)
int check(int chosen_character, int init_c, int init_r, int c, int r)
{
    int ans;
    ans =((c>=0 && c<=12 && r>=0 && r<=8)) &&
         (      (
                        (init_c%2==0) &&
                        (
                                (init_c-1==c && init_r==r) ||
                                (init_c==c && init_r-1==r) ||
                                (init_c+1==c && init_r==r) ||
                                (init_c+1==c && init_r+1==r) ||
                                (init_c==c && init_r+1==r) ||
                                (init_c-1==c && init_r+1==r)

                        )
                ) ||
                (
                        (init_c%2==1) &&
                        (
                                (init_c-1==c && init_r-1==r) ||
                                (init_c==c && init_r-1==r) ||
                                (init_c+1==c && init_r-1==r) ||
                                (init_c+1==c && init_r==r) ||
                                (init_c==c && init_r+1==r) ||
                                (init_c-1==c && init_r==r)

                        )
                )

         ) &&
         (
                 (map[c][r].attitude!=1)
         );
    return ans;


}

//this function is for actions of characters
void character_action(int chosen_character)
{
    switch (chosen_character)
    {

        case 1:
            //Sherlock Holmes moves
            printf("You chose Sherlock Holmes. You can move this character 1 to 3 cell\n");
            for(int count=0; count<3; count++)
            {
                printf("Enter the coordinate of the cell you want to go with format (column row)\n");
                int c,r;
                scanf("%d %d",&c,&r);
                if(check(chosen_character,position[chosen_character][0],position[chosen_character][1],c,r))
                {
                    position[chosen_character][0]=c;
                    position[chosen_character][1]=r;

                    if(count!=2)
                    {
                        printf("Do you want to do another move? 1)Yes 2)No\n");
                        int answ;
                        scanf("%d",&answ);
                        if(answ==1)
                        {
                            continue;
                        }
                        else
                        {
                            break;
                        }
                    }

                }
                else
                {
                    printf("Your action was wrong. Please choose another coordinate.\n");
                    count--;
                    continue;
                }

            }
            //Sherlock Holmes actions


            break;
        case 2:

            //John Watson moves
            printf("You chose John Watson. You can move this character 1 to 3 cell\n");
            for(int count=0; count<3; count++)
            {
                printf("Enter the coordinate of the cell you want to go with format (column row)\n");
                int c,r;
                scanf("%d %d",&c,&r);
                if(check(chosen_character,position[chosen_character][0],position[chosen_character][1],c,r))
                {
                    position[chosen_character][0]=c;
                    position[chosen_character][1]=r;

                    if(count!=2)
                    {
                        printf("Do you want to do another move? 1)Yes 2)No\n");
                        int answ;
                        scanf("%d",&answ);
                        if(answ==1)
                        {
                            continue;
                        }
                        else
                        {
                            break;
                        }
                    }

                }
                else
                {
                    printf("Your action was wrong. Please choose another coordinate.\n");
                    count--;
                    continue;
                }

            }
            //John Watson actions


            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
    }
}

//this gives us the real position in map
void corresponding_position(int a, int b, int *x,int *y)
{
    if(a%2==0)
    {
        *x = 6*a + 2;
        *y = 4*b + 3;
    }
    else
    {
        *x = 6*a + 2;
        *y = 4*b + 1;
    }

}
void correspond()
{
    for(int i=0; i<9; i++)
    {
        for(int j=0; j<13; j++)
        {
            corresponding_position(j,i,&real_position[j][i][0],&real_position[j][i][1]);
        }
    }
}

//



