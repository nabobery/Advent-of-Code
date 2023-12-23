#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <array>
#include <queue>
#include <map>
#include <set>

using namespace std;

using coord = array<int, 3>;

struct brick{
    coord start, end;
    brick(coord s, coord e): start(s), end(e){}
};

bool compare(brick b1, brick b2){
    return b1.start[2] < b2.start[2];
}

class Solution
{
    public:
    vector<brick> bricks;
    // map of bricks that support the given brick is support and indegree is bricks that are supported by the given brick
    map<int, set<int>> support, supported;
    // unsafe bricks are the bricks that are supported by only one brick and can fall down after removing that brick
    set<int> unsafebricks;
    void init(ifstream& input){
        string line;
        regex pattern(R"(\d+)");
        while (getline(input, line)){   
            sregex_iterator it(line.begin(), line.end(), pattern), end;
            vector<int> coords;
            for (; it != end; ++it)
                coords.push_back(stoi(it->str()));
            bricks.push_back(brick(coord{coords[0], coords[1], coords[2]}, coord{coords[3], coords[4], coords[5]}));
        }
    }

    void part1(){
        int result = 0;
        // sort bricks by z coordinate of start
        sort(bricks.begin(), bricks.end(), compare);
        // map of highest z coordinate for each x,y coordinate and the brick index that is at that height
        map<pair<int, int>, pair<int,int>> highest;
        for(int i = 0; i < bricks.size();i++){
            int max_height = 0;
            // finding the highest z coordinate for each x,y coordinate cube in the brick
            for(int x = bricks[i].start[0]; x <= bricks[i].end[0]; x++)
                for(int y = bricks[i].start[1]; y <= bricks[i].end[1]; y++)
                    max_height = max(max_height, highest.count({x, y}) ? highest[{x, y}].first : 0);
            // finding the height of the brick
            int height = bricks[i].end[2] - bricks[i].start[2] + 1;
            // updating the highest z coordinate for each x,y coordinate cube in the brick
            for(int x = bricks[i].start[0]; x <= bricks[i].end[0]; x++){
                for(int y = bricks[i].start[1]; y <= bricks[i].end[1]; y++){
                    // if the highest z coordinate is the same as what can be reached by current brick, then the current brick is supported by the brick at that coordinate
                    auto temp = highest.count({x, y}) ? highest[{x, y}] : make_pair(0, -1);
                    // we add the current brick to the indegree of the brick that supports it
                    // and we add the brick that supports the current brick to the support of the current brick
                    if(temp.first == max_height){
                        supported[temp.second].insert(i);
                        support[i].insert(temp.second);
                    }
                    // we update the highest z coordinate for each x,y coordinate cube in the brick
                    highest[{x, y}] = {max_height + height, i};
                }
            }
        }
        // we remove the brick that is supported by -1(which is ground) from the support of all bricks
        for(auto it : supported[-1])
            support.erase(it);
        // remove the ground
        supported.erase(-1);
        for(auto it : support){
            if(it.second.size() == 1)
                unsafebricks.insert(it.second.begin(), it.second.end());
        }
        // Number of safe bricks that can be removed is the total number of bricks minus the number of unsafe bricks
        cout<<"Part 1: "<<bricks.size() - unsafebricks.size()<<"\n";
    }

    // We need to find the number of bricks that will fall down if we remove the unsafe bricks
    // We can do this by finding the number of bricks that are supported by the unsafe bricks and can do this by using topological sort
    void part2(){
        int result = 0;
        // for each unsafe brick 
        for(auto it : unsafebricks){
            // we find the indegree of each brick that is number of bricks that are support a brick
            map<int,int> indegree;
            // we find the indegree of each brick that is supported by another brick
            for(auto it : support) indegree[it.first] = it.second.size();
            queue<int> q;
            /// we add the unsafe brick to the queue and remove it and see if any other brick becomes unsafe
            q.push(it);
            while(!q.empty()){
                int curr = q.front();
                q.pop();
                result++;
                // we remove the brick from the support of the bricks that it supports
                for(auto it : supported[curr]){
                    indegree[it]--;
                    // if in this process any brick becomes unsafe we add it to the queue
                    if(indegree[it] == 0) q.push(it);
                }
            }
            // subtracting the unsafe brick itself
            result--;
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