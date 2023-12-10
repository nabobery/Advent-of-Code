#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>
#include <map>

using namespace std;

class Solution
{
    public:
    int n, m;
    vector<string> grid;
    map<char, vector<char>> pipes = {
        {'|', {'N', 'S'}},
        {'-', {'E', 'W'}},
        {'L', {'N', 'E'}},
        {'J', {'N', 'W'}},
        {'7', {'S', 'W'}},
        {'F', {'S', 'E'}},
        {'S', {'S', 'E', 'W', 'N'}},
    };
    map<char,pair<int,int>> directions = {
        {'N', {-1, 0}},
        {'S', {1, 0}},
        {'E', {0, 1}},
        {'W', {0, -1}},
    };
    map<char, string> directions_to_pipe = {
        {'N', "|7FS"},
        {'S', "|JLS"},
        {'E', "-J7S"},
        {'W', "-LFS"},
    };
    map<pair<int,int>,pair<int,int>> parent;
    vector<pair<int,int>> path;
    void init(ifstream& input){
        string line;
        while (getline(input, line))
        {
            grid.push_back(line);
        }
        n = grid.size();
        m = grid[0].size();
    }
    bool withinBounds(int i, int j){
        return i >= 0 && i < n && j >= 0 && j < m;
    }
    vector<pair<int,int>> getNeighbours(int i, int j){
        vector<pair<int,int>> neighbours;
        if(grid[i][j] == '.') return neighbours;
        for(auto dir : pipes[grid[i][j]]){
            int x = directions[dir].first, y = directions[dir].second;
            if(withinBounds(i+x, j+y) && directions_to_pipe[dir].find(grid[i+x][j+y]) != string::npos){
                neighbours.push_back({i+x, j+y});
            }
        }
        return neighbours;
    }
    void part1(){
        queue<pair<int, int>> q;
        // find the starting point
        pair<int,int> start;
        for(int i = 0; i < grid.size(); i++){
            for(int j = 0; j < grid[0].size(); j++){
                if(grid[i][j] == 'S'){
                    start = {i, j};
                    q.push(start);
                    break;
                }
            }
        }
        parent[start] = {-1, -1};
        while(!q.empty()){
            auto curr = q.front();
            q.pop();
            int i = curr.first, j = curr.second;
            if(curr == start && parent[start] != make_pair(-1, -1)) break;
            for (auto neighbour : getNeighbours(i, j))
            {
                if(parent[curr] == neighbour) continue;
                parent[neighbour] = curr;
                q.push(neighbour);
            }
        }
        path.push_back(start);
        while(true){
            auto curr = parent[path.back()];
            if(curr == start) break;
            path.push_back(curr);
        }
        cout<<"Part 1: "<<path.size()/2<<"\n";
    }
    
    // Pick's theorem for area of polygon
    // A = i + b/2 - 1
    // i = number of points inside the polygon = A - b/2 + 1
    // We can find the area of the polygon by shoelace formula
    // A = 1/2 * sum(x[i]*y[i+1] - x[i+1]*y[i])
    // b = number of points on the boundary of the polygon
    void part2(){
        path.push_back(path[0]);
        int area = 0;
        for(int i = 0; i < path.size()-1; i ++){
            area += path[i].first*path[i+1].second - path[i+1].first*path[i].second;
        }
        if(area < 0) area *= -1;
        area /= 2;
        int result = area - path.size()/2 + 1;
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