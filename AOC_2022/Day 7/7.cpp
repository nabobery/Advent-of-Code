#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cctype>
#include <vector>
#include <algorithm>
using namespace std;

#define ll long long int

ll result = 0;
ll total_size_possible = 40000000, total_size = 0;

ifstream in("input.txt");
string line;

bool ignore(string str){
    return ((line.find("$ ls") != string::npos) || (line.find("dir") == 0));
}

vector<int> vec;

ll helper(){
    ll temp = 0;
    while (getline(in, line))
    {
        if(ignore(line)) continue;
        else{
            if(any_of(line.begin(), line.end(), ::isdigit)){
                ll x = 0;
                for(int i = 0; i < line.size(); i++){
                    if(isdigit(line[i])){
                        x = x*10 + (line[i] - '0');
                    }
                }
                temp += x;
            }
            else{
                if(line == "$ cd .."){
                    if(temp < 1e5) result += temp;
                    vec.push_back(temp);
                    return temp;
                }
                else{
                    temp += helper();
                }
            }
        }
    }
    return temp;
}

int main()
{
    ll tmp;
    getline(in, line);
    getline(in, line);
    while (getline(in, line))
    {
        if(any_of(line.begin(), line.end(), ::isdigit)){
            ll x = 0;
            for(int i = 0; i < line.size(); i++){
                if(isdigit(line[i])){
                    x = x*10 + (line[i] - '0');
                }
            }
            total_size += x;
        }
        else if(line.find("$ cd ") == string::npos)
        {
            continue;
        }
        else{
            tmp = helper();
            total_size += tmp;
            vec.push_back(tmp);
        }
    }
    cout<<result<<"\n";
    int to_remove = total_size - total_size_possible;
    sort(vec.begin(), vec.end());
    //for (auto i: vec) cout<<i<<" ";
    cout<<"\n";
    cout<<vec[lower_bound(vec.begin(), vec.end(), to_remove) - vec.begin()]<<"\n";
    return 0;
}
