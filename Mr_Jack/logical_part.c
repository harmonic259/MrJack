#include <stdio.h>
//struct contains attributes of each cell
struct cell
{
    // 0: empty/ 1: home/ 2: opened well/ 3: closed well/ 4: on light/ 5: off light/ 7: unavailable
    int attitude[7];
    //0: empty/ 1: SH/ 2: JW/ 3: JS/ 4: IL/ 5: MS/ 6: SG/ 7: WG/ 8: JB
    int character[7];
};
//array of position of each cell for finding them
struct cell map[13][9];

void init_map()
{

}