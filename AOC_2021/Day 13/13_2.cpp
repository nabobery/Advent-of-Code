#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<algorithm>
#include <unordered_set>
#include <utility>
using namespace std;

struct hashFunction{
  size_t operator()(const pair<int , int> &x) const{
    return x.first ^ x.second;
  }
};

int main(){
    ifstream in("input.txt");
    if (!in.is_open()) {
        cerr << "Could not open the file - '"<< endl;
        return EXIT_FAILURE;
    }
    string temp[2];
    string str;
    unordered_set<pair<int, int>, hashFunction> final_dots;
    for(int i=0;i< 950;i++){
        getline(in,str);
        stringstream x(str);
        int j  = 0;
        while(getline(x,str,',')){
            temp[j] = str;
            j++;
        }
        pair<int, int> pr;
        pr = make_pair(stoi(temp[0]),stoi(temp[1]));
        final_dots.insert(pr);
    }
    getline(in,str);
    unordered_set<pair<int, int>, hashFunction> temp_dots;
    for(int i = 0; i < 12;i++){
        getline(in,str);
        stringstream x(str);
        int j  = 0;
        while(getline(x,str,'=')){
            temp[j] = str;
            j++;
        }
        int index = stoi(temp[1]);
        char axis = temp[0].at(temp[0].length() - 1);
        if(axis == 'x'){
            for(auto itr : final_dots){
                if(itr.first > index){
                    itr.first = 2 * index - itr.first;
                }
                temp_dots.insert(itr);
            }
        }
        else{
            for(auto itr : final_dots){
                if(itr.second > index){
                    itr.second = 2 * index - itr.second;
                }
                temp_dots.insert(itr);
            }
        }
        unordered_set<pair<int, int>, hashFunction> temp;
        final_dots = temp_dots;
        temp_dots = temp;
        temp_dots.clear();
        //final_dots = temp_dots;
        //temp_dots.clear();
    }
    int max_x = -1, max_y = -1;
    for(auto itr : final_dots){
        max_x = max(max_x, itr.first);
        max_y = max(max_y, itr.second);
    }
    char result[max_x + 1][max_y + 1];
    for(int i = 0; i <= max_x;i++){
        for(int j = 0; j <= max_y;j++){
            result[i][j] = '.';
        }
    }
    for(auto itr : final_dots){
        result[itr.first][itr.second] = '#';
    }
    for(int i = 0; i <= max_y;i++){
        for(int j = 0; j <= max_x;j++){
            cout<<result[j][i];
        }
        cout<<endl;
    }
    return 0;
}