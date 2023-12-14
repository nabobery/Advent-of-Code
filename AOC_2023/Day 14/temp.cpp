#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <chrono>

using namespace std;

class Solution
{
    public:
    vector<string> grid, copyGrid;
    void init(ifstream& input){
        string line;
        while (getline(input, line))
        {
            grid.push_back(line);
        }
        copyGrid = grid;
    }

    void simulate(char direction){
        if(direction == 'N'){
            for (int i = 1; i < grid.size(); i++)
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
            for(int j = 1; j < grid[0].size(); j++){
                for (int i = 0; i < grid.size(); i++)
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
            for (int i = grid.size() - 2; i >= 0; i--)
            {
                for(int j = 0; j < grid[i].size(); j++){
                    if(grid[i][j] == 'O'){
                        int k = i;
                        while(k < grid.size() - 1 && grid[k+1][j] == '.'){
                            k++;
                        }
                        grid[i][j] = '.';
                        grid[k][j] = 'O';
                    }
                }
            }
        }
        else if(direction == 'E'){
            for(int j = grid[0].size() - 2; j >= 0; j--){
                for (int i = 0; i < grid.size(); i++)
                {
                    if(grid[i][j] == 'O'){
                        int k = j;
                        while(k < grid[0].size() - 1 && grid[i][k+1] == '.'){
                            k++;
                        }
                        grid[i][j] = '.';
                        grid[i][k] = 'O';
                    }
                }
            }
        }
    }

    void part1(){
        int result = 0;
        for (int i = 1; i < grid.size(); i++)
        {
            simulate('N');
        }
        for (int i = 0; i < grid.size(); i++)
        {
            for(int j = 0; j < grid[i].size(); j++){
                if(grid[i][j] == 'O'){
                    result += (grid.size() - i);
                }
            }
        }
        cout<<"Part 1: "<<result<<"\n";
    }

    void printGrid(){
        for (int i = 0; i < grid.size(); i++)
        {
            cout<<grid[i]<<"\n";
        }
        cout<<"\n";
    }

    string gridToString(){
        string result = "";
        for (int i = 0; i < grid.size(); i++)
        {
            result += grid[i];
        }
        return result;
    }

    void part2(){
        int result = 0, cycles = 1000000000;
        grid = copyGrid;
        char directions[4] = {'N', 'W', 'S', 'E'};
        unordered_map<string, int> seen;
        int sim = 0;
        for(int i = 0; i < cycles;i++){
            for(int j = 0;j < 4;j++){
                simulate(directions[j]);
            }
            string gridString = gridToString();
            // if seen before, we can skip all the cycles in between and just simulate the remaining cycles
            if(seen.count(gridString)){
                // the cycle length is the difference between the current grid and the grid we saw before
                int temp = i+1 - seen[gridString];
                // the remaining cycles is the total cycles minus the current cycle
                int remaining = cycles - (i+1);
                // the number of cycles we can skip is the remaining cycles divided by the cycle length times the cycle length
                int skip = (remaining / temp) * temp;
                // the current(remaining) cycle is the number of cycles we can skip plus the current cycle
                sim = i+1 + skip;
                break;
            }
            seen[gridString] = i+1;
        }
        // simulate the remaining cycles
        for(int i = sim; i < cycles;i++){
            for(int j = 0;j < 4;j++){
                simulate(directions[j]);
            }
        }
        // get the total load
        for (int i = 0; i < grid.size(); i++)
        {
            for(int j = 0; j < grid[i].size(); j++){
                if(grid[i][j] == 'O'){
                    result += (grid.size() - i);
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
    // calculate time taken to run program
    auto start = chrono::high_resolution_clock::now();
    Solution s;
    s.init(in);
    s.part1();
    s.part2();
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "Time taken by function: " << double(duration.count())/1000000 << " seconds" << "\n";
    return 0;
}