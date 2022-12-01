#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<algorithm>
using namespace std;

int grid[10][10];

void dfs(int x, int y){
    if(grid[x][y] == -1){
        return;
    }
    grid[x][y]++;
    if(grid[x][y] < 10){
        return;
    }
    grid[x][y] = -1;
    for(int i = max(0, x -1); i <= min(9,x+1); i++){
        for(int j = max(0, y - 1); j <= min(9,y+1); j++){
            dfs(i,j);
        }
    }
}

int main(){
    ifstream in("input.txt");
    if (!in.is_open()) {
        cerr << "Could not open the file - '"<< endl;
        return EXIT_FAILURE;
    }
    for(int i = 0; i < 10; i++) {
        string str;
        getline(in,str);
        for(int j = 0; j < 10; j++) {
            grid[i][j] = str[j] - '0';
        }
    }
    /*for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            cout << grid[i][j];
        }
        cout << endl;
    }*/
    int result = 0;
    for(int i = 0; i < 100; i++){
        for(int j = 0; j < 10; j++) {
            for(int k = 0; k < 10; k++){
                dfs(j,k);
            }
        }
        for(int j = 0; j < 10; j++) {
            for(int k = 0; k < 10; k++) {
                if(grid[j][k] == -1){
                    result++;
                    grid[j][k] = 0;
                }
            }
        }   
    }
    cout << result << endl;
    return 0;
}