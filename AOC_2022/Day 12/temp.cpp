#include <bits/stdc++.h>
#include <regex>
using namespace std;

pair<int,int> e;

bool withinBoundary(int n, int m, pair<int,int> p){
    if((p.first >= 0 && p.first < n) && (p.second >= 0 && p.second < m))
        return true;
    return false;
}

void bfs(vector<string> grid, pair<int,int> pos){
    queue<pair<int,int>> q;
    q.push(pos);
    grid[pos.first][pos.second] = 'a';
    vector<vector<int>> dp(grid.size(),vector<int>(grid[0].size(),-1));
    dp[pos.first][pos.second] = 0;
    //int c = 0;
    while(!q.empty()){
        pair<int,int> curr = q.front();
        q.pop();
        //c++;
        //cout<<c<<"\n";
        if(grid[curr.first][curr.second] == 'E'){
            e = make_pair(curr.first,curr.second);
            cout<<dp[curr.first][curr.second]<<"\n";
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
    cout << "Not Found" << endl;
}

void bfs2(vector<string> grid, pair<int,int> pos){
    queue<pair<int,int>> q;
    q.push(pos);
    grid[pos.first][pos.second] = 'z';
    vector<vector<int>> dp(grid.size(),vector<int>(grid[0].size(),-1));
    dp[pos.first][pos.second] = 0;
    //int c = 0;
    while(!q.empty()){
        pair<int,int> curr = q.front();
        //cout<<grid[curr.first][curr.second]<<" "<<curr.first<<" "<<curr.second<<" "<<dp[curr.first][curr.second]<<"\n";
        q.pop();
        //c++;
        //cout<<c<<"\n";
        if(grid[curr.first][curr.second] == 'a'){
            cout<<dp[curr.first][curr.second]<<"\n";
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
    cout << "Not Found" << endl;
}

// void bfs3(vector<string> grid, pair<int,int> pos){
//     queue<pair<int,int>> q;
//     q.push(pos);
//     grid[pos.first][pos.second] = 'a';
//     vector<vector<int>> dp(grid.size(),vector<int>(grid[0].size(),-1));
//     dp[pos.first][pos.second] = 0;
//     //int c = 0;

// }

int main()
{
    ifstream in("input.txt");
    if (!in.is_open())
    {
        cerr << "Could not open the file - '" << endl;
        return EXIT_FAILURE;
    }
    string line;
    vector<string> grid;
    while (getline(in, line))
    {
        grid.push_back(line);
    }
    pair<int,int> pos;
    for(int i = 0; i < grid.size();i++){
        for(int j = 0; j < grid[i].size();j++){
            if(grid[i][j] == 'S'){
                pos.first = i;
                pos.second = j;
            }
        }
    }
    // cout<<pos.first<<" "<<pos.second<<endl;
    bfs(grid,pos);
    bfs2(grid, e);
    //bfs3(grid,pos);
    return 0;
}