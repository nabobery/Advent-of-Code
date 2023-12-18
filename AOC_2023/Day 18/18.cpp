#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <unordered_map>

using ll = long long;

using namespace std;

struct digplan
{
    char dir;
    int dist;
    string color;
};

class Solution
{
    public:
    vector<digplan> plans;
    // 0 - right, 1 - down, 2 - left, 3 - up
    pair<int,int> dirs[4] = {{0,1},{1,0},{0,-1},{-1,0}};
    unordered_map<char, int> char_to_dir = {{'R', 0}, {'D', 1}, {'L', 2}, {'U', 3}};
    void init(ifstream& input){
        string line;
        regex pattern(R"(([A-Z])\s+(\d+)\s+\((#[0-9a-f]+)\))");
        while (getline(input, line))
        {
            smatch matches;
            if (regex_search(line, matches, pattern)){
                digplan plan;
                plan.dir = matches[1].str()[0];
                plan.dist = stoi(matches[2].str());
                plan.color = matches[3].str();
                plans.push_back(plan);
            }
        }
    }

    int gethexadecimal(string color){
        int result = 0;
        for(int i = 1; i < 6; i++){
            if(color[i] >= '0' && color[i] <= '9')
                result = result*16 + (color[i] - '0');
            else
                result = result*16 + (color[i] - 'a' + 10);
        }
        return result;
    }

    // We need to find number of points inside the polygon + number of points on the boundary
    // We can use Pick's theorem to find the number of points inside the polygon i = A - b/2 + 1
    // resut = i +b = A - b/2 + 1 + b = A + b/2 + 1
    ll helper(bool part2){
        ll area = 0, boundarylength = 0;
        pair<ll,ll> curr = {0,0}, prev = {0,0};
        for(int i = 0; i < plans.size(); i++){ 
            int dir, dist;
            if(part2){
                dir = plans[i].color[plans[i].color.size()-1] - '0';
                dist = gethexadecimal(plans[i].color);
            }
            else {
                dir = char_to_dir[plans[i].dir];
                dist = plans[i].dist;
            }
            boundarylength += dist;
            curr = {prev.first + dist*dirs[dir].first, prev.second + dist*dirs[dir].second};
            area += (prev.first*curr.second - curr.first*prev.second);
            prev = curr;
        }
        area = abs(area)/2;
        return (area + boundarylength/2 + 1);
    }

    void part1(){
        cout<<"Part 1: "<<helper(false)<<"\n";
    }

    void part2(){
        cout<<"Part 2: "<<helper(true)<<"\n";
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