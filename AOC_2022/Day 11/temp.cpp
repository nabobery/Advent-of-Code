#include <bits/stdc++.h>
using namespace std;

class Monkey {
    private:
        vector<long long> items;
        string operations;
        long long divide;
        long long true_divide;
        long long false_divide;
        long long inspect;
        // math is pain :(
        long long magickey = 2*3*5*7*11*13*17*19;

    public:
        Monkey(vector<long long> items, string operations, 
            long long divide, long long true_divide, long long false_divide) {
            this->items = items;
            this->operations = operations;
            this->divide = divide;
            this->true_divide = true_divide;
            this->false_divide = false_divide;
            this->inspect = 0;
        }

        void accept(long long num) {
            items.push_back(num);
        }

        vector<pair<long long, long long>> throwAway(bool is_first) {
            vector<pair<long long, long long>> move;
            for (auto &item : items) {
                inspect += 1;
                long long new_value = item;
                long long old_value = (operations[1] == 'o') ? 
                    new_value : stoi(operations.substr(1));
                if (operations[0] == '+') {
                    new_value += old_value;
                } else {
                    new_value *= old_value;
                }
                (is_first) ? new_value /= 3 : new_value %= magickey;
                if (new_value % divide == 0) {
                    move.push_back({true_divide, new_value});
                } else {
                    move.push_back({false_divide, new_value});
                }
            }
            items.clear();
            return move;
        }

        long long get_inspect() {
            return inspect;
        }
};

class Solution {
    private:
        vector<Monkey> simulate1;
        vector<Monkey> simulate2;
    
    public:
        Solution() {
            string line;
            vector<long long> items;
            string operations;
            long long divide = 1;
            long long true_divide = 1;
            long long false_divide = 1;
            while (getline(cin, line)) {
                // first line 
                getline(cin, line);
                stringstream ss1(line); 
                string temp; ss1 >> temp >> temp;
                long long item; char buff;
                while (ss1 >> item) {
                    items.push_back(item);
                    ss1 >> buff;
                }
                // second line 
                getline(cin, line);
                stringstream ss2(line); 
                ss2 >> temp >> temp >> temp >> temp;
                string op, operations;
                while (ss2 >> op) {
                    operations += op;
                }
                // third line
                getline(cin, line);
                stringstream ss3(line);
                ss3 >> temp >> temp >> temp;
                ss3 >> divide;
                // fourth line
                getline(cin, line);
                stringstream ss4(line);
                ss4 >> temp >> temp >> temp >> temp >> temp;
                ss4 >> true_divide;
                // fifth line
                getline(cin, line);
                stringstream ss5(line);
                ss5 >> temp >> temp >> temp >> temp >> temp;
                ss5 >> false_divide;
                getline(cin, line);
                Monkey object(items, operations, divide, true_divide, false_divide);
                simulate1.push_back(object);
                simulate2.push_back(object);
                items.clear();
            }
        }

        void q1() {
            for (long long i = 0; i < 20; i++) {
                for (auto &monkey : simulate1) {
                    vector<pair<long long, long long>> test = monkey.throwAway(true);
                    for (auto it: test) {
                        simulate1[it.first].accept(it.second);
                    }
                }
            }
            vector<long long> ans;
            for (auto &monkey : simulate1) {
                ans.push_back(monkey.get_inspect());
            }
            sort(ans.rbegin(), ans.rend());
            for(auto it: ans){
                cout << it << " ";
            }
            cout << endl;
            cout << ans[0] * ans[1] << '\n';
        }
        
        void q2() {
            for (long long i = 0; i < 10000; i++) {
                for (auto &monkey : simulate2) {
                    vector<pair<long long, long long>> test = monkey.throwAway(false);
                    for (auto it: test) {
                        simulate2[it.first].accept(it.second);
                    }
                }
            }
            vector<long long> ans;
            for (auto &monkey : simulate2) {
                ans.push_back(monkey.get_inspect());
            }
            sort(ans.rbegin(), ans.rend());
            cout << ans[0] * ans[1] << '\n';
        }
};

int main() {
    Solution ans;
    ans.q1();
    ans.q2();
}