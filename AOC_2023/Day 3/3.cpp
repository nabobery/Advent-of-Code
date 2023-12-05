#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <map>

using namespace std;

struct Number{
    int start_x, end_x, y, value;
};

class Solution
{
    public:
    pair<int,int> dir[8] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1,1}, {1, -1}, {-1, 1}, {-1, -1}};
    vector<string> input;
    void init(ifstream& input){
        string line;
        while (getline(input, line))
        {
            this->input.push_back(line);
        }
    }

    bool isSymbol(char c){
        return !isdigit(c) && c != '.';
    }

    void part1(){
        int res = 0;
        set<pair<int, int>> symbol_pos;
        for(int i = 0; i < input.size(); i++){
            for(int j = 0; j < input[i].size(); j++){
                if(isSymbol(input[i][j])){
                    symbol_pos.insert({i, j});
                }
            }
        }
        vector<Number> numbers;
        for(int i = 0; i < input.size(); i++){
            for(int j = 0; j < input[i].size(); j++){
                if(isdigit(input[i][j])){
                    int k = j;
                    while(k < input[i].size() && isdigit(input[i][k])){
                        k++;
                    }
                    int value = stoi(input[i].substr(j, k - j));
                    numbers.push_back({j, k-1, i, value});
                    j = k;
                }
            }
        }
        for(int i = 0; i < numbers.size(); i++){
            bool b = false;
            for(int x = numbers[i].start_x; x <= numbers[i].end_x; x++){
                int y = numbers[i].y;
                for(int j = 0; j < 8; j++){
                    if(symbol_pos.count({y + dir[j].second, x+dir[j].first})){
                        b = true;
                        break;
                    }
                }
                if(b){
                    res += numbers[i].value;
                    break;
                }
            }
        }
        cout<<"Part 1: "<<res<<"\n";
    }

    void part2(){
        long long res = 0;
        set<pair<int, int>> symbol_pos;
        for(int i = 0; i < input.size(); i++){
            for(int j = 0; j < input[i].size(); j++){
                if(isSymbol(input[i][j]) && input[i][j] == '*'){
                    symbol_pos.insert({i, j});
                }
            }
        }
        map<pair<int, int>, vector<Number>> gears;
        vector<Number> numbers;
        for(int i = 0; i < input.size(); i++){
            for(int j = 0; j < input[i].size(); j++){
                if(isdigit(input[i][j])){
                    int k = j;
                    while(k < input[i].size() && isdigit(input[i][k])){
                        k++;
                    }
                    int value = stoi(input[i].substr(j, k - j));
                    numbers.push_back({j, k-1, i, value});
                    j = k;
                }
            }
        }
        for(int i = 0; i < numbers.size(); i++){
            bool b = false;
            for(int x = numbers[i].start_x; x <= numbers[i].end_x; x++){
                int y = numbers[i].y;
                for(int j = 0; j < 8; j++){
                    if(symbol_pos.count({y + dir[j].second, x+dir[j].first})){
                        b = true;
                        gears[{y + dir[j].second, x+dir[j].first}].push_back(numbers[i]);
                        break;
                    }
                }
                if(b){
                    break;
                }
            }
        }
        for(auto& [pos, nums] : gears){
            if(nums.size() == 2){
                res += nums[0].value * nums[1].value;
            }
        }
        cout<<"Part 2: "<<res<<"\n";
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
