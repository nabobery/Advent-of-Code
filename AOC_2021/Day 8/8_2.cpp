#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<algorithm>
#include <set>
using namespace std;

int subtract(string s1, string s2){
    set<char> temp;
    for(int i=0; i< s1.length(); i++){
        temp.insert(s1[i]);
    }
    for(int i = 0; i < s2.length() ; i++){
        if(temp.find(s2[i]) != temp.end()){
            temp.erase(s2[i]);
        }
    }
    return(temp.size());
}

int main(){
    ifstream in("input.txt");
    if (!in.is_open()) {
        cerr << "Could not open the file - '"<< endl;
        return EXIT_FAILURE;
    }
    int j;
    int result = 0;
    for(int i=0; i < 200; i++){
        int result1 = 0;
        string str;
        getline(in,str);
        stringstream x(str);
        string temp[2];
        j = 0;
        while (getline(x, str, '|')) {  
            temp[j] = str;
            j++;
        }
        string input[10];
        stringstream temp1(temp[0]);
        j = 0;
        while (getline(temp1, str, ' ')) {  
            input[j] = str;
            j++;
        }
        string numbers[10];
        for(int k=0; k < 10; k++) {
            if(input[k].size() == 2){
                numbers[1] = input[k];
            }
            else if(input[k].size() == 3){
                numbers[7] = input[k];
            }
            else if(input[k].size() == 4){
                numbers[4] = input[k];
            }
            else if(input[k].size() == 7){
                numbers[8] = input[k];
            } 
        }
        for(int k=0; k < 10; k++) {
            if(input[k].size() == 6){
                if(subtract(input[k], numbers[1]) == 5){
                    numbers[6] = input[k];
                }
                else if(subtract(input[k], numbers[4]) == 2){
                    numbers[9] = input[k];
                }
                else{
                    numbers[0] = input[k];
                }
            }
            else if(input[k].size() == 5){
                if(subtract(input[k], numbers[4]) == 3){
                    numbers[2] = input[k];
                }
                else if(subtract(input[k], numbers[1]) == 3){
                    numbers[3] = input[k];
                }
                else{
                    numbers[5] = input[k];
                }
            }
        }
        /*for(int k=0; k < 10; k++){
            cout<<input[k]<<endl;
        }*/
        /*for(int k =0; k < 10; k++){
            cout<< k<<" "<<numbers[k]<<endl;
        }*/
        string output[4];
        stringstream temp2(temp[1]);
        j = 0;
        while (getline(temp2, str, ' ')) {  
            if (str.length() == 0) {
                  continue;
            }
            output[j] = str;
            j++;
        }
        for(int k = 0; k < 10; k++){
            sort(numbers[k].begin(), numbers[k].end());
        }
        for(int k = 0; k < 4; k++){
            sort(output[k].begin(), output[k].end());
            for(int l = 0; l < 10; l++){
                if(numbers[l] == output[k]){
                    result1 = result1*10 + l;
                }
            }
        }
        /*for(int k=0; k < 4; k++){
            cout<<output[k]<<endl;
        }*/   
        result += result1;
    }
    cout<<result<<endl;
    return 0;
}