#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <queue>

using namespace std;

using ll = long long;

struct ffmoduleState{
    vector<string> rhs;
    bool state;
};

struct conjunctionmoduleState{
    vector<string> rhs;
    map<string, bool> state;
};

class Solution
{
    public:
    vector<string> broadcasters;
    map<string, ffmoduleState> flipflopmodules;
    map<string, conjunctionmoduleState> conjunctionmodules;
    map<string, vector<string>> inputs;
    void init(ifstream& input){
        string line;
        while (getline(input, line))
        {
            if(line.find("%") != string::npos){
                string ff = line.substr(1,line.find(" ")-1), rhs = line.substr(line.find(">") + 1);
                stringstream ss(rhs);
                string token;
                vector<string> dest;
                while(getline(ss, token, ',')){
                    inputs[token.substr(1)].push_back(ff);
                    dest.push_back(token.substr(1));
                }
                flipflopmodules[ff].rhs = dest;
                flipflopmodules[ff].state = false;
            }
            else if(line.find("&") != string::npos){
                string con = line.substr(1,line.find(" ")-1), rhs = line.substr(line.find(">") + 1);
                stringstream ss(rhs);
                string token;
                vector<string> dest;
                while(getline(ss, token, ',')){
                    inputs[token.substr(1)].push_back(con);
                    dest.push_back(token.substr(1));
                }
                conjunctionmodules[con].rhs = dest;
            }
            else {
                string rhs = line.substr(line.find(">") + 1);
                stringstream ss(rhs);
                string token;
                while(getline(ss, token, ',')){
                    broadcasters.push_back(token.substr(1));
                }
            }
        }
        for(auto& con : conjunctionmodules){
            for(auto& input: inputs[con.first]){
                con.second.state[input] = false;
            }
        }
    }

    // false implies low pulse and true implies high pulse
    pair<int,int> simulate(){
        int lp = 1+broadcasters.size(), hp = 0;
        queue<pair<string, bool>> q;
        for(auto& b : broadcasters){
            q.push({b, false});
        }
        while(!q.empty()){
            pair<string, bool> curr = q.front();
            q.pop();
            if(flipflopmodules.find(curr.first) != flipflopmodules.end()){
                if(curr.second) continue;
                else{
                    if(flipflopmodules[curr.first].state){
                        flipflopmodules[curr.first].state = false;
                        for(auto& dest : flipflopmodules[curr.first].rhs){
                            q.push({dest, false});
                            if(conjunctionmodules.find(dest) != conjunctionmodules.end()){
                                conjunctionmodules[dest].state[curr.first] = false;
                            } 
                        }
                        lp += flipflopmodules[curr.first].rhs.size();
                    }
                    else{
                        flipflopmodules[curr.first].state = true;
                        for(auto& dest : flipflopmodules[curr.first].rhs){
                            q.push({dest, true});
                            if(conjunctionmodules.find(dest) != conjunctionmodules.end()){
                                conjunctionmodules[dest].state[curr.first] = true;
                            }
                        }
                        hp += flipflopmodules[curr.first].rhs.size();
                    }
                }
            }
            else if(conjunctionmodules.find(curr.first) != conjunctionmodules.end()){
                bool b = true;
                for(auto& input : conjunctionmodules[curr.first].state){
                    b &= input.second;
                }
                if(b){
                    for(auto& dest : conjunctionmodules[curr.first].rhs){
                        q.push({dest, false});
                        if(conjunctionmodules.find(dest) != conjunctionmodules.end()){
                            conjunctionmodules[dest].state[curr.first] = false;
                        }
                    }
                    lp += conjunctionmodules[curr.first].rhs.size();
                }
                else{
                    for(auto& dest : conjunctionmodules[curr.first].rhs){
                        q.push({dest, true});
                        if(conjunctionmodules.find(dest) != conjunctionmodules.end()){
                            conjunctionmodules[dest].state[curr.first] = true;
                        }
                    }
                    hp += conjunctionmodules[curr.first].rhs.size();
                }
            }
        }
        return {lp, hp};
    }

    void part1(){
        int lp = 0, hp = 0;
        long long result;
        bool b = false;
        for(int i = 0; i < 1000;i++){
            pair<int,int> res = simulate();
            lp += res.first;
            hp += res.second;
        }
        result = lp;
        result *= hp;
        cout << "Part 1: " << result << "\n";
    }


    ll lcm(ll a, ll b){
        return (a*b)/__gcd(a,b);
    }

    bool shouldSimulate(map<string,int>& visited){
        for(auto it : visited){
            if(it.second == 0) return true;
        }
        return false;
    }

    void simulate2(map<string,int>& visited){
        int res = 0;
        while(shouldSimulate(visited)){
            queue<pair<string, bool>> q;
            for(auto& b : broadcasters){
                q.push({b, false});
            }
            while(!q.empty()){
                pair<string, bool> curr = q.front();
                q.pop();
                if(!curr.second && visited.count(curr.first)){
                    if(visited[curr.first] == 0) visited[curr.first] = res+1;
                }
                if(flipflopmodules.find(curr.first) != flipflopmodules.end()){
                    if(curr.second) continue;
                    else{
                        if(flipflopmodules[curr.first].state){
                            flipflopmodules[curr.first].state = false;
                            for(auto& dest : flipflopmodules[curr.first].rhs){
                                q.push({dest, false});
                                if(conjunctionmodules.find(dest) != conjunctionmodules.end()){
                                    conjunctionmodules[dest].state[curr.first] = false;
                                } 
                            }
                        }
                        else{
                            flipflopmodules[curr.first].state = true;
                            for(auto& dest : flipflopmodules[curr.first].rhs){
                                q.push({dest, true});
                                if(conjunctionmodules.find(dest) != conjunctionmodules.end()){
                                    conjunctionmodules[dest].state[curr.first] = true;
                                }
                            }
                        }
                    }
                }
                else if(conjunctionmodules.find(curr.first) != conjunctionmodules.end()){
                    bool b = true;
                    for(auto& input : conjunctionmodules[curr.first].state){
                        b &= input.second;
                    }
                    if(b){
                        for(auto& dest : conjunctionmodules[curr.first].rhs){
                            q.push({dest, false});
                            if(conjunctionmodules.find(dest) != conjunctionmodules.end()){
                                conjunctionmodules[dest].state[curr.first] = false;
                            }
                        }
                    }
                    else{
                        for(auto& dest : conjunctionmodules[curr.first].rhs){
                            q.push({dest, true});
                            if(conjunctionmodules.find(dest) != conjunctionmodules.end()){
                                conjunctionmodules[dest].state[curr.first] = true;
                            }
                        }
                    }
                }
            }
            res++;
        }
    }

    void part2(){
        // reset everything to false
        for(auto& ff : flipflopmodules){
            ff.second.state = false;
        }
        for(auto& con : conjunctionmodules){
            for(auto& input: inputs[con.first]){
                con.second.state[input] = false;
            }
        }
        map<string, int> visited;
        // rx should get low pulse
        // zh is immediate conjunction module of rx
        // for rx to get low pulse, all inputs to zh should be high pulse
        // and for all inputs to zh to be high pulse, all inputs to those inputs should be low pulse
        // so we need to find the lcm of all the cycles required for each input to those inputs to get low pulse
        for(auto it : inputs["zh"]){
            visited[it] = 0;
        }
        simulate2(visited);
        ll result = 1;
        for(auto it : visited){
            //cout<<it.second<<" ";
            result = lcm(result, it.second);
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