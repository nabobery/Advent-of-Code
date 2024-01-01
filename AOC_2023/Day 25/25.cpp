#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <climits>  

using namespace std;

class Solution
{
    public:
    unordered_map<string, int> vertex_to_int;
    unordered_map<int, unordered_set<int>> graph;
    void init(ifstream& input){
        string line;
        regex pattern(R"([a-z]{3})");
        int i = 0;
        while (getline(input, line))
        {
            regex_iterator<string::iterator> it(line.begin(), line.end(), pattern);
            regex_iterator<string::iterator> end;
            string a = it->str();
            it++;
            if (!vertex_to_int.count(a))
                vertex_to_int[a] = i++;
            while (it != end){
                string b = it->str();
                if (!vertex_to_int.count(b))
                    vertex_to_int[b] = i++;
                graph[vertex_to_int[a]].insert(vertex_to_int[b]);
                graph[vertex_to_int[b]].insert(vertex_to_int[a]);
                it++;
            }
        }
    }

    // O(V^3) Algorithm for Global Min Cut using Stoer-Wagner Algorithm
    pair<int, vector<int>> globalMinCut() {
        // initialize best which is a pair of min cut (int) and first partition (vector<int>)
        pair<int, vector<int>> best = {INT_MAX, {}};
        // n is the number of vertices
        int n = graph.size();
        // mat is the adjacency matrix
        vector<vector<int>> mat(n, vector<int>(n, 0));
        // fill the adjacency/weight matrix
        for (auto& [src, neighbors] : graph) {
            for (int dest : neighbors) {
                mat[src][dest] = 1;
            }
        }
        // co is the vector of vectors of vertices in each partition
        vector<vector<int>> co(n);
        // initialize each partition with its own vertex
        for(int i = 0; i < n;i++) co[i] = {i};
        // go over n-1 phases and 0th vertex is always picked first in each phase
        for(int ph = 1; ph < n;ph++) {
            // w is the vector of weights of 0th vertex
            vector<int> w = mat[0];
            // choose s and t as the same vertex (0th vertex)
            size_t s = 0, t = 0;
            // go over n-ph phases
            for(int j = 0; j < n-ph;j++){
                // mark t as visited
                w[t] = INT_MIN;
                // find the vertex t has maximum connectivity to the previously visited vertices
                s = t, t = max_element(w.begin(), w.end()) - w.begin();
                // update the weights of each vertex
                for(int i = 0; i < n;i++) w[i] += mat[t][i];
            }
            // update best if we found a better min cut
            best = min(best, {w[t] - mat[t][t], co[t]});
            // merge s and t into s
            co[s].insert(co[s].end(), co[t].begin(), co[t].end());
            // update adjacency matrix
            for(int i = 0; i < n;i++) mat[s][i] += mat[t][i];
            for(int i = 0; i < n;i++) mat[i][s] = mat[s][i];
            // mark t as visited
            mat[0][t] = INT_MIN;
        }
        return best;
    }

    void part1(){
        auto [minCut, nodes] = globalMinCut();
        int result = nodes.size();
        result *= (vertex_to_int.size() - nodes.size());
        cout<<"Part 1: "<<result<<"\n";
    }

    void part2(){
        cout<<"Happy Christmas!\n THE END\n";
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