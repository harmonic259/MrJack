#include <stdio.h>
#include <stdlib.h>
//struct contains attributes of each cell
struct cell
{
    // 0: empty/ 1: home/ 2: opened well/ 3: closed well/ 4: on light/ 5: off light/ 7: unavailable
    int attitude;
    //0: empty/ 1: SH/ 2: JW/ 3: JS/ 4: IL/ 5: MS/ 6: SG/ 7: WG/ 8: JB
    int character;
};
//array of position of each cell for finding them
struct cell map[13][9];

void init_map()
{
    FILE* fpin = fopen("C:\\Users\\ASUS-pc Ishragh\\Documents\\GitHub\\MrJack\\Mr_Jack\\initial_map_attitude.txt","r");
    for(int j = 0; j < 9; j++)
    {
        for(int i=0; i<13; i++)
        {
            fscanf(fpin, "%d",&(map[i][j].attitude));
        }
    }
    fclose(fpin);


}