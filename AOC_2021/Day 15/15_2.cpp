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
    /*int initial_grid[100][100];
    int grid[500][500];
    Cell dist[500][500];
    set <Cell> st;
    for(int i = 0; i < 100; i++) {
        string str;
        getline(in,str);
        for(int j = 0; j < 100; j++) {
            initial_grid[i][j] = str[j] - '0';
        }
    }
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++) {
            for(int k  = 0 ; k < 100; k++){
                for(int l = 0; l < 100; l++){
                    grid[i*100 + k][j*100 + l] = initial_grid[k][l] + i + j;
                    if(grid[i*100 + k][j*100 + l] > 9){
                        grid[i*100 + k][j*100 + l] -= 9;
                    }
                    dist[i*100 + k][j*100 + l] = Cell(i*100 + k,j*100 + l);
                    st.insert(dist[i*100 + k][j*100 + l]);
                }
            }
        }
    }*/
    /*for(int i = 0; i < 100; i++) {
        for(int j = 0; j < 100; j++){
            cout << grid[i][j];
        }
        cout<<"\n";
    }*/
    /*int grid[500][500];
    Cell dist[500][500];
    set <Cell> st;
    for(int i = 0; i < 100; i++) {
        string str;
        getline(in,str);
        for(int j = 0; j < 100; j++) {
            int val = str[j] - '0';
            for(int k = i; k < 500; k += 100) {
                for(int l = j; l < 500; l += 100) {
                    grid[k][l] = val + (k / 100) + (l / 100);
                    if(grid[k][l] > 9){
                        grid[k][l] -= 9;
                    }
                    dist[k][l] = Cell(k,l);
                    st.insert(dist[k][l]);
                }
            }
        }
    }*/
    vector<vector<int>> initial_grid;
    //Cell dist[500][500];
    vector<vector<Cell>> dist;
    for(int i = 0; i < 500; i++){
        dist.push_back(vector<Cell>{});
        for(int j = 0; j<500;j++){
            dist[i].push_back(Cell(i,j));
        }
    }
    set <Cell> st;
    string str;
    for(int i = 0; i < 100; i++) {
        string str;
        getline(in,str);
        initial_grid.push_back(vector<int>{});
        for(int j = 0; j < 100; j++) {
            initial_grid.back().push_back(str[j] - '0');
        }
    }
    /*for(int i = 0; i < initial_grid.size(); i++) {
        for(int j = 0; j < initial_grid[0].size(); j++) {
            cout << initial_grid[i][j];
        }
        cout << endl;
    }*/
    int grid[500][500];
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            for(int k = 0; k < initial_grid.size(); k++) {
                for(int l = 0; l < initial_grid[0].size(); l++){
                    grid[i*initial_grid.size() + k][j*initial_grid[0].size() + l] = (initial_grid[k][l] + i + j - 1)%9 + 1;
                    st.insert(dist[i*initial_grid.size() + k][j*initial_grid[0].size() + l]);
                }
            }
        }
    }
    /*for(int i = 0; i < 500; i++){
        for(int j = 0; j < 500; j++){
            cout << grid[i][j];
        }
        cout<<"\n";
    }*/
    st.erase(dist[0][0]);
    dist[0][0].distance = 0;
    st.insert(dist[0][0]);
    Cell curr;
    while(!st.empty()) {
        curr = *st.begin();
        st.erase(curr);
        curr.visited = true;
        dist[curr.x][curr.y] = curr;
        if(curr.x == 499 && curr.y == 499){
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
        if(curr.x < 499 && !dist[curr.x + 1][curr.y].visited){
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
        if(curr.y < 499 && !dist[curr.x][curr.y + 1].visited){
            temp = dist[curr.x][curr.y + 1];
            if(curr.distance + grid[curr.x][curr.y + 1] < temp.distance){
                st.erase(temp);
                temp.distance = curr.distance + grid[curr.x ][curr.y+ 1];
                st.insert(temp);
            }
        }
    }
    int result = dist[499][499].distance;
    cout<<result<<endl;
    return 0;
}