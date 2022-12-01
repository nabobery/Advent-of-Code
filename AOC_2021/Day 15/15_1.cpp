#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<algorithm>
#include<climits>
#include<set>
using namespace std;

struct Cell{
    int x, y;
    int distance = INT_MAX;
    bool visited = false;
    Cell(){
    }
    Cell(int a, int b){
        x = a;
        y = b;
    }
};

bool operator<(const Cell& a, const Cell& b){
	if (a.distance == b.distance)
	{
		if (a.x != b.x)
			return (a.x < b.x);
		else
			return (a.y < b.y);
	}
	return (a.distance < b.distance);
}

int main(){
    ifstream in("input.txt");
    if (!in.is_open()) {
        cerr << "Could not open the file - '"<< endl;
        return EXIT_FAILURE;
    }
    int grid[100][100];
    Cell dist[100][100];
    set <Cell> st;
    for(int i = 0; i < 100; i++) {
        string str;
        getline(in,str);
        for(int j = 0; j < 100; j++) {
            grid[i][j] = str[j] - '0';
            dist[i][j] = Cell(i,j);
            st.insert(dist[i][j]);
        }
    }
    st.erase(dist[0][0]);
    dist[0][0].distance = 0;
    st.insert(dist[0][0]);
    /*auto itr = st.begin();
    cout<<itr->distance<<endl;*/
    /*for(auto itr = st.begin(); itr != st.end(); itr++) {
        cout << itr->distance << endl;
    }*/
    Cell curr;
    while(!st.empty()) {
        curr = *st.begin();
        st.erase(curr);
        curr.visited = true;
        dist[curr.x][curr.y] = curr;
        if(curr.x == 99 && curr.y == 99){
            break;
        }
        Cell temp;
        if(curr.x > 0 && !dist[curr.x - 1][curr.y].visited){
            temp = dist[curr.x - 1][curr.y];
            if(curr.distance + grid[curr.x - 1][curr.y] < temp.distance){
                st.erase(temp);
                temp.distance = curr.distance + grid[curr.x - 1][curr.y];
                st.insert(temp);
            }
        } 
        if(curr.x < 99 && !dist[curr.x + 1][curr.y].visited){
            temp = dist[curr.x + 1][curr.y];
            if(curr.distance + grid[curr.x + 1][curr.y] < temp.distance){
                st.erase(temp);
                temp.distance = curr.distance + grid[curr.x + 1][curr.y];
                st.insert(temp);
            }
        } 
        if(curr.y > 0 && !dist[curr.x][curr.y - 1].visited){
            temp = dist[curr.x][curr.y - 1];
            if(curr.distance + grid[curr.x][curr.y - 1] < temp.distance){
                st.erase(temp);
                temp.distance = curr.distance + grid[curr.x][curr.y - 1];
                st.insert(temp);
            }
        } 
        if(curr.y < 99 && !dist[curr.x][curr.y + 1].visited){
            temp = dist[curr.x][curr.y + 1];
            if(curr.distance + grid[curr.x][curr.y + 1] < temp.distance){
                st.erase(temp);
                temp.distance = curr.distance + grid[curr.x ][curr.y+ 1];
                st.insert(temp);
            }
        }
    }
    int result = dist[99][99].distance;
    cout<<result<<endl;
    return 0;
}