#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<algorithm>
#include "cstdlib"
#include <regex>
using namespace std;

bool simulate(int x, int y, int x_start, int x_end, int y_start, int y_end){
    int pos_x = 0, pos_y = 0;
    while(pos_x <= x_end && pos_y >= y_end){
        pos_x += x;
        pos_y += y;
        if(x > 0){
            x--;
        }
        else if(x < 0){
            x++;
        }
        y--;
        if(pos_x >= x_start && pos_y <= y_start && pos_x <= x_end && pos_y >= y_end){
            return true;
        }
    }
    return false;
}

int main(){
    std::ifstream in("../input.txt");
    if (!in.is_open()) {
        std::cerr << "Could not open the file - '"<< "\n";
        return EXIT_FAILURE;
    }
    std::string str;
    getline(in, str);
    int range[4];
    regex exp("(-?\\d+)");
    auto begin = std::sregex_iterator(str.begin(), str.end(), exp);
    auto end = std::sregex_iterator();
    int i  = 0;
    for (std::sregex_iterator itr = begin; itr != end; ++itr) {
        std::smatch match = *itr;
        range[i] = std::stoi(match.str());
        i++;
    }
    int result = 0;
    int x_start = min(range[0],range[1]);
    int x_end = max(range[0],range[1]);
    int y_start = max(range[2],range[3]);
    int y_end = min(range[2],range[3]);
    //cout<<y_start<<endl;
    //cout<<y_end<<endl;
    for(int y = y_end; y <= -y_end; y++){
        for(int x = 0; x <= x_end;x++){
            if(simulate(x,y,x_start,x_end, y_start, y_end)){
                result++;
            }
        }
    }
    cout<<result<<"\n";
    return 0;
}