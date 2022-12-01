#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<algorithm>
#include<utility>
using namespace std;

vector<char> parse(string str){
    vector<char> result;
    for(int i=0; i<str.length(); i++){
        switch(str[i]){
            case '[': 
            case ']':
                result.push_back(str[i]);
                break;
            case ',': 
                result.push_back('_');
                break;
            default : 
                result.push_back(str[i] - '0');
        }
    }
    return result;
}

bool explode(vector <char> &num){
    int count = 0;
    for(int i = 0; i < num.size(); i++){
        if(count == 5){
            count = i;
            break;
        }
        if(num[i] == '['){
            count++;
        }
        if(num[i] == ']'){
            count--;
        }
    }
    if(count == 0){
        return false;
    }
    for(int i = count - 1; i >= 0;i--){
        if(num[i] < '['){
            num[i] = (char)(num[i] + num[count]);
            break;
        }
    }
    for(int i = count + 3; i < num.size();i++){
        if(num[i] < '['){
            num[i] = (char)(num[i] + num[count + 2]);
            break;
        }
    }
    for(int i = 0; i < 5; i++){
        auto it = num.begin() + count - 1;
        num.erase(it);
    }
    num.insert(num.begin() + count-1, 0);
    return true;
}

bool split(vector <char> &num){
    int index = -1;
    for(int i = 0; i < num.size(); i++){
        if(num[i] < '[' && num[i] > 9){
            index = i;
            break;
        }
    }
    if(index == -1){
        return false;
    }
    int temp = num[index];
    num.erase(num.begin() + index);
    num.insert(num.begin() + index, ']');
    if(temp % 2 == 0){
        num.insert(num.begin() + index, (char)temp/2);
    }
    else{
        num.insert(num.begin() + index, (char)(temp/2 + 1));
    }
    num.insert(num.begin() + index,'_');
    num.insert(num.begin() + index, (char)temp/2);
    num.insert(num.begin() + index,'[');
    return true;
}

vector <char> addSnailNumbers(vector <char> num1, vector <char> num2){
    vector <char> result;
    result.push_back('[');
    result.insert(result.end(), num1.begin(), num1.end());
    result.push_back('_');
    result.insert(result.end(), num2.begin(), num2.end());
    result.push_back(']');
    return result;
}

void reduce(vector <char> &num){
    bool flag = true;
    while(flag){
        flag = explode(num);
        if(flag){
            continue;
        }
        flag = split(num);
    }
}

pair<int, int> magnitude(vector<char> num, int low){
    int result;
    int index = low + 1;
    if(num[index] == '['){
        pair<int, int> temp = magnitude(num, index);
        result = 3*temp.first;
        index = temp.second + 2;
    }
    else{
        result = 3*num[index];
        index += 2;
    }
    if(num[index] == '['){
        pair<int, int> temp = magnitude(num, index);
        result += 2*temp.first;
        index = temp.second + 1;
    }
    else{
        result += 2*num[index];
        index += 1;
    }
    return std::make_pair(result,index);

}

int main(){
    ifstream in("input.txt");
    if (!in.is_open()) {
        cerr << "Could not open the file - '"<< endl;
        return EXIT_FAILURE;
    }
    string str;
    getline(in, str);
    vector <char> prev = parse(str);
    while(getline(in, str)){
        prev = addSnailNumbers(prev, parse(str));
        reduce(prev);
    }
    int result = magnitude(prev,0).first;
    cout << result << endl;
    return 0;
}