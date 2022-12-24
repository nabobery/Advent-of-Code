#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cctype>
#include <vector>
#include <algorithm>
#include <cstdlib>
using namespace std;

int main()
{
    ifstream in("input.txt");
    if (!in.is_open())
    {
        cerr << "Could not open the file - '" << endl;
        return EXIT_FAILURE;
    }
    string line;
    int i_h = 250, j_h = 250, i_t = 250, j_t = 250;
    char grid[1000][1000];
    for(int i = 0; i < 1000;i++){
        for(int j = 0; j < 1000; j++){
            grid[i][j] = '.';
        }
    }
    grid[i_h][j_h] = 's';
    while (getline(in, line))
    {
        char dir = line[0];
        int dist = stoi(line.substr(1));
        if(dir == 'U'){
            while(dist--){
                i_h--;
                int diff_i = i_h - i_t, diff_j = j_h - j_t;
                if(abs(diff_i) <= 1 && abs(diff_j) <= 1){
                    continue;
                }
                else{
                    i_t = i_h+1;
                    j_t = j_h;
                    grid[i_t][j_t] = '#';
                }
            }
        }
        else if(dir == 'D'){
            while(dist--){
                i_h++;
                int diff_i = i_h - i_t, diff_j = j_h - j_t;
                if(abs(diff_i) <= 1  && abs(diff_j) <= 1){
                    continue;
                }
                else{
                    i_t = i_h-1;
                    j_t = j_h;
                    grid[i_t][j_t] = '#';
                }
            }
        }
        else if(dir == 'L'){
            while (dist--)
            {
                j_h--;
                int diff_i = i_h - i_t, diff_j = j_h - j_t;
                if(abs(diff_i) <= 1  && abs(diff_j) <= 1){
                    continue;
                }
                else{
                    j_t = j_h+1;
                    i_t = i_h;
                    grid[i_t][j_t] = '#';
                }
            }
        }
        else if(dir == 'R'){
            while (dist--)
            {
                j_h++;
                int diff_i = i_h - i_t, diff_j = j_h - j_t;
                if(abs(diff_i) <= 1  && abs(diff_j) <= 1){
                    continue;
                }
                else{
                    j_t = j_h-1;
                    i_t = i_h;
                    grid[i_t][j_t] = '#';
                }
            }  
        }  
    }
    int result = 0;
    for(int i = 0; i < 1000;i++){
        for(int j = 0; j < 1000; j++){
            if(grid[i][j] == '#' || grid[i][j] == 's'){
                result++;
            }
        }
    }
    cout << result<<"\n";
    return 0;
}
    