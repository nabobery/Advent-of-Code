#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <set>

using namespace std;

class Solution
{
    public:
    vector<vector<int>> grid;
    int n,m;
    unordered_map<char, pair<int,int>> dir = {{'N', {-1,0}}, {'S', {1,0}}, {'E', {0,1}}, {'W', {0,-1}}};
    void init(ifstream& input){
        string line;
        while (getline(input, line)){
            vector<int> row;
            for(char c : line){
                row.push_back(c - '0');
            }
            grid.push_back(row);
        }
        n = grid.size();
        m = grid[0].size();
    }

    bool isValid(pair<int,int> curr){
        return curr.first >= 0 && curr.first < n && curr.second >= 0 && curr.second < m;
    }
    void dfs(pair<int,int> curr, char d, int step_count, int path_cost, vector<vector<int>>& visited, set<pair<int,int>> path){
        if(curr.first == n-1 && curr.second == m-1){
            if(path_cost)
            cout<<"Path: "<<path_cost<< "\n";
            for(int i = 0; i < n;i++){
                for(int j = 0; j < m;j++){
                    if(path.count({i,j})) cout<<"#";
                    else cout<<grid[i][j];
                }
                cout<<"\n";
            }
            cout<<"\n";
            return;
        }
        //cout<<curr.first<<" "<<curr.second<<" "<<d<<" "<<step_count<<" "<<path_cost<<"\n";
        if(step_count < 3){
            // if same direction continuation
            pair<int,int> next = {curr.first + dir[d].first, curr.second + dir[d].second};
            if(isValid(next)){
                if(path_cost + grid[next.first][next.second] > visited[next.first][next.second]) return;
                path.insert(next);
                visited[next.first][next.second] = min(visited[next.first][next.second], path_cost + grid[next.first][next.second]);
                dfs(next, d, step_count+1, path_cost + grid[next.first][next.second], visited, path);
                path.erase(next);
            }
            // else change direction to 90 degree left or right
            if(d == 'N' || d == 'S'){
                next = {curr.first + dir['E'].first, curr.second + dir['E'].second};
                if(isValid(next)){
                    //if(path_cost + grid[next.first][next.second] > visited[next.first][next.second]) return;
                    visited[next.first][next.second] = min(visited[next.first][next.second], path_cost + grid[next.first][next.second]);
                    path.insert(next);
                    dfs(next, 'E', 1, path_cost + grid[next.first][next.second], visited, path);
                    path.erase(next);
                }
                next = {curr.first + dir['W'].first, curr.second + dir['W'].second};
                if(isValid(next)){
                    //if(path_cost + grid[next.first][next.second] > visited[next.first][next.second]) return;
                    visited[next.first][next.second] = min(visited[next.first][next.second], path_cost + grid[next.first][next.second]);
                    path.insert(next);
                    dfs(next, 'W', 1, path_cost + grid[next.first][next.second], visited, path);
                    path.erase(next);
                }
            }
            else{
                next = {curr.first + dir['N'].first, curr.second + dir['N'].second};
                if(isValid(next)){
                    //if(path_cost + grid[next.first][next.second] > visited[next.first][next.second]) return;
                    visited[next.first][next.second] = min(visited[next.first][next.second], path_cost + grid[next.first][next.second]);
                    path.insert(next);
                    dfs(next, 'N', 1, path_cost + grid[next.first][next.second], visited, path);
                    path.erase(next);
                }
                next = {curr.first + dir['S'].first, curr.second + dir['S'].second};
                if(isValid(next)){
                    //if(path_cost + grid[next.first][next.second] > visited[next.first][next.second]) return;
                    visited[next.first][next.second] = min(visited[next.first][next.second], path_cost + grid[next.first][next.second]);
                    path.insert(next);
                    dfs(next, 'S', 1, path_cost + grid[next.first][next.second], visited, path);
                    path.erase(next);
                }
            }
        }
        else{
            pair<int,int> next;
            if(d == 'N' || d == 'S'){
                next = {curr.first + dir['E'].first, curr.second + dir['E'].second};
                if(isValid(next)){
                    //if(path_cost + grid[next.first][next.second] > visited[next.first][next.second]) return;
                    visited[next.first][next.second] = min(visited[next.first][next.second], path_cost + grid[next.first][next.second]);
                    path.insert(next);
                    dfs(next, 'E', 1, path_cost + grid[next.first][next.second], visited, path);
                    path.erase(next);
                }
                next = {curr.first + dir['W'].first, curr.second + dir['W'].second};
                if(isValid(next)){
                    //if(path_cost + grid[next.first][next.second] > visited[next.first][next.second]) return;
                    visited[next.first][next.second] = min(visited[next.first][next.second], path_cost + grid[next.first][next.second]);
                    path.insert(next);
                    dfs(next, 'W', 1, path_cost + grid[next.first][next.second], visited, path);
                    path.erase(next);
                }
            }
            else{
                next = {curr.first + dir['N'].first, curr.second + dir['N'].second};
                if(isValid(next)){
                    //if(path_cost + grid[next.first][next.second] > visited[next.first][next.second]) return;
                    visited[next.first][next.second] = min(visited[next.first][next.second], path_cost + grid[next.first][next.second]);
                    path.insert(next);
                    dfs(next, 'N', 1, path_cost + grid[next.first][next.second], visited, path);
                    path.erase(next);
                }
                next = {curr.first + dir['S'].first, curr.second + dir['S'].second};
                if(isValid(next)){
                    //if(path_cost + grid[next.first][next.second] > visited[next.first][next.second]) return;
                    visited[next.first][next.second] = min(visited[next.first][next.second], path_cost + grid[next.first][next.second]);
                    path.insert(next);
                    dfs(next, 'S', 1, path_cost + grid[next.first][next.second], visited, path);
                    path.erase(next);
                }
            }
        }
    }

    void part1(){
        int result = 0;
        vector<vector<int>> visited(n, vector<int>(m, INT_MAX));
        set<pair<int,int>> path;
        visited[0][0] = 0;
        dfs({0,0}, 'E', 1, 0, visited, path);
        for(auto row: visited){
            for(auto col: row){
                cout<<col<<" ";
            }
            cout<<"\n";
        }
        //cout<<"Part 1: "<<visited[n-1][m-1]<<"\n";
    }
    void part2(){

    }

};

int main(){
    ifstream in("test.txt");
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