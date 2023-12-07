#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

// Ordering
// Five of a kind
// Four of a kind
// Full house
// Three of a kind
// Two pairs
// One pair
// High card
int ordering(string s, bool newOrder){
    unordered_map<char, int> m;
    for (int i = 0; i < s.length(); i++)
    {
        m[s[i]]++;
    }
    if(!newOrder || m.count('J') == 0){
        // Five of a kind
        if(m.size() == 1) return 7;
        // Four of a kind or Full house
        if(m.size() == 2){
            for (auto i : m)
            {
                if(i.second == 4) return 6;
                if(i.second == 3) return 5;
            }
        }
        // Three of a kind or Two pairs
        if(m.size() == 3){
            for (auto i : m)
            {
                if(i.second == 3) return 4;
                if(i.second == 2) return 3;
            }
        }
        // One pair
        if(m.size() == 4) return 2;
        // High card
        return 1;
    }
    int cnt = m['J'];
    // 5 => five of a kind
    // 4,1 / 3,2 => can be made into fice of A KIND
    if(m.size() == 1 || m.size() == 2) return 7;
    // 3,1,1 => four of a kind
    // 2,2,1 => full house/four of a kind
    if(m.size() == 3){
        for (auto i : m)
        {
            if(i.second == 3) return 6;
            if(i.second == 2) {
                if(cnt == 1) return 5;
                return 6;
            }
        }
    }
    // 2,1,1,1 => three of a kind / two pair but 2 pair always better
    if(m.size() == 4){
        return 4;
    }
    // 1,1,1,1,1 => high card => one pair 
    return 2;
}

// card order : A, K, Q, J, T, 9, 8, 7, 6, 5, 4, 3, or 2
int cardOrder(char c, bool newOrder){
    if(c == 'A') return 14;
    if(c == 'K') return 13;
    if(c == 'Q') return 12;
    if(c == 'J'){
        if(newOrder) return 1;
        return 11;
    }
    if(c == 'T') return 10;
    return c - '0';
}

bool cmp(pair<string, int> a, pair<string, int> b){
    if(ordering(a.first, false) != ordering(b.first, false)){
        return ordering(a.first, false) < ordering(b.first, false);
    }
    else{
        for(int i = 0; i < a.first.length(); i++){
            if(cardOrder(a.first[i],false) != cardOrder(b.first[i], false))
                return cardOrder(a.first[i], false) < cardOrder(b.first[i], false);
        }
    }
    return a.second < b.second;
}

bool cmp2(pair<string, int> a, pair<string, int> b){
    if(ordering(a.first, true) != ordering(b.first, true)){
        return ordering(a.first, true) < ordering(b.first, true);
    }
    else{
        for(int i = 0; i < a.first.length(); i++){
            if(cardOrder(a.first[i],true) != cardOrder(b.first[i], true))
                return cardOrder(a.first[i], true) < cardOrder(b.first[i], true);
        }
    }
    return a.second < b.second;
}

class Solution
{
    public:
    vector<pair<string, int>> hands;
    void init(ifstream& input){
        string line;
        while (getline(input, line))
        {
            hands.push_back(make_pair(line.substr(0, line.find(" ")), stoi(line.substr(line.find(" ")+1))));   
        }
    }
    void part1(){
        long long result = 0;
        sort(hands.begin(), hands.end(), cmp);
        for (int i = 0; i < hands.size(); i++)
        {
            result += hands[i].second * (i+1);
        }
        cout<<"Part 1: "<<result<<"\n";
    }
    void part2(){
        long long result = 0;
        sort(hands.begin(), hands.end(), cmp2);
        for (int i = 0; i < hands.size(); i++)
        {
            result += hands[i].second * (i+1);
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