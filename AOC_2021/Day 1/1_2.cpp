#include<iostream>
#include<fstream>
#include <vector>
using namespace std;

int main(){
    string filename("input.txt");
    vector<int> numbers;
    int number;
    ifstream input_file(filename);
     if (!input_file.is_open()) {
        cerr << "Could not open the file - '" << filename << "'" << endl;
        return EXIT_FAILURE;
    }

    while (input_file >> number) {
        numbers.push_back(number);
    }
    int result = 0;
    for(int i = 3; i < numbers.size(); i ++) {
        if(numbers[i] > numbers[i - 3]){
            result++;
        }
    }
    cout << result<<"\n";
    input_file.close();

    return EXIT_SUCCESS;
}