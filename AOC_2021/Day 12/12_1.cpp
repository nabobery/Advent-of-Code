#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<cstring>
#include <unordered_set>
#include <unordered_map>
using namespace std;

int result;
unordered_map<string,unordered_set<string>> map;
string start = "start";

bool isBig(string str){
    for(int i=0; i<str.length(); i++){
        if(islower(str[i]))
            return false;
    }
    return true;
}

void dfs(string curr,unordered_set<string> path){
    if(curr == "end"){
        result++;
        return;
    }
    //auto& set = map[curr];
    for(auto child : map[curr]){
        if(isBig(child)){
            unordered_set<string> newpath(path);
            newpath.insert(child);
            dfs(child,newpath);
        }
        else if(!(path.find(child) != path.end())){
            unordered_set<string> newpath(path);
            newpath.insert(child);
            dfs(child,newpath);
        }
    }
}

int main() {
    ifstream in("input.txt");
    if (!in.is_open()) {
        cerr << "Could not open the file - '"<< endl;
        return EXIT_FAILURE;
    }
    string str;
    string curr[2];
    for (int i = 0; i < 24; i++){
        getline(in,str);
        stringstream x(str);
        int j = 0;
        while (getline(x, str, '-')) {  
            curr[j] = str;
            j++;
        }
        auto& curr1Set = map[curr[0]];
        curr1Set.insert(curr[1]);
        map[curr[0]] = curr1Set;
        auto& curr2Set = map[curr[1]];
        curr2Set.insert(curr[0]);
        map[curr[1]] = curr2Set;
    }
    /*for(auto it = map.begin(); it != map.end(); it++){
        cout <<"First: " <<(*it).first << " Second: ";
        for(auto itr = (*it).second.begin(); itr != (*it).second.end(); itr++){
            cout<<(*itr)<<" ";
        }
        cout<<"\n";
    }*/
    result = 0;
    unordered_set<string> path;
    path.insert(start);
    /*auto& set = map[curr[0]];
    for(auto child : set){
        cout<<child<<" ";
    }
    cout<<"\n";*/
    dfs(start, path);
    cout<<result<<endl;
    return 0;
}