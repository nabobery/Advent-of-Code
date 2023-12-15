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

    // standard merge intervals Algo
    void mergeIntervals(vector<pair<int, int>>& intervals){
        sort(intervals.begin(), intervals.end());
        int index = 0;
        for(int i = 1; i < intervals.size(); i++){
            // if overlaps
            if(intervals[index].second >= intervals[i].first){
                intervals[index].second = max(intervals[index].second, intervals[i].second);
            }
            // if not overlaps
            else{
                index++;
                intervals[index] = intervals[i];
            }
        }
        intervals.resize(index+1);
    }

    void part1(){
        int y = 2000000, result = 0;
        vector<pair<int, int>> intervals;
        // for each sensor, find the manhattan distance, and then find the x distance
        for(auto it : sensorstobeacons){
            int manhattan = abs(it.first.first - it.second.first) + abs(it.first.second - it.second.second), y_dist = abs(it.first.second - y);
            // if y distance is greater than manhattan distance, then it is not the shortest distance to sensor
            if(y_dist > manhattan) continue;
            int x_dist = manhattan - y_dist;
            // add the x interval for the possible beacons  on y = 2000000 for each sensor
            intervals.push_back({it.first.first - x_dist, it.first.first + x_dist});
        }
        // merge the intervals
        mergeIntervals(intervals);
        pair<int,int> beacon_y;
        // find the beacon on y = 2000000
        for(auto it : sensorstobeacons){
            if(it.second.second == y){
                beacon_y = it.second;
                break;
            }
        }
        for(auto it : intervals){
            // add the length of the interval to the result
            result += it.second - it.first + 1;
            // if beacon is in the interval, then subtract 1 from the result
            if(beacon_y.second == y && beacon_y.first >= it.first && beacon_y.first <= it.second) result--;
        }
        cout<<"Part 1: "<<result<<"\n";
    }

    void part2(){
        long long result = 0;
        int mx = 4000000;
        // for each y, find the intervals of possible beacons
        for(int i = 0; i <= mx;i++){
            vector<pair<int, int>> intervals;
            // same as part 1
            for(auto it : sensorstobeacons){
                int manhattan = abs(it.first.first - it.second.first) + abs(it.first.second - it.second.second), y_dist = abs(it.first.second - i);
                if(y_dist > manhattan) continue;
                int x_dist = manhattan - y_dist;
                intervals.push_back({max(0,it.first.first - x_dist), min(mx, it.first.first + x_dist)});
            }
            mergeIntervals(intervals);
            // if there is only one interval, then it has a continuous range of beacons
            // else there is a discontinuity which is the the distress Beacon
            // Interval is of the form [x1, x2], [x2+2, x3]
            if (intervals.size() > 1){
                result = (intervals[0].second + 1);
                result *= mx;
                result += i;
                break;
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