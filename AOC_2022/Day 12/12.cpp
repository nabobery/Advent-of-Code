#include <bits/stdc++.h>
using namespace std;

class Solution
{
    public:
        vector<string> grid;
        pair<int,int> start,finish;
        void init(ifstream& input){
            string line;
            while(getline(input,line)){
                grid.push_back(line);
            }
            for(int i = 0; i < grid.size();i++){
                for(int j = 0; j < grid[i].size();j++){
                    if(grid[i][j] == 'S'){
                        start.first = i;
                        start.second = j;
                    }
                }
            }
        }
        bool withinBoundary(int n, int m, pair<int,int> p){
            if((p.first >= 0 && p.first < n) && (p.second >= 0 && p.second < m))
                return true;
            return false;
        }
        void part1(){
            queue<pair<int,int>> q;
            q.push(start);
            grid[start.first][start.second] = 'a';
            vector<vector<int>> dp(grid.size(),vector<int>(grid[0].size(),-1));
            dp[start.first][start.second] = 0;
            while(!q.empty()){
                pair<int,int> curr = q.front();
                q.pop();
                if(grid[curr.first][curr.second] == 'E'){
                    finish = make_pair(curr.first,curr.second);
                    cout<<"Part1: "<<dp[curr.first][curr.second]<<"\n";
                    return;
                }
                char ch = grid[curr.first][curr.second] + 1;
                if(withinBoundary(grid.size(),grid[0].size(),make_pair(curr.first+1,curr.second))){
                    if(grid[curr.first+1][curr.second] == 'E'){
                        if(grid[curr.first][curr.second] >= 'y' && dp[curr.first+1][curr.second] == -1){
                            q.push(make_pair(curr.first+1,curr.second));
                            dp[curr.first+1][curr.second] = dp[curr.first][curr.second]+1;
                        }
                    }
                    else{
                        if(grid[curr.first+1][curr.second] <= ch && dp[curr.first+1][curr.second] == -1){
                            q.push(make_pair(curr.first+1,curr.second));
                            dp[curr.first+1][curr.second] = dp[curr.first][curr.second]+1;
                        }
                    }
                }      
                if(withinBoundary(grid.size(),grid[0].size(),make_pair(curr.first-1,curr.second))){
                    if(grid[curr.first-1][curr.second] == 'E'){
                        if(grid[curr.first][curr.second] >= 'y' && dp[curr.first-1][curr.second] == -1){
                            q.push(make_pair(curr.first-1,curr.second));
                            dp[curr.first-1][curr.second] = dp[curr.first][curr.second]+1;
                        }
                    }
                    else{
                        if(grid[curr.first-1][curr.second] <= ch && dp[curr.first-1][curr.second] == -1){
                            q.push(make_pair(curr.first-1,curr.second));
                            dp[curr.first-1][curr.second] = dp[curr.first][curr.second]+1;
                        }
                    }
                }
                if(withinBoundary(grid.size(),grid[0].size(),make_pair(curr.first,curr.second+1))){
                    if(grid[curr.first][curr.second+1] == 'E'){
                        if(grid[curr.first][curr.second] >= 'y' && dp[curr.first][curr.second+1] == -1){
                            q.push(make_pair(curr.first,curr.second+1));
                            dp[curr.first][curr.second+1] = dp[curr.first][curr.second]+1;
                        }
                    }
                    else{
                        if(grid[curr.first][curr.second+1] <= ch && dp[curr.first][curr.second+1] == -1){
                            q.push(make_pair(curr.first,curr.second+1));
                            dp[curr.first][curr.second+1] = dp[curr.first][curr.second]+1;
                        }
                    }
                }
                if(withinBoundary(grid.size(),grid[0].size(),make_pair(curr.first,curr.second-1))){
                    if(grid[curr.first][curr.second-1] == 'E'){
                        if(grid[curr.first][curr.second] >= 'y' && dp[curr.first][curr.second-1] == -1){
                            q.push(make_pair(curr.first,curr.second-1));
                            dp[curr.first][curr.second-1] = dp[curr.first][curr.second]+1;
                        }
                    }
                    else{
                        if(grid[curr.first][curr.second-1] <= ch && dp[curr.first][curr.second-1] == -1){
                            q.push(make_pair(curr.first,curr.second-1));
                            dp[curr.first][curr.second-1] = dp[curr.first][curr.second]+1;
                        }
                    }
                }  
            }
        }
        void part2(){
            queue<pair<int,int>> q;
            q.push(finish);
            grid[finish.first][finish.second] = 'z';
            vector<vector<int>> dp(grid.size(),vector<int>(grid[0].size(),-1));
            dp[finish.first][finish.second] = 0;
            while(!q.empty()){
                pair<int,int> curr = q.front();
                q.pop();
                if(grid[curr.first][curr.second] == 'a'){
                    cout<<"Part2: "<<dp[curr.first][curr.second]<<"\n";
                    return;
                }
                char ch = grid[curr.first][curr.second]-1;
                if(withinBoundary(grid.size(),grid[0].size(),make_pair(curr.first+1,curr.second))){
                    if(grid[curr.first+1][curr.second] == 'a'){
                        if(grid[curr.first][curr.second] <= 'b' && dp[curr.first+1][curr.second] == -1){
                            q.push(make_pair(curr.first+1,curr.second));
                            dp[curr.first+1][curr.second] = dp[curr.first][curr.second]+1;
                        }
                    }
                    else{
                        if((grid[curr.first+1][curr.second] >= ch) && dp[curr.first+1][curr.second] == -1){
                            q.push(make_pair(curr.first+1,curr.second));
                            dp[curr.first+1][curr.second] = dp[curr.first][curr.second]+1;
                        }
                    }
                    
                }      
                if(withinBoundary(grid.size(),grid[0].size(),make_pair(curr.first-1,curr.second))){
                    if(grid[curr.first-1][curr.second] == 'a'){
                        if(grid[curr.first][curr.second] <= 'b' && dp[curr.first-1][curr.second] == -1){
                            q.push(make_pair(curr.first-1,curr.second));
                            dp[curr.first-1][curr.second] = dp[curr.first][curr.second]+1;
                        }
                    }
                    else{
                        if(grid[curr.first-1][curr.second] >= ch && dp[curr.first-1][curr.second] == -1){
                            q.push(make_pair(curr.first-1,curr.second));
                            dp[curr.first-1][curr.second] = dp[curr.first][curr.second]+1;
                        }
                    }   
                }
                if(withinBoundary(grid.size(),grid[0].size(),make_pair(curr.first,curr.second+1))){
                    if(grid[curr.first][curr.second+1] == 'a'){
                        if(grid[curr.first][curr.second] <= 'b' && dp[curr.first][curr.second+1] == -1){
                            q.push(make_pair(curr.first,curr.second+1));
                            dp[curr.first][curr.second+1] = dp[curr.first][curr.second]+1;
                        }
                    }
                    else{
                        if(grid[curr.first][curr.second+1] >= ch && dp[curr.first][curr.second+1] == -1){
                            q.push(make_pair(curr.first,curr.second+1));
                            dp[curr.first][curr.second+1] = dp[curr.first][curr.second]+1;
                        }
                    }
                }
                if(withinBoundary(grid.size(),grid[0].size(),make_pair(curr.first,curr.second-1))){
                    if(grid[curr.first][curr.second-1] == 'a'){
                        if(grid[curr.first][curr.second] <= 'b' && dp[curr.first][curr.second-1] == -1){
                            q.push(make_pair(curr.first,curr.second-1));
                            dp[curr.first][curr.second-1] = dp[curr.first][curr.second]+1;
                        }
                    }
                    else{
                        if(grid[curr.first][curr.second-1] >= ch && dp[curr.first][curr.second-1] == -1){
                            q.push(make_pair(curr.first,curr.second-1));
                            dp[curr.first][curr.second-1] = dp[curr.first][curr.second]+1;
                        }
                    }
                }  
            }
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
