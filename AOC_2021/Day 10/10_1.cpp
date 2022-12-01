#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<stack>
using namespace std;

int score(char ch){
    if(ch == ')')
        return 3;
    else if(ch == ']')
        return 57;
    else if(ch == '}')
        return 1197;
    else 
        return 25137;

}

int main(){
    ifstream in("input.txt");
    if (!in.is_open()) {
        cerr << "Could not open the file - '"<< endl;
        return EXIT_FAILURE;
    }
    string str;
    stack <char> stk;
    int result = 0;
    while(getline(in,str)){
        //cout<<str<< endl;
        for(char ch : str){
            if(ch == '('){
                stk.push(')');
                continue;
            }
            else if(ch == '['){
                stk.push(']');
                continue;
            }
            else if(ch == '{'){
                stk.push('}');
                continue;
            }
            else if(ch == '<'){
                stk.push('>');
                continue;
            }
            if(!stk.empty() && stk.top() != ch){
                result += score(ch);
            }
            stk.pop();
        }
    }
    cout<<result<<endl;
    return 0;
}