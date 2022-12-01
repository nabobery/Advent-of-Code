#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<algorithm>
#include<utility>
#include<climits>
using namespace std;

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

long long int getVal(int start, int end){
    long long int result = 0;
    for(int i =  start; i < end;i+=5){
        for(int j = i + 1; j < i + 5; j++){
            result <<= 1;
            if(binary_str[j] == '1'){
                result += 1;
            }
        }
    }
    return result;
}

long long int sumOfPackets(vector <long long int> list){
    long long int result = 0;
    for(int i = 0; i < list.size(); i++){
        result += list[i];
    }
    return result;
}

long long int productOfPackets(vector <long long int> list){
    long long int result = 1;
    for(int i = 0; i < list.size(); i++){
        result *= list[i];
    }
    return result;
}

long long int minOfPackets(vector <long long int> list){
    long long int result = LLONG_MAX;
    for(int i = 0; i < list.size(); i++){
        result = min(result, list[i]);
    }
    return result;
}

long long int maxOfPackets(vector <long long int> list){
    long long int result = LLONG_MIN;
    for(int i = 0; i < list.size(); i++){
        result = max(result, list[i]);
    }
    return result;
}

long long int greaterThan(vector <long long int> list){
    return (list[0] > list[1]) ? 1 : 0;
}

long long int lessThan(vector <long long int> list){
    return (list[0] < list[1]) ? 1 : 0;
}

long long int equalTo(vector <long long int> list){
    return (list[0] == list[1]) ? 1 : 0;
}

std::pair<int, long long int> decode(int index){
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
        long long int val =  getVal(index + 6, currIndex + 5);
        return make_pair(currIndex + 5, val);
    }
    int currIndex;
    vector <long long int> list;
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
            std::pair<int, long long int> p = decode(currIndex);
            currIndex = p.first;
            list.push_back(p.second);
        }
    }
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
            std::pair<int, long long int> p = decode(currIndex);
            currIndex = p.first;
            list.push_back(p.second);
        }
    }
    long long int result = 0;
    switch(currId){
        case 0 : 
            result = sumOfPackets(list);
            break;
        case 1 : 
            result = productOfPackets(list);
            break;
        case 2 : 
            result = minOfPackets(list);
            break;
        case 3 : 
            result = maxOfPackets(list);
            break;
        case 5 : 
            result = greaterThan(list);
            break;
        case 6 : 
            result = lessThan(list);
            break;
        case 7 : 
            result = equalTo(list);
            break;
        default:
            break;
    }
    return make_pair(currIndex, result);

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
    long long int result = decode(0).second;
    cout<<result<<endl;
    return 0;
}