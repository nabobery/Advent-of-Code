#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <unordered_set>
#include <unordered_map>

using namespace std;

class Card{
    public:
        unordered_set<int> winning_number, numbers;
};

string trimWhiteSpace(string& input) {
    size_t start = input.find_first_not_of(" \t\n\r");
    size_t end = input.find_last_not_of(" \t\n\r");
    if (start == std::string::npos || end == std::string::npos) {
        return "";
    }
    return input.substr(start, end - start + 1);
}

bool isOver(unordered_map<int, int>& not_visited){
    bool b = true;
    for(auto it : not_visited){
        if(it.second != 0){
            b = false;
            break;
        }
    }
    return b;
}

class Solution
{
    public:
    vector<Card> cards;
    void init(ifstream& input){
        string line;
        while(getline(input, line)){
            string card = line.substr(line.find(":")+1);
            Card c;
            string win_nums = card.substr(0, card.find(" |")), nums = card.substr(card.find("|")+1), temp;
            for(int i = 0; i < win_nums.size(); i+= 3){
                temp = win_nums.substr(i, 3);
                c.winning_number.insert(stoi(trimWhiteSpace(temp)));
            }
            for(int i = 0; i < nums.size(); i+= 3){
                temp = nums.substr(i, 3);
                c.numbers.insert(stoi(trimWhiteSpace(temp)));
            }
            cards.push_back(c);
        }
    }
    void part1(){
        long long result = 0;
        for(Card c: cards){
            int cnt = 0;
            for(int num: c.numbers){
                if(c.winning_number.count(num)){
                    cnt++;
                }
            }
            if(cnt > 0) 
                result += pow(2, cnt-1);
        }
        cout<<"Part 1: "<<result<<"\n";
    }

    void part2(){
        int result = 0;
        vector<int> scores;
        for(Card c: cards){
            int cnt = 0;
            for(int num: c.numbers){
                if(c.winning_number.count(num)){
                    cnt++;
                }
            }
            scores.push_back(cnt);
        }
        vector<int> visited(scores.size(), 1);
        for(int i = 0; i < scores.size(); i++){
            for(int j = i+1; j <= i+scores[i]; j++){
                visited[j] += visited[i];
            }
        }
        for(int i = 0; i < scores.size(); i++){
            result += visited[i];
        }
        cout<<"Part 2: "<<result<<"\n";
    }
    // void part2(){
    //     long long result = 0;
    //     vector<int> scores;
    //     for(Card c: cards){
    //         int cnt = 0;
    //         for(int num: c.numbers){
    //             if(c.winning_number.count(num)){
    //                 cnt++;
    //             }
    //         }
    //         scores.push_back(cnt);
    //     }
    //     unordered_map<int, int> visited, not_visited;
    //     for(int i = 0; i < scores.size(); i++){
    //         not_visited[i] = 1;
    //     }
    //     while(!isOver(not_visited)){
    //         for(auto it : not_visited){
    //             visited[it.first] += it.second;
    //             for(int i = it.first+1; i <= it.first+scores[it.first]; i++){
    //                 not_visited[i] += it.second;
    //             }
    //             not_visited[it.first] = 0;
    //         }
    //     }
    //     for(auto it : visited){
    //         result += it.second;
    //     }
    //     cout<<"Part 2: "<<result<<"\n";
    // }

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