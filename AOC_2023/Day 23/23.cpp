#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <queue>

using namespace std;

struct edge{
    pair<int,int> vertex;
    int weight;
};

class Solution
{
    public:
    vector<string> grid;
    pair<int,int> start, goal;
    int n,m;
    pair<int,int> dirs[4] = {{0,1},{0,-1},{1,0},{-1,0}};
    map<char,int> slopetoDir = {{'>', 0}, {'<', 1}, {'v', 2}, {'^', 3}};
    void init(ifstream& input){
        string line;
        while (getline(input, line)) grid.push_back(line);
        n = grid.size();
        m = grid[0].size();
        for (int j = 0; j < m; j++){
            if(grid[0][j] == '.'){
                start = {0,j};
                break;
            }
        }
        for (int j = 0; j < m; j++){
            if(grid[n-1][j] == '.'){
                goal = {n-1,j};
                break;
            }
        }
    }

    bool inBounds(pair<int,int> p){
        return p.first >= 0 && p.first < n && p.second >= 0 && p.second < m;
    }

    int helper(bool part2){
        // find all vertices that are paths(.) that can be junctions and add them to the graph
        set<pair<int,int>> vertices;
        vertices.insert(start);
        vertices.insert(goal);
        for(int i = 0; i < n; i++){
            for(int j = 0; j < m; j++){
                int neighbors = 0;
                // check if the current cell is a junction i.e., having more than 2 paths to go 
                // else it is just a normal straight path or a dead end
                for(auto& dir : dirs){
                    if(inBounds({i+dir.first,j+dir.second}) && grid[i+dir.first][j+dir.second] != '#') neighbors++;
                }
                if(neighbors > 2 && grid[i][j] != '#') vertices.insert({i,j});
            }
        }
        // build the graph with the vertices and edges
        map<pair<int,int>,vector<edge>> graph;
        for(auto& v : vertices){
            queue<edge> q;
            // bfs to find all the paths from the current vertex
            q.push({v,0});
            set<pair<int,int>> visited;
            while(!q.empty()){
                auto curr = q.front();
                q.pop();
                // if the current vertex is already visited then skip it
                if(visited.count(curr.vertex)) continue;
                visited.insert(curr.vertex);
                // if the current vertex is a junction then add it to the graph
                if(vertices.count(curr.vertex) && curr.vertex != v){
                    graph[v].push_back(curr);
                    continue;
                }
                // else add all the neighbors of the current vertex to the queue
                for(auto& dir : dirs){
                    auto next = make_pair(curr.vertex.first+dir.first,curr.vertex.second+dir.second);
                    if(inBounds(next) && grid[next.first][next.second] != '#'){
                        if(!part2 && slopetoDir.count(grid[next.first][next.second]) && dirs[slopetoDir[grid[next.first][next.second]]] != dir) continue;
                        q.push({next,curr.weight+1});
                    }
                }
            }
        }
        // dfs to find the longest path from start to goal
        int result = 0;
        // visited set to avoid cycles
        set<pair<int,int>> visited;
        // dfs function
        auto dfs = [&](auto&& dfs, pair<int,int> curr, int weight, set<pair<int,int>>& visited){
            // if the current vertex is already visited then skip it
            if(visited.count(curr)) return;
            // else add it to the visited set
            visited.insert(curr);
            // if the current vertex is the goal then update the result
            if(curr == goal){
                result = max(result,weight);
                visited.erase(curr);
                return;
            }
            // visit all the neighbors of the current vertex
            for(auto& e : graph[curr]) dfs(dfs,e.vertex,weight+e.weight,visited);
            // remove the current vertex from the visited set
            visited.erase(curr);
        };
        // call the dfs function
        dfs(dfs,start,0,visited);
        return result;
    }

    void part1(){
        cout << "Part 1: " << helper(false) << "\n";
    }

    void part2(){
        cout << "Part 2: " << helper(true) << "\n";
    }

};

int main(){
    ifstream in("input.txt");
    if (!in.is_open()){
        cerr << "Could not open the file - '" << endl;
        return EXIT_FAILURE;
    }
    Solution s;
    s.init(in);
    s.part1();
    s.part2();
    return 0;
}