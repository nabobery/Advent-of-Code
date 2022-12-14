#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

int main()
{
    ifstream in("input.txt");
    if (!in.is_open())
    {
        cerr << "Could not open the file - '" << endl;
        return EXIT_FAILURE;
    }
    string line;
    int result = 0, curr = 0;
    while (getline(in, line))
    {
        if (line.empty()){
            if(curr> result) result = curr;
            curr = 0;
        }  
        else
            curr += stoi(line);
    }
    if(curr > result) result = curr;
    cout<<result<<"\n";
    return 0;
}