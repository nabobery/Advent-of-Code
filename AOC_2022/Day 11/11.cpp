#include <bits/stdc++.h>
#include <regex>
using namespace std;

#define ll long long int

ll mod = 1;

class Monkey{
    public:
        queue<ll> items;
        int divisibility_test;
        int if_true, if_false;
        string ops;
};

ll evaluate(string operation, ll val){
    stack<ll> operands;
    stack<char> operators;
    for (int i = 0; i < operation.length(); i++) {
        if (isdigit(operation[i])) {
            int number = 0;
            while (i < operation.length() && isdigit(operation[i])) {
                number = number * 10 + (operation[i] - '0');
                i++;
            }
            i--;
            operands.push(number);
        }
        else if (operation[i] == '+' || operation[i] == '*') {
            operators.push(operation[i]);
        }
        else if (operation.substr(i, 3) == "old") {
            operands.push(val);
            i += 2;
        }
    }
    while (!operators.empty()) {
        ll operand2 = operands.top();
        operands.pop();
        ll operand1 = operands.top();
        operands.pop();
        char op = operators.top();
        operators.pop();

        if (op == '+') {
            operands.push(operand1 + operand2);
        } else if (op == '*') {
            operands.push(operand1 * operand2);
        }
    }
    return operands.top();
}


ll simulate(vector<Monkey> monkeys, int rounds, bool divide){
    vector<ll> items_inspected(monkeys.size(),0);
    while(rounds--){
        for(int i = 0; i < monkeys.size();i++){
            while(!monkeys[i].items.empty()){
                ll old = monkeys[i].items.front();
                monkeys[i].items.pop();
                items_inspected[i]++;
                ll new_item = evaluate(monkeys[i].ops, old) % mod;
                if(divide) new_item /= 3;
                if(new_item % monkeys[i].divisibility_test == 0){
                    monkeys[monkeys[i].if_true].items.push(new_item);
                }
                else{
                    monkeys[monkeys[i].if_false].items.push(new_item);
                }
            }
        }
    }
    sort(items_inspected.begin(), items_inspected.end());
    ll result = items_inspected[items_inspected.size()-1] * items_inspected[items_inspected.size()-2];
    // for (int i = 0; i < items_inspected.size(); i++) {
    //     cout << items_inspected[i] << " ";
    // }
    // cout << "\n";
    cout << result << "\n";
}

int main()
{
    ifstream in("input.txt");
    if (!in.is_open())
    {
        cerr << "Could not open the file - '" << endl;
        return EXIT_FAILURE;
    }
    regex item("Starting items: ([0-9, ]*)");
    regex operation("Operation: new = ([^\n]*)");
    regex div_test("Test: divisible by ([0-9]+)");
    regex ifTrue("If true: throw to monkey ([0-9]+)");
    regex ifFalse("If false: throw to monkey ([0-9]+)");
    smatch match;
    string line;
    vector<Monkey> monkeys;
    while(getline(in,line)){
        if(line.find("Monkey") != string::npos){
            Monkey m;
            while (getline(in,line) && !line.empty()){
                if (regex_search(line, match, item)) {
                    string items_str = match[1].str();
                    stringstream ss(items_str);
                    string item;
                    while (getline(ss, item, ',')) {
                        m.items.push(stoll(item));
                    }
                }
                else if (regex_search(line, match, operation)) {
                    m.ops = match[1].str();
                }
                else if (regex_search(line, match, div_test)) {
                    m.divisibility_test = stoi(match[1].str());
                }
                else if (regex_search(line, match, ifTrue)) {
                    m.if_true = stoi(match[1].str());
                }
                else if (regex_search(line, match, ifFalse)) {
                    m.if_false = stoi(match[1].str());
                }
            }
            monkeys.push_back(m);
        }
    }
    for (int i = 0; i < monkeys.size(); i++) {
        mod *= monkeys[i].divisibility_test;
    }
    // parse check
    // for(auto m: monkeys){
    //     cout<<"Items : \n";
    //     for(auto i: m.items){
    //         cout << i << " ";
    //     }
    //     cout << "\n";
    //     cout<<"Operation : \n";
    //     cout<< m.ops << "\n";
    //     cout<<"Divisibility test : "<< m.divisibility_test << "\n";
    //     cout<<"If true : "<< m.if_true << "\n";
    //     cout<<"If false : "<< m.if_false << "\n";
    // }
    // part 1
    simulate(monkeys, 20, true);
    // part 2
    simulate(monkeys, 10000, false);
    return 0;
}


