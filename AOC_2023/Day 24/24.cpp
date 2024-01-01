#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <array>
#include <cmath>

using namespace std;
using ll = long long;

using T = array<double, 3>;

struct hailstone
{
    T positions, velocities;
};

class Solution
{
public:
    vector<hailstone> hailstones;
    void init(ifstream &input)
    {
        string line;
        regex pattern(R"(-?\d+)");
        while (getline(input, line))
        {
            vector<string> matches;
            sregex_iterator it(line.begin(), line.end(), pattern), end;
            while (it != end)
            {
                matches.push_back(it->str());
                it++;
            }
            hailstone h;
            h.positions = {double(stoll(matches[0])), double(stoll(matches[1])), double(stoll(matches[2]))};
            h.velocities = {double(stoll(matches[3])), double(stoll(matches[4])), double(stoll(matches[5]))};
            hailstones.push_back(h);
        }
    }

    // x1 + vx1 * t1 = x2 + vx2 * t2
    // y1 + vy1 * t1 = y2 + vy2 * t2
    // solving for t1 and t2
    // vy1 * (x1 + vx1 * t1) = vy1 * (x2 + vx2 * t2) => vy1 * x1 + vy1 * vx1 * t1 = vy1 * x2 + vy1 * vx2 * t2 
    // vx1 * (y1 + vy1 * t1) = vx1 * (y2 + vy2 * t2) => vx1 * y1 + vx1 * vy1 * t1 = vx1 * y2 + vx1 * vy2 * t2 
    // vy1 * x1 + - vx1*y1 = vy1 * x2 + - vx1*y2 + vy1 * vx2 * t2 - vy2 * vx1 * t2
    // vy1(x1 -x2) + vx1(y2 -y1) = t2(vy1vx2 - vy2vx1)
    // t2 = (vy1(x1 -x2) + vx1(y2 -y1)) / (vy1vx2 - vy2vx1)
    // t1 = (x2 + vx2 * t2 - x1) / vx1
    // function to see if the 2 hailstones collide
    bool helper(pair<int,int> hs, ll mn, ll mx){
        auto [i, j] = hs;
        double x1 = hailstones[i].positions[0], y1 = hailstones[i].positions[1];
        double x2 = hailstones[j].positions[0], y2 = hailstones[j].positions[1];
        double vx1 = hailstones[i].velocities[0], vy1 = hailstones[i].velocities[1];
        double vx2 = hailstones[j].velocities[0], vy2 = hailstones[j].velocities[1];
        double t2 = (vy1*(x1 - x2) + vx1*(y2 - y1)) / (vy1*vx2 - vy2*vx1);
        double t1 = (x2 + vx2 * t2 - x1) / vx1;
        if(t1 < 0 || t2 < 0) return false;
        double posx = x1 + vx1 * t1, posy = y1 + vy1 * t1;
        if(posx < mn || posx > mx || posy < mn || posy > mx) return false;
        return true;
    }

    void part1()
    {   
        int result = 0;
        ll mn = 200000000000000, mx = 400000000000000;
        for(int i = 0; i < hailstones.size(); i++){
            for(int j = i+1; j < hailstones.size(); j++){
                if(helper({i,j}, mn, mx)) result++;
            }
        }
        cout<<"Part 1: "<<result<<"\n";
    }

    // function to find determinant/ minor of a matrix
    double determinant(vector<vector<double>> &mat){
        double det = 0;
        if (mat.size() == 1)
            return mat[0][0];
        else if (mat.size() == 2)
            return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
        else
        {
            for (int i = 0; i < mat.size(); i++)
            {
                vector<vector<double>> temp(mat.size() - 1, vector<double>(mat.size() - 1));
                for (int j = 1; j < mat.size(); j++)
                {
                    for (int k = 0; k < mat.size(); k++)
                    {
                        if (k < i)
                            temp[j - 1][k] = mat[j][k];
                        else if (k > i)
                            temp[j - 1][k - 1] = mat[j][k];
                    }
                }
                det += mat[0][i] * pow(-1, i) * determinant(temp);
            }
        }
        return det;
    }

