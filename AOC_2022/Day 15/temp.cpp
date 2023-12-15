#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <unordered_set>

using namespace std;

class Solution
{
    public:
    map<pair<int, int>, pair<int, int>> sensorstobeacons;
    void init(ifstream& input){
        string line;
        // "Sensor at x=\d+, y=\d+: closest beacon is at x=\d+, y=\d+"
        regex pattern(R"(-?\d+)");
        while (getline(input, line))
        {
            vector<string> matches;
            sregex_iterator it(line.begin(), line.end(), pattern), end;
            while(it != end){
                matches.push_back(it->str());
                it++;
            }
            sensorstobeacons[{stoi(matches[0]), stoi(matches[1])}] = {stoi(matches[2]), stoi(matches[3])};
        }
    }
    void part1(){
        int y = 2000000;
        unordered_set<int> possibleBeacons;
        for(auto it : sensorstobeacons){
            //cout<<it.first.first<<" "<<it.first.second<<" "<<it.second.first<<" "<<it.second.second<<"\n";
            int manhattan = abs(it.first.first - it.second.first) + abs(it.first.second - it.second.second);
            int y_1 = it.first.second - manhattan, y_2 = it.first.second + manhattan;
            if(y_1 <= y && y_2 >= y){
                if(y <= it.first.second){
                    int x1 = it.first.first - (y-y_1), x2 = it.first.first + (y-y_1);
                    for(int i = x1; i <= x2; i++){
                        int temp = abs(i - it.first.first) + abs(y - it.first.second);
                        if(temp <= manhattan){
                            possibleBeacons.insert(i);
                        }
                    }
                }
                else{
                    int x1 = it.first.first - (y_2-y), x2 = it.first.first + (y_2-y);
                    for(int i = x1; i <= x2; i++){
                        int temp = abs(i - it.first.first) + abs(y - it.first.second);
                        if(temp <= manhattan){
                            possibleBeacons.insert(i);
                        }
                    }
                }
            }
        }
        for(auto it : sensorstobeacons){
            if(it.second.second == y){
                possibleBeacons.erase(it.second.first);
            }
        }
        cout<<"Part 1: "<<possibleBeacons.size()<<"\n";
    }

    bool isDistressBeacon(int x, int y){
        bool b = true;
        for(auto it : sensorstobeacons){
            int manhattan = abs(it.first.first - it.second.first) + abs(it.first.second - it.second.second);
            int temp = abs(x - it.first.first) + abs(y - it.first.second);
            if(temp <= manhattan || (x == it.second.first && y == it.second.second)){
                b = false;
                break;
            }
        }
        return b;
    }
    void part2(){
        int mn = 0, mx = 4000000;
        long long result = 0;
        for(int i = mn; i <= mx;i++){
            for(int j = mn; j <= mx;j++){
                if(isDistressBeacon(i, j)){
                    result = 4000000*i + j;
                    break;
                }
            }
            if(result != 0)
                break;
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