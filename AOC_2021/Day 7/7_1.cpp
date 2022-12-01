#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include <algorithm>
#include <cstdlib>
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
    sort(input.begin(), input.end());
    int n = input.size();
    int median;
    if(n % 2 != 0){
        median = input[n/2];
    }
    else {
        median = (input[n/2] + input[n/2 + 1])/2;
    }
    int result = 0;
    for(int i = 0; i < n; i++) {
        result += abs(input[i] - median);
    }
    cout << result << endl;
    return 0;
}