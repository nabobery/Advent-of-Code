#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<algorithm>
using namespace std;

int version_sum;

string binary_str;

void hex_to_bin(char ch){
    switch(ch){
        case '0':
            binary_str += "0000";
            break;
        case '1':
            binary_str += "0001";
            break;
        case '2':
            binary_str += "0010";
            break;
        case '3':
            binary_str += "0011";
            break;
        case '4':
            binary_str += "0100";
            break;
        case '5':
            binary_str += "0101";
            break;
        case '6':
            binary_str += "0110";
            break;
        case '7':
            binary_str += "0111";
            break;
        case '8':
            binary_str += "1000";
            break;
        case '9':
            binary_str += "1001";
            break;
        case 'A':
            binary_str += "1010";
            break;
        case 'B':
            binary_str += "1011";
            break;
        case 'C':
            binary_str += "1100";
            break;
        case 'D':
            binary_str += "1101";
            break;
        case 'E':
            binary_str += "1110";
            break;
        case 'F':
            binary_str += "1111";
            break;
    }
}

int decode(int index){
    int currVer = 0;
    for(int i = index; i < index + 3; i++){
        currVer <<= 1;
        if(binary_str[i] == '1'){
            currVer += 1;
        }
    }
    version_sum += currVer;
    int currId = 0;
    for(int i = index + 3; i < index + 6; i++){
        currId <<= 1;
        if(binary_str[i] == '1'){
            currId += 1;
        }
    }
    if(currId == 4){
        int currIndex = index + 6;
        while(binary_str[currIndex] == '1'){
            currIndex += 5;
        }
        return currIndex + 5;
    }
    int currIndex;
    // length type id 
    //if LTI 1 then 11 bits length represents number of sub packets
    if(binary_str[index + 6] == '1'){ 
        int numOfSub = 0;
        for(int i = index + 7; i < index + 18;i++){
            numOfSub <<= 1;
            if(binary_str[i] == '1'){
                numOfSub += 1;
            }
        }
        currIndex = index + 18;
        for(int i = 0; i < numOfSub; i++){
            currIndex = decode(currIndex);
        }
    }
    // else 15 bits length represnt number of bits in subpackets
    else{
        int numOfBits = 0;
        for(int i = index + 7; i < index + 22;i++){
            numOfBits <<= 1;
            if(binary_str[i] == '1'){
                numOfBits += 1;
            }
        }
        currIndex = index + 22;
        while(currIndex < index + 22 + numOfBits){
            currIndex = decode(currIndex);
        }
    }
    return currIndex;
}


int main(){
    ifstream in("input.txt");
    if (!in.is_open()) {
        cerr << "Could not open the file - '"<< endl;
        return EXIT_FAILURE;
    }
    string str;
    getline(in, str);
    for(int i = 0; i < str.size(); i++) {
        hex_to_bin(str[i]);
    }
    //cout << binary_str<< endl;
    version_sum = 0;
    decode(0);
    cout<<version_sum<<endl;
    return 0;
}