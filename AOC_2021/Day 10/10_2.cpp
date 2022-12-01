#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<stack>
#include <map>
#include<algorithm>
using namespace std;

int score(char ch){
    if(ch == ')')
        return 1;
    else if(ch == ']')
        return 2;
    else if(ch == '}')
        return 3;
    else 
        return 4;
}

int main(){
    //ifstream in("test.txt");
    ifstream in("input.txt");
    if (!in.is_open()) {
        cerr << "Could not open the file - '"<< endl;
        return EXIT_FAILURE;
    }
    string str;
    vector<long long int> scores;
    map <char,char> brackets;
    brackets.insert(pair<char,char>('(',')'));
    brackets.insert(pair<char,char>('[',']'));
    brackets.insert(pair<char,char>('{','}'));
    brackets.insert(pair<char,char>('<','>'));
    while(getline(in,str)){
        //cout << str <<endl;
        bool flag = false;
        stack<char> stk;
        for(char ch : str){
            if(brackets.find(ch) != brackets.end()){
                stk.push(ch);
            }
            /*if(brackets.count(ch)){
                stk.push(ch);
            }*/
            else if(ch == brackets.at(stk.top())){
                stk.pop();
            }
            else
                flag = true; // for corrupted
        }
        if(!flag){
            long long int temp = 0;
            while(!stk.empty()){
                temp *= 5;
                temp += score(brackets.at(stk.top()));
                stk.pop();
            }
            scores.push_back(temp);
        }
    }
    sort(scores.begin(), scores.end());
    //cout<< scores.size()<<endl;
    long long int result = scores[scores.size()/2];
    /*for(auto s : scores){
        cout<< s << endl;
    }*/
    cout<<result<<endl;
    return 0;
}