#include<iostream>
#include<fstream>
#include<vector>
using namespace std;

/*string binaryComplement(string a){
    string b = "";
    for(int i = 0; i < 12;i++){
        if(a[i] == '1'){
            b[i] += '0';
        }
        else{
            b[i] += '1';
        }
    }
    return b;
}*/

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
    string input[1000];
    ifstream input_file("input.txt");
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"<< endl;
        return EXIT_FAILURE;
    }
    int i = 0;
    string gamma, temp, epsilon = "";
    while(i < 1000 && input_file >> input[i]) {
        i++;
    }
    vector <int> count(12, 0);
    for(i = 0; i < 1000; i++){
        temp = input[i];
        for(int j = 0; j < 12; j++){
            if(temp[j] == '0'){
                count[j]++;
            }
        }
    }
    for(int j = 0; j < 12; j++){
        if(count[j] > 1000 - count[j]){
            gamma += '0';
        }
        else{
            gamma += '1';
        }
    }
    for(int i = 0; i < 12;i++){
        if(gamma[i] == '1'){
            epsilon += '0';
        }
        else{
            epsilon += '1';
        }
    }
    cout<<gamma<<endl;
    cout<<epsilon<<endl;
    int result = binarytoDecimal(gamma)*binarytoDecimal(epsilon);
    cout<<result<<endl;
    return 0;
}