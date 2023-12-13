#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <tuple>
#include <functional>

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

    void generatePossibilities(string& row, int ind, vector<string>& result, string curr){
        if(ind == row.size()){
            result.push_back(curr);
            return;
        }
        if(row[ind] == '?'){
            generatePossibilities(row, ind+1, result, curr+'#');
            generatePossibilities(row, ind+1, result, curr+'.');
        }
        else{
            generatePossibilities(row, ind+1, result, curr+row[ind]);
        }
    }

    vector<int> gettContiguousSequence(string row){
        vector<int> result;
        int count = 0;
        for(int i = 0; i < row.size(); i++){
            if(row[i] == '#'){
                count++;
            }
            else{
                if(count != 0){
                    result.push_back(count);
                    count = 0;
                }
            }
        }
        if(count != 0){
            result.push_back(count);
        }
        return result;
    }

    int numberOfArrangements(string row, vector<int> info){
        vector<string> res;
        generatePossibilities(row, 0, res, "");
        int result = 0;
        for(int i = 0; i < res.size(); i++){
            if(gettContiguousSequence(res[i]) == info){
                result++;
            }
        }
        return result;
    }

    // long long helper(string& row, vector<int>& info){
    //     function<long long(int,int,int)> numberOfArrangementsMemoized = [&](int i, int ci, int groupSize) -> long long{
    //         tuple<int, int, int> key = {i, ci, groupSize};
    //         if(dp.find(key) != dp.end()) 
    //             return dp[key];
    //         if(i == row.size())
    //             return (ci == info.size() && groupSize == 0) || (ci == info.size() - 1 && info[ci] == groupSize);
    //         long long result = 0;    
    //         if(row[i] == '.' || row[i] == '?'){
    //             if(groupSize == 0) result += numberOfArrangementsMemoized(i+1, ci, 0);
    //             else if(ci < info.size() && groupSize == info[ci]) result += numberOfArrangementsMemoized(i+1, ci+1, 0);
    //         }
    //         if(row[i] == '#' || row[i] == '?'){
    //             result += numberOfArrangementsMemoized(i+1, ci, groupSize+1);
    //         }
    //         return dp[key] = result;
    //     };
    //     return numberOfArrangementsMemoized(0, 0, 0);
    // }

    long long numberOfArrangementsMemoized(string& row, vector<int>& info, int i, int ci, int groupSize){
        tuple<int, int, int> key = {i, ci, groupSize};
        // if this state is already computed, return the result
        if(dp.count(key)){
            return dp[key];
        }
        // if we have reached the end of the string
        if(i == row.size()){
            // if we have no more damaged ContiguousSequence sizes to match and if the current group size is 0, we have found a valid arrangement
            if(ci == info.size() && groupSize == 0){
                return 1LL;
            }
            // if we are at the last damaged ContiguousSequence size and if the current group size matches the damaged ContiguousSequence size, we have found a valid arrangement
            else if((ci == info.size()-1) && info[ci] == groupSize){
                return 1LL;
            }
            // else we have not found a valid arrangement
            return 0LL;
        }
        // initialize the result to 0
        long long result = 0;
        // if the current character is a '.',
        if(row[i] == '.'){
            // if the current group size is 0, we just move to the nex index
            if(groupSize == 0){
                result += numberOfArrangementsMemoized(row, info, i+1, ci, 0);
            }
            // if the current group size is not 0 and if the current group size matches the damaged ContiguousSequence size,
            // as we need a . between 2 groups
            // we can start a new group and move forward with next index
            else if(ci < info.size() && info[ci] == groupSize){
                result += numberOfArrangementsMemoized(row, info, i+1, ci+1, 0);
            }
            // else we return 0 as we get a invalid configuration
        }
        // if the current character is a '#', we can only add it to the current group
        else if(row[i] == '#'){
            result += numberOfArrangementsMemoized(row, info, i+1, ci, groupSize+1);
        }
        // if the current character is a '?', we can either use it as '.' or '#'
        else{
            // if we use it as '.' 
            // we can only use it when there's no group or to separate 2 groups
            // anything else will result in invalid configuration
            if(groupSize == 0){
                result += numberOfArrangementsMemoized(row, info, i+1, ci, 0);
            }
            else if(ci < info.size() && info[ci] == groupSize){
                result += numberOfArrangementsMemoized(row, info, i+1, ci+1, 0);
            }
            // if we use it as '#'
            result += numberOfArrangementsMemoized(row, info, i+1, ci, groupSize+1);
        }
        // store the result in the dp map and return it
        return dp[key] = result;
    }

    void part1(){
        long long result = 0;
        for(int i = 0; i < rows.size(); i++){
            result += numberOfArrangements(rows[i], info[i]);
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
            result += numberOfArrangementsMemoized(rows[i], info[i], 0, 0, 0);
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