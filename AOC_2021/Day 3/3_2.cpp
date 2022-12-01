#include<iostream>
#include<fstream>
#include<vector>
using namespace std;

int binarytoDecimal(string binary){
    int dec_value = 0;
    int base = 1;
    for (int i = 11; i >= 0; i--) {
        if (binary[i] == '1')
            dec_value += base;
        base = base * 2;
    }
    return dec_value;
}

int main(){
    vector<string> input;
    ifstream input_file("input.txt");
    string bin;
    if (!input_file.is_open()) {
        cerr << "Could not open the file"<< endl;
        return EXIT_FAILURE;
    }
    while(input_file >> bin) {
        input.push_back(bin);
    }
    // storing count of 0
    vector <int> count(12, 0);
    string oxy_generator, co2_scrubber;
    vector <string> temp = input;
    for(int i = 0; i < 12; i++){
        for(int j = 0; j < temp.size(); j++){
            if(temp[j].at(i) == '0'){
                count[i]++;
            }
        }
        if(count[i] <= temp.size() - count[i]){
            for(int j = 0; j < temp.size(); j++){
                if(temp[j].at(i) == '0'){
                    temp.erase(temp.begin() + j--);
                }
            }
        }
        else{
            for(int j = 0; j < temp.size(); j++){
                if(temp[j].at(i) == '1'){
                    temp.erase(temp.begin() + j--);
                }
            }
        }
    }
    oxy_generator = temp[0];
    cout<<oxy_generator<<endl;
    vector <string> temp2 = input;
    // storing count of 1
    vector<int> count2(12,0);
    for(int i = 0; i < 12; i++){
        for(int j = 0; j < temp2.size(); j++){
            if(temp2[j].at(i) == '1'){
                count2[i]++;
            }
        }
        if(count2[i] >= temp2.size() - count2[i]){
            for(int j = 0; j < temp2.size(); j++){
                if(temp2[j].at(i) == '1'){
                    temp2.erase(temp2.begin() + j--);
                }
            }
        }
        else{
            for(int j = 0; j < temp2.size(); j++){
                if(temp2[j].at(i) == '0'){
                    temp2.erase(temp2.begin() + j--);
                }
            }
        }
    }
    co2_scrubber = temp2[0];
    cout<<co2_scrubber<<endl;
    int result = binarytoDecimal(oxy_generator)*binarytoDecimal(co2_scrubber);
    cout<<result<<endl;
    return 0;
}