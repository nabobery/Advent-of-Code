#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

class Solution
{
    public:
    vector<string> hashes;
    void init(ifstream& input){
        string line;
        while (getline(input, line))
        {
            stringstream ss(line);
            string token;
            while (getline(ss, token, ','))
            {
                hashes.push_back(token);
            }
        }
    }

    int hashAlgorithm(string hash){
        int result = 0;
        for (int i = 0; i < hash.length(); i++)
        {
            result += hash[i];
            result *= 17;
            result %= 256;
        }
        return result;
    }

    void part1(){
        int result = 0;
        for (int i = 0; i < hashes.size(); i++)
        {
            result += hashAlgorithm(hashes[i]);
        }
        cout<<"Part 1: "<<result<<"\n";
    }
    void part2(){
        int result = 0;
        vector<map<string,pair<int,int>>> boxes(256);
        for(auto hash : hashes){
            if(hash.find("=") != string::npos){
                int index = hash.find("=");
                string label = hash.substr(0,index);
                int boxNum = hashAlgorithm(label);
                int lens = stoi(hash.substr(index+1));
                if(boxes[boxNum].find(label) != boxes[boxNum].end()){
                    boxes[boxNum][label].first = lens;
                }
                else{
                    boxes[boxNum][label] = make_pair(lens, boxes[boxNum].size());
                }
            }
            else if(hash.find("-") != string::npos){
                int index = hash.find("-");
                string label = hash.substr(0,index);
                int boxNum = hashAlgorithm(label);
                if(boxes[boxNum].find(label) != boxes[boxNum].end()){
                    int pos = boxes[boxNum][label].second;
                    boxes[boxNum].erase(label);
                    for(auto it = boxes[boxNum].begin(); it != boxes[boxNum].end(); it++){
                        if(it->second.second > pos){
                            it->second.second--;
                        }
                    }
                }
            }
        }
        for(int i = 0; i < boxes.size(); i++){
            for(auto it = boxes[i].begin(); it != boxes[i].end(); it++){
                result += (it->second.second+1)*(i+1)*(it->second.first);
            }
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