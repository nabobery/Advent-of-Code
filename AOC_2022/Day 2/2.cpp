#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

// opponent : A for Rock, B for Paper, and C for Scissors
// you : X for Rock, Y for Paper, and Z for Scissors
// 1 for Rock, 2 for Paper, and 3 for Scissors
// 0 if you lost, 3 if the round was a draw, and 6 if you won

int main(){
    ifstream in("input.txt");
    if (!in.is_open())
    {
        cerr << "Could not open the file - '" << endl;
        return EXIT_FAILURE;
    }
    string line;
    int score = 0;
    while (getline(in, line))
    {
        char opponent = line[0];
        char player = line[2];
        if((opponent - 'A') != (player - 'X')){
            if((player == 'X' && opponent == 'C') || (player == 'Y' && opponent == 'A') || (player == 'Z' && opponent == 'B')){
                score += 6;
            }       
        } 
        else score += 3;
        score += (player - 'X' + 1);
    }
    cout<<score<<"\n";
    in.clear();
    in.seekg(0, ios::beg);
    score = 0;
    // X means you need to lose, Y means you need to end the round in a draw, and Z means you need to win
    while (getline(in, line))
    {
        char opponent = line[0];
        char player = line[2];
        if(player == 'X'){
            if(opponent == 'A') score += 3;
            else if(opponent == 'B') score += 1;
            else score += 2;
        }
        else if(player == 'Y'){
            score += 3;
            score += (opponent - 'A' + 1);
        }
        else{
            score += 6;
            if(opponent == 'A') score += 2;
            else if(opponent == 'B') score += 3;
            else score += 1;
        }
    }
    cout<<score<<"\n";
    return 0;
}