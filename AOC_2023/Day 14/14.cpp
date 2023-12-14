#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

class Solution
{
    public:
    int n, m;
    vector<string> grid, copyGrid;
    void init(ifstream& input){
        string line;
        while (getline(input, line))
        {
            grid.push_back(line);
        }
        copyGrid = grid;
        n = grid.size(), m = grid[0].size();
    }

    void simulate(char direction){
        if(direction == 'N'){
            for (int i = 1; i < n; i++)
            {
                for(int j = 0; j < grid[i].size(); j++){
                    if(grid[i][j] == 'O'){
                        int k = i;
                        while(k > 0 && grid[k-1][j] == '.'){
                            k--;
                        }
                        grid[i][j] = '.';
                        grid[k][j] = 'O';
                    }
                }
            }
        }
        else if(direction == 'W'){
            for(int j = 1; j < m; j++){
                for (int i = 0; i < n; i++)
                {
                    if(grid[i][j] == 'O'){
                        int k = j;
                        while(k > 0 && grid[i][k-1] == '.'){
                            k--;
                        }
                        grid[i][j] = '.';
                        grid[i][k] = 'O';
                    }
                }
            }
        }
        else if(direction == 'S'){
            for (int i = n - 2; i >= 0; i--)
            {
                for(int j = 0; j < grid[i].size(); j++){
                    if(grid[i][j] == 'O'){
                        int k = i;
                        while(k < n - 1 && grid[k+1][j] == '.'){
                            k++;
                        }
                        grid[i][j] = '.';
                        grid[k][j] = 'O';
                    }
                }
            }
        }
        else if(direction == 'E'){
            for(int j = m - 2; j >= 0; j--){
                for (int i = 0; i < n; i++)
                {
                    if(grid[i][j] == 'O'){
                        int k = j;
                        while(k < m - 1 && grid[i][k+1] == '.'){
                            k++;
                        }
                        grid[i][j] = '.';
                        grid[i][k] = 'O';
                    }
                }
            }
        }
    }

    string gridToString(){
        string result = "";
        for(string s : grid)
            result += s;
        return result;
    }

    int totalLoad(){
        int result = 0;
        for (int i = 0; i < n; i++){
            for(int j = 0; j < m; j++){
                if(grid[i][j] == 'O'){
                    result += (n - i);
                }
            }
        }
        return result;
    }

    void part1(){
        simulate('N');
        cout<<"Part 1: "<<totalLoad()<<"\n";
    }

    void part2(){
        int cycles = 1000000000,sim;
        grid = copyGrid;
        char directions[4] = {'N', 'W', 'S', 'E'};
        unordered_map<string, int> seen;
        for(int i = 1; i <= cycles;i++){
            for(char direction : directions)
                simulate(direction);
            string gridString = gridToString();
            if(seen.count(gridString)){
                sim = i + (((cycles - i)/(i - seen[gridString])) * (i - seen[gridString]));
                break;
            }
            seen[gridString] = i;
        }
        for(int i = sim+1; i <= cycles;i++){
            for(char direction : directions)
                simulate(direction);
        }
        cout<<"Part 2: "<<totalLoad()<<"\n";
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