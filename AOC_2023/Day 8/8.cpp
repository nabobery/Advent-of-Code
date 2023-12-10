#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <numeric>

using namespace std;

class Solution
{
    public:
    string directions;
    map<string, pair<string,string>> nodes;
    void init(ifstream& input){
        string line;
        while (getline(input, line))
        {
            if(line.find("=") != string::npos){
                string node = line.substr(0, line.find(" ="));
                string left = line.substr(line.find("(")+1, 3);
                string right = line.substr(line.find(")")-3, 3);
                nodes[node] = make_pair(left, right);
            }
            else{
                if(directions.empty())
                    directions = line;
            }
        }

    }
    void part1(){
        int result = 0, n = directions.size();
        string start = "AAA", end = "ZZZ";
        while(start != end){
            if(directions[result%n] == 'L'){
                start = nodes[start].first;
            }
            else{
                start = nodes[start].second;
            }
            result++;
        }
        cout<<"Part 1: "<<result<<"\n";
    }
    void part2(){
        long long result = 1;
        int n = directions.size();
        vector<string> path;
        for(auto it : nodes){
            if(it.first[2] == 'A')
                path.push_back(it.first);
        }
        vector<long long> shortestPath(path.size());
        for(int i = 0; i < path.size(); i++){
            int cnt  = 0;
            while(true){
                if(directions[cnt%n] == 'L'){
                    path[i] = nodes[path[i]].first;
                }
                else{
                    path[i] = nodes[path[i]].second;
                }
                cnt++;
                if(path[i][2] == 'Z')
                    break;
            }
            shortestPath[i] = cnt;
        }
        for(auto it : shortestPath){
            result = lcm(result, it);
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