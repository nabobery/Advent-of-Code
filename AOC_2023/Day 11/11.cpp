#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>

using namespace std;

class Solution
{
    public:
    vector<string> grid;
    vector<pair<int,int>> galaxy_coords;
    vector<pair<int,int>> increments;
    bool is_empty_row(int i){
        for (auto& c : grid[i])
        {
            if (c != '.')
            {
                return false;
            }
        }
        return true;
    }
    bool is_empty_col(int j){
        for (int i = 0; i < grid.size(); i++)
        {
            if (grid[i][j] != '.')
            {
                return false;
            }
        }
        return true;
    }
    void init(ifstream& input){
        string line;
        while (getline(input, line))
        {
            grid.push_back(line);
        }
        set<int> rows, cols;
        for (int i = 0; i < grid.size(); i++)
        {
            if (is_empty_row(i))
            {
                rows.insert(i);
            }
        }
        for (int j = 0; j < grid[0].size(); j++)
        {
            if (is_empty_col(j))
            {
                cols.insert(j);
            }
        }
        for (int i = 0; i < grid.size(); i++)
        {
            for (int j = 0; j < grid[0].size(); j++)
            {
                if(grid[i][j] == '#'){
                    int inc_i = 0, inc_j = 0;
                    for(auto it : rows){
                        if(it < i){
                            inc_i++;
                        }
                    }
                    for(auto it : cols){
                        if(it < j){
                            inc_j++;
                        }
                    }
                    increments.push_back({inc_i, inc_j});
                    galaxy_coords.push_back({i, j});
                }
            }
        }
    }
    
    // 2 times expansion means only one increment is needed
    void part1(){
        int result = 0;
        for(int i = 0; i < galaxy_coords.size()-1; i++){
            for(int j = i+1; j < galaxy_coords.size(); j++){
                result += abs(galaxy_coords[i].first + increments[i].first - galaxy_coords[j].first - increments[j].first) + abs(galaxy_coords[i].second + increments[i].second - galaxy_coords[j].second - increments[j].second);
            }
        }
        cout<<"Part 1: "<<result<<"\n";
    }
    // 1000000 times expansion means 999999 increments are needed
    void part2(){
        long long result = 0;
        int expansion = 999999;
        for(int i = 0; i < galaxy_coords.size()-1; i++){
            for(int j = i+1; j < galaxy_coords.size(); j++){
                result += abs(galaxy_coords[i].first - galaxy_coords[j].first + expansion*(increments[i].first - increments[j].first)) + abs(galaxy_coords[i].second - galaxy_coords[j].second + expansion*(increments[i].second - increments[j].second));
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