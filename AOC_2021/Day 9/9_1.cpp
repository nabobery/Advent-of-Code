#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
using namespace std;

int main(){
    ifstream in("input.txt");
    if (!in.is_open()) {
        cerr << "Could not open the file - '"<< endl;
        return EXIT_FAILURE;
    }
    int input[100][100];
    for(int i = 0; i < 100; i++) {
        string str;
        getline(in,str);
        for(int j = 0; j < 100; j++) {
            input[i][j] = str[j] - '0';
        }
    }
    /*for(int j = 0; j < 100; j++) {
        cout << input[0][j];
    }*/
    int result = 0;
    for(int i = 0; i < 100; i++) {
        for(int j = 0; j < 100; j++) {
            if(i == 0){
                if(j == 0){
                    if(input[i][j] < input[i][j+1] && input[i][j] < input[i+1][j]){
                        result = result + input[i][j] + 1;
                    }
                }
                else if(j == 99){
                    if(input[i][j] < input[i][j-1] && input[i][j] < input[i+1][j]){
                        result = result + input[i][j] + 1;
                    }
                }
                else{
                    if(input[i][j] < input[i][j+1] && input[i][j] < input[i][j-1] && input[i][j] < input[i+1][j]){
                        result = result + input[i][j] + 1;
                    }
                }
            }
            else if(i == 99){
                if(j == 0){
                    if(input[i][j] < input[i-1][j] && input[i][j] < input[i][j+1]){
                        result = result + input[i][j] + 1;
                    }
                }
                else if(j == 99){
                    if(input[i][j] < input[i][j-1] && input[i][j] < input[i-1][j]){
                        result = result + input[i][j] + 1;
                    }
                }
                else{
                    if(input[i][j] < input[i][j+1] && input[i][j] < input[i-1][j] && input[i][j] < input[i][j-1]){
                        result = result + input[i][j] + 1;
                    }
                }
            }
            else if(j == 0 && (i > 0 && i < 99)){
                if(input[i][j] < input[i][j+1] && input[i][j] < input[i-1][j] && input[i][j] < input[i+1][j]){
                    result = result + input[i][j] + 1;
                }
            }
            else if(j == 99 && (i > 0 && i < 99)){
                if(input[i][j] < input[i][j-1] && input[i][j] < input[i-1][j] && input[i][j] < input[i+1][j]){
                    result = result + input[i][j] + 1;
                }
            }
            else if((i > 0 && i < 99) && (j > 0 && j < 99)){
                if(input[i][j] < input[i + 1][j] && input[i][j] < input[i - 1][j] && input[i][j] < input[i][j-1] && input[i][j] < input[i][j+1]){
                    result = result + input[i][j] + 1;
                }
            }
        }
    }
    cout<<result<<endl;
    return 0;
}