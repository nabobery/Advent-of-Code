#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cctype>
#include <vector>
#include <set>
#include <algorithm>
#include <cstdlib>
using namespace std;

int main()
{
    ifstream in("input.txt");
    if (!in.is_open())
    {
        cerr << "Could not open the file - '" << endl;
        return EXIT_FAILURE;
    }
    string line;
    set<int> cycles({20,60,100,140,180,220});
    int cycle = 1, x = 1, result = 0;
    while (getline(in, line))
    {
        if(line.find("addx") != string::npos)
        {
            int num = stoi(line.substr(5));
            cycle++;
            if(cycles.find(cycle) != cycles.end()){
                result += (cycle*x);
            }       
            cycle++;
            x += num;
            if(cycles.find(cycle) != cycles.end()){
                result += (cycle*x);
            }
        }
        else{
            cycle++;
            if(cycles.find(cycle) != cycles.end())
            {
                result += (cycle*x);
            }
        }
    }
    cout<<result<<"\n";
    // part 2
    in.clear();
    in.seekg(0, ios::beg);
    char image[6][40];
    for(int i = 0; i < 6; i++)
    {
        for(int j = 0; j < 40; j++)
        {
            image[i][j] = '.';
        }
    }
    x = 1, cycle = 0;
    while (getline(in,line))
    {
        int i = cycle / 40, j = cycle % 40;
        if(line.find("addx") != string::npos)
        {
            int num = stoi(line.substr(5));
            if(j >= x-1 && j <= x+1){
                image[i][j] = '#';
            }
            j = (cycle+1) % 40;
            if(j >= x-1 && j <= x+1){
                image[i][j] = '#';
            }
            cycle += 2; 
            x += num;
        }
        else{
            if(j >= x-1 && j <= x+1){
                image[i][j] = '#';
            }
            cycle++;   
        }  
    }
    for(int i = 0; i < 6; i++)
    {
        for(int j = 0; j < 40; j++)
        {
            cout<<image[i][j];
        }
        cout<<"\n";
    }
    return 0;
}