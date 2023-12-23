#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>
#include <set>

using namespace std;

using ll = long long;

class Solution
{
    public:
    int n, m;
    vector<string> grid;
    pair<int,int> start;
    pair<int,int> dirs[4] = {{0,1},{1,0},{0,-1},{-1,0}};
    void init(ifstream& input){
        string line;
        while (getline(input, line))
            grid.push_back(line);
        n = grid.size();
        m = grid[0].size();
        for(int i = 0; i < grid.size(); i++){
            for(int j = 0; j < grid[i].size(); j++){
                if(grid[i][j] == 'S'){
                    start = {i,j};
                    break;
                }
            }
        }
    }

    bool isValid(pair<int,int> pos){
        while(pos.first < 0) pos.first += n;
        while(pos.second < 0) pos.second += m;
        pos.first %= n;
        pos.second %= m;
        return grid[pos.first][pos.second] == '.';
    }

    void part1(){
        queue<pair<int,int>> q;
        q.push(start);
        int steps = 0;
        while(steps < 64 && !q.empty()){
            int size = q.size();
            for(int i = 0; i < size; i++){
                pair<int,int> curr = q.front();
                q.pop();
                grid[curr.first][curr.second] = '.';
                for(int j = 0; j < 4; j++){
                    pair<int,int> next = {curr.first + dirs[j].first, curr.second + dirs[j].second};
                    if(isValid(next)){
                        grid[next.first][next.second] = 'O';
                        q.push(next);
                    }
                }
            }
            steps++;
        }
        cout<<"Part 1: "<< q.size() << "\n";
    }

    ll function(ll n, vector<int> temp){
        ll  y0 = temp[0], y1 = temp[1], y2 = temp[2];
        ll a = (y2 - 2*y1 + y0)/2;
        ll b = y2 - y1 - a;
        ll c = y1;
        ll result = a*n*n + b*n + c;
        return result;
    }    

    void part2(){
        for(int i = 0; i < grid.size(); i++){
            for(int j = 0; j < grid[i].size(); j++){
                if(grid[i][j] == 'O'){
                    grid[i][j] = '.';
                }
            }
        }
        grid[start.first][start.second] = '.';
        vector<int> temp;
        int steps = 0;
        set<pair<int,int>> prevVisited, currVisited;
        currVisited.insert(start);
        while(true){
            int size = currVisited.size();
            if(steps%n == n/2) temp.push_back(size);
            if(temp.size() == 3) break;
            prevVisited.clear();
            for(auto it : currVisited){
                for(int j = 0; j < 4; j++){
                    pair<int,int> next = {it.first + dirs[j].first, it.second + dirs[j].second};
                    if(isValid(next)){
                        prevVisited.insert(next);
                    }
                }
            }
            currVisited = prevVisited;
            steps++;
        }
        cout<<"Part 2: "<< function(ll(26501365/n), temp) << "\n";
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