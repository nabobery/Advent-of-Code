#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <climits>

using namespace std;

struct mapping{
    long long source, destination, range;
};

// comparator for sort mappings by source
bool cmp(mapping a, mapping b){
    return a.source < b.source;
}

class Solution
{
    public:
    vector<long long> seeds;
    vector<vector<mapping>> maps;
    void init(ifstream& input){
        string line;
        regex pattern("\\b(\\d+)\\s+(\\d+)\\s+(\\d+)\\b");
        while (getline(input, line))
        {
            if(line.find("seeds:") != string::npos){
                regex re("\\d+");
                sregex_iterator next(line.begin(), line.end(), re);
                sregex_iterator end;
                while (next != end) {
                    smatch match = *next;
                    seeds.push_back(stoll(match.str()));
                    next++;
                }
            }
            else if(line.find("map:") != string::npos){
                vector<mapping> map;
                while(getline(input, line)){
                    smatch match;
                    if(regex_search(line, match, pattern)){
                        mapping m;
                        m.destination = stoll(match[1]);
                        m.source = stoll(match[2]);
                        m.range = stoll(match[3]);
                        map.push_back(m);
                    }
                    else{
                        maps.push_back(map);
                        break;
                    }
                }
            }
        }
    }

    // just find mapping for each seed and get the location of the seed
    long long getLocation(long long seed){
        long long location = seed;
        for(auto it : maps){
            for(auto m : it){
                if(m.source <= location && location < m.source+ m.range){
                    location += m.destination - m.source;
                    break;
                }
            }
        }
        return location;
    }

    // find all possible locations for each seed and get the minimum
    void part1(){
        long long result = LLONG_MAX;
        for(auto it : seeds){
            result = min(result, getLocation(it));
        }
        cout<<"Part 1: "<< result <<"\n";
    }

    void part2(){
        long long result = LLONG_MAX;
        // sort all mappings by source
        for(auto& it : maps){
            sort(it.begin(), it.end(), cmp);
        }
        // go through each seed range (start, range length)
        for(int i = 0; i < seeds.size(); i+= 2){
            // this vector will hold all possible (start, range length) for intemediate/location for the given seed range
            vector<pair<long long, long long>> mapRanges = {{seeds[i], seeds[i+1]}};
            // go through each mapping such as seed->soil->fertilizer->water->light->temperature->humidity->location
            for(auto& it : maps){
                // this vector will hold all possible (start, range length) for current intemediate(mapping) for the seed
                vector<pair<long long, long long>> newRanges;
                // go through each value in mapRanges and update it based on current mapping
                for(auto& t : mapRanges){
                    // this vector will hold all possible ranges for current mapping for the seed if there's any intersection or not
                    vector<pair<long long, long long>> temp;
                    // go through each mapping and update the (start, range length)
                    for(auto& m : it){
                        long long offset = m.destination - m.source;
                        // if there's overlap between current mapping and current (start, range length)
                        if(t.first >= m.source && t.first < m.source + m.range){
                            // if the current mapping covers the whole range
                            if(m.source + m.range >= t.first + t.second){
                                temp.push_back({t.first + offset, t.second});
                            }
                            // if the current mapping covers only part of the (start, range length)
                            else{
                                // update the range by updating start and range length and add the overlapped part to temp
                                t.second -= (m.source + m.range - t.first);
                                temp.push_back({t.first + offset, m.source + m.range - t.first});
                                t.first = m.source + m.range;
                            }
                        }
                    }
                    // if there's no intersection between current mapping and current range, just add the current range to temp
                    if(temp.empty()){
                        newRanges.push_back(t);
                    }
                    // if there's intersection, add all possible ranges to newRanges
                    else{
                        newRanges.insert(newRanges.end(), temp.begin(), temp.end());
                    }
                }
                // update mapRanges with newRanges
                mapRanges = newRanges;
            }
            // get the minimum location for the current seed range
            auto minElement = std::min_element(mapRanges.begin(), mapRanges.end(),
            [](const auto& a, const auto& b) {
                return a.first < b.first;
            }); 
            // update the result
            result = min(minElement->first, result);
        }
        cout<<"Part 2: "<< result <<"\n";
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