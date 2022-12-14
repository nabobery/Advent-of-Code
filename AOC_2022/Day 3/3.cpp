#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cctype>
#include <vector>
using namespace std;

int main(){
    ifstream in("input.txt");
    if (!in.is_open())
    {
        cerr << "Could not open the file - '" << endl;
        return EXIT_FAILURE;
    }
    string line;
    int result = 0,size = 0;
    int counter1[52],counter2[52],counter3[52];
    while (getline(in,line))
    {
        memset(counter1,0,sizeof(counter1));
        memset(counter2,0,sizeof(counter2));
        for(int i = 0; i < line.length()/2;i++){
            if(islower(line[i]))
                counter1[line[i] - 'a']++;
            else
                counter1[line[i] - 'A' + 26]++;
        }
        for(int i = line.length()/2;i < line.length();i++){
            if(islower(line[i]))
                counter2[line[i] - 'a']++;
            else
                counter2[line[i] - 'A' + 26]++;
        }
        for(int i = 0; i < 52;i++){
            if(counter1[i] != 0 && counter2[i] != 0){
                result += (i+1);
                break;
            }
        }
        size++;
    }
    cout<<result<<"\n";
    in.clear();
    in.seekg(0, ios::beg);
    vector<string> lines(size);
    int c = 0;
    result = 0;
    while(getline(in,line))
    {
        lines[c] = line;
        c++;
    }
    for(int i = 0; i < lines.size();i++){
        memset(counter1,0,sizeof(counter1));
        memset(counter2,0,sizeof(counter2));
        memset(counter3,0,sizeof(counter3));
        for (int j = 0; j < lines[i].length(); j++)
        {
            if(islower(lines[i][j]))
                counter1[lines[i][j] - 'a']++;
            else
                counter1[lines[i][j] - 'A' + 26]++;
        }
        i++;
        for (int j = 0; j < lines[i].length(); j++)
        {
            if(islower(lines[i][j]))
                counter2[lines[i][j] - 'a']++;
            else
                counter2[lines[i][j] - 'A' + 26]++;
        }
        i++;
        for (int j = 0; j < lines[i].length(); j++)
        {
            if(islower(lines[i][j]))
                counter3[lines[i][j] - 'a']++;
            else
                counter3[lines[i][j] - 'A' + 26]++;
        }
        for(int i = 0; i < 52;i++){
            if(counter1[i] != 0 && counter2[i] != 0 && counter3[i] != 0){
                result += (i+1);
                break;
            }
        }
    }
    cout<<result<<"\n";
    return 0;
}