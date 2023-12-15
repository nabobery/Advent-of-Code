#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>

using namespace std;

class Solution
{
    public:
    vector<vector<char>> grid, copyGrid;
    int wallIndex = 0;
    void init(ifstream& input){
        string line;
        grid.resize(200, vector<char>(1000, '.'));
        regex pattern(R"((\d+),(\d+))");
        while (getline(input, line))
        {
            sregex_iterator it(line.begin(), line.end(), pattern), end;
            pair<int, int> prev = {stoi(it->str(1)), stoi(it->str(2))};
            it++;
            wallIndex = max(wallIndex, prev.second);
            while(it != end){
                pair<int, int> curr = {stoi(it->str(1)), stoi(it->str(2))};
                if(prev.first == curr.first){
                    for(int i = min(prev.second, curr.second); i <= max(prev.second, curr.second); i++){
                        grid[i][prev.first] = '#';
                    }
                }
                else{
                    for(int i = min(prev.first, curr.first); i <= max(prev.first, curr.first); i++){
                        grid[prev.second][i] = '#';
                    }
                }
                prev = curr;
                wallIndex = max(wallIndex, prev.second);
                it++;
            }
        }
        wallIndex += 2;
        copyGrid = grid;
    }

    bool dfs(pair<int, int> pos){
        if(pos.first == grid.size() - 1 || grid[0][500] == 'o'){
            return false;
        }
        if(grid[pos.first+1][pos.second] == '.'){
            return dfs({pos.first + 1, pos.second});
        }
        if(grid[pos.first + 1][pos.second] == '#' || grid[pos.first+1][pos.second] == 'o'){
            if(grid[pos.first+1][pos.second - 1] == '.'){
               return dfs({pos.first, pos.second - 1});
            }
            else if(grid[pos.first+1][pos.second + 1] == '.'){
                return dfs({pos.first, pos.second + 1});
            }
            else{
                grid[pos.first][pos.second] = 'o';
                return true;
            }
        }
        return false;
    }

    void part1(){
        int result = 0;
        grid[0][500] = '+';
        while(dfs({0, 500})){
            result++;
        }
        cout<<"Part 1: "<<result<<"\n";
    }
    void part2(){
        int result = 0;
        grid = copyGrid;
        grid[0][500] = '+';
        for(int j = 0; j < grid[0].size(); j++){
            grid[wallIndex][j] = '#';
        }
        while(dfs({0, 500})){
            result++;
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