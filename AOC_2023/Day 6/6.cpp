#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <cmath>

using namespace std;

class Solution
{
    public:
    vector<int> times, distances;
    void init(ifstream& input){
        string line;
        regex re("\\d+");
        while (getline(input, line))
        {
            if(line.find("Time") != string::npos){
                smatch match;
                sregex_iterator next(line.begin(), line.end(), re);
                sregex_iterator end;
                while (next != end) {
                    smatch match = *next;
                    times.push_back(stoi(match.str()));
                    next++;
                }
            }
            else if(line.find("Distance") != string::npos){
                smatch match;
                sregex_iterator next(line.begin(), line.end(), re);
                sregex_iterator end;
                while (next != end) {
                    smatch match = *next;
                    distances.push_back(stoi(match.str()));
                    next++;
                }
            }
        }
    }

    void part1(){
        long long result = 1;
        for(int i = 0; i < times.size(); i++){
            int time = times[i], distance = distances[i];
            for(int hold = 1; hold <= time; hold++){
                if((time-hold)*hold > distance){
                    result *= (time - 2*hold + 1);
                    break;
                    
                }
            }
            // Equation for the above loop : -hold^2 + (time)*hold - distance > 0
            // hold^2 - (time)*hold + distance <= 0
            // Using quadratic formula to find the roots
        }
        cout << "Part 1: " << result << "\n";
    }

    void part2(){
        long long time = times[0],distance = distances[0];
        int result;
        for(int i = 1; i < times.size(); i++){
            time *= pow(10, floor(log10(times[i]) + 1));
            time += times[i];
            distance *= pow(10, floor(log10(distances[i]) + 1));
            distance += distances[i];
        }
        for(int hold = 1; hold <= time; hold++){
            if((time-hold)*hold > distance){
                result = time - 2*hold + 1;
                break;
            }
        }
        cout << "Part 2: " << result << "\n";
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