#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include <algorithm>
#include <cstdlib>
#include<cmath>
using namespace std;

int main(){
    ifstream in("input.txt");
    if (!in.is_open()) {
        cerr << "Could not open the file - '"<< endl;
        return EXIT_FAILURE;
    }
    string str;
    getline(in,str);
    stringstream x(str);
    vector<int> input;
    while(getline(x,str, ',')){
        input.push_back(stoi(str)); 
    }
    int n = input.size();
    float sum = 0;
    for(int i = 0; i < n; i++){
        sum += input[i];
    }
    int mean = round(sum/n);
    int result1 = 0, result2 = 0, result3 = 0;
    for(int i = 0; i < n; i++){
        result1 += ((input[i] - mean)*(input[i] - mean) + abs(input[i] - mean))/2;
        result2 += ((input[i] - mean -1)*(input[i] - mean -1) + abs(input[i] - mean -1))/2;
        result3 += ((input[i] - mean +1)*(input[i] - mean +1) + abs(input[i] - mean +1))/2;    
    }
    //int result = min(result1, result2, result3);
    cout << result1 << endl;
    cout << result2 << endl;
    cout << result3 << endl;
    return 0;
}