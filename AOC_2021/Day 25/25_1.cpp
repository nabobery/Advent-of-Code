#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<algorithm>
#include<climits>
using namespace std;

bool hasStopped(char grid[137][139]){
    for(int i = 0; i < 137; i++){
        for(int j = 0; j < 139;j++){
            if(grid[i][j] == '>'){
                if(j == 138){
                    if(grid[i][0] == '.'){
                        return false;
                    }
                    else{
                        continue;
                    }
                }
                else{
                    if(grid[i][j + 1] == '.'){
                        return false;
                    }
                    else{
                        continue;
                    }
                }
            }
            else if(grid[i][j] == 'v'){
                if(i == 136){
                    if(grid[0][j] == '.'){
                        return false;
                    }
                    else{
                        continue;
                    }
                }
                else{
                    if(grid[i+1][j] == '.'){
                        return false;
                    }
                    else{
                        continue;
                    }
                }
            }
            else{
                continue;
            }
        }
    }
    return true;
}

void step(char (&grid)[137][139]){
    for(int i = 0; i < 137;i++){
        bool flag = false;
        for(int j = 0; j < 139;j++){
            if(grid[i][j] == '>'){
                if(j == 0){
                    if(grid[i][1] == '.'){
                        grid[i][1] = '>';
                        grid[i][j] = '.';
                        j++;
                        flag = true;
                    }
                    else{
                        continue;
                    }
                }
                else if(j == 138){
                    if(grid[i][0] == '.' && !flag){
                        grid[i][0] = '>';
                        grid[i][j] = '.';
                    }
                    else{
                        continue;
                    }
                }
                else{
                    if(grid[i][j + 1] == '.'){
                        grid[i][j+1] = '>';
                        grid[i][j] = '.';
                        j++;
                    }
                    else{
                        continue;
                    }
                }
            }
        }
    }
    for(int i = 0; i < 139; i++){
        bool flag = false;
        for(int j = 0; j < 137;j++){
            if(grid[j][i] == 'v'){
                if(j == 0){
                    if(grid[1][i] == '.'){
                        grid[1][i] = grid[j][i];
                        grid[j][i] = '.';
                        j++;
                        flag = true;
                    }
                    else{
                        continue;
                    }
                }
                else if(j == 136){
                    if(grid[0][i] == '.' && !flag){
                        grid[0][i] = grid[j][i];
                        grid[j][i] = '.';
                    }
                    else{
                        continue;
                    }
                }
                else{
                    if(grid[j+1][i] == '.'){
                        grid[j+1][i] = grid[j][i];
                        grid[j][i] = '.';
                        j++;
                    }
                    else{
                        continue;
                    }
                }
            }
        }
    }
}

int main(){
    ifstream in("../input.txt");
    if (!in.is_open()) {
        cerr << "Could not open the file - '"<< endl;
        return EXIT_FAILURE;
    }
    string str;
    char grid[137][139];
    for(int i = 0; i < 137; i++){
        getline(in, str);
        for(int j = 0; j < 139;j++){
            grid[i][j] = str[j];
        }
    }
    int result = 0;
    while(!hasStopped(grid)){
        step(grid);
        result++;
    }

    //cout<<str.size()<<endl; 139
    cout<<result + 1<<"\n";
    return 0;
}

