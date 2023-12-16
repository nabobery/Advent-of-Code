#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <unordered_map>

using namespace std;

class Solution
{
    public:
    vector<string> grid;
    unordered_map<char, pair<int,int>> dir = {{'N', {-1,0}}, {'S', {1,0}}, {'E', {0,1}}, {'W', {0,-1}}};
    void init(ifstream& input){
        string line;
        while (getline(input, line))
        {
            grid.push_back(line);
        }
    }

    bool inValid(pair<int,int> curr){
        return curr.first < 0 || curr.first >= grid.size() || curr.second < 0 || curr.second >= grid[0].size();
    }

    void dfs(pair<int,int> curr, char d, vector<vector<set<char>>>& visited){
        if(inValid(curr)) return;
        if(visited[curr.first][curr.second].count(d)) return;
        visited[curr.first][curr.second].insert(d);
        if(grid[curr.first][curr.second] == '.'){
            pair<int,int> next = {curr.first + dir[d].first, curr.second + dir[d].second};
            dfs(next, d, visited);
        }
        else if(grid[curr.first][curr.second] == '/'){
            if(d == 'N'){
                pair<int,int> next = {curr.first + dir['E'].first, curr.second + dir['E'].second};
                dfs(next, 'E', visited);
            }
            else if(d == 'S'){
                pair<int,int> next = {curr.first + dir['W'].first, curr.second + dir['W'].second};
                dfs(next, 'W', visited);
            }
            else if(d == 'E'){
                pair<int,int> next = {curr.first + dir['N'].first, curr.second + dir['N'].second};
                dfs(next, 'N', visited);
            }
            else if(d == 'W'){
                pair<int,int> next = {curr.first + dir['S'].first, curr.second + dir['S'].second};
                dfs(next, 'S', visited);
            }
        }
        else if(grid[curr.first][curr.second] == '\\'){
            if(d == 'N'){
                pair<int,int> next = {curr.first + dir['W'].first, curr.second + dir['W'].second};
                dfs(next, 'W', visited);
            }
            else if(d == 'S'){
                pair<int,int> next = {curr.first + dir['E'].first, curr.second + dir['E'].second};
                dfs(next, 'E', visited);
            }
            else if(d == 'E'){
                pair<int,int> next = {curr.first + dir['S'].first, curr.second + dir['S'].second};
                dfs(next, 'S', visited);
            }
            else if(d == 'W'){
                pair<int,int> next = {curr.first + dir['N'].first, curr.second + dir['N'].second};
                dfs(next, 'N', visited);
            }
        }
        else if(grid[curr.first][curr.second] == '|'){
            if(d == 'N' || d == 'S'){
                pair<int,int> next = {curr.first + dir[d].first, curr.second + dir[d].second};
                dfs(next, d, visited);
            }
            else if(d == 'E' || d == 'W'){
                pair<int,int> next1 = {curr.first + dir['N'].first, curr.second + dir['N'].second}, next2 = {curr.first + dir['S'].first, curr.second + dir['S'].second};
                dfs(next1, 'N', visited);
                dfs(next2, 'S', visited);
            }
        }
        else if(grid[curr.first][curr.second] == '-'){
            if(d == 'E' || d == 'W'){
                pair<int,int> next = {curr.first + dir[d].first, curr.second + dir[d].second};
                dfs(next, d, visited);
            }
            else if(d == 'N' || d == 'S'){
                pair<int,int> next1 = {curr.first + dir['E'].first, curr.second + dir['E'].second}, next2 = {curr.first + dir['W'].first, curr.second + dir['W'].second};
                dfs(next1, 'E', visited);
                dfs(next2, 'W', visited);
            }
        }  
    }

    int energise(pair<int,int> start, char dir){
        vector<vector<set<char>>> visited(grid.size(), vector<set<char>>(grid[0].size(), set<char>()));
        int result = 0;
        dfs(start, dir, visited);
        for(auto row: visited){
            for(auto col: row){
                if(col.size() > 0) result++;
            }
        }
        return result;
    }

    void part1(){
        int result = energise({0,0}, 'E');
        cout<<"Part 1: "<<result<<"\n";
    }

    void part2(){
        int result = 0;
        // leftmost and rightmost column
        for(int i = 0; i < grid.size();i++){
            result = max(result, energise({i,0}, 'E'));
            result = max(result, energise({i,grid[0].size()-1}, 'W'));
        }
        // topmost and bottommost row
        for(int i = 0; i < grid[0].size();i++){
            result = max(result, energise({0,i}, 'S'));
            result = max(result, energise({grid.size()-1,i}, 'N'));
        }
        cout<<"Part 2: "<<result<<"\n";
    }

};

int main(){
    ifstream in("input.txt");
    if (!in.is_open())
    {
        cerr << "Could not open the file - '" << endl;
        return EXIT_FAILURE;
    }
    Solution s;
    s.init(in);
    s.part1();
    s.part2();
    return 0;
}