
#ifndef MR_JACK_LOGICAL_PART_H
#define MR_JACK_LOGICAL_PART_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <math.h>

/////0: empty/ 1: SH/ 2: JW/ 3: JS/ 4: IL/ 5: MS/ 6: SG/ 7: WG/ 8: JB

//real position of  a cell in map
int real_position[13][9][2];

//position of each character
int position[9][2];

//this variable implies turn
int turn;
int round_counter=0;

//mr jack
int mrJack;

int another_character=0;

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
    //0: empty/ 1: home/ 2: opened well/ 3: closed well/ 4: on light/ 5: off light/ 7: unavailable/ 8: banned
    int attitude;
    //0: empty/ 1: SH/ 2: JW/ 3: JS/ 4: IL/ 5: MS/ 6: SG/ 7: WG/ 8: JB
    int character;

    int visibility;
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
struct character_node* cards_saved=NULL;

struct character_node* suspects_cards=NULL;
struct character_node* suspects_cards_saved=NULL;

//breaking list of cards in two lists of 4 cards, one for odd turns and other for even turns
struct character_node* odd_cards=NULL;
struct character_node* odd_cards_saved=NULL;

struct character_node* even_cards=NULL;
struct character_node* even_cards_saved=NULL;

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

//pop (delete first)
void pop(struct character_node **list)
{
    struct character_node *tmp;

    if(list == NULL || *list == NULL) return;
    tmp = *list;
    *list = (*list)->next;
    free(tmp);
}

