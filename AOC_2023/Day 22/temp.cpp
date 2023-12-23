// #include <iostream>
// #include <fstream>
// #include <string>
// #include <vector>
// #include <algorithm>
// #include <regex>
// #include <set>
// #include <array>
// //#include <unordered_set>

// using namespace std;

// // struct coord{
// //     int x,y,z;
// //     coord(int x, int y, int z): x(x), y(y), z(z){}
// // };

// // struct hash_tuple {
// //     template <class T1, class T2, class T3>
// //     size_t operator()(const tuple<T1, T2, T3>& p) const
// //     {
// //         auto hash1 = hash<T1>{}(get<0>(p));
// //         auto hash2 = hash<T2>{}(get<1>(p));
// //         auto hash3 = hash<T3>{}(get<2>(p));
// //         return hash1 ^ hash2 ^ hash3;
// //     }
// // };

// // using coord = tuple<int, int, int>;

// using coord = array<int, 3>;


// struct brick{
//     coord start, end;
//     brick(coord start, coord end): start(start), end(end){}
// };

// // bool compare(brick a, brick b){
// //     return  a.start.z < b.start.z;
// // }

// //auto cmp = [](coord a, coord b) { return a.x < b.x || (a.x == b.x && a.y < b.y) || (a.x == b.x && a.y == b.y && a.z < b.z); };

// class Solution
// {
//     public:
//     vector<brick> bricks, bricksCopy;
//     //vector<set<coord, decltype(cmp)>> brickCubes;
//     //vector<unordered_set<coord, hash_tuple>> brickCubes;
//     vector<set<coord>> brickCubes;
//     pair<int,int> results;
//     //bool b = true;
//     void init(ifstream& input){
//         string line;
//         regex pattern(R"(\d+)");
//         while (getline(input, line))
//         {   
//             sregex_iterator it(line.begin(), line.end(), pattern), end;
//             vector<int> coords;
//             for (; it != end; ++it) {
//                 coords.push_back(stoi(it->str()));
//             }
//             //bricks.push_back(brick(coord(coords[0], coords[1], coords[2]), coord(coords[3], coords[4], coords[5])));
//             bricks.push_back(brick({coords[0], coords[1], coords[2]}, {coords[3], coords[4], coords[5]}));
//             //set<coord, decltype(cmp)> temp;
//             //unordered_set<coord, hash_tuple> temp;
//             set<coord> temp;
//             if(coords[0] != coords[3]){
//                 for(int i = coords[0]; i <= coords[3]; i++){
//                     //cout<<i<<" "<<coords[1]<<" "<<coords[2]<<"\n";
//                     //temp.insert(coord(i, coords[1], coords[2]));
//                     temp.insert({i, coords[1], coords[2]});
//                 }
//             }
//             else if(coords[1] != coords[4]){
//                 for(int i = coords[1]; i <= coords[4]; i++){
//                     //cout<<coords[0]<<" "<<i<<" "<<coords[2]<<"\n";
//                     //temp.insert(coord(coords[0], i, coords[2]));
//                     temp.insert({coords[0], i, coords[2]});
//                 }
//             }
//             else if(coords[2] != coords[5]){
//                 for(int i = coords[2]; i <= coords[5]; i++){
//                     //cout<<coords[0]<<" "<<coords[1]<<" "<<i<<"\n";
//                     //temp.insert(coord(coords[0], coords[1], i));
//                     temp.insert({coords[0], coords[1], i});
//                 }
//             }
//             //cout<<temp.size()<<"\n";
//             // for(auto& cube : temp){
//             //     cout<<cube.x<<" "<<cube.y<<" "<<cube.z<<"\n";
//             // }
//             brickCubes.push_back(temp);
//             //b &= coords[0] <= coords[3] && coords[1] <= coords[4] && coords[2] <= coords[5];
//         }
//         //cout<<b<<"\n";
//         bricksCopy = bricks;
//     }



