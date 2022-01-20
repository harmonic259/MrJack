#include <stdio.h>
//struct contains attributes of each cell
struct cell
{

    //0 if it hasn't home/ 1 if it has home
    int home;
    //0,0 if it hasn't chah/ 1,0 if it has closed chah/ 1,1 if it has opened chah
    int chah[1];
    //0,0 if it hasn't light/ 1,0 if it has "off" light/ 1,1 if it has "on" light
    int light[1];
    //0: empty/ 1: SH/ 2: JW/ 3: JS/ 4: IL/ 5: MS/ 6: SG/ 7: WG/ 8: JB
    int character[8];
    
};
//array of position of each cell for finding them
struct cell map[13][9];

