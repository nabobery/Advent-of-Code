#include<iostream>
#include<fstream>
#include <sstream>
#include <vector>
using namespace std;

int main(){
    int horizontal = 0, depth = 0, number;
    string line, direction;
    ifstream input_file("input.txt");
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"<< endl;
        return EXIT_FAILURE;
    }
    while (getline(input_file, line)){
        istringstream temp(line);
        temp>>direction;
        temp>>number;
        if(direction == "forward"){
            horizontal += number;
        }
        else if(direction == "up"){
            depth -= number;
        }
        else{
            depth += number;
        }

    }
    int result = horizontal*depth;
    cout << result << endl;

    return 0;
}