//delete a node by its key
void delete_node(struct character_node** head_ref, int key)
{
    // Store head node
    struct character_node *temp = *head_ref, *prev;

    // If head node itself holds the key to be deleted
    if (temp != NULL && temp->character == key) {
        *head_ref = temp->next; // Changed head
        free(temp); // free old head
        return;
    }

    // Search for the key to be deleted, keep track of the
    // previous node as we need to change 'prev->next'
    while (temp != NULL && temp->character != key) {
        prev = temp;
        temp = temp->next;
    }

    // If key was not present in linked list
    if (temp == NULL)
        return;

    // Unlink the node from linked list
    prev->next = temp->next;

    free(temp); // Free memory
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
void save()
{
    FILE* fpin = fopen("C:\\Users\\ASUS-pc Ishragh\\Documents\\GitHub\\MrJack\\Mr_Jack\\changes.bin","wb");
    if(fpin == NULL)
    {
        printf("Cannot create file\n");
    }
    else
    {
        for(int i=0; i<9; i++)
        {
            for(int j=0; j<13; j++)
            {
                fwrite(&map[j][i].attitude, sizeof(int),1,fpin);
                fwrite(&map[j][i].character, sizeof(int),1,fpin);
                fwrite(&map[j][i].visibility, sizeof(int),1,fpin);
                fwrite(&real_position[j][i], sizeof(int),1,fpin);
            }
        }

        for(int i=1; i<9; i++)
        {
            fwrite(&position[i][0], sizeof(int),1,fpin);
            fwrite(&position[i][1], sizeof(int),1,fpin);
        }

        fwrite(&turn, sizeof(int),1,fpin);
        fwrite(&round_counter, sizeof(int),1,fpin);
        fwrite(&mrJack, sizeof(int),1,fpin);

        cards_saved=NULL;
        suspects_cards_saved=NULL;
        odd_cards_saved=NULL;
        even_cards_saved=NULL;

        struct character_node* temp;
        temp=cards;
        while (temp!=NULL)
        {
            fwrite(&(temp->character), sizeof(int),1,fpin);
            push(&cards_saved,temp->character);
            temp=temp->next;
        }
        temp=suspects_cards;
        while (temp!=NULL)
        {
            fwrite(&(temp->character), sizeof(int),1,fpin);
            push(&suspects_cards_saved,temp->character);
            temp=temp->next;
        }
        temp=odd_cards;
        while (temp!=NULL)
        {
            fwrite(&(temp->character), sizeof(int),1,fpin);
            push(&odd_cards_saved,temp->character);
            temp=temp->next;
        }
        temp=even_cards;
        while (temp!=NULL)
        {
            fwrite(&(temp->character), sizeof(int),1,fpin);
            push(&even_cards_saved,temp->character);
            temp=temp->next;
        }


    }
    fclose(fpin);
}
void load()
{
    FILE* fpin = fopen("C:\\Users\\ASUS-pc Ishragh\\Documents\\GitHub\\MrJack\\Mr_Jack\\changes.bin","rb");
    if(fpin == NULL)
    {
        printf("Cannot find file\n");
    }
    else
    {
        for(int i=0; i<9; i++)
        {
            for(int j=0; j<13; j++)
            {
                fread(&map[j][i].attitude, sizeof(int),1,fpin);
                fread(&map[j][i].character, sizeof(int),1,fpin);
                fread(&map[j][i].visibility, sizeof(int),1,fpin);
                fread(&real_position[j][i], sizeof(int),1,fpin);
            }
        }

        for(int i=1; i<9; i++)
        {
            fread(&position[i][0], sizeof(int),1,fpin);
            fread(&position[i][1], sizeof(int),1,fpin);
        }

        fread(&turn, sizeof(int),1,fpin);
        fread(&round_counter, sizeof(int),1,fpin);
        fread(&mrJack, sizeof(int),1,fpin);

    }


    cards=NULL;
    suspects_cards=NULL;
    odd_cards=NULL;
    even_cards=NULL;

    struct character_node* temp;
    temp=cards_saved;
    while (temp!=NULL)
    {
        fread(&(temp->character), sizeof(int),1,fpin);
        push(&cards,temp->character);
        temp=temp->next;
    }
    temp=suspects_cards_saved;
    while (temp!=NULL)
    {
        fread(&(temp->character), sizeof(int),1,fpin);
        push(&suspects_cards,temp->character);
        temp=temp->next;
    }
    temp=odd_cards_saved;
    while (temp!=NULL)
    {
        fread(&(temp->character), sizeof(int),1,fpin);
        push(&odd_cards,temp->character);
        temp=temp->next;
    }
    temp=even_cards_saved;
    while (temp!=NULL)
    {
        fread(&(temp->character), sizeof(int),1,fpin);
        push(&even_cards,temp->character);
        temp=temp->next;
    }




    fclose(fpin);
}

//swap
void swap(int *a, int *b)
{
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
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


//check if a move is true or not (returns 0 or 1)
int check(int init_c, int init_r, int c, int r)
{
    int ans;
    ans =(((c>=0 && c<=12 && r>=0 && r<=8)) &&
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
                 (map[c][r].attitude!=1&&map[c][r].attitude!=8&&map[c][r].attitude!=4&&map[c][r].attitude!=5)
         ))||(map[init_c][init_r].attitude==2&&map[c][r].attitude==2);
    return ans;


}
//this checks adjacency
int check_adjacency(int init_c, int init_r, int c, int r)
{
    int ans;
    ans =(((c>=0 && c<=12 && r>=0 && r<=8)) &&
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

          ) );
    return ans;


}

