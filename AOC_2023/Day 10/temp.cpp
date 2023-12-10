#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

// | is a vertical pipe connecting north and south. 
// - is a horizontal pipe connecting east and west. 
// L is a 90-degree bend connecting north and east. 
// J is a 90-degree bend connecting north and west. 
// 7 is a 90-degree bend connecting south and west. 
// F is a 90-degree bend connecting south and east. 
// . is ground; there is no pipe in this tile.
// S is the starting position of the animal; there is a pipe on this tile, but your sketch doesn't show what shape the pipe has

bool withinBounds(int i, int j, int n, int m){
    return i >= 0 && i < n && j >= 0 && j < m;
}

class Solution
{
    public:
    vector<string> grid;
    vector<vector<int>> stepGrid;
    void init(ifstream& input){
        string line;
        while (getline(input, line))
        {
            grid.push_back(line);
        }
        stepGrid.resize(grid.size(), vector<int>(grid[0].size(), -1));
    }
    void part1(){
        queue<pair<int, int>> q;
        for(int i = 0; i < grid.size(); i++){
            for(int j = 0; j < grid[0].size(); j++){
                if(grid[i][j] == 'S'){
                    q.push({i, j});
                    stepGrid[i][j] = 0;
                    break;
                }
            }
        }
        auto start = q.front();
        q.pop();
        if(withinBounds(start.first+1, start.second, grid.size(), grid[0].size()) && (grid[start.first+1][start.second] == '|'  || grid[start.first+1][start.second] == 'L')){
            stepGrid[start.first+1][start.second] = 1;
            q.push({start.first+1, start.second});
        }
        if(withinBounds(start.first, start.second+1, grid.size(), grid[0].size()) && (grid[start.first][start.second+1] == '-' || grid[start.first][start.second+1] == 'J')){
            stepGrid[start.first][start.second+1] = 1;
            q.push({start.first, start.second+1});
        }
        if(withinBounds(start.first-1, start.second, grid.size(), grid[0].size()) && (grid[start.first-1][start.second] == 'F' || grid[start.first-1][start.second] == '|' || grid[start.first-1][start.second] == '7')){
            stepGrid[start.first-1][start.second] = 1;
            q.push({start.first-1, start.second});
        }
        if(withinBounds(start.first, start.second-1, grid.size(), grid[0].size()) && (grid[start.first][start.second-1] == '-')){
            stepGrid[start.first][start.second-1] = 1;
            q.push({start.first, start.second-1});
        }
        while(!q.empty()){
            auto curr = q.front();
            q.pop();
            int i = curr.first, j = curr.second;
            // north and south 
            if(grid[i][j] == '|'){
                if(withinBounds(i+1, j, grid.size(), grid[0].size()) && stepGrid[i+1][j] == -1){
                    stepGrid[i+1][j] = stepGrid[i][j] + 1;
                    q.push({i+1, j});
                }
                else if(withinBounds(i-1, j, grid.size(), grid[0].size()) && stepGrid[i-1][j] == -1){
                    stepGrid[i-1][j] = stepGrid[i][j] + 1;
                    q.push({i-1, j});
                }
            }
            // east and west
            else if(grid[i][j] == '-'){
                if(withinBounds(i, j+1, grid.size(), grid[0].size()) && stepGrid[i][j+1] == -1){
                    stepGrid[i][j+1] = stepGrid[i][j] + 1;
                    q.push({i, j+1});
                }
                else if(withinBounds(i, j-1, grid.size(), grid[0].size()) && stepGrid[i][j-1] == -1){
                    stepGrid[i][j-1] = stepGrid[i][j] + 1;
                    q.push({i, j-1});
                }
            }
            // north and east
            else if(grid[i][j] == 'L'){
                if(withinBounds(i, j+1, grid.size(), grid[0].size()) && stepGrid[i][j+1] == -1 && stepGrid[i-1][j] != -1){
                    stepGrid[i][j+1] = stepGrid[i][j] + 1;
                    q.push({i, j+1});
                }
                else if(withinBounds(i-1, j, grid.size(), grid[0].size()) && stepGrid[i-1][j] == -1 && stepGrid[i][j+1] != -1){
                    stepGrid[i-1][j] = stepGrid[i][j] + 1;
                    q.push({i-1, j});
                }
            }
            // north and west
            else if(grid[i][j] == 'J'){
                if(withinBounds(i-1, j, grid.size(), grid[0].size()) && stepGrid[i-1][j] == -1 && stepGrid[i][j-1] != -1){
                    stepGrid[i-1][j] = stepGrid[i][j] + 1;
                    q.push({i-1, j});
                }
                else if(withinBounds(i, j-1, grid.size(), grid[0].size()) && stepGrid[i][j-1] == -1 && stepGrid[i-1][j] != -1){
                    stepGrid[i][j-1] = stepGrid[i][j] + 1;
                    q.push({i, j-1});
                }
            }
            // south and west
            else if(grid[i][j] == '7'){
                if(withinBounds(i+1, j, grid.size(), grid[0].size()) && stepGrid[i+1][j] == -1 && stepGrid[i][j-1] != -1){
                    stepGrid[i+1][j] = stepGrid[i][j] + 1;
                    q.push({i+1, j});
                }
                else if(withinBounds(i, j-1, grid.size(), grid[0].size()) && stepGrid[i][j-1] == -1 && stepGrid[i+1][j] != -1){
                    stepGrid[i][j-1] = stepGrid[i][j] + 1;
                    q.push({i, j-1});
                }
            }
            // south and east
            else if(grid[i][j] == 'F'){
                if(withinBounds(i, j+1, grid.size(), grid[0].size()) && stepGrid[i][j+1] == -1 && stepGrid[i+1][j] != -1){
                    stepGrid[i][j+1] = stepGrid[i][j] + 1;
                    q.push({i, j+1});
                }
                else if(withinBounds(i+1, j, grid.size(), grid[0].size()) && stepGrid[i+1][j] == -1 && stepGrid[i][j+1] != -1){
                    stepGrid[i+1][j] = stepGrid[i][j] + 1;
                    q.push({i+1, j});
                }
            }
        }
        int loopSize = 0;
        for(int i = 0; i < grid.size(); i++){
            for(int j = 0; j < grid[0].size(); j++){
                if(stepGrid[i][j] != -1){
                    loopSize = max(loopSize, stepGrid[i][j]);
                }
            }
        }
        // to count the start in the loop
        cout<<"Part 1: "<<(loopSize+1)/2<<"\n";
    }
    // Point in Polygon using ray casting algorithm
    void part2(){
        int result = 0;
        for(int i = 0; i < grid.size(); i++){
            int temp = 0;
            for(int j = 0; j < grid[i].size(); j++){
                if(stepGrid[i][j] != -1){
                    // either use this or 7, F,| to count number of intersections while passing the horizontal ray throight the polygon/loop
                    if(grid[i][j] == 'L' || grid[i][j] == 'J' || grid[i][j] == '|'){
                        temp++;
                    }
                }
                else{
                    if(temp%2){
                        result++;
                    }
                }
            }
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