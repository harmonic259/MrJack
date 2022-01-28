#include <stdio.h>
#include "logical_part.h"
#include <windows.h>

void init_display()
{

    init_map();
    correspond();

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



}


void game_loop()
{
    int rr;
    printf("1)Start new game     2)load the previous game\n");
    scanf("%d",&rr);
    if(rr==2)
    {
        load();

        while (round_counter<8)
        {
            odd_turn(mrJack);
            even_turn(mrJack);
        }
    }
    else
    {
        int mr_jack;
        init_map();

        printf("Mr.Jack! Please enter 1 to see your character.\n" );
        int c;
        scanf("%d",&c);
        if(c==1)
        {
            mr_jack = find(suspects_cards, 0)->character;
            mrJack = mr_jack;
            printf("You are character %d\n", mr_jack);
            pop(&suspects_cards);
            printf("Enter 2 to hide it\n");
            scanf("%d",&c);
            if(c==2)
            {
                system("cls");
            }

        }
        init_display();
        while (round_counter<8)
        {
            odd_turn(mr_jack);
            even_turn(mr_jack);
        }
    }
}

int main()
{
    game_loop();

    int a;
    scanf("%d",&a);
}

