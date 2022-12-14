#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cctype>
#include <vector>
using namespace std;

int main(){
    ifstream in("input.txt");
    if (!in.is_open())
    {
        cerr << "Could not open the file - '" << endl;
        return EXIT_FAILURE;
    }
    string line, temp[2];
    pair<int,int> p,q;
    int result = 0;
    while (getline(in,line))
    {
        int pos = line.find(",");
        temp[0] = line.substr(0, pos);
        temp[1] = line.substr(pos+1);
        pos = temp[0].find("-");
        p.first = stoi(temp[0].substr(0, pos));
        p.second = stoi(temp[0].substr(pos+1));
        pos = temp[1].find("-");
        q.first = stoi(temp[1].substr(0, pos));
        q.second = stoi(temp[1].substr(pos+1));
        if((p.first <= q.first && p.second >= q.second) || (p.first >= q.first && p.second <= q.second)){
            // cout<<p.first<<" "<<p.second<<" "<<q.first<<" "<<q.second<<"\n";
            result++;
        }
    }
    cout<<result<<"\n";
    in.clear();
    in.seekg(0, ios::beg);
    result = 0;
    while (getline(in,line))
    {
        int pos = line.find(",");
        temp[0] = line.substr(0, pos);
        temp[1] = line.substr(pos+1);
        pos = temp[0].find("-");
        p.first = stoi(temp[0].substr(0, pos));
        p.second = stoi(temp[0].substr(pos+1));
        pos = temp[1].find("-");
        q.first = stoi(temp[1].substr(0, pos));
        q.second = stoi(temp[1].substr(pos+1));
        if((p.first <= q.first && p.second >= q.first) || (p.first >= q.first && q.second >= p.first)){
            //cout<<p.first<<" "<<p.second<<" "<<q.first<<" "<<q.second<<"\n";
            result++;
        }
    }
    cout<<result<<"\n";
    return 0;
}
