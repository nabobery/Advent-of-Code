#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<climits>
#include<cstdlib>
#include <algorithm>
using namespace std;

int result;
int score;

int Score(vector<vector<int>> &arr){
    int total = 0;
    for (auto& row: arr) {
        for (auto& col:row) {
            total += col;
        }
    }
    return total;
}

void helper(int draw[],vector<vector<int>> &rows, vector<vector<int>> &cols){
    std::vector<int>::iterator it;
    for (int i = 0; i < 100; i++) {
        for (auto& row:rows) {
            if ((it = find(row.begin(),row.end(),draw[i])) != row.end()) {
                row.erase(it);
                if (row.empty()) {
                    if (i < result) {
                        result = i;
                        score = Score(rows) * draw[i];
                    }
                    return;
                }
                break;
            }
        }
        for (auto& col:cols) {
            if ((it = find(col.begin(),col.end(),draw[i])) != col.end()){
                col.erase(it);
                if (col.empty()) {
                    if (i < result) {
                        result = i;
                        score = Score(cols) * draw[i];
                    }
                    return;
                }
                break;
            }
        }
    }
}

int main(){
    int draw[100];
    result = INT_MAX;
    score = 0;
    ifstream in("input.txt");
    if (!in.is_open()) {
        cerr << "Could not open the file - '"<< endl;
        return EXIT_FAILURE;
    }
    string str;
    getline(in,str);
    stringstream x(str);
    int i = 0;
    while (getline(x, str, ',')) {  
        draw[i] = stoi(str);
        i++;
    }
    vector<vector<int>> rows(5);
    vector<vector<int>> cols(5);
    string tmp;
    for(int j = 0; j < 100; j++){
        getline(in, tmp);
        for(int k = 0; k < 5; k++){
            getline(in, tmp);
            stringstream v(tmp);
            string tmp2;
            int l = 0;
            while (getline(v, tmp2, ' ')) {  
                if (tmp2.length() == 0) {
                  continue;
                }
                rows[k].push_back(stoi(tmp2));
                cols[l].push_back(stoi(tmp2));
                l++;
            } 
        }
        helper(draw,rows,cols);
        for(int k = 0; k < 5; k++){
            rows[k].clear();
            cols[k].clear();
        }
    }
    cout<<score << endl;
    return 0;
}