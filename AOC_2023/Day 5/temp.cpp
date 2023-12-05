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

bool cmp(mapping a, mapping b){
    return a.source < b.source;
}

class Solution
{
    public:
    string args[8] = {"seeds:", "seed-to-soil map:", "soil-to-fertilizer map:", "fertilizer-to-water map:", "water-to-light map:", "light-to-temperature map:", "temperature-to-humidity map:", "humidity-to-location map:"};
    vector<long long> seeds;
    vector<mapping> seed_to_soil;
    vector<mapping> soil_to_fertilizer;
    vector<mapping> fertilizer_to_water;
    vector<mapping> water_to_light;
    vector<mapping> light_to_temperature;
    vector<mapping> temperature_to_humidity;
    vector<mapping> humidity_to_location;
    void init(ifstream& input){
        string line;
        while (getline(input, line))
        {
            if(line.find(args[0]) != string::npos){
                regex re("\\d+");
                sregex_iterator next(line.begin(), line.end(), re);
                sregex_iterator end;
                while (next != end) {
                    smatch match = *next;
                    seeds.push_back(stoll(match.str()));
                    next++;
                }
            }
            else if(line.find(args[1]) != string::npos){
                regex pattern("\\b(\\d+)\\s+(\\d+)\\s+(\\d+)\\b");
                while(getline(input, line)){
                    smatch match;
                    if(regex_search(line, match, pattern)){
                        mapping m;
                        m.destination = stoll(match[1]);
                        m.source = stoll(match[2]);
                        m.range = stoll(match[3]);
                        seed_to_soil.push_back(m);
                    }
                    else break;
                }
            }
            else if(line.find(args[2]) != string::npos){
                regex pattern("\\b(\\d+)\\s+(\\d+)\\s+(\\d+)\\b");
                while(getline(input, line)){
                    smatch match;
                    if(regex_search(line, match, pattern)){
                        mapping m;
                        m.destination = stoll(match[1]);
                        m.source = stoll(match[2]);
                        m.range = stoll(match[3]);
                        soil_to_fertilizer.push_back(m);
                    }
                    else break;
                }
            }
            else if(line.find(args[3]) != string::npos){
                regex pattern("\\b(\\d+)\\s+(\\d+)\\s+(\\d+)\\b");
                while(getline(input, line)){
                    smatch match;
                    if(regex_search(line, match, pattern)){
                        mapping m;
                        m.destination = stoll(match[1]);
                        m.source = stoll(match[2]);
                        m.range = stoll(match[3]);
                        fertilizer_to_water.push_back(m);
                    }
                    else break;
                }
            }
            else if(line.find(args[4]) != string::npos){
                regex pattern("\\b(\\d+)\\s+(\\d+)\\s+(\\d+)\\b");
                while(getline(input, line)){
                    smatch match;
                    if(regex_search(line, match, pattern)){
                        mapping m;
                        m.destination = stoll(match[1]);
                        m.source = stoll(match[2]);
                        m.range = stoll(match[3]);
                        water_to_light.push_back(m);
                    }
                    else break;
                }
            }
            else if(line.find(args[5]) != string::npos){
                regex pattern("\\b(\\d+)\\s+(\\d+)\\s+(\\d+)\\b");
                while(getline(input, line)){
                    smatch match;
                    if(regex_search(line, match, pattern)){
                        mapping m;
                        m.destination = stoll(match[1]);
                        m.source = stoll(match[2]);
                        m.range = stoll(match[3]);
                        light_to_temperature.push_back(m);
                    }
                    else break;
                }
            }
            else if(line.find(args[6]) != string::npos){
                regex pattern("\\b(\\d+)\\s+(\\d+)\\s+(\\d+)\\b");
                while(getline(input, line)){
                    smatch match;
                    if(regex_search(line, match, pattern)){
                        mapping m;
                        m.destination = stoll(match[1]);
                        m.source = stoll(match[2]);
                        m.range = stoll(match[3]);
                        temperature_to_humidity.push_back(m);
                    }
                    else break;
                }
            }
            else if(line.find(args[7]) != string::npos){
                regex pattern("\\b(\\d+)\\s+(\\d+)\\s+(\\d+)\\b");
                while(getline(input, line)){
                    smatch match;
                    if(regex_search(line, match, pattern)){
                        mapping m;
                        m.destination = stoll(match[1]);
                        m.source = stoll(match[2]);
                        m.range = stoll(match[3]);
                        humidity_to_location.push_back(m);
                    }
                    else break;
                }
            }
        }
    }
    void print(){
        for(int i = 0; i < 8; i++){
            cout<< args[i] <<"\n";
            if(i == 0){
                for(auto it = seeds.begin(); it != seeds.end(); it++){
                    cout<< *it << " ";
                }
                cout<<"\n";
            }
            else if(i == 1){
                for(int j = 0; j < seed_to_soil.size(); j++){
                    cout<< seed_to_soil[j].destination << " " << seed_to_soil[j].source << " " << seed_to_soil[j].range << "\n";
                }
            }
            else if(i == 2){
                for(int j = 0; j < soil_to_fertilizer.size(); j++){
                    cout<< soil_to_fertilizer[j].destination << " " << soil_to_fertilizer[j].source << " " << soil_to_fertilizer[j].range << "\n";
                }
            }
            else if(i == 3){
                for(int j = 0; j < fertilizer_to_water.size(); j++){
                    cout<< fertilizer_to_water[j].destination << " " << fertilizer_to_water[j].source << " " << fertilizer_to_water[j].range << "\n";
                }
            }
            else if(i == 4){
                for(int j = 0; j < water_to_light.size(); j++){
                    cout<< water_to_light[j].destination << " " << water_to_light[j].source << " " << water_to_light[j].range << "\n";
                }
            }
            else if(i == 5){
                for(int j = 0; j < light_to_temperature.size(); j++){
                    cout<< light_to_temperature[j].destination << " " << light_to_temperature[j].source << " " << light_to_temperature[j].range << "\n";
                }
            }
            else if(i == 6){
                for(int j = 0; j < temperature_to_humidity.size(); j++){
                    cout<< temperature_to_humidity[j].destination << " " << temperature_to_humidity[j].source << " " << temperature_to_humidity[j].range << "\n";
                }
            }
            else if(i == 7){
                for(int j = 0; j < humidity_to_location.size(); j++){
                    cout<< humidity_to_location[j].destination << " " << humidity_to_location[j].source << " " << humidity_to_location[j].range << "\n";
                }
            }
        }
    }

