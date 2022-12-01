#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
using namespace std;

int main(){
    //vector<int> state(9,0);
    long long int state[9] = {0};
    ifstream in("input.txt");
    if (!in.is_open()) {
        cerr << "Could not open the file - '"<< endl;
        return EXIT_FAILURE;
    }
    string str;
    getline(in,str);
    stringstream x(str);
    while(getline(x,str, ',')){
        state[stoi(str)]++; 
    }
    for(int i = 0; i < 256;i++){
        long long int temp = state[0];
        for(int j = 0; j < 8; j++){
            state[j] = state[j+1];
        }
        state[6] += temp;
        state[8] = temp;
    }
    long long int result = 0;
    for(int i = 0; i < 9; i++){
        result += state[i];
    }
    cout << result << endl;
    return 0;
}