//check validity of moves of miss stealthy
int check_stealthy(int init_c, int init_r, int c, int r)
{
    int ans;
    ans =((c>=0 && c<=12 && r>=0 && r<=8)) &&
         (
                 (
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

         )||(map[init_c][init_r].attitude==2&&map[c][r].attitude==2);
    return ans;
}
int check_goodly(int init_c, int init_r, int c, int r)
{
    if(abs(init_c-position[6][0])+abs(init_r-position[6][1])<abs(c-position[6][0])+abs(r-position[6][1]))
        return 0;
    else
        return 1;
}
int check_last_position(int chosen_character, int c, int r)
{
    if(turn==0&&map[c][r].character!=0&&map[c][r].character!=chosen_character)
    {
        if(mrJack==map[c][r].character)
        {
            printf("Detective! You won the game\n");
        }
        else
        {
            printf("Mr.Jack! You won the game\n");
        }

    }
    else if(turn==1&&map[c][r].character!=0&&map[c][r].character!=chosen_character)
    {
        return 0;
    }
    else if(turn==1&&map[c][r].attitude==7)
    {
        if(mrJack==map[c][r].character)
        {

            printf("Mr.Jack! You won the game\n");
        }
        else
        {
            return 0;
        }

    }
    else
    {
        int ans;
        ans = (map[c][r].attitude != 1 && map[c][r].attitude != 8 && map[c][r].attitude != 4 &&
               map[c][r].attitude != 5);
        return ans;
    }
}
//this function is for displaying characters with their name
char* num_to_name(int character_num)
{
    switch (character_num)
    {
        case 1:
            return "SH";
            break;
        case 2:
            return "JW";
            break;
        case 3:
            return "JS";
            break;
        case 4:
            return "IL";
            break;
        case 5:
            return "MS";
            break;
        case 6:
            return "SG";
            break;
        case 7:
            return "WG";
            break;
        case 8:
            return "JB";
            break;
    }
}
//this function is for displaying attitudes with some notations
//0: empty/ 1: home/ 2: opened well/ 3: closed well/ 4: on light/ 5: off light/ 7: unavailable/ 8: banned
char* num_to_notation(int attitude_num)
{
    switch (attitude_num)
    {
        case 0:
            return "    ";
            break;
        case 1:
            return "H   ";
            break;
        case 2:
            return "W+  ";
            break;
        case 3:
            return "W-  ";
            break;
        case 4:
            return "L+  ";
            break;
        case 5:
            return "L-  ";
            break;
        case 6:
            return "    ";
            break;
        case 7:
            return "Exit";
            break;
        case 8:
            return "Ban ";
            break;
    }
}


//displaying map
void display()
{
    system("cls");
    for(int i=0; i<=38; i++)
    {
        for(int j=0; j<=79; j++)
        {
            int flag=1;
            for(int x=0; x<9; x++)
            {
                for(int y=0; y<13; y++)
                {
                    if((j==real_position[y][x][0]||j==real_position[y][x][0]+1||j==real_position[y][x][0]+2)&&i==real_position[y][x][1])
                    {
                        if(map[y][x].character!=0)
                        {
                            if(j==real_position[y][x][0])
                                printf("%c", num_to_name(map[y][x].character)[0]);
                            else if(j==real_position[y][x][0]+1)
                                printf("%c", num_to_name(map[y][x].character)[1]);
                            else
                                printf("%d", map[y][x].character);
                            flag=0;
                        }
                    }
                    else if((j>=real_position[y][x][0]&&j<=real_position[y][x][0]+3)&&i==real_position[y][x][1]+1)
                    {

                        for(int xx=0; xx<4; xx++)
                        {
                            if(j==xx+real_position[y][x][0])
                                printf("%c", num_to_notation(map[y][x].attitude)[xx]);
                        }
                        flag=0;

                    }
                }
            }

            if(flag)
            {
                if((i==1&&(j==0||j==79))||(i==2&&(j==1||j==78)))
                    printf(" ");

                else if(((j%12==8||j%12==9||j%12==10||j%12==11)&&i%4==0)||((j%12==2||j%12==3||j%12==4||j%12==5)&&i%4==2))
                    printf("_");
                else if(((i%4==1)&&(j%12==7))||((i%4==2)&&(j%12==6))||((i%4==3)&&(j%12==1))||((i%4==0)&&(j%12==0)))
                {
                    if(i!=0)
                        printf("/");
                    else
                        printf(" ");

                }
                else if(((i%4==1)&&(j%12==0))||((i%4==2)&&(j%12==1))||((i%4==3)&&(j%12==6))||((i%4==0)&&(j%12==7)))
                {
                    if(i!=0)
                        printf("\\");
                    else
                        printf(" ");
                }


                else
                    printf(" ");
            }

        }
        printf("\n");
    }
    printf("1)continue 2)save\n");
    int in;
    scanf("%d",&in);
    if(in==2)
    {
        save();
    }
}

//moving characters to cell (c,r)
void move(int chosen_character, int c, int r)
{

    map[position[chosen_character][0]][position[chosen_character][1]].character=another_character;
    another_character=map[c][r].character;
    position[chosen_character][0]=c;
    position[chosen_character][1]=r;
    map[c][r].character=chosen_character;
    display();
}


//this function is for actions of characters
void character_action(int chosen_character)
{
    //initial position
    int x0,x1;
    switch (chosen_character)
    {

        case 1:
            x0=position[chosen_character][0];
            x1=position[chosen_character][1];
            display();
            //Sherlock Holmes moves
            printf("You chose Sherlock Holmes. You can move this character 1 to 3 cell\n");
            for(int count=0; count<3; count++)
            {
                printf("Enter the coordinate of the cell you want to go with format (column row)\n");
                int c,r;
                scanf("%d %d",&c,&r);
                if(check(position[chosen_character][0],position[chosen_character][1],c,r))
                {
                    move(chosen_character,c,r);


                    if(count!=3)
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

                            if(check_last_position(chosen_character,c,r))
                                break;
                            else
                            {
                                printf("Your last position was wrong. Please move in another way from the beginning.\n");
                                count=0;
                                move(chosen_character,x0,x1);
                                continue;
                            }

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
            printf("Now enter 1 for picking up the top suspect card\n");
            int a;
            scanf("%d",&a);
            if(a==1)
            {
                printf("Character %d\n", find(suspects_cards,0)->character);
                printf("Now enter 2 to hide this\n");
                scanf("%d",&a);
                if(a==2)
                {
                    display();
                }
            }


            break;
        case 2:
            x0=position[chosen_character][0];
            x1=position[chosen_character][1];
            display();
            //John Watson moves
            printf("You chose John Watson. You can move this character 1 to 3 cell\n");
            for(int count=0; count<3; count++)
            {
                printf("Enter the coordinate of the cell you want to go with format (column row)\n");
                int c,r;
                scanf("%d %d",&c,&r);
                if(check(position[chosen_character][0],position[chosen_character][1],c,r))
                {
                    move(chosen_character,c,r);

                    if(count!=3)
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
                            if(check_last_position(chosen_character,c,r))
                                break;
                            else
                            {
                                printf("Your last position was wrong. Please move in another way from the beginning.\n");
                                count=0;
                                move(chosen_character,x0,x1);
                                continue;
                            }
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
            for(int i=0;i<9; i++)
            {
                for(int j=0; j<13; j++)
                {
                    map[j][i].visibility=0;
                }
            }
            printf("Now choose one of these directions:\n1)north  2)north-east  3)south-east  4)south  5)south-west  6)north-west\n");

            scanf("%d",&a);
            int w0=position[chosen_character][0],w1=position[chosen_character][1];
            if(w0%2==0)
            {
                switch (a)
                {
                    case 1:
                        while (w1>=0&&w1<=8&&w0>=0&&w0<=12&&map[w0][w1].attitude!=1)
                        {
                            w1--;
                            map[w0][w1].visibility=1;
                        }
                        break;
                    case 2:
                        while (w1>=0&&w1<=8&&w0>=0&&w0<=12&&map[w0][w1].attitude!=1)
                        {
                            w0++;
                            map[w0][w1].visibility=1;
                        }
                        break;
                    case 3:
                        while (w1>=0&&w1<=8&&w0>=0&&w0<=12&&map[w0][w1].attitude!=1)
                        {
                            w0++;
                            w1++;
                            map[w0][w1].visibility=1;
                        }
                        break;
                    case 4:
                        while (w1>=0&&w1<=8&&w0>=0&&w0<=12&&map[w0][w1].attitude!=1)
                        {
                            w1++;
                            map[w0][w1].visibility=1;
                        }
                        break;
                    case 5:
                        while (w1>=0&&w1<=8&&w0>=0&&w0<=12&&map[w0][w1].attitude!=1)
                        {
                            w0--;
                            w1++;
                            map[w0][w1].visibility=1;
                        }
                        break;
                    case 6:
                        while (w1>=0&&w1<=8&&w0>=0&&w0<=12&&map[w0][w1].attitude!=1)
                        {
                            w0--;
                            map[w0][w1].visibility=1;
                        }
                        break;
                }
            }
            else
            {
                switch (a)
                {
                    case 1:
                        while (w1>=0&&w1<=8&&w0>=0&&w0<=12&&map[w0][w1].attitude!=1)
                        {
                            w1--;
                            map[w0][w1].visibility=1;
                        }
                        break;
                    case 2:
                        while (w1>=0&&w1<=8&&w0>=0&&w0<=12&&map[w0][w1].attitude!=1)
                        {
                            w0++;
                            w1--;
                            map[w0][w1].visibility=1;
                        }
                        break;
                    case 3:
                        while (w1>=0&&w1<=8&&w0>=0&&w0<=12&&map[w0][w1].attitude!=1)
                        {
                            w0++;
                            map[w0][w1].visibility=1;
                        }
                        break;
                    case 4:
                        while (w1>=0&&w1<=8&&w0>=0&&w0<=12&&map[w0][w1].attitude!=1)
                        {
                            w1++;
                            map[w0][w1].visibility=1;
                        }
                        break;
                    case 5:
                        while (w1>=0&&w1<=8&&w0>=0&&w0<=12&&map[w0][w1].attitude!=1)
                        {
                            w0--;
                            map[w0][w1].visibility=1;
                        }
                        break;
                    case 6:
                        while (w1>=0&&w1<=8&&w0>=0&&w0<=12&&map[w0][w1].attitude!=1)
                        {
                            w0--;
                            w1--;
                            map[w0][w1].visibility=1;
                        }
                        break;
                }
            }


            break;
        case 3:
            x0=position[chosen_character][0];
            x1=position[chosen_character][1];
            display();
            printf("You chose John Smith Choose your first action:\n1)moving    2)turn on one light and turn off other\n");
            scanf("%d",&a);
            if(a==1)
            {
                printf("You can move this character 1 to 3 cell\n");
                for(int count=0; count<3; count++)
                {
                    printf("Enter the coordinate of the cell you want to go with format (column row)\n");
                    int c,r;
                    scanf("%d %d",&c,&r);
                    if(check(position[chosen_character][0],position[chosen_character][1],c,r))
                    {
                        move(chosen_character,c,r);

                        if(count!=3)
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
                                if(check_last_position(chosen_character,c,r))
                                    break;
                                else
                                {
                                    printf("Your last position was wrong. Please move in another way from the beginning.\n");
                                    count=0;
                                    move(chosen_character,x0,x1);
                                    continue;
                                }
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
                printf("Choose a light which you want to turn it off\n");
                int a1,a2;
                scanf("%d %d",&a1,&a2);
                map[a1][a2].attitude=5;
                printf("Choose a light which you want to turn it on\n");
                scanf("%d %d",&a1,&a2);
                map[a1][a2].attitude=4;
                display();
            }
            else
            {
                printf("Choose a light which you want to turn it off\n");
                int a1,a2;
                scanf("%d %d",&a1,&a2);
                map[a1][a2].attitude=5;
                printf("Choose a light which you want to turn it on\n");
                scanf("%d %d",&a1,&a2);
                map[a1][a2].attitude=4;
                display();
                printf("You can move this character 1 to 3 cell\n");
                for(int count=0; count<3; count++)
                {
                    printf("Enter the coordinate of the cell you want to go with format (column row)\n");
                    int c,r;
                    scanf("%d %d",&c,&r);
                    if(check(position[chosen_character][0],position[chosen_character][1],c,r))
                    {
                        move(chosen_character,c,r);

                        if(count!=3)
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
                                if(check_last_position(chosen_character,c,r))
                                    break;
                                else
                                {
                                    printf("Your last position was wrong. Please move in another way from the beginning.\n");
                                    count=0;
                                    move(chosen_character,x0,x1);
                                    continue;
                                }
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
            }

            break;
        case 4:
            x0=position[chosen_character][0];
            x1=position[chosen_character][1];
            display();
            printf("You chose Inspecteur Lestrade. You can move this character 1 to 3 cell\n");
            for(int count=0; count<3; count++)
            {
                printf("Enter the coordinate of the cell you want to go with format (column row)\n");
                int c,r;
                scanf("%d %d",&c,&r);
                if(check(position[chosen_character][0],position[chosen_character][1],c,r))
                {
                    move(chosen_character,c,r);


                    if(count!=3)
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
                            if(check_last_position(chosen_character,c,r))
                                break;
                            else
                            {
                                printf("Your last position was wrong. Please move in another way from the beginning.\n");
                                count=0;
                                move(chosen_character,x0,x1);
                                continue;
                            }
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
            printf("Now enter the coordinate of the cell you want to pick up its banner\nThis cell must be on first or last column\n");
            int b1,b2;
            scanf("%d %d",&b1,&b2);
            if(b1==12)
            {
                map[b1][b2].attitude=7;
                map[b1-1][b2].attitude=7;
            }
            else if(b1==0)
            {
                map[b1][b2].attitude=7;
                map[b1+1][b2].attitude=7;
            }
            printf("Now enter the coordinate of the cell you want to put the banner on it\n");
            scanf("%d %d",&b1,&b2);
            if(b1==12)
            {
                map[b1][b2].attitude=8;
                map[b1-1][b2].attitude=8;
            }
            else if(b1==0)
            {
                map[b1][b2].attitude=8;
                map[b1+1][b2].attitude=8;
            }
            display();

            break;
        case 5:
            x0=position[chosen_character][0];
            x1=position[chosen_character][1];
            display();
            printf("You chose Miss Stealthy. You can move this character 1 to 4 cell\n");
            for(int count=0; count<4; count++)
            {
                printf("Enter the coordinate of the cell you want to go with format (column row)\n");
                int c,r;
                scanf("%d %d",&c,&r);
                if(check_stealthy(position[chosen_character][0],position[chosen_character][1],c,r))
                {
                    move(chosen_character,c,r);


                    if(count!=3)
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
                            if(check_last_position(chosen_character,c,r))
                                break;
                            else
                            {
                                printf("Your last position was wrong. Please move in another way from the beginning.\n");
                                count=0;
                                move(chosen_character,x0,x1);
                                continue;
                            }
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

            break;
        case 6:
            x0=position[chosen_character][0];
            x1=position[chosen_character][1];
            display();
            printf("You chose Sergent Goodly. Choose your first action:\n1)moving    2)whistle\n");
            scanf("%d",&a);
            if(a==1)
            {
                printf("You can move this character 1 to 3 cell\n");
                for(int count=0; count<3; count++)
                {
                    printf("Enter the coordinate of the cell you want to go with format (column row)\n");
                    int c,r;
                    scanf("%d %d",&c,&r);
                    if(check(position[chosen_character][0],position[chosen_character][1],c,r))
                    {
                        move(chosen_character,c,r);

                        if(count!=3)
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
                                if(check_last_position(chosen_character,c,r))
                                    break;
                                else
                                {
                                    printf("Your last position was wrong. Please move in another way from the beginning.\n");
                                    count=0;
                                    move(chosen_character,x0,x1);
                                    continue;
                                }
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
                //whistle
                printf("Now you can move other characters 3 times by your whistle\n");
                int whistle=0;
                while( whistle++ <3)
                {
                    printf("Choose a character (number 1 to 8 except 6)\n");
                    int characterX;
                    scanf("%d",&characterX);
                    printf("Enter the coordinate of the cell you want to go with format (column row)\n");
                    int c,r;
                    scanf("%d %d",&c,&r);
                    if(check(position[characterX][0],position[characterX][1],c,r)&&
                    check_goodly(position[characterX][0],position[characterX][1],c,r))
                    {
                        move(characterX,c,r);


                    }
                    else
                    {
                        printf("Your action was wrong. Please choose another coordinate.\n");
                        whistle--;
                        continue;
                    }

                }
            }
            else
            {
                //whistle
                printf("Now you can move other characters 3 times by your whistle\n");
                int whistle=0;
                while( whistle++ <3)
                {
                    printf("Choose a character (number 1 to 8 except 6)\n");
                    int characterX;
                    scanf("%d",&characterX);
                    printf("Enter the coordinate of the cell you want to go with format (column row)\n");
                    int c,r;
                    scanf("%d %d",&c,&r);
                    if(check(position[characterX][0],position[characterX][1],c,r)&&
                       check_goodly(position[characterX][0],position[characterX][1],c,r))
                    {
                        move(characterX,c,r);


                    }
                    else
                    {
                        printf("Your action was wrong. Please choose another coordinate.\n");
                        whistle--;
                        continue;
                    }

                }
                printf("You can move this character 1 to 3 cell\n");
                for(int count=0; count<3; count++)
                {
                    printf("Enter the coordinate of the cell you want to go with format (column row)\n");
                    int c,r;
                    scanf("%d %d",&c,&r);
                    if(check(position[chosen_character][0],position[chosen_character][1],c,r))
                    {
                        move(chosen_character,c,r);

                        if(count!=3)
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
                                if(check_last_position(chosen_character,c,r))
                                    break;
                                else
                                {
                                    printf("Your last position was wrong. Please move in another way from the beginning.\n");
                                    count=0;
                                    move(chosen_character,x0,x1);
                                    continue;
                                }
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
            }


            break;
        case 7:
            x0=position[chosen_character][0];
            x1=position[chosen_character][1];
            display();
            printf("You chose Sir William Gull. You can move this character 1 to 3 cell\n");
            printf("You can only one of these actions:\n1)moving   2)replacement with other character\n");
            scanf("%d",&a);
            if(a==1)
            {
                printf("You can move this character 1 to 3 cell\n");
                for(int count=0; count<3; count++)
                {
                    printf("Enter the coordinate of the cell you want to go with format (column row)\n");
                    int c,r;
                    scanf("%d %d",&c,&r);
                    if(check(position[chosen_character][0],position[chosen_character][1],c,r))
                    {
                        move(chosen_character,c,r);

                        if(count!=3)
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
                                if(check_last_position(chosen_character,c,r))
                                    break;
                                else
                                {
                                    printf("Your last position was wrong. Please move in another way from the beginning.\n");
                                    count=0;
                                    move(chosen_character,x0,x1);
                                    continue;
                                }
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
            }
            else
            {
                printf("Please enter the label of character you want to replace your position with him/her\n");
                scanf("%d",&a);
                map[position[chosen_character][0]][position[chosen_character][1]].character=a;
                map[position[a][0]][position[a][1]].character=chosen_character;
                swap(&position[chosen_character][0],&position[a][0]);
                swap(&position[chosen_character][1],&position[a][1]);
                display();
            }

            break;
        case 8:
            x0=position[chosen_character][0];
            x1=position[chosen_character][1];
            display();
            printf("You chose Jeremy Bert. Choose your first action:\n1)moving    2)open a well and close another one\n");
            scanf("%d",&a);
            if(a==1)
            {
                printf("You can move this character 1 to 3 cell\n");
                for(int count=0; count<3; count++)
                {
                    printf("Enter the coordinate of the cell you want to go with format (column row)\n");
                    int c,r;
                    scanf("%d %d",&c,&r);
                    if(check(position[chosen_character][0],position[chosen_character][1],c,r))
                    {
                        move(chosen_character,c,r);

                        if(count!=3)
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
                                if(check_last_position(chosen_character,c,r))
                                    break;
                                else
                                {
                                    printf("Your last position was wrong. Please move in another way from the beginning.\n");
                                    count=0;
                                    move(chosen_character,x0,x1);
                                    continue;
                                }
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
                printf("Choose the well which you want to open it\n");
                int a1,a2;
                scanf("%d %d",&a1,&a2);
                map[a1][a2].attitude=2;
                printf("Choose the well which you want to close it \n");
                scanf("%d %d",&a1,&a2);
                map[a1][a2].attitude=3;
                display();
            }
            else
            {
                printf("Choose the well which you want to open it\n");
                int a1,a2;
                scanf("%d %d",&a1,&a2);
                map[a1][a2].attitude=2;
                printf("Choose the well which you want to close it \n");
                scanf("%d %d",&a1,&a2);
                map[a1][a2].attitude=3;
                display();
                printf("You can move this character 1 to 3 cell\n");
                for(int count=0; count<3; count++)
                {
                    printf("Enter the coordinate of the cell you want to go with format (column row)\n");
                    int c,r;
                    scanf("%d %d",&c,&r);
                    if(check(position[chosen_character][0],position[chosen_character][1],c,r))
                    {
                        move(chosen_character,c,r);

                        if(count!=3)
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
                                if(check_last_position(chosen_character,c,r))
                                    break;
                                else
                                {
                                    printf("Your last position was wrong. Please move in another way from the beginning.\n");
                                    count=0;
                                    move(chosen_character,x0,x1);
                                    continue;
                                }
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
            }
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

int is_visible(int x, int y)
{

    for(int i=0; i<9; i++)
    {
        for(int j=0; j<13; j++)
        {
            if(map[j][i].visibility==1)
            {
                return 1;
            }
            if(check_adjacency(x,y,j,i))
            {
                if(map[j][i].attitude==4||map[j][i].character!=0)
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}

void even_turn(int MrJack);

void odd_turn(int MrJack)
{
    printf("This is round %d\n",round_counter+1);

    make_two_pack_of_cards();
    turn=0;
    printf("Detective! Choose one of these characters:\n");
    for(int i=0; i<4; i++)
    {
        printf("character %d\n", find(odd_cards,i)->character);
    }
    int c;
    scanf("%d",&c);
    character_action(c);
    delete_node(&odd_cards,c);
    turn=1;
    printf("Mr.Jack! Choose one of these characters:\n");
    for(int i=0; i<3; i++)
    {
        printf("character %d\n", find(odd_cards,i)->character);
    }
    scanf("%d",&c);
    character_action(c);
    delete_node(&odd_cards,c);
    turn=1;
    printf("Mr.Jack! Choose one of these characters:\n");
    for(int i=0; i<2; i++)
    {
        printf("character %d\n", find(odd_cards,i)->character);
    }
    scanf("%d",&c);
    character_action(c);
    delete_node(&odd_cards,c);
    turn=0;
    printf("Detective! Choose one of these characters:\n");
    for(int i=0; i<1; i++)
    {
        printf("character %d\n", find(odd_cards,i)->character);
    }
    scanf("%d",&c);
    character_action(c);
    delete_node(&odd_cards,c);
    if(is_visible(position[MrJack][0],position[MrJack][1]))
    {
        printf("Ok! Mr.Jack is visible\n");
    }
    else
    {
        printf("Ok! Mr.Jack is invisible\n");
    }

    round_counter++;
    display();



}

void even_turn(int MrJack)
{
    printf("This is round %d\n",round_counter+1);
    turn=1;
    printf("Mr.Jack! Choose one of these characters:\n");
    for(int i=0; i<4; i++)
    {
        printf("character %d\n", find(even_cards,i)->character);
    }
    int c;
    scanf("%d",&c);
    character_action(c);
    delete_node(&even_cards,c);
    turn=0;
    printf("Detective! Choose one of these characters:\n");
    for(int i=0; i<3; i++)
    {
        printf("character %d\n", find(even_cards,i)->character);
    }
    scanf("%d",&c);
    character_action(c);
    delete_node(&even_cards,c);
    turn=0;
    printf("Detective! Choose one of these characters:\n");
    for(int i=0; i<2; i++)
    {
        printf("character %d\n", find(even_cards,i)->character);
    }
    scanf("%d",&c);
    character_action(c);
    delete_node(&even_cards,c);
    turn=1;
    printf("Mr.Jack! Choose one of these characters:\n");
    for(int i=0; i<1; i++)
    {
        printf("character %d\n", find(even_cards,i)->character);
    }
    scanf("%d",&c);
    character_action(c);
    delete_node(&even_cards,c);
    if(is_visible(position[MrJack][0],position[MrJack][1]))
    {
        printf("Ok! Mr.Jack is visible\n");
        printf("characters :");
        for(int i=1; i<9; i++)
        {
            if(is_visible(position[MrJack][0],position[MrJack][1])==0)
                printf("%d ",i);
        }
        printf("are not Mr.Jack!\n");
    }
    else
    {
        printf("Ok! Mr.Jack is invisible\n");
        printf("characters :");
        for(int i=1; i<9; i++)
        {
            if(is_visible(position[MrJack][0],position[MrJack][1])==1)
                printf("%d ",i);
        }
        printf("are not Mr.Jack!\n");
    }

    round_counter++;
    display();


}


#endif //MR_JACK_LOGICAL_PART_H



