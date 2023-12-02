#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

class Solution
{
    public:
    vector<string> input;
    void init(ifstream& input){
        string line;
        while (getline(input, line))
        {
            this->input.push_back(line);
        }
    }
    void part1(){
        int result = 0;
        for(auto str : input){
            for(auto ch : str){
                if(isdigit(ch)){
                    result += 10*(ch - '0');
                    break;
                }
            }
            for(int i = str.size()-1;i >= 0;i--){
                if(isdigit(str[i])){
                    result += (str[i] - '0');
                    break;
                }
            }
        }
        cout<<"Part1: "<<result<<"\n";
    }
    void part2(){
        int result = 0;
        unordered_map<string, int> m = {{"one", 1}, {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5}, {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9}};
        unordered_map<char, vector<string>> start = {{'o' ,{"one"}}, {'t', {"two", "three"}}, {'f', {"four"}}, {'s', {"six", "seven"}}, {'e', {"eight"}}, {'n', {"nine"}}};
        unordered_map<char, vector<string>> finish = {{'e', {"one", "three", "five", "nine"}}, {'o', {"two"}}, {'x', {"six"}}, {'n', {"seven"}}, {'t', {"eight"}}, {'n', {"seven"}} , {'r', {"four"}}};
        for(auto str : input){
            for(int i = 0; i < str.size();i++){
                if(start.count(str[i])){
                    int temp = -1;
                    for(auto s : start[str[i]]){
                        if(i + s.size() > str.size())
                            continue;
                        if(m.count(str.substr(i, s.size()))){
                            temp = 10*m[str.substr(i, s.size())];
                            break;
                        }
                    }
                    if(temp != -1){
                        result += temp;
                        break;
                    }
                }
                else if(isdigit(str[i])){
                    result += 10*(str[i] - '0');
                    break;
                }
            }
            for(int i = str.size()-1;i >= 0;i--){
                if(finish.count(str[i])){
                    int temp = -1;
                    for(auto s : finish[str[i]]){
                        if(i - (int)s.size() < 0)
                            continue;
                        if(m.count(str.substr(i - s.size() + 1, s.size()))){
                            temp = m[str.substr(i - s.size() + 1, s.size())];
                            break;
                        }
                    }
                    if(temp != -1){
                        result += temp;
                        break;
                    }
                }
                else if(isdigit(str[i])){
                    result += (str[i] - '0');
                    break;
                }
            }
        }
        cout<<"Part2: "<<result<<"\n";
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