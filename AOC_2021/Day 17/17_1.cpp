#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include <iterator>
#include<algorithm>
#include "cstdlib"
#include <regex>

using namespace std;

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
    /*for(i = 0; i < 4;i++){
        std::cout<<range[i]<<"\n";
    }*/
    int x_start = range[0];
    int x_end = range[1];
    int y_start = range[2];
    int y_end = range[3];
    int max_y = abs(min(y_start,y_end)) - 1;
    /*bool flag = true;
    while(flag){
            max_y += 1;
        if(-(max_y + 1) < min(y_start, y_end)){
            flag = false;
        }
    }*/
    int result = (max_y)*(max_y + 1)/2 ;
    cout<<result<<endl;
    return 0;
}