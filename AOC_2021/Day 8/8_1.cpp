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
    int result = 0, size;
    for(int i=0; i < 200; i++){
        string str;
        getline(in,str);
        stringstream x(str);
        string temp;
        while (getline(x, str, '|')) {  
            temp = str;
        }
        stringstream v(temp);
        string tmp2;
        while(getline(v, tmp2,' ')){
            size = tmp2.length();
            if(size == 2 || size == 3 || size == 4 || size == 7){
                result++;
            }
        }
    }
    cout << result << endl;
    return 0;
}