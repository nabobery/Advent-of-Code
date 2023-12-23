#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <map>

using namespace std;

class Solution
{
    public:
    vector<string> grid;
    pair<int,int> start, goal;
    int part1result = 0, part2result = 0;
    pair<int,int> dirs[4] = {{0,1},{0,-1},{1,0},{-1,0}};
    map<char,int> slopetoDir = {{'>', 0},{'v', 2}};
    void init(ifstream& input){
        string line;
        while (getline(input, line))
        {
            grid.push_back(line);
        }
        for (int j = 0; j < grid[0].size(); j++){
            if(grid[0][j] == '.'){
                start = {0,j};
                break;
            }
        }
        for (int j = 0; j < grid[grid.size()-1].size(); j++){
            if(grid[grid.size()-1][j] == '.'){
                goal = {grid.size()-1,j};
                break;
            }
        }
    }

    bool inBounds(pair<int,int> p){
        return p.first >= 0 && p.first < grid.size() && p.second >= 0 && p.second < grid[0].size();
    }

    void dfs(set<pair<int, int>> visited, pair<int,int> curr){
        if(curr == goal){
            part1result = max(part1result, (int)visited.size());
            return;
        }
        if(visited.count(curr) || !inBounds(curr) || grid[curr.first][curr.second] == '#') return;
        visited.insert(curr);
        if(slopetoDir.count(grid[curr.first][curr.second])){
            int dir = slopetoDir[grid[curr.first][curr.second]];
            dfs(visited, {curr.first + dirs[dir].first, curr.second + dirs[dir].second});
        }
        else{
            for(auto d : dirs){
                dfs(visited, {curr.first + d.first, curr.second + d.second});
            }
        }
    }

    void dfs2(set<pair<int, int>> visited, pair<int,int> curr){
        if(curr == goal){
            part2result = max(part2result, (int)visited.size());
            return;
        }
        if(visited.count(curr) || !inBounds(curr) || grid[curr.first][curr.second] == '#') return;
        visited.insert(curr);
        for(auto d : dirs){
            dfs2(visited, {curr.first + d.first, curr.second + d.second});
        }
    }

    void part1(){
        set<pair<int, int>> visited;
        dfs(visited, start);
        cout << "Part 1: " << part1result << "\n";
    }

    void part2(){
        set<pair<int, int>> visited;
        dfs2(visited, start);
        cout << "Part 2: " << part2result << "\n";
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