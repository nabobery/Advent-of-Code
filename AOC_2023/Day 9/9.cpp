#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Solution
{
    public:
    vector<vector<int>> histories;
    void init(ifstream& input){
        string line;
        while (getline(input, line))
        {
            int num;
            stringstream ss(line);
            vector<int> history;
            while (ss >> num)
            {
                history.push_back(num);
            }
            histories.push_back(history);
        }
    }
    bool allZeroes(vector<int> history){
        for (auto num : history)
        {
            if (num != 0)
            {
                return false;
            }
        }
        return true;
    }
    int extrapolatedHistory(vector<int> history){
        int result = 0;
        while(!allZeroes(history)){
            vector<int> temp;
            for(int i = 1;i < history.size();i++){
                temp.push_back(history[i] - history[i-1]);
            }
            result += history.back();
            history = temp;
        }
        return result;
    }

    int extrapolatedHistoryBeginning(vector<int> history){
        int result = history[0], cnt = 1;
        while(!allZeroes(history)){
            vector<int> temp;
            for(int i = 1;i < history.size();i++){
                temp.push_back(history[i] - history[i-1]);
            }
            // beginning.push_back(history[0]);
            history = temp;
            if(cnt%2) result -= history[0];
            else result += history[0];
            cnt++;
        }
        // int result = 0;
        // for(int i = beginning.size()-1;i >= 0;i--){
        //     result = beginning[i] - result;
        // }
        return result;
    }

    void part1(){
        int result = 0;
        for (auto history : histories)
        {
            result += extrapolatedHistory(history);
        }
        cout<<"Part 1: "<<result<<"\n";
    }
    void part2(){
        int result = 0;
        for (auto history : histories)
        {
            result += extrapolatedHistoryBeginning(history);
        }
        cout<<"Part 1: "<<result<<"\n";
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