#include <stdio.h>
#include "dreidel.h"
#include "mtrand.h"




char spin_dreidel(void) {                                                                                   //this function will simulate the dreidel
    int dreidel_result = mtrand_rand64() % 4;                                                               //splits the optons into 4 possible outcomes
    
    if (dreidel_result == 0) {                                                                              //converts number to side of dreidel 
        return 'G';
    }
    else if (dreidel_result == 1) {                                                                         //converts number to side of dreidel
        return 'H';
    }
    else if (dreidel_result == 2) {                                                                         //converts number to side of dreidel
        return 'N';
    }
    else {                                                                                                  //final side of the dreidel
        return 'S';
    }
}




int play_game (int n_players , int coins_per_player , int * n_rounds) {
    
    char * player_name[] = {"Aharon", "Batsheva", "Chanah", "David", "Ephraim", "Faige", "Gamaliel", "Hannah"};     //list of potential player names
    
    
    int player_wallet[n_players];                                                                                   //creates wallets for all the players that will be playing
    for (int i = 0; i < n_players; i++) {
        player_wallet[i] = coins_per_player;
    }
    
    int pot = 0;                                                                                                    //pot starts out empty so set to zero
    int current_player = 0;                                                                                         //this will count who's turn it currently is
    int players_in = n_players;                                                                                     //this is for how many players are playing (out of 8)
    extern int v;                                                                                                   //will be used just for the v option on play-dreidel


    while(players_in > 1) {
        if (current_player == 0) {
            *n_rounds += 1;                                                                                         //progresses the rounds
        }
        if (player_wallet[current_player] != -1) {                                                                  //spin dreidel if you are not eliminated
            char spin = spin_dreidel();

            if (spin == 'G') {
                player_wallet[current_player] = player_wallet[current_player] + pot;                                //if dreidel is G, add whole pot to current player wallet
                pot = 0;
            }
            if (spin == 'H') {
                player_wallet[current_player] = player_wallet[current_player] + (pot / 2);                          //if dreidel is H, take half of pot. subtract what was taken out form pot so other half is left
                pot = pot - (pot / 2);
            }
            if (spin == 'N') {
                //nothing happens
            }
            if (spin == 'S') {                                                                                      //if dreidel is S, player has to put 1 coin in pot
                if (player_wallet[current_player] == 0) {                                                           //if no more coins to give, eliminate player
                    player_wallet[current_player] = -1;
                    players_in = players_in - 1;                                                                    //make sure to subtract 1 from players playing
                    if (v == 1) {                                                                                   //if v was selected, print eliminated message
                        printf("%s: eliminated in round %d of a %d player game \n", player_name[current_player], *n_rounds, n_players);
                    }
                } else {
                    player_wallet[current_player] = player_wallet[current_player] - 1;
                    pot = pot + 1;
                }
            }
        }
        current_player += 1;                                                                                        //moves player to next one in line
        current_player = current_player % n_players;
    }

    int winner = -1;
    for (int i = 0; i < n_players; i++) {                                                                           //confirms then returns who the winner is 
        if(player_wallet[i] != -1) {
            winner = i;
            break;
        }
    }
    return winner;
}


