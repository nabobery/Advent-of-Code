#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Solution
{
    public:
    vector<vector<string>> patterns;
    void init(ifstream& input){
        string line;
        vector<string> vec;
        while (getline(input, line))
        {
            if(line.empty()){
                patterns.push_back(vec);
                vec.clear();
                continue;
            }
            vec.push_back(line);
        }
        patterns.push_back(vec);
    }

    pair<bool,int> isHorizontal(vector<string> vec, bool part2){
        int n = vec.size(), smudges = 0;
        for(int i = 0; i < n-1;i++){
            smudges = 0;
            int l = i, r = i+1;
            while(l >= 0 && r < n){
                for(int i = 0; i < vec[l].size();i++){
                    if(vec[l][i] != vec[r][i]){
                        smudges++;
                        if(smudges > 1){
                            break;
                        }
                    }
                }
                if(smudges > 1){
                    break;
                }
                l--;
                r++;
            }
            if((smudges == 1 && part2) || (!part2 && smudges == 0)){
                return {true, i+1};
            }
        }
        return {false, -1};
    }

    pair<bool,int> isVertical(vector<string> vec, bool part2){
        int m  = vec[0].size(), smudges = 0;
        for(int i = 0; i < m-1;i++){
            int l = i, r = i+1;
            smudges = 0;
            while(l >= 0 && r < m){
                for(int j = 0; j < vec.size();j++){
                    if(vec[j][l] != vec[j][r]){
                        smudges++;
                        if(smudges > 1){
                            break;
                        }
                    }
                }
                if(smudges > 1){
                    break;
                }
                l--;
                r++;
            }
            if((smudges == 1 && part2) || (!part2 && smudges == 0)){
                return {true, i+1};
            }
        }
        return {false, -1};
    }

    void part1(){
        int result = 0;
        for(int i = 0; i < patterns.size();i++){
            pair<bool,int> h = isHorizontal(patterns[i], false);
            if(h.first){
                result += 100*h.second;
                continue;
            }
            pair<bool,int> v = isVertical(patterns[i], false);
            if(v.first){
                result += v.second;
                continue;
            }
        }
        cout<<"Part 1: "<<result<<"\n";
    }
    void part2(){
        int result = 0;
        for(int i = 0; i < patterns.size();i++){
            pair<bool,int> h = isHorizontal(patterns[i], true);
            if(h.first){
                result += 100*h.second;
                continue;
            }
            pair<bool,int> v = isVertical(patterns[i], true);
            if(v.first){
                result += v.second;
                continue;
            }
        }
        cout<<"Part 2: "<<result<<"\n";
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