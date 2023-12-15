#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Solution
{
    public:
    vector<pair<string, string>> packets;
    void init(ifstream& input){
        string line;
        while (getline(input, line))
        {
            string temp = line;
            getline(input, line);
            packets.push_back({temp, line});
            getline(input, line);
        }
    }

    vector<string> helper(string s){
        vector<string> result;
        string temp = "";
        int depth = 0;
        for(int i = 1; i < s.size()-1; i++){
            if(s[i] == '['){
                depth++;
            }
            else if(s[i] == ']'){
                depth--;
            }
            else if(s[i] == ','){
                if(depth == 0){
                    result.push_back(temp);
                    temp = "";
                }
            }
            if(s[i] != ',' || (s[i] == ',' && depth != 0)){
                temp += s[i];
            }
        }
        if(temp != "") result.push_back(temp);
        return result;
    }

    int compare(vector<string> left, vector<string> right){
        for(int i = 0; i < min(left.size(), right.size()); i++){
            // if both left and right are integers
            if(left[i][0] != '[' && right[i][0] != '['){
                if(stoi(left[i]) < stoi(right[i])){
                    return -1;
                }
                else if(stoi(left[i]) > stoi(right[i])){
                    return 1;
                }
            }
            // else if left is a list
            else if(left[i][0] == '[' && right[i][0] != '['){
                int temp = compare(helper(left[i]), helper( "[" + right[i] + "]"));
                if(temp != 0){
                    return temp;
                }
            }
            // else if right is a list
            else if(left[i][0] != '[' && right[i][0] == '['){
                int temp = compare(helper("[" + left[i] + "]"), helper(right[i]));
                if(temp != 0){
                    return temp;
                }
            }
            // else if both left and right are lists
            else{
                int temp = compare(helper(left[i]), helper(right[i]));
                if(temp != 0){
                    return temp;
                }
            }
        }
        return left.size() - right.size();
    }

    void part1(){
        int result = 0;
        for(int i = 0; i < packets.size();i++){
            if(compare(helper(packets[i].first), helper(packets[i].second)) < 0){
                result += (i+1);
            }
        }
        cout<<"Part1: "<<result<<"\n";
    }

    void part2(){
        vector<vector<string>> allPackets;
        for(auto it : packets){
            allPackets.push_back(helper(it.first));
            allPackets.push_back(helper(it.second));
        }
        // adding divider packets 
        allPackets.push_back(helper("[[2]]"));
        allPackets.push_back(helper("[[6]]"));
        // sorting all packets based on the compare function
        sort(allPackets.begin(), allPackets.end(), [&](vector<string> left, vector<string> right){
            return compare(left, right) < 0;
        });
        int result = 1;
        for(int i = 0; i < allPackets.size(); i++){
            if(allPackets[i].empty()) continue;
            if((allPackets[i][0] == "[2]" || allPackets[i][0] == "[6]") && allPackets[i].size() == 1){
                result *= (i+1);
            }
        }
        cout<<"Part2: "<<result<<"\n";
    }

};

int main(){
    ifstream in("input.txt");
    if (!in.is_open())
    {
        cerr << "Could not open the file - '" << "\n";
        return EXIT_FAILURE;
    }
    Solution s;
    s.init(in);
    s.part1();
    s.part2();
    return 0;
}