    long long getLocation(long long seed){
        long long soil = seed;
        for(auto it : seed_to_soil){
            if(it.source <= seed && seed < it.source + it.range){
                soil = it.destination + seed - it.source;
                break;
            }
        }
        long long fertilizer = soil;
        for(auto it : soil_to_fertilizer){
            if(it.source <= soil && soil < it.source + it.range){
                fertilizer = it.destination + soil - it.source;
                break;
            }
        }
        long long water = fertilizer;
        for(auto it : fertilizer_to_water){
            if(it.source <= fertilizer && fertilizer < it.source + it.range){
                water = it.destination + fertilizer - it.source;
                break;
            }
        }
        long long light = water;
        for(auto it : water_to_light){
            if(it.source <= water && water < it.source + it.range){
                light = it.destination + water - it.source;
                break;
            }
        }
        long long temperature = light;
        for(auto it : light_to_temperature){
            if(it.source <= light && light < it.source + it.range){
                temperature = it.destination + light - it.source;
                break;
            }
        }
        long long humidity = temperature;
        for(auto it : temperature_to_humidity){
            if(it.source <= temperature && temperature < it.source + it.range){
                humidity = it.destination + temperature - it.source;
                break;
            }
        }
        long long location = humidity;
        for(auto it : humidity_to_location){
            if(it.source <= humidity && humidity < it.source + it.range){
                location = it.destination + humidity - it.source;
                break;
            }
        }
        return location;
    }

    bool validSeed(long long location){
        long long humidity = location;
        for(auto it : humidity_to_location){
            if(it.destination <= location && location < it.destination + it.range){
                humidity = it.source + location - it.destination;
                break;
            }
        }
        long long temperature = humidity;
        for(auto it : temperature_to_humidity){
            if(it.destination <= humidity && humidity < it.destination + it.range){
                temperature = it.source + humidity - it.destination;
                break;
            }
        }
        long long light = temperature;
        for(auto it : light_to_temperature){
            if(it.destination <= temperature && temperature < it.destination + it.range){
                light = it.source + temperature - it.destination;
                break;
            }
        }
        long long water = light;
        for(auto it : water_to_light){
            if(it.destination <= light && light < it.destination + it.range){
                water = it.source + light - it.destination;
                break;
            }
        }
        long long fertilizer = water;
        for(auto it : fertilizer_to_water){
            if(it.destination <= water && water < it.destination + it.range){
                fertilizer = it.source + water - it.destination;
                break;
            }
        }
        long long soil = fertilizer;
        for(auto it : soil_to_fertilizer){
            if(it.destination <= fertilizer && fertilizer < it.destination + it.range){
                soil = it.source + fertilizer - it.destination;
                break;
            }
        }
        bool result = false;
        for(auto it : seed_to_soil){
            if(it.destination <= soil && soil < it.destination + it.range){
                long long seed = it.source + soil - it.destination;
                for(int i = 0; i < seeds.size(); i+= 2){
                    if(seeds[i] <= seed && seed < seeds[i] + seeds[i+1]){
                        result = true;
                        break;
                    }
                }
                if(result) break;
            }
        }
        return result;
    }

    void part1(){
        long long result = LLONG_MAX;
        for(auto it : seeds){
            //cout<< it << " " << getLocation(it) << "\n";
            result = min(result, getLocation(it));
        }
        cout<<"Part 1: "<< result <<"\n";
    }

    void part2(){
        long long mn = LLONG_MAX, mx = LLONG_MIN;
        for(auto it : humidity_to_location){
            mn = min(mn, it.destination);
            mx = max(mx, it.destination);
        }
        long long result = mn;
        for(long long i = mn; i <= mx; i++){
            if(validSeed(i)){
                result = i;
                break;
            }
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