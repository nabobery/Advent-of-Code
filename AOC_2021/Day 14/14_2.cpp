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
    ifstream in("input.txt");
    if (!in.is_open()) {
        cerr << "Could not open the file - '"<< endl;
        return EXIT_FAILURE;
    }
    unordered_map <string,char> rules;
    string poly_template;
    getline(in, poly_template);
    string str;
    getline(in, str);
    for(int i = 0; i < 100;i++){
        getline(in,str);
        rules.insert({str.substr(0,2),str[6]});
    }
    unordered_map <string,long long int> counter;
    for(int i = 0; i < poly_template.size() - 1;i++) {
        string temp = poly_template.substr(i,2);
        if(!(counter.find(temp) != counter.end())){
            counter.insert({temp, 1});
        }
        else{
            counter[temp]++;
        }
    }
    unordered_map <string,long long int> new_counter;
    for(int i = 0; i < 40; i++) {
        for(auto itr = counter.begin(); itr != counter.end();itr++) {
            char insert_ch = rules[itr->first];
            string curr = itr->first;
            string one  = "";
            one = curr[0];
            one = one + insert_ch;
            new_counter[one] += itr->second;
            string two  = "";
            two = insert_ch;
            two = two + curr[1];
            new_counter[two] += itr->second;
        }
        counter = new_counter;
        new_counter.clear();
    }
    long long int arr[26];
    for(int i = 0; i < 26; i++){
        arr[i] = 0;
    }
    for(auto it = counter.begin(); it != counter.end();it++){
        arr[it->first.at(0) - 'A'] += it->second;
        arr[it->first.at(1) - 'A'] += it->second;
    }
    long long int most = LLONG_MIN;
    long long int least = LLONG_MAX;
    for(int i = 0; i < 26;i++){
        if(arr[i] == 0){
            continue;
        }
        most = max(most, arr[i]);
        least = min(least, arr[i]);
    }
    most = (most + 1)/2;
    least = (least + 1)/2;
    long long int result = most - least;
    cout<<result<<endl;
    return 0;
}