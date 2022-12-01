#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<queue>
#include<algorithm>
using namespace std;

int input[100][100];
//vector<int> basin;
int dfs(int x, int y){
    int temp = input[x][y];
    if(temp == 9){
        return 0;
    }
    input[x][y] = -1;
    int count = 1;
    if(x > 0 && input[x-1][y] > temp){
        count += dfs(x - 1, y);
    }
    if (x < 99 && input[x + 1][y] > temp) {
        count += dfs(x + 1, y);
    }
    if (y > 0 && input[x][y - 1] > temp) {
        count += dfs(x, y - 1);
    }
    if (y < 99 && input[x][y + 1] > temp) {
        count += dfs(x, y + 1);
    }
    return count;
}

int main(){
    ifstream in("input.txt");
    if (!in.is_open()) {
        cerr << "Could not open the file - '"<< endl;
        return EXIT_FAILURE;
    }
    for(int i = 0; i < 100; i++) {
        string str;
        getline(in,str);
        for(int j = 0; j < 100; j++) {
            input[i][j] = str[j] - '0';
        }
    }
    priority_queue<int, vector<int>, greater<int> > q;
    q.push(0);
    q.push(0);
    q.push(0);
    for(int i = 0; i < 100; i++) {
        for(int j = 0; j < 100; j++){
            if(input[i][j] == -1 || input[i][j] == 9){
                continue;
            }
            if(i > 0 && input[i-1][j] <= input[i][j]){
                continue;
            }
            if(i < 99 && input[i+1][j] <= input[i][j]){
                continue;
            }
            if(j > 0 && input[i][j-1] <= input[i][j]){
                continue;
            }
            if(j < 99 && input[i][j+1] <= input[i][j]){
                continue;
            }
            //basin.push_back(dfs(i,j));
            q.push(dfs(i,j));
            q.pop();
        }
    }
    /*sort(basin.begin(),basin.end());
    int n = basin.size();
    int result = basin[n -1]*basin[n-2]*basin[n-3];
    cout<<result<<endl;*/
    int result = 1;
    while(!q.empty()){
        result *= q.top();
        q.pop();
    }
    cout<<result<<endl;

    return 0;
}