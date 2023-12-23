#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>
//#include <unordered_set>
#include <set>

using namespace std;

using ll = long long;

ofstream out;

// struct hash_pair {
//     template <class T1, class T2>
//     size_t operator()(const pair<T1, T2>& p) const{
//         return std::hash<T1>()(p.first) ^ std::hash<T2>()(p.second);
//     }
// };

// Observations: 
// 1. The garden plots are in a grid of size 131*131 i.e. a Square of size 131
// 2. The Start is at the center of the grid i.e. (65,65)
// 3. The garden plots are symmetric about the center of the grid, which means the horizontal and vertical lines passing through the center of the grid are garden plots
// 4. The least number of steps required to reach another grid is 65
// 5. The number of garden plots increases quadratically for every 131 steps(x = 131 + 65*n, y = ax^2 + bx + c) therefore we can use polynomial extrapolation to find the number of garden plots for any number of steps
// 6. The number of garden plots grows equally like a diamond from the center (like the manhattan diamond)
// 7. After expanding by 65 steps, the corners of the diamond are the "start points", just shifted up/down/left/right.
// 8. The number of new spaces we add is just a linear function, and the cumulative sum of a linear function is a quadratic function
// 9. The reason for perfect Quadratic is that we can reach all corners within 65 steps

class Solution
{
    public:
    int n, m;
    vector<string> grid;
    vector<string> tempGrid;
    pair<int,int> start;
    pair<int,int> dirs[4] = {{0,1},{1,0},{0,-1},{-1,0}};
    void init(ifstream& input){
        string line;
        while (getline(input, line))
        {
            grid.push_back(line);
        }
        n = grid.size();
        m = grid[0].size();
        for(int i = 0; i < grid.size(); i++){
            for(int j = 0; j < grid[i].size(); j++){
                if(grid[i][j] == 'S'){
                    start = {i,j};
                    break;
                }
            }
        }
        // int count = 0;
        // for(int i = 0; i < grid.size(); i++){
        //     for(int j = 0; j < grid[i].size(); j++){
        //         if(grid[i][j] != '#'){
        //             count++;
        //         }
        //     }
        // }
        // cout<<count<<"\n";
    }

    bool isValid(pair<int,int> pos){
        pos.first %= n;
        pos.second %= m;
        return pos.first >= 0 && pos.first < n && pos.second >= 0 && pos.second < m;
    }

    void part1(){
        queue<pair<int,int>> q;
        q.push(start);
        int steps = 0;
        while(steps < 64 && !q.empty()){
            int size = q.size();
            //cout<<steps<<" "<<size<<"\n";
            for(int i = 0; i < size; i++){
                pair<int,int> curr = q.front();
                q.pop();
                grid[curr.first][curr.second] = '.';
                for(int j = 0; j < 4; j++){
                    pair<int,int> next = {curr.first + dirs[j].first, curr.second + dirs[j].second};
                    if(isValid(next) && grid[next.first][next.second] == '.'){
                        grid[next.first][next.second] = 'O';
                        q.push(next);
                    }
                }
            }
            steps++;
        }
        cout<<"Part 1: "<< q.size() << "\n";
    }

    bool isValid2(pair<int,int> pos){
        return pos.first >= 0 && pos.first < tempGrid.size() && pos.second >= 0 && pos.second < tempGrid[0].size();
    }

    void printSim(){
        grid[start.first][start.second] = '.';
        for(int i = 0; i < grid.size(); i++){
            string temp = grid[i];
            for(int j = 0; j < 3;j++) temp += grid[i];
            tempGrid.push_back(temp);
        }
        for(int i = 0; i < 2;i++){
            for(int j = 0; j < grid.size(); j++){
                tempGrid.push_back(tempGrid[j]);
            }
        }
        queue<pair<int,int>> q;
        start = {start.first + grid.size(), start.second + grid.size()};
        q.push(start);
        int steps = 0;
        while(!q.empty()){
            int size = q.size();
            out<<steps<<" "<<size<<"\n";
            for(int i = 0; i < size; i++){
                pair<int,int> curr = q.front();
                q.pop();
                tempGrid[curr.first][curr.second] = '.';
                for(int j = 0; j < 4; j++){
                    pair<int,int> next = {curr.first + dirs[j].first, curr.second + dirs[j].second};
                    if(isValid2(next) && tempGrid[next.first][next.second] == '.'){
                        tempGrid[next.first][next.second] = 'O';
                        q.push(next);
                    }
                }
            }
            steps++;
        }
    }

