#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cctype>
#include <vector>
#include <algorithm>
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
    string grid[99];
    int p = 0;
    while (getline(in, line))
    {
        grid[p++] = line;
    }
    bool visible[99][99];
    for (int i = 0; i < 99; i++)
    {
        for (int j = 0; j < 99; j++)
        {
            if (i == 0 || i == 98 || j == 0 || j == 98)
            {
                visible[i][j] = true;
            }
            else
                visible[i][j] = false;
        }
    }
    int max_height;
    for (int i = 0; i < 99; i++)
    {
        max_height = 0;
        for (int j = 0; j < 99; j++)
        {
            if (grid[i][j] > max_height)
            {
                max_height = grid[i][j];
                visible[i][j] = true;
            }
        }
        max_height = 0;
        for (int j = 98; j >= 0; j--)
        {
            if (grid[i][j] > max_height)
            {
                max_height = grid[i][j];
                visible[i][j] = true;
            }
        }
    }
    for (int j = 0; j < 99; j++)
    {
        max_height = 0;
        for (int i = 0; i < 99; i++)
        {
            if (grid[i][j] > max_height)
            {
                max_height = grid[i][j];
                visible[i][j] = true;
            }
        }
        max_height = 0;
        for (int i = 98; i >= 0; i--)
        {
            if (grid[i][j] > max_height)
            {
                max_height = grid[i][j];
                visible[i][j] = true;
            }
        }
    }
    int result = 0;
    for (int i = 0; i < 99; i++)
    {
        for (int j = 0; j < 99; j++)
        {
            if (visible[i][j])
            {
                result++;
            }
        }
    }
    cout << result << "\n";

    // part 2
    result = 0;
    for(int i = 1; i < 98;i++){
        for(int j = 1; j < 98;j++){
            int curr = 1,k;
            for(k = i+1; k < 98;k++){
                if(grid[i][j] <= grid[k][j])
                    break;
            }
            curr *= (k-i);
            for(k = i-1; k > 0;k--){
                if(grid[i][j] <= grid[k][j])
                    break;
            }
            curr *= (i-k);
            for(k = j+1; k < 98;k++){
                if(grid[i][j] <= grid[i][k])
                    break;
            }
            curr *= (k-j);
            for(k = j-1; k > 0;k--){
                if(grid[i][j] <= grid[i][k])
                    break;
            }
            curr *= (j-k);
            result = max(result,curr);
        }
    }
    cout << result << "\n";
    return 0;
}
