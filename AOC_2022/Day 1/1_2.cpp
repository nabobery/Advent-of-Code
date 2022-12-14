#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
using namespace std;

int main()
{
    ifstream in("input.txt");
    if (!in.is_open())
    {
        cerr << "Could not open the file - '" << endl;
        return EXIT_FAILURE;
    }
    string line;
    int result = 0, curr = 0;
    priority_queue<int, vector<int>, greater<int> > pq;
    while (getline(in, line))
    {
        if (line.empty()){
            if(pq.size() >= 3) {
                if(pq.top() < curr){
                    pq.pop();
                    pq.push(curr);
                }
            }
            else pq.push(curr);
            curr = 0;
        }  
        else
            curr += stoi(line);
    }
    if(pq.top() < curr){
        pq.pop();
        pq.push(curr);
    }
    while(!pq.empty()){
        result += pq.top();
        pq.pop();
    }
    cout<<result<<"\n";
    return 0;
}