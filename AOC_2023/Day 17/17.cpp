#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

struct state{
    pair<int,int> coord;
    int dir;
    int stepCount;
};

struct comparator {
    bool operator()(const pair<int, state>& p1, const pair<int, state>& p2) const {
        return p1.first > p2.first;
    }
};

const int INF = 1e7;

class Solution
{
    public:
    vector<vector<int>> grid;
    int n,m;
    //unordered_map<char, pair<int,int>> dir = {{'N', {-1,0}}, {'S', {1,0}}, {'E', {0,1}}, {'W', {0,-1}}};
    // N -> 0, E -> 1, S -> 2, W -> 3
    // so that we can go right by adding 1 and left by subtracting 1
    vector<pair<int,int>> dirs = {{-1,0}, {0,1}, {1,0}, {0,-1}};
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

    bool isValid(pair<int,int>& curr){
        return curr.first >= 0 && curr.first < n && curr.second >= 0 && curr.second < m;
    }

    vector<state> getNeighbours(state curr, int minStepCount, int maxStepCount){
        vector<state> neighbours;
        if(curr.stepCount >= minStepCount){
            int d1 = (curr.dir + 1) % 4;
            int d2 = (curr.dir + 3) % 4;
            pair<int,int> next = {curr.coord.first + dirs[d1].first, curr.coord.second + dirs[d1].second};
            if(isValid(next)){
                neighbours.push_back({next, d1, 1});
            }
            next = {curr.coord.first + dirs[d2].first, curr.coord.second + dirs[d2].second};
            if(isValid(next)){
                neighbours.push_back({next, d2, 1});
            }
        }
        if(curr.stepCount < maxStepCount){
            pair<int,int> next = {curr.coord.first + dirs[curr.dir].first, curr.coord.second + dirs[curr.dir].second};
            if(isValid(next)){
                neighbours.push_back({next, curr.dir, curr.stepCount + 1});
            }
        }
        return neighbours;
    }

    int helper(int minStepCount, int maxStepCount){
        priority_queue<pair<int, state>, vector<pair<int, state>>, comparator> pq;
        vector<vector<vector<vector<int>>>> visited(n, vector<vector<vector<int>>>(m, vector<vector<int>>(4, vector<int>(maxStepCount+1, INF))));
        for(int i = 0; i < dirs.size();i++){
            pq.push({0, {{0,0}, i, 0}});
            visited[0][0][i][0] = 0;
        }
        while(!pq.empty()){
            auto [heatloss, curr] = pq.top();
            pq.pop();
            if(heatloss > visited[curr.coord.first][curr.coord.second][curr.dir][curr.stepCount]) continue;
            for(auto next : getNeighbours(curr, minStepCount, maxStepCount)){
                int newHeatLoss = heatloss + grid[next.coord.first][next.coord.second];
                if(newHeatLoss < visited[next.coord.first][next.coord.second][next.dir][next.stepCount]){
                    visited[next.coord.first][next.coord.second][next.dir][next.stepCount] = newHeatLoss;
                    pq.push({newHeatLoss, next});
                }
            }
            
        }
        int ans = INF;
        for(int i = 0; i < 4;i++){
            for(int j = 0; j <= maxStepCount;j++){
                ans = min(ans, visited[n-1][m-1][i][j]);
            }   
        }
        return ans;
    }

    void part1(){
        cout<<"Part 1: "<<helper(1, 3)<<"\n";
    }

    void part2(){
        cout<<"Part 2: "<<helper(4, 10)<<"\n";
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