    // polynomial extrapolation / lagrange interpolation
    // y = ax^2 + bx + c
    // We have y1 = f(x1), y2 = f(x2), y3 = f(x3) and x1,x2,x3 are 0(131*0 + 65), 1 (131*1 + 65 = 196), 2 (131*2 + 65 = 327)
    // So, we can find a,b,c using the following equations
    // y1 = ax1^2 + bx1 + c, y2 = ax2^2 + bx2 + c, y3 = ax3^2 + bx3 + c
    // a = (y1 - 2y2 + y3)/(x1^2 - 2x2^2 + x3^2) = (y1 - 2y2 + y3)/(0^2 - 2*1^2 + 2^2) = (y1 - 2y2 + y3)/2
    // b = (y1 - y2)/(x1 - x2) - a(x1 + x2) = (y1 - y2)/(0 - 1) - a(0 + 1) = (y2 - y1) - a
    // c = y1 - ax1^2 - bx1 = y1
    // Now, we can find f(n) = an^2 + bn + c
    // f(n) = n*(n-1)/2 * (y3-2y2+y1) + n*(y2-y1) + y1  or
    //  a = (y1 - 2y2 + y3)/2, b = (y2 - y1) - a, c = y1 and f(n) = a*n^2 + b*n + c
    // a = 11413, b = 19262, c = 3874
    ll function(ll n, vector<int> temp){
        ll  a = temp[0], b = temp[1], c = temp[2];
        //cout<<a<<" "<<b<<" "<<c<<" "<<n<<"\n";
        ll result = a+n*(b-a) + n*(n-1)/2*((c-b)-(b-a));
        return result;
    }

    string pointToString(pair<int,int> p){
        return to_string(p.first) + "," + to_string(p.second);
    }

    bool isValid3(pair<int,int> pos){
        while(pos.first < 0) pos.first += n;
        while(pos.second < 0) pos.second += m;
        pos.first %= n;
        pos.second %= m;
        return grid[pos.first][pos.second] == '.';
    }

    // (65, 3874) -> (196, 34549) -> (327, 95798)
    // 26501365 = 202300 * 131 + 65 => 26501365%131 = 65
    // we pass n to calculate f(n) which gives us the number of garden plots for steps = 65 + 131*n 
    // as f is a quadratic function, we can use polynomial extrapolation to find the value of f(n)
    // But as we don't have f co-efficients, we can use the first 3 values of f(n) to find the co-efficients
    // So, we pass n,y1 = f(x1), y2 = f(x2), y3 = f(x3) to the function
    void part2(){
        //printSim();
        for(int i = 0; i < grid.size(); i++){
            for(int j = 0; j < grid[i].size(); j++){
                if(grid[i][j] == 'O'){
                    grid[i][j] = '.';
                }
            }
        }
        grid[start.first][start.second] = '.';
        vector<int> temp;
        int steps = 0;
        //unordered_set<pair<int,int>, hash_pair> prevVisited, currVisited;
        set<pair<int,int>> prevVisited, currVisited;
        currVisited.insert(start);
        while(true){
            int size = currVisited.size();
            if(steps%n == n/2) temp.push_back(size);
            if(temp.size() == 3) break;
            prevVisited.clear();
            //cout<<steps<<" "<<size<<"\n";
            for(auto it : currVisited){
                for(int j = 0; j < 4; j++){
                    pair<int,int> next = {it.first + dirs[j].first, it.second + dirs[j].second};
                    // cout<<next.first<<" "<<next.second<<"\n";
                    if(isValid3(next)){
                        prevVisited.insert(next);
                    }
                }
            }
            currVisited = prevVisited;
            steps++;
        }
        cout<<"Part 2: "<< function(ll(26501365/n), temp) << "\n";
    }
};

int main(){
    ifstream in("input.txt");
    out = ofstream("output.txt");
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