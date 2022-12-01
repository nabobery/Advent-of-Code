#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<cstring>
#include <unordered_map>
#include <climits>
#include <algorithm>
#include<ctime>
using namespace std;

int main(){
    clock_t t1,t2;
    t1=clock();
    ifstream in("input.txt");
    if (!in.is_open()) {
        cerr << "Could not open the file - '"<< endl;
        return EXIT_FAILURE;
    }
    unordered_map <string,char> rules;
    string poly_template;
    getline(in, poly_template);
    //cout << poly_template << endl;
    string str;
    getline(in, str);
    string temp[3];
    for(int i = 0; i < 100;i++){
        getline(in,str);
        stringstream x(str);
        int j = 0;
        while (getline(x, str, ' ')) {  
            temp[j] = str;
            j++;
        }
        char ch = temp[2].at(0);
        string tmp = temp[0];
        rules.insert({tmp,ch});
    }
    /*
    for(int i = 0; i < 100;i++){
        getline(in,str);
        rules.insert({str.substr(0,2),str[6]});
    }*/
    string process = "";
    for(int i = 0; i < 10;i++){
        process = "";
        int j;
        for(j = 1; j < poly_template.size(); j++){
            string pair = poly_template.substr(j-1,2);
            process += poly_template[j-1];
            process += rules[pair];
        }
        process += poly_template[j-1];
        poly_template = process;
    }
    unordered_map<char, int> count;
    for(char i : poly_template){
        count[i]++;
    }
    /* 
    int arr[26] = {};
    for(char i : poly_template){
        arr[i - 'A']++;
    }
    */
    int most = INT_MIN;
    int least = INT_MAX;
    for(auto itr = count.begin(); itr != count.end(); itr++){
        most = max(itr->second, most);
        least = min(itr->second, least);
    }
    int result = most - least;
    t2=clock();
    cout<<result<<endl;
    float diff = ((float)t2-(float)t1)/CLOCKS_PER_SEC;
    cout << "Running time: " << diff << endl;
    return 0;
}