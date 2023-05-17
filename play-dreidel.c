#include <stdio.h>
#include <unistd.h> // For getopt().
#include <stdlib.h>
#include "mtrand.h"
#include "dreidel.h"

#define OPTIONS "p:c:s:v"                                                                                       //options for user

int v = 0;                                                                                                      //v is 0 normally, unless user inputs it

int main(int argc, char **argv) {
    int opt = 0;                                                                                                
    int p = 4;                                                                                                  //these are the default inputs if none are given by user
    int c = 3;
    int s = 613;
    

    int n_rounds = 0;                                                                                           //round number starts at 0
    char * player_name[] = {"Aharon", "Batsheva", "Chanah", "David", "Ephraim", "Faige", "Gamaliel", "Hannah"}; //list of potential player names

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {                                                         //part of the getopt format, given in previous asgn pdf
        switch (opt) {


        case 'p':                                                                                               //for when user uses -p (player number)
            p = (uint32_t)strtoul(optarg, NULL, 10);
            if (p < 2 || p > 8) {
                printf ("The number of players must be between 2–8 (inclusive) \n");                            //print message when they inputted incorrect value
                return -1;
            }
            break;
                

        case 'c':                                                                                               //for when user uses -c (how many coins each start with)
            c = (uint32_t)strtoul(optarg, NULL, 10);
            if (c < 1 || c > 20) {
                printf ("The number of coins must be between 1–20, (inclusive) \n");                            //print message when they inputted incorrect value
                return -1;
            }
            break;
            

        case 's':
            s = (uint32_t)strtoul(optarg, NULL, 10);                                                            //for when user uses -s (seed number)
            break;

        case 'v':                                                                                               //for when user uses -v (elimination message)
            v = 1;                                                                                              //taken by dreidel.c to print the message
            break;

        default:
            return -1;                                                                                          //stops program when invalid option is given
        }
    }

mtrand_seed(s);                                                                                                 //inputs the seed for mtrand
int winner = play_game(p, c, &n_rounds);                                                                        //starts game
printf("%s %d %d %d %d\n", player_name[winner], p, c, n_rounds, s);                                             //prints who the winner is, how many played, how many coins each started with, how many rounds it lasted, and the seed


return 0;                                                                                                       //stops the code
}


