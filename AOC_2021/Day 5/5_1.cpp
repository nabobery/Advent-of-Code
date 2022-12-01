#include<iostream>
#include<fstream>
#include<string>
#include <regex>
#include <iterator>
#include <algorithm>
using namespace std;

int main(){
    ifstream in("input.txt");
    if (!in.is_open()) {
        cerr << "Could not open the file - '"<< endl;
        return EXIT_FAILURE;
    }
    int grid[1000][1000];
    for(int i = 0; i < 1000; i++){
        for(int j = 0; j < 1000; j++){
            grid[i][j] = 0;
        }
    }
    int x1, y1, x2, y2, result  = 0;
    string tmp;
    regex number("\\d+");
    int k;
    for(int i = 0; i < 500;i++){
        getline(in, tmp);
        auto words_begin = sregex_iterator(tmp.begin(), tmp.end(), number);
        auto words_end = sregex_iterator();
        k = 0;
        for (sregex_iterator j = words_begin; j != words_end; ++j) {
            smatch match = *j;                                         
            string match_str = match.str(); 
            if(k == 0){
                x1 = stoi(match_str);
            }
            else if(k == 1){
                y1 = stoi(match_str);
            }
            else if(k == 2){
                x2 = stoi(match_str);
            }
            else{
                y2 = stoi(match_str);
            }
            k++;
        } 
        if(x1 == x2){
            for(k = min(y1,y2); k <= max(y1,y2); k++){
                grid[x1][k]++;
            }
        }
        else if(y1 == y2){
            for(k = min(x1,x2); k <= max(x1,x2); k++){
                grid[k][y1]++;
            }
        }
    }
    for(int i = 0; i < 1000; i++){
        for(int j = 0; j < 1000; j++){
            if(grid[i][j] >= 2){
                result++;
            }
        }
    }
    cout<<result<<endl;
    return 0;
}