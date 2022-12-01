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
    /*int temp[2];
    int max_x = -1, max_y = -1;
    for(int i=0; i < 950; i++){
        string str;
        getline(in,str);
        stringstream x(str);
        int j  = 0;
        while(getline(x,str,',')){
            temp[j] = stoi(str);
            j++;
        }
        max_x = max(temp[0],max_x);
        max_y = max(temp[1],max_y);
    }
    cout<<max_x<<endl; // 1310
    cout<<max_y<<endl; // 893 */
    string temp[2];
    string str;
    vector<pair<int,int>> initial_dots;
    for(int i=0;i< 950;i++){
        getline(in,str);
        stringstream x(str);
        int j  = 0;
        while(getline(x,str,',')){
            temp[j] = str;
            j++;
        }
        initial_dots.emplace_back(stoi(temp[0]),stoi(temp[1]));
    }
    getline(in,str);
    unordered_set<pair<int, int>, hashFunction> final_dots;
    /*for(pair<int,int> itr : initial_dots){
        cout << itr.first << " " << itr.second << endl;
        cout<<"\n";
    }*/
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
        for(auto itr : initial_dots){
            if(itr.first > index){
                itr.first = 2 * index - itr.first;
            }
            //pair<int, int> temp = itr;
            final_dots.insert(itr);
        }
    }
    else{
        for(auto itr : initial_dots){
            if(itr.second > index){
                itr.second = 2 * index - itr.second;
            }
            //final_dots.insert(itr);
            //pair<int, int> temp = itr;
            final_dots.insert(itr);
        }
    }
    int result = final_dots.size();
    std::cout<<result<<endl;
    return 0;
}