#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <tuple>
#include <unordered_map>

using namespace std;

struct hash_tuple {
    template <class T1, class T2, class T3>
    size_t operator()(const tuple<T1, T2, T3>& p) const
    {
        auto hash1 = hash<T1>{}(get<0>(p));
        auto hash2 = hash<T2>{}(get<1>(p));
        auto hash3 = hash<T3>{}(get<2>(p));
        return hash1 ^ hash2 ^ hash3;
    }
};

class Solution
{
    public:
    vector<string> rows;
    vector<vector<int>> info;
    unordered_map<tuple<int, int , int>, long long, hash_tuple> dp;
    void init(ifstream& input){
        string line;
        while (getline(input, line))
        {
            rows.push_back(line.substr(0, line.find(" ")));
            string record = line.substr(line.find(" ")+1);
            vector<int> temp;
            stringstream ss(record);
            string token;
            while (getline(ss, token, ','))
            {
                temp.push_back(stoi(token));
            }
            info.push_back(temp);
        }
    }

    long long helper(string& row, vector<int>& info, int i, int ci, int groupSize){
        tuple<int, int, int> key = {i, ci, groupSize};
        if(dp.find(key) != dp.end()) 
            return dp[key];
        if(i == row.size())
            return (ci == info.size() && groupSize == 0) || (ci == info.size() - 1 && info[ci] == groupSize);
        long long result = 0;    
        if(row[i] == '.' || row[i] == '?'){
            if(groupSize == 0) result += helper(row, info, i+1, ci, 0);
            else if(ci < info.size() && groupSize == info[ci]) result += helper(row, info, i+1, ci+1, 0);
        }
        if(row[i] == '#' || row[i] == '?'){
            result += helper(row, info, i+1, ci, groupSize+1);
        }
        return dp[key] = result;
    }

    void part1(){
        long long result = 0;
        for(int i = 0; i < rows.size(); i++){
            dp.clear();
            result += helper(rows[i], info[i], 0,0,0);
        }
        cout<<"Part 1: " << result << "\n";
    }

    void part2(){
        long long result = 0;
        for(int i = 0; i < rows.size(); i++){
            string row = rows[i];
            vector<int> record = info[i];
            for(int j = 1; j < 5;j++){
                rows[i] += "?";
                rows[i] += row;
                info[i].insert(info[i].end(), record.begin(), record.end());
            }
            dp.clear();
            result += helper(rows[i], info[i], 0, 0, 0);
        }
        cout<<"Part 2: " << result << "\n";
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