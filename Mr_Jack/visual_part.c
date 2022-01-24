#include <stdio.h>
#include "logical_part.h"
#include <windows.h>

void display()
{

    for(int i=0; i<=38; i++)
    {
        for(int j=0; j<=79; j++)
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
        printf("\n");
    }
}


int main()
{
    display();

    printf("%d",position[5][1]);







    int a;
    scanf("%d",&a);
}

