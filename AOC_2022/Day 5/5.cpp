#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cctype>
#include <vector>
#include <regex>
#include <stack>
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
    bool flag = false;
    vector<stack<char>> vec(9);
    while (getline(in, line))
    {
        if(!flag){
            if(line.size() == 0){
                for(int i = 0; i < 9;i++){
                    stack<char> tmp;
                    while(!vec[i].empty()){
                        tmp.push(vec[i].top());
                        vec[i].pop();
                    }
                    vec[i] = tmp;
                }
                flag = true;
            }
            else{
                for(int i = 0; i < line.size();i++){
                    if(line[i] == ' ') continue;
                    else if(line[i] == '['){
                        vec[i/4].push(line[i+1]);
                        i += 2;
                    }
                    else{
                        break;
                    }
                }
            }
        }
        else{
            regex rgx("move ((1[0-9])|([1-9])|(2[0-9])|(3[0-9])) from ([1-9]) to ([1-9])");
            smatch match;
            if (regex_search(line, match, rgx))
            {
                int count = 0, size, tostk, fromstk;
                for(size_t t = match.size()-1; t > 0;t--){
                    if(match[t].str().empty()){
                        continue;
                    }
                    else{
                        if(count == 0) tostk = stoi(match[t].str());
                        else if(count == 1) fromstk = stoi(match[t].str());
                        else if(count == 2) size = stoi(match[t].str());
                        else break;
                        count++;
                    }
                }
                //cout<<"move " <<size<<" from "<<fromstk<<" to "<<tostk<<"\n";
                while(size--){
                    vec[tostk-1].push(vec[fromstk-1].top());
                    vec[fromstk-1].pop();
                }
            }
        }
    }
    // for(auto it : vec){
    //     while(!it.empty()){
    //         cout << it.top() << " ";
    //         it.pop();
    //     }
    //     cout << "\n";
    // }
    for(auto it : vec){
        if(!it.empty()) cout<<it.top();
    }
    cout<<"\n";
    // part 2
    in.clear();
    in.seekg(0, ios::beg);
    flag = false;
    for(int i = 0; i < 9;i++){
        while(!vec[i].empty()){
            vec[i].pop();
        }
    }
    while (getline(in, line))
    {
        if(!flag){
            if(line.size() == 0){
                for(int i = 0; i < 9;i++){
                    stack<char> tmp;
                    while(!vec[i].empty()){
                        tmp.push(vec[i].top());
                        vec[i].pop();
                    }
                    vec[i] = tmp;
                }
                flag = true;
            }
            else{
                for(int i = 0; i < line.size();i++){
                    if(line[i] == ' ') continue;
                    else if(line[i] == '['){
                        vec[i/4].push(line[i+1]);
                        i += 2;
                    }
                    else{
                        break;
                    }
                }
            }
        }
        else{
            regex rgx("move ((1[0-9])|([1-9])|(2[0-9])|(3[0-9])) from ([1-9]) to ([1-9])");
            smatch match;
            if (regex_search(line, match, rgx))
            {
                int count = 0, size, tostk, fromstk;
                for(size_t t = match.size()-1; t > 0;t--){
                    if(match[t].str().empty()){
                        continue;
                    }
                    else{
                        if(count == 0) tostk = stoi(match[t].str());
                        else if(count == 1) fromstk = stoi(match[t].str());
                        else if(count == 2) size = stoi(match[t].str());
                        else break;
                        count++;
                    }
                }
                //cout<<"move " <<size<<" from "<<fromstk<<" to "<<tostk<<"\n";
                stack<char> temp;
                while(size--){
                    temp.push(vec[fromstk-1].top());
                    vec[fromstk-1].pop();
                }
                while(!temp.empty()){
                    //cout<<temp.top()<<" ";
                    vec[tostk-1].push(temp.top());
                    temp.pop();
                }
                //cout<<"\n";
            }
        }
    }
    // for(auto it : vec){
    //     while(!it.empty()){
    //         cout << it.top() << " ";
    //         it.pop();
    //     }
    //     cout << "\n";
    // }
    for(auto it : vec){
        if(!it.empty()) cout<<it.top();
    }
    cout<<"\n";
    return 0;
}