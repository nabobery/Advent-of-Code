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
    // x, m, a, s;
    int xmas[4];
};

struct workflow{
    int cond, value;
    string result;
};

class Solution
{
    public:
    vector<partRatings> parts;
    map<string, vector<workflow>> rules;
    map<string, int> operationmapping = {
        {"x>", 0},
        {"x<", 1},
        {"m>", 2},
        {"m<", 3},
        {"a>", 4},
        {"a<", 5},
        {"s>", 6},
        {"s<", 7},
    };
    void init(ifstream& input){
        string line;
        regex partRatingspattern(R"(\d+)");
        regex workflowpattern(R"((\w+)\{(.*)\})");
        smatch match;
        while (getline(input, line))
        {
            if(line.find(":") != string::npos){
                if(regex_search(line, match, workflowpattern)){
                    string key = match[1].str();
                    string wfsstr = match[2].str();
                    vector<workflow> wfs;
                    stringstream ss(wfsstr);
                    string wf;
                    while (getline(ss, wf, ','))
                    {
                        workflow w;
                        w.cond = 8;
                        w.value = -1;
                        if(wf.find(":") != string::npos){
                            int ind1 = wf.find(":");
                            for(auto it : operationmapping){
                                if(wf.find(it.first) != string::npos){
                                    int ind2 = wf.find(it.first);
                                    w.cond = it.second;
                                    w.value = stoi(wf.substr(ind2+2, ind1-ind2-2));
                                    w.result = wf.substr(ind1+1);
                                    break;
                                }
                            }
                        }
                        else{
                            w.result = wf;
                        }
                        wfs.push_back(w);
                    }
                    rules[key] = wfs;
                }
            }
            else{
                partRatings pr;
                int i = 0;
                while (regex_search(line, match, partRatingspattern))
                {
                    pr.xmas[i] = stoi(match.str());
                    line = match.suffix();
                    i++;
                }
                if(i != 0) parts.push_back(pr);
            }
        }
    }

