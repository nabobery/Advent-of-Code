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

#define sgn(v) ( ( (v) < 0 ) ? -1 : ( (v) > 0 ) )

int main()
{
    ifstream in("input.txt");
    if (!in.is_open())
    {
        cerr << "Could not open the file - '" << endl;
        return EXIT_FAILURE;
    }
    string line;
    set<pair<int,int>> visited;
    pair<int,int> head = {0,0};
    pair<int,int> tail = {0,0};
    visited.insert(tail);
    while (getline(in, line))
    {
        char dir = line[0];
        int dist = stoi(line.substr(1));
        while(dist--){
            switch(dir){
                case 'U': head.second++; break;
                case 'D': head.second--; break;
                case 'L': head.first--; break;
                case 'R': head.first++; break;
            }
            int diff_x = head.first - tail.first,diff_y = head.second - tail.second;
            if(abs(diff_x) > 1 || abs(diff_y) > 1){
                tail.first += sgn(diff_x);
                tail.second += sgn(diff_y);
                visited.insert(tail);
            }
        }
    }
    cout << visited.size() << "\n";
    
    // Part 2
    in.clear();
    in.seekg(0, ios::beg);
    visited.clear();
    vector<pair<int,int>> rope(10, {0,0});
    visited.insert({0,0});
    while (getline(in, line))
    {
        char dir = line[0];
        int dist = stoi(line.substr(1));
        while(dist--){
            switch(dir){
                case 'U': rope[0].second++; break;
                case 'D': rope[0].second--; break;
                case 'L': rope[0].first--; break;
                case 'R': rope[0].first++; break;
            }
            for(int i = 1; i < 10;i++){
                int diff_x = rope[i].first - rope[i-1].first,diff_y = rope[i].second - rope[i-1].second;
                if(abs(diff_x) > 1 || abs(diff_y) > 1){
                    rope[i].first -= sgn(diff_x);
                    rope[i].second -= sgn(diff_y);    
                }
            }
            visited.insert(rope[9]);
        }
    }
    cout<<visited.size()<<"\n";
    return 0;
}
    