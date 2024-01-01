#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <regex>
#include <fstream>
#include <string>
#include <iterator>

using namespace std;

using namespace std;

class Solution
{
    public:
    unordered_map<string, unordered_set<string>> adj;
    unordered_map<string, int> vertex_to_int;
    unordered_map<int, unordered_set<int>> graph;
    void init(ifstream& input){
        string line;
        regex pattern(R"([a-z]{3})");
        while (getline(input, line))
        {
            regex_iterator<string::iterator> it(line.begin(), line.end(), pattern);
            regex_iterator<string::iterator> end;
            string a = it->str();
            it++;
            while (it != end){
                string b = it->str();
                adj[a].insert(b);
                adj[b].insert(a);
                it++;
            }
        }
        int i = 0;
        for (auto& [k, v] : adj){
            if (!vertex_to_int.count(k)){
                vertex_to_int[k] = i;
                i++;
            }
            for(auto& s : v){
                if (!vertex_to_int.count(s)){
                    vertex_to_int[s] = i;
                    i++;
                }
                graph[vertex_to_int[k]].insert(vertex_to_int[s]);
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
        // for each phase, we perform a maximum adjacency search which results in finding two vertices s and t
        // we then merge s and t into s and update the adjacency matrix
        // V-1 iterations are performed and each iteration costs O(V^2)
        // Choose s and t (if same component we merge them else we find min cut to divide them into 2 components)
        for(int ph = 1; ph < n;ph++) {
            // w is the vector of weights of 0th vertex
            vector<int> w = mat[0];
            // choose s and t as the same vertex (0th vertex)
            size_t s = 0, t = 0;
            // go over n-ph phases
            for(int j = 0; j < n-ph;j++){// O(V^2) -> O(E log V) with prio. queue
                // mark t as visited
                w[t] = INT_MIN;
                // find the vertex t has maximum connectivity to the previously visited vertices
                s = t, t = max_element(w.begin(), w.end()) - w.begin();
                // update the weights of each vertex
                for(int i = 0; i < n;i++) w[i] += mat[t][i];
            }
            // min cut({s,t}) = sum of all weights connected to s
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