    bool isPartAccepted(partRatings pr){
        string workflowCurr = "in";
        while(workflowCurr != "R" && workflowCurr != "A"){
            for(auto it : rules[workflowCurr]){
                if(it.cond == 8){
                    workflowCurr = it.result;
                    break;
                }
                else{
                    if(it.cond == 0){
                        if(pr.xmas[0] > it.value){
                            workflowCurr = it.result;
                            break;
                        }
                    }
                    else if(it.cond == 1){
                        if(pr.xmas[0] < it.value){
                            workflowCurr = it.result;
                            break;
                        }
                    }
                    else if(it.cond == 2){
                        if(pr.xmas[1] > it.value){
                            workflowCurr = it.result;
                            break;
                        }
                    }
                    else if(it.cond == 3){
                        if(pr.xmas[1] < it.value){
                            workflowCurr = it.result;
                            break;
                        }
                    }
                    else if(it.cond == 4){
                        if(pr.xmas[2] > it.value){
                            workflowCurr = it.result;
                            break;
                        }
                    }
                    else if(it.cond == 5){
                        if(pr.xmas[2] < it.value){
                            workflowCurr = it.result;
                            break;
                        }
                    }
                    else if(it.cond == 6){
                        if(pr.xmas[3] > it.value){
                            workflowCurr = it.result;
                            break;
                        }
                    }
                    else if(it.cond == 7){
                        if(pr.xmas[3] < it.value){
                            workflowCurr = it.result;
                            break;
                        }
                    }
                }
            }
        }
        return workflowCurr == "A";
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

    ll getCombinations(vector<pair<int,int>>& intervals){
        ll result = 1;
        for(int i = 0; i < 4;i++){
            result *= intervals[i].second - intervals[i].first + 1;
        }
        return result;
    }

    ll helper(vector<pair<int,int>> intervals, string workflowCurr){
        ll result = 0;
        for(auto it : rules[workflowCurr]){
            if(it.cond == 8){
                if(it.result == "A"){
                    result += getCombinations(intervals);
                }
                else if(it.result != "R"){
                    result += helper(intervals, it.result);
                }
            }
            else{
                if(it.cond == 0){
                    if(it.value < intervals[0].second){
                        int temp = intervals[0].first;
                        intervals[0].first = max(intervals[0].first, it.value+1);
                        if(it.result == "A"){
                            result += getCombinations(intervals);
                        }
                        else if(it.result != "R"){
                            result += helper(intervals, it.result);
                        }
                        intervals[0].first = temp;
                        intervals[0].second = min(intervals[0].second, it.value);
                    }
                }
                else if(it.cond == 1){
                    if(it.value > intervals[0].first){
                        int temp = intervals[0].second;
                        intervals[0].second = min(intervals[0].second, it.value-1);
                        if(it.result == "A"){
                            result += getCombinations(intervals);
                        }
                        else if(it.result != "R"){
                            result += helper(intervals, it.result);
                        }
                        intervals[0].second = temp;
                        intervals[0].first = max(intervals[0].first, it.value);
                    }
                }
                else if(it.cond == 2){
                    if(it.value < intervals[1].second){
                        int temp = intervals[1].first;
                        intervals[1].first = max(intervals[1].first, it.value+1);
                        if(it.result == "A"){
                            result += getCombinations(intervals);
                        }
                        else if(it.result != "R"){
                            result += helper(intervals, it.result);
                        }
                        intervals[1].first = temp;
                        intervals[1].second = min(intervals[1].second, it.value);
                    }
                }
                else if(it.cond == 3){
                    if(it.value > intervals[1].first){
                        int temp = intervals[1].second;
                        intervals[1].second = min(intervals[1].second, it.value-1);
                        if(it.result == "A"){
                            result += getCombinations(intervals);
                        }
                        else if(it.result != "R"){
                            result += helper(intervals, it.result);
                        }
                        intervals[1].second = temp;
                        intervals[1].first = max(intervals[1].first, it.value);
                    }
                }
                else if(it.cond == 4){
                    if(it.value < intervals[2].second){
                        int temp = intervals[2].first;
                        intervals[2].first = max(intervals[2].first, it.value+1);
                        if(it.result == "A"){
                            result += getCombinations(intervals);
                        }
                        else if(it.result != "R"){
                            result += helper(intervals, it.result);
                        }
                        intervals[2].first = temp;
                        intervals[2].second = min(intervals[2].second, it.value);
                    }
                }
                else if(it.cond == 5){
                    if(it.value > intervals[2].first){
                        int temp = intervals[2].second;
                        intervals[2].second = min(intervals[2].second, it.value-1);
                        if(it.result == "A"){
                            result += getCombinations(intervals);
                        }
                        else if(it.result != "R"){
                            result += helper(intervals, it.result);
                        }
                        intervals[2].second = temp;
                        intervals[2].first = max(intervals[2].first, it.value);
                    }
                }
                else if(it.cond == 6){
                    if(it.value < intervals[3].second){
                        int temp = intervals[3].first;
                        intervals[3].first = max(intervals[3].first, it.value+1);
                        if(it.result == "A"){
                            result += getCombinations(intervals);
                        }
                        else if(it.result != "R"){
                            result += helper(intervals, it.result);
                        }
                        intervals[3].first = temp;
                        intervals[3].second = min(intervals[3].second, it.value);
                    }
                }
                else if(it.cond == 7){
                    if(it.value > intervals[3].first){
                        int temp = intervals[3].second;
                        intervals[3].second = min(intervals[3].second, it.value-1);
                        if(it.result == "A"){
                            result += getCombinations(intervals);
                        }
                        else if(it.result != "R"){
                            result += helper(intervals, it.result);
                        }
                        intervals[3].second = temp;
                        intervals[3].first = max(intervals[3].first, it.value);
                    }
                }
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