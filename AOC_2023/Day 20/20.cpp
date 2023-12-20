#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <numeric>

using namespace std;

using ll = long long;

struct ffModule{
    vector<string> rhs;
    bool state;
};

struct conModule{
    vector<string> rhs;
    unordered_map<string, bool> state;
};

struct state{
    string parent, curr;
    bool state;
};

class Solution
{
    public:
    vector<string> broadcasters;
    unordered_map<string, ffModule> flipflopmodules;
    unordered_map<string, conModule> conjunctionmodules;
    unordered_map<string, vector<string>> inputs;
    pair<ll,ll> part1result = {0,0};
    vector<int> part2result;
    unordered_set<string> visited;
    void init(ifstream& input){
        string line, token;
        while (getline(input, line)){
            if(!isalpha(line[0])){
                string mod = line.substr(1,line.find(" ")-1), rhs = line.substr(line.find(">") + 1);
                stringstream ss(rhs);
                vector<string> dest;
                while(getline(ss, token, ',')){
                    inputs[token.substr(1)].push_back(mod);
                    dest.push_back(token.substr(1));
                }
                if(line[0] == '%'){
                    flipflopmodules[mod].rhs = dest;
                    flipflopmodules[mod].state = false;
                }
                else conjunctionmodules[mod].rhs = dest;
            }
            else {
                string rhs = line.substr(line.find(">") + 1);
                stringstream ss(rhs);
                while(getline(ss, token, ','))
                    broadcasters.push_back(token.substr(1));
            }
        }
        for(auto& con : conjunctionmodules)
            for(auto& input: inputs[con.first])
                con.second.state[input] = false;
        for(auto it : inputs["zh"]) visited.insert(it);
        simulate();
    }

    void simulate(){
        int push = 0;
        while(!visited.empty() || push < 1000){
            queue<state> q;
            for(auto& b : broadcasters) q.push({"button", b, false});
            while(!q.empty()){
                state curr = q.front();
                q.pop();
                if(push < 1000){
                    if(curr.state) part1result.second++;
                    else part1result.first++;
                }
                if(!curr.state && visited.count(curr.curr)){
                    visited.erase(curr.curr);
                    part2result.push_back(push+1);
                    if(visited.empty()) return;
                }
                if(flipflopmodules.count(curr.curr)){
                    if(curr.state) continue;
                    else{
                        if(flipflopmodules[curr.curr].state){
                            flipflopmodules[curr.curr].state = false;
                            for(auto& dest : flipflopmodules[curr.curr].rhs)
                                q.push({curr.curr, dest, false});
                        }
                        else{
                            flipflopmodules[curr.curr].state = true;
                            for(auto& dest : flipflopmodules[curr.curr].rhs)
                                q.push({curr.curr, dest, true});
                        }
                    }
                }
                else if(conjunctionmodules.count(curr.curr)){
                    conjunctionmodules[curr.curr].state[curr.parent] = curr.state;
                    bool b = true;
                    for(auto& state : conjunctionmodules[curr.curr].state)
                        b &= state.second;
                    if(b){
                        for(auto& dest : conjunctionmodules[curr.curr].rhs)
                            q.push({curr.curr, dest, false});
                    }
                    else{
                        for(auto& dest : conjunctionmodules[curr.curr].rhs)
                            q.push({curr.curr, dest, true});
                    }
                }
            }
            push++;
        }
    }

    void part1(){
        cout << "Part 1: " << part1result.first*part1result.second << "\n";
    }

    void part2(){
        ll result = 1;
        for(auto& cycle : part2result) result = lcm(result, cycle);
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