//     void helper(){
//         //sort(bricks.begin(), bricks.end(), compare);
//         // simulate initial fall
//         //set<coord, decltype(cmp)> seen;
//         //unordered_set<coord, hash_tuple> seen;
//         set<coord> seen;
//         for(auto& bc : brickCubes){
//             for(auto& cube : bc){
//                 seen.insert(cube);
//             }
//         }
//         // for(auto& brick : bricks){
//         //     if(brick.start.x != brick.end.x){
//         //         for(int i = brick.start.x; i <= brick.end.x; i++){
//         //             seen.insert(coord(i, brick.start.y, brick.start.z));
//         //         }
//         //     }
//         //     else if(brick.start.y != brick.end.y){
//         //         for(int i = brick.start.y; i <= brick.end.y; i++){
//         //             seen.insert(coord(brick.start.x, i, brick.start.z));
//         //         }
//         //     }
//         //     else if(brick.start.z != brick.end.z){
//         //         for(int i = brick.start.z; i <= brick.end.z; i++){
//         //             seen.insert(coord(brick.start.x, brick.start.y, i));
//         //         }
//         //     }
//         // }
//         // simulate initial fall
//         //int cnt = 0;
//         while(true){
//             bool falling = false;
//             for(int i = 0; i < bricks.size(); i++){
//                 bool canFall = true;
//                 for(auto& cube : brickCubes[i]){
//                     if(cube[2] == 1 || (seen.count({cube[0], cube[1], cube[2]-1}) && !brickCubes[i].count({cube[0], cube[1], cube[2]-1}))){
//                         canFall = false;
//                         break;
//                     }
//                 }
//                 if(canFall){
//                     falling = true;
//                     //set<coord, decltype(cmp)> temp ;
//                     set<coord> temp;
//                     for(auto& cube : brickCubes[i]){
//                         //temp.insert(coord(cube.x, cube.y, cube.z-1));
//                         //seen.insert(coord(cube.x, cube.y, cube.z-1));
//                         temp.insert({cube[0], cube[1], cube[2]-1});
//                         seen.insert({cube[0], cube[1], cube[2]-1});
//                         seen.erase(cube); 
//                     }
//                     brickCubes[i] = temp;
//                 }
//             }
//             if(!falling){
//                 break;
//             }
//         }
//         int res1 = 0, res2 = 0;
//         // remove one brick one by one and see if any of the other bricks falls -> part 1 to calculate how many bricks can be removed without any other brick falling
//         // part 2 calculate how mnay bricks can fall if one brick is removed
//         for(int i = 0; i < bricks.size(); i++){
//             //set<coord, decltype(cmp)> tempSeen = seen;
//             set<coord> tempSeen = seen;
//             //vector<set<coord, decltype(cmp)>> tempBrickCubes = brickCubes;
//             vector<set<coord>> tempBrickCubes = brickCubes;
//             for(auto& cube : brickCubes[i]){
//                 tempSeen.erase(cube);
//             }
//             set<int> fallingBricks;
//             while(true){
//                 bool falling = false;
//                 for(int j = 0; j < bricks.size(); j++){
//                     if(j == i) continue;
//                     bool canFall = true;
//                     for(auto& cube : tempBrickCubes[j]){
//                         if(cube[2] == 1 || (tempSeen.count({cube[0], cube[1], cube[2]-1}) && !tempBrickCubes[j].count({cube[0], cube[1], cube[2]-1}))){
//                             canFall = false;
//                             break;
//                         }
//                     }
//                     if(canFall){
//                         fallingBricks.insert(j);
//                         falling = true;
//                         //set<coord, decltype(cmp)> temp ;
//                         set<coord> temp;
//                         for(auto& cube : tempBrickCubes[j]){
//                             tempSeen.erase(cube);
//                             // tempSeen.insert(coord(cube.x, cube.y, cube.z-1));
//                             // temp.insert(coord(cube.x, cube.y, cube.z-1));
//                             tempSeen.insert({cube[0], cube[1], cube[2]-1});
//                             temp.insert({cube[0], cube[1], cube[2]-1});
//                         }
//                         tempBrickCubes[j] = temp;
//                     }
//                 }
//                 if(!falling){
//                     break;
//                 }
//             }
//             if(fallingBricks.empty()){
//                 res1++;
//             }
//             else res2 += fallingBricks.size();
//         }
//         results = {res1, res2};
//     }

//     void part1(){
//         cout<<"Part 1: "<< results.first<<"\n";
//     }

//     void part2(){
//         cout<<"Part 2: "<< results.second<<"\n";
//     }

// };

// int main(){
//     ifstream in("input.txt");
//     if (!in.is_open())
//     {
//         cerr << "Could not open the file - '" << endl;
//         return EXIT_FAILURE;
//     }
//     Solution s;
//     s.init(in);
//     s.helper();
//     s.part1();
//     s.part2();
//     return 0;
// }


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <set>
#include <array>

using namespace std;

class Solution
{
    public:
    vector<vector<int>> bricks, bricksCopy;
    //bool b = true;
    void init(ifstream& input){
        string line;
        regex pattern(R"(\d+)");
        while (getline(input, line))
        {   
            sregex_iterator it(line.begin(), line.end(), pattern), end;
            vector<int> coords;
            for (; it != end; ++it) {
                coords.push_back(stoi(it->str()));
            }
            bricks.push_back({coords[0], coords[1], coords[2], coords[3], coords[4], coords[5]});
            //b &= coords[0] <= coords[3] && coords[1] <= coords[4] && coords[2] <= coords[5];
        }
        //cout<<b<<"\n";
        bricksCopy = bricks;
    }

    bool simulate(){
        bool b = false;
        set<array<int, 3>> seen;
        for(auto& brick : bricks){
            for(int x = brick[0]; x <= brick[3];x++){
                for(int y = brick[1]; y <= brick[4]; y++){
                    seen.insert({x, y, brick[5]});
                }
            }
        }
        vector<vector<int>> temp;
        for(auto& brick : bricks){
            bool canFall = true;
            for(int x = brick[0]; x <= brick[3];x++){
                for(int y = brick[1]; y <= brick[4]; y++){
                    if(brick[2] == 1 || seen.count({x, y, brick[2]-1})){
                        canFall = false;
                        break;
                    }
                }
                if(!canFall) break;
            }
            if(canFall){
                b = true;
                temp.push_back({brick[0], brick[1], brick[2]-1, brick[3], brick[4], brick[5]-1});
            }
            else temp.push_back(brick);
        }
        bricks = temp;
        return b;
    }

    void part1(){
        while(simulate());
        int result = 0;
        bricksCopy = bricks;
        for(int i = 0; i < bricksCopy.size(); i++){
            bricks = bricksCopy;
            bricks.erase(bricks.begin()+i);
            if(!simulate()){
                result++;
            }
        }
        cout<<"Part 1: "<<result<<"\n";
    }

    void part2(){
        int result = 0;
        for(int i = 0; i < bricksCopy.size(); i++){
            bricks = bricksCopy;
            bricks.erase(bricks.begin()+i);
            vector<vector<int>> temp = bricks;
            while(simulate());
            for(int j = 0; j < bricks.size(); j++){
                if(bricks[j] != temp[j]){
                    result++;
                }
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