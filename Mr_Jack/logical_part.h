
#ifndef MR_JACK_LOGICAL_PART_H
#define MR_JACK_LOGICAL_PART_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

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


//check if an action is true or not (returns 0 or 1)
int check(int init_c, int init_r, int c, int r)
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
//check validity of moves of miss stealthy
int check_stealthy(int init_c, int init_r, int c, int r)
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

         );
    return ans;
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
                    if(j==real_position[y][x][0]&&i==real_position[y][x][1])
                    {
                        if(map[y][x].character!=0)
                        {
                            if(map[y][x].character!=0)
                                printf("%d",map[y][x].character);
                            flag=0;
                        }
                    }
                    else if(j==real_position[y][x][0]&&i==real_position[y][x][1]+1)
                    {

                        printf("%d",map[y][x].attitude);
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
        
    }
}

//moving characters to cell (c,r)
void move(int chosen_character, int c, int r)
{
    map[position[chosen_character][0]][position[chosen_character][1]].character=0;
    position[chosen_character][0]=c;
    position[chosen_character][1]=r;
    map[c][r].character=chosen_character;
    display();
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
                if(check(position[chosen_character][0],position[chosen_character][1],c,r))
                {
                    move(chosen_character,c,r);


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
            printf("Now choose one of these directions:\n!)north 2)north east 3)south east 4)south 5)south west 6)north west\n");

            scanf("%d",&a);
            switch (a)
            {
                case 1:

                    break;
                case 2:
                    break;
                case 3:
                    break;
                case 4:
                    break;
                case 5:
                    break;
                case 6:
                    break;
            }


            break;
        case 3:
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
            }

            break;
        case 4:
            printf("You chose Inspecteur Lestrade. You can move this character 1 to 3 cell\n");
            for(int count=0; count<3; count++)
            {
                printf("Enter the coordinate of the cell you want to go with format (column row)\n");
                int c,r;
                scanf("%d %d",&c,&r);
                if(check(position[chosen_character][0],position[chosen_character][1],c,r))
                {
                    move(chosen_character,c,r);


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
            printf("Now enter the coordinate of the cell you want to pick up its banner\nThis cell must be on first or last column\n");
            int b1,b2;
            scanf("%d %d",&b1,&b2);
            if(b1==12)
            {
                map[b1][b2].attitude=1;
                map[b1-1][b2].attitude=1;
            }
            else if(b1==0)
            {
                map[b1][b2].attitude=1;
                map[b1+1][b2].attitude=1;
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

            break;
        case 6:
            printf("You chose Sir William Gull.\n");

            break;
        case 7:
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
            if(check(x,y,j,i))
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
    make_two_pack_of_cards();
    printf("Detective! Choose one of these characters:\n");
    for(int i=0; i<4; i++)
    {
        printf("character %d\n", find(odd_cards,i)->character);
    }
    int c;
    scanf("%d",&c);
    character_action(c);
    delete_node(&odd_cards,c);
    printf("Mr.Jack! Choose one of these characters:\n");
    for(int i=0; i<3; i++)
    {
        printf("character %d\n", find(odd_cards,i)->character);
    }
    scanf("%d",&c);
    character_action(c);
    delete_node(&odd_cards,c);
    printf("Mr.Jack! Choose one of these characters:\n");
    for(int i=0; i<2; i++)
    {
        printf("character %d\n", find(odd_cards,i)->character);
    }
    scanf("%d",&c);
    character_action(c);
    delete_node(&odd_cards,c);
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
    printf("Enter n for going next round\n");


}

void even_turn(int MrJack)
{

    printf("Mr.Jack! Choose one of these characters:\n");
    for(int i=0; i<4; i++)
    {
        printf("character %d\n", find(even_cards,i)->character);
    }
    int c;
    scanf("%d",&c);
    character_action(c);
    delete_node(&even_cards,c);
    printf("Detective! Choose one of these characters:\n");
    for(int i=0; i<3; i++)
    {
        printf("character %d\n", find(even_cards,i)->character);
    }
    scanf("%d",&c);
    character_action(c);
    delete_node(&even_cards,c);
    printf("Detective! Choose one of these characters:\n");
    for(int i=0; i<2; i++)
    {
        printf("character %d\n", find(even_cards,i)->character);
    }
    scanf("%d",&c);
    character_action(c);
    delete_node(&even_cards,c);
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
    }
    else
    {
        printf("Ok! Mr.Jack is invisible\n");
    }
    printf("Enter n for going next round\n");

}

#endif //MR_JACK_LOGICAL_PART_H



