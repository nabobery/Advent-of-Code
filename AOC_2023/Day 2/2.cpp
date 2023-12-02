// 12 red cubes, 13 green cubes, and 14 blue cubes

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct Iteration{
    int red, green, blue;
};

class Game{
    public:
    vector<Iteration> iterations;
    int minRed = 0, minGreen = 0, minBlue = 0;
    Game(string line){
        string inp = line.substr(line.find(":")+2);
        stringstream ss(inp);
        string token;
        while (getline(ss, token, ';'))
        {
            Iteration i;
            stringstream ss2(token);
            string token2;
            int red = 0, green = 0, blue = 0;
            while (getline(ss2, token2, ','))
            {
                stringstream ss3(token2);
                int num;
                ss3 >> num;
                if(token2.find("red") != string::npos){
                    red = num;
                }else if(token2.find("green") != string::npos){
                    green = num;
                }else if(token2.find("blue") != string::npos){
                    blue = num;
                }
            }
            i.red = red; i.green = green; i.blue = blue;
            iterations.push_back(i);
        }
    }
    bool isGamePlayable(int red, int green, int blue){
        for (int i = 0; i < iterations.size(); i++)
        {
            if(iterations[i].red > red || iterations[i].green > green || iterations[i].blue > blue){
                return false;
            }
        }
        return true;
    }

    int power(){
        int result;
        for (int i = 0; i < iterations.size(); i++)
        {
            minRed = max(minRed, iterations[i].red);
            minGreen = max(minGreen, iterations[i].green);
            minBlue = max(minBlue, iterations[i].blue);
        }
        result = minRed*minGreen*minBlue;
        return result;
    }

};

class Solution
{
    public:
    int red = 12, green = 13, blue = 14;
    vector<Game> games;
    void init(ifstream& input){
        string line;
        while (getline(input, line))
        {
            games.push_back(Game(line));
        }
    }
    void part1(){
        int result = 0;
        for (int i = 0; i < games.size(); i++)
        {
            if(games[i].isGamePlayable(red, green, blue)){
                result += (i+1);
            }
        }
        cout<<"Part 1: "<<result<<"\n";
    }
    void part2(){
        int result = 0;
        for (int i = 0; i < games.size(); i++)
        {
            result += games[i].power();
        }
        cout<<"Part 2: "<<result<<"\n";
    }

};

int main(){
    ifstream in("input.txt");
    if (!in.is_open())
    {
        cerr << "Could not open the file - '" << endl;
        return EXIT_FAILURE;
    }
    Solution s;
    s.init(in);
    s.part1();
    s.part2();
    return 0;
}