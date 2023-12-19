#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <regex>

using namespace std;

using ll = long long;

struct partRatings{
    int xmas[4];
};

class Solution
{
    public:
    vector<partRatings> parts;
    map<string, vector<string>> rules;
    map<char, int> chartoIndex = {{'x', 0}, {'m', 1}, {'a', 2}, {'s', 3}};
    void init(ifstream& input){
        string line;
        regex partRatingspattern(R"(\d+)");
        regex workflowpattern(R"((\w+)\{(.*)\})");
        smatch match;
        while (getline(input, line)){
            if(line.find(":") != string::npos){
                if(regex_search(line, match, workflowpattern)){
                    string key = match[1].str();
                    string wfsstr = match[2].str();
                    vector<string> flows;
                    stringstream ss(wfsstr);
                    string workflow;
                    while (getline(ss, workflow, ','))
                        flows.push_back(workflow);
                    rules[key] = flows;
                }
            }
            else{
                partRatings pr;
                int i = 0;
                while (regex_search(line, match, partRatingspattern)){
                    pr.xmas[i] = stoi(match.str());
                    line = match.suffix();
                    i++;
                }
                if(i != 0) parts.push_back(pr);
            }
        }
    }

    bool isPartAccepted(partRatings& pr){
        string curr = "in";
        while(curr != "R" && curr != "A"){
            for(auto flow : rules[curr]){
                if(flow.find(":") != string::npos){
                    bool b = false;
                    string next = flow.substr(flow.find(":")+1), expr = flow.substr(0, flow.find(":"));
                    if(expr.find("<") != string::npos){
                        for(auto it : chartoIndex){
                            if(expr.find(it.first) != string::npos){
                                if(pr.xmas[it.second] < stoi(expr.substr(expr.find("<")+1))){
                                    curr = next;
                                    b = true;
                                    break;
                                }
                            }
                        }
                    }
                    else if(expr.find(">") != string::npos){
                        for(auto it : chartoIndex){
                            if(expr.find(it.first) != string::npos){
                                if(pr.xmas[it.second] > stoi(expr.substr(expr.find(">")+1))){
                                    curr = next;
                                    b = true;
                                    break;
                                }
                            }
                        }
                    }
                    if(b) break;
                }
                else{
                    curr = flow;
                    break;
                }
            }
        }
        return curr == "A";
    }

    void part1(){
        ll result = 0;
        for(auto it : parts){
            if(isPartAccepted(it)){
                for(int i = 0; i < 4; i++){
                    result += it.xmas[i];
                }
            }
        }
        cout << "Part 1: " << result << "\n";
    }

    ll getCombinations(vector<pair<int,int>>& xmasIntervals){
        ll result = 1;
        for(auto it : xmasIntervals){
            result *= it.second - it.first + 1;
        }
        return result;
    }

    ll helper(vector<pair<int,int>> xmasIntervals, string workflowCurr){
        ll result = 0;
        for(auto flow : rules[workflowCurr]){
            if(flow.find(":") != string::npos){
                string next = flow.substr(flow.find(":")+1), expr = flow.substr(0, flow.find(":"));
                vector<pair<int,int>> xmasIntervalsNext = xmasIntervals;
                // x < val if true => x [1, val-1] else x [val, 4000] so we update the interval accordingly
                if(expr.find("<") != string::npos){
                    for(auto it : chartoIndex){
                        if(expr.find(it.first) != string::npos){
                            int val = stoi(expr.substr(expr.find("<")+1));
                            if(val > xmasIntervalsNext[it.second].first){
                                xmasIntervalsNext[it.second].second = min(xmasIntervalsNext[it.second].second, val-1);
                                if(next == "A") result += getCombinations(xmasIntervalsNext);
                                else if(next != "R") result += helper(xmasIntervalsNext, next);
                                xmasIntervals[it.second].first = max(xmasIntervals[it.second].first, val);
                            }
                        }
                    
                    }
                }
                else if(expr.find(">") != string::npos){
                    for(auto it : chartoIndex){
                        if(expr.find(it.first) != string::npos){
                            int val = stoi(expr.substr(expr.find(">")+1));
                            if(val < xmasIntervalsNext[it.second].second){
                                xmasIntervalsNext[it.second].first = max(xmasIntervalsNext[it.second].first, val+1);
                                if(next == "A") result += getCombinations(xmasIntervalsNext);
                                else if(next != "R") result += helper(xmasIntervalsNext, next);
                                xmasIntervals[it.second].second = min(xmasIntervals[it.second].second, val);
                            }
                        }
                    }
                }
            }
            else{
                if(flow == "A") result += getCombinations(xmasIntervals);
                else if(flow != "R") result += helper(xmasIntervals, flow);
            }
        }
        return result;
    }

    void part2(){
        vector<pair<int,int>> xmasIntervals = {{1, 4000}, {1, 4000}, {1, 4000}, {1, 4000}};
        string workflowCurr = "in";
        cout << "Part 2: " << helper(xmasIntervals, workflowCurr) << "\n";
    }
};

int main(){
    ifstream in("input.txt");
    if (!in.is_open()){
        cerr << "Could not open the file - '" << "\n";
        return EXIT_FAILURE;
    }
    Solution s;
    s.init(in);
    s.part1();
    s.part2();
    return 0;
}