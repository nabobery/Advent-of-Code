#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cctype>
#include <vector>
#include <set>
using namespace std;

int main()
{
    ifstream in("input.txt");
    if (!in.is_open())
    {
        cerr << "Could not open the file - '" << endl;
        return EXIT_FAILURE;
    }
    string line;
    // for test cases
    // while(getline(in,line)){
    //     int result,l = 0;
    //     set<char> s;
    //     s.insert(line[0]);
    //     for(int i = 1; i < line.size();i++){
    //         if(s.find(line[i]) != s.end()){
    //             while(l < i){
    //                 if(line[l] != line[i]){
    //                     s.erase(line[l]);
    //                     l++;
    //                 }
    //                 else{
    //                     l++;
    //                     break;
    //                 }
    //             }
    //         }
    //         else{
    //             s.insert(line[i]);
    //             if(s.size() == 4){
    //                 result = i+1;
    //                 break;
    //             }
    //         }
    //     }
    //     cout<<result<<"\n";
    // }
    getline(in, line);
    //cout<<line<<"\n";
    //cout<<line.size()<<"\n";
    int result,l = 0;
    set<char> s;
    s.insert(line[0]);
    for(int i = 1; i < line.size();i++){
        if(s.find(line[i]) != s.end()){
            while(l < i){
                if(line[l] != line[i]){
                    s.erase(line[l]);
                    l++;
                }
                else{
                    l++;
                    break;
                }
            }
        }
        else{
            s.insert(line[i]);
            if(s.size() == 4){
                result = i+1;
                break;
            }
        }
    }
    cout<<result<<"\n";
    // part 2
    in.clear();
    in.seekg(0, ios::beg);
    getline(in, line);
    //cout<<line<<"\n";
    //cout<<line.size()<<"\n";
    result,l = 0;
    s.clear();
    s.insert(line[0]);
    for(int i = 1; i < line.size();i++){
        if(s.find(line[i]) != s.end()){
            while(l < i){
                if(line[l] != line[i]){
                    s.erase(line[l]);
                    l++;
                }
                else{
                    l++;
                    break;
                }
            }
        }
        else{
            s.insert(line[i]);
            if(s.size() == 14){
                result = i+1;
                break;
            }
        }
    }
    cout<<result<<"\n";
    return 0;
}