    // function to find Co-Factor Matrix of a matrix
    vector<vector<double>> cofactorMatrix(vector<vector<double>>& matrix) {
        int n = matrix.size();
        vector<vector<double>> cofactors(n, vector<double>(n));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                vector<vector<double>> submatrix(n - 1, vector<double>(n - 1));
                for (int k = 0; k < n; k++) {
                    for (int l = 0; l < n; l++) {
                        if (k != i && l != j) {
                            submatrix[k < i ? k : k - 1][l < j ? l : l - 1] = matrix[k][l];
                        }
                    }
                }
                cofactors[i][j] = pow(-1, i + j) * determinant(submatrix);
            }
        }
        return cofactors;
    }

    // function to find inverse of a matrix
    vector<vector<double>> inverse(vector<vector<double>>& mat)
    {
        vector<vector<double>> inv(mat.size(), vector<double>(mat.size()));
        double det = determinant(mat);
        if (det == 0)
        {
            cout << "Inverse of the matrix doesn't exist" << "\n";
            return inv;
        }
        vector<vector<double>> cofactors = cofactorMatrix(mat);
        for (int i = 0; i < mat.size(); i++){
            for (int j = 0; j < mat.size(); j++)
            {
                inv[i][j] = cofactors[j][i] / det;
            }
        }
        return inv;
    }

    // function to multiply 2 matrices nxn and nx1
    vector<ll> multiply(vector<vector<double>> mat1, vector<double> mat2){
        vector<ll> res(mat1.size(),0);
        for (int i = 0; i < mat1.size(); i++)
        {
            for (int j = 0; j < mat2.size(); j++)
            {
                res[i] += mat1[i][j] * mat2[j];
            }
        }
        return res;
    }

    // Let the initial position of rock be (px, py, pz) and the initial velocity be (vx, vy, vz)
    // If the rock has to hit any hailstone i with position (xi, yi, zi) and velocity (vxi, vyi, vzi)
    // px + vx * ti = xi + vxi * ti, py + vy * ti = yi + vyi * ti, pz + vz * ti = zi + vzi * ti
    // Now, as we need to find (px,py,pz) and (vx,vy,vz) such that the above equations are satisfied for all i

    // Let's eliminate ti from the above equations
    // (px - xi) / (vxi - vx) = (py - yi) / (vyi - vy) = (pz - zi) / (vzi - vz)

    // Let's take the first 3 hailstones and solve for (px,py,pz) and (vx,vy,vz)
    // (px - x1) / (vx1 - vx) = (py - y1) / (vy1 - vy) = (pz - z1) / (vz1 - vz)
    // (px - x2) / (vx2 - vx) = (py - y2) / (vy2 - vy) = (pz - z2) / (vz2 - vz)
    // (px - x3) / (vx3 - vx) = (py - y3) / (vy3 - vy) = (pz - z3) / (vz3 - vz)
    // Now, we get 3 equations from the above 3 equations
    // (px - x1) / (vx1 - vx) = (py - y1) / (vy1 - vy) = (px-x1)*(vy1-vy) = (py-y1)*(vx1-vx) => px*vy1 - px*vy - x1*vy1 + x1*vy = py*vx1 - py*vx - y1*vx1 + y1*vx

    // px*vy - py*vx = px*vy1 - x1*vy1 + x1*vy - py*vx1 + y1*vx1 - y1*vx (1)
    // px*vy - py*vx = px*vy2 - x2*vy2 + x2*vy - py*vx2 + y2*vx2 - y2*vx (2)
    // px*vy - py*vx = px*vy3 - x3*vy3 + x3*vy - py*vx3 + y3*vx3 - y3*vx (3)

    // Now, let's get similar equations for yz and xz pairings
    // py*vz - pz*vy = py*vz1 - y1*vz1 + y1*vz - pz*vy1 + z1*vy1 - z1*vy (4)
    // py*vz - pz*vy = py*vz2 - y2*vz2 + y2*vz - pz*vy2 + z2*vy2 - z2*vy (5)
    // py*vz - pz*vy = py*vz3 - y3*vz3 + y3*vz - pz*vy3 + z3*vy3 - z3*vy (6)

    // px*vz - pz*vx = px*vz1 - x1*vz1 + x1*vz - pz*vx1 + z1*vx1 - z1*vx (7)
    // px*vz - pz*vx = px*vz2 - x2*vz2 + x2*vz - pz*vx2 + z2*vx2 - z2*vx (8)
    // px*vz - pz*vx = px*vz3 - x3*vz3 + x3*vz - pz*vx3 + z3*vx3 - z3*vx (9)

    // Now, we have 9 equations and 6 unknowns (px,py,pz,vx,vy,vz) and we can solve for them by equating {1,2,3} and {4,5,6} and {7,8,9}
    // We try to get 6 equations and 6 unknowns and then solve for them
    // px*vy1 - x1*vy1 + x1*vy - py*vx1 + y1*vx1 - y1*vx = px*vy2 - x2*vy2 + x2*vy - py*vx2 + y2*vx2 - y2*vx => (vy1 - vy2)*px + (vx2 - vx1)*py = (x2*vy2 - x1*vy1) + (y1*vx1 - y2*vx2)

    // (vy1 - vy2)*px + (vx2 - vx1)*py + (y1-y2)*vx + (x2-x1)*vy = y2*vx2 - y1*vx1 - x2*vy2 + x1*vy1 (1 & 2)
    // (vy1 - vy3)*px + (vx3 - vx1)*py + (y1-y3)*vx + (x3-x1)*vy = y3*vx3 - y1*vx1 - x3*vy3 + x1*vy1 (1 & 3)

    // (vz1 - vz2)*py + (vy2 - vy1)*pz + (y1-y2)*vz + (z2-z1)*vy = z2*vy2 - z1*vy1 - y2*vz2 + y1*vz1 (4 & 5)
    // (vz1 - vz3)*py + (vy3 - vy1)*pz + (y1-y3)*vz + (z3-z1)*vy = z3*vy3 - z1*vy1 - y3*vz3 + y1*vz1 (4 & 6)

    // (vz2 - vz1)*px + (vx1 - vx2)*pz + (z1-z2)*vx + (x2-x1)*vz = x2*vz2 - x1*vz1 - z2*vx2 + z1*vx1 (7 & 8)
    // (vz3 - vz1)*px + (vx1 - vx3)*pz + (z1-z3)*vx + (x3-x1)*vz = x3*vz3 - x1*vz1 - z3*vx3 + z1*vx1 (7 & 9)

    // Now, we can solve for (px,py,pz) and (vx,vy,vz) using the above 6 equations
    // AX = B
    // X = A^-1 * B

    // X = (px,py,pz,vx,vy,vz)

    void part2()
    {
        T p1, p2, p3, v1, v2, v3;
        p1 = hailstones[0].positions;
        p2 = hailstones[1].positions;
        p3 = hailstones[2].positions;
        v1 = hailstones[0].velocities;
        v2 = hailstones[1].velocities;
        v3 = hailstones[2].velocities;
        vector<vector<double>> A = {{
            {v1[1] - v2[1], v2[0] - v1[0], 0, p2[1] - p1[1], p1[0] - p2[0], 0},
            {v1[1] - v3[1], v3[0] - v1[0], 0, p3[1] - p1[1], p1[0] - p3[0], 0},
            {0, v1[2] - v2[2], v2[1] - v1[1], 0, p2[2] - p1[2], p1[1] - p2[1]},
            {0, v1[2] - v3[2], v3[1] - v1[1], 0, p3[2] - p1[2], p1[1] - p3[1]},
            {v2[2] - v1[2], 0, v1[0] - v2[0], p1[2] - p2[2], 0, p2[0] - p1[0]},
            {v3[2] - v1[2], 0, v1[0] - v3[0], p1[2] - p3[2], 0, p3[0] - p1[0]}
        }};
        vector<double> B = {
            v2[0]*p2[1] + v1[1]*p1[0] - v1[0]*p1[1] - v2[1]*p2[0],
            v3[0]*p3[1] + v1[1]*p1[0] - v1[0]*p1[1] - v3[1]*p3[0],
            v2[1]*p2[2] + v1[2]*p1[1] - v1[1]*p1[2] - v2[2]*p2[1],
            v3[1]*p3[2] + v1[2]*p1[1] - v1[1]*p1[2] - v3[2]*p3[1],
            v2[2]*p2[0] + v1[0]*p1[2] - v1[2]*p1[0] - v2[0]*p2[2],
            v3[2]*p3[0] + v1[0]*p1[2] - v1[2]*p1[0] - v3[0]*p3[2]
        };
        vector<ll> res = multiply(inverse(A), B);
        //cout<< res[0] << " " << res[1] << " " << res[2] << " " << res[3] << " " << res[4] << " " << res[5] << "\n";
        cout<<"Part 2 : "<<res[0] + res[1] + res[2]<<"\n";
    }
};

int main()
{
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