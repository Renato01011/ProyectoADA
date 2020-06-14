#include <iostream>
#include <limits>
#include <vector>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <iterator>
#include <string>
using namespace std;

struct values {
    double w;
    vector<pair<pair<int, int>, pair<int, int>>> match = {};
};

//Matrix for storing already calculated values
vector<vector<values>> M;

values Min_Matching_Memoizado(vector <pair<double, double>>& A, vector <pair<double, double>>& B, int i, int j) {
    if (i == 0 && j > 0) {
        if (M[i][j].w == numeric_limits<double>::max()) {
            double temp = 0;
            for (int a = 0; a <= j; a++) {
                temp += B[a].second - B[a].first + 1;
                M[i][j].match.push_back(make_pair(A[i], B[a]));
            }
            M[i][j].w = (A[i].second - A[i].first + 1) / temp;
            return M[i][j];
        }
        else {
            return M[i][j];
        }
    }
    else if (i > 0 && j == 0) {
        if (M[i][j].w == numeric_limits<double>::max()) {
            double temp = 0;
            for (int a = 0; a <= i; a++) {
                temp += A[a].second - A[a].first + 1;
                M[i][j].match.push_back(make_pair(A[a], B[j]));
            }
            M[i][j].w = temp / (B[j].second - B[j].first + 1);
            return M[i][j];
        }
        else {
            return  M[i][j];
        }
    }
    else if (i == 0 && j == 0) {
        if (M[i][j].w == numeric_limits<double>::max()) {
            M[i][j].match.push_back(make_pair(A[i], B[j]));
            M[i][j].w = (A[i].second - A[i].first + 1) / (B[j].second - B[j].first + 1);
            return M[i][j];
        }
        else { 
            return M[i][j]; 
        }
    }
    else if (i > 0 && j > 0) {
        if (M[i][j].w == numeric_limits<double>::max()) {
            double min_1 = numeric_limits<double>::max();
            double min_2 = numeric_limits<double>::max();
            double min_3 = numeric_limits<double>::max();
            double temp = 0;
            double temp_1 = 0;
            int a_1 = 0; int a_2 = 0;
            for (int a = i - 1; a > 0; a--) {
                temp_1 = 0;
                temp = 0;
                for (int b = a; b <= i; b++) {
                    temp_1 += A[b].second - A[b].first + 1;
                }
                if (M[a - 1][j - 1].w == numeric_limits<double>::max()) {
                    M[a - 1][j - 1] = Min_Matching_Memoizado(A, B, a - 1, j - 1);
                    temp = M[a - 1][j - 1].w + (temp_1 / (B[j].second - B[j].first + 1));
                    if (temp < min_1) {
                        min_1 = temp;
                        a_1 = a;
                    }
                }
                else {
                    temp = M[a - 1][j - 1].w + (temp_1 / (B[j].second - B[j].first + 1));
                    if (temp < min_1) {
                        min_1 = temp;
                        a_1 = a;
                    }
                }
            }

            for (int a = j - 1; a > 0; a--) {
                temp_1 = 0;
                temp = 0;
                for (int b = a; b <= j; b++) {
                    temp_1 += B[b].second - B[b].first + 1;
                }
                if (M[i - 1][a - 1].w == numeric_limits<double>::max()) {
                    M[i - 1][a - 1] = Min_Matching_Memoizado(A, B, i - 1, a - 1);
                    temp = M[i - 1][a - 1].w + ((A[i].second - A[i].first + 1) / temp_1);
                    if (temp < min_2) {
                        min_2 = temp;
                        a_2 = a;
                    }
                }
                else {
                    temp = M[i - 1][a - 1].w + ((A[i].second - A[i].first + 1) / temp_1);
                    if (temp < min_2) {
                        min_2 = temp;
                        a_2 = a;
                    }
                }
            }

            if (M[i - 1][j - 1].w == numeric_limits<double>::max()) {
                M[i - 1][j - 1] = Min_Matching_Memoizado(A, B, i - 1, j - 1);
                min_3 = M[i - 1][j - 1].w + ((A[i].second - A[i].first + 1) / (B[j].second - B[j].first + 1));
            }
            else {
                min_3 = M[i - 1][j - 1].w + ((A[i].second - A[i].first + 1) / (B[j].second - B[j].first + 1));
            }
            
            int cond = 0;
            double min = min_1;
            if (min > min_2) { min = min_2; cond = 1; }
            else if (min > min_3) { min = min_3; cond = 2; }
            switch (cond) {
            case 0:
                M[i][j].match = M[a_1 - 1][j - 1].match;
                for (int a = a_1; a <= i; a++) {
                    M[i][j].match.push_back(make_pair(A[a], B[j]));
                }
                break;
            case 1:
                M[i][j].match = M[i - 1][a_2 - 1].match;
                for (int a = a_2; a <= j; a++) {
                    M[i][j].match.push_back(make_pair(A[i], B[a]));
                }
                break;
            case 2:
                M[i][j].match = M[i - 1][j - 1].match;
                M[i][j].match.push_back(make_pair(A[i], B[j]));
                break;
            }
            M[i][j].w = min;
            return M[i][j];
        }
        else {
            return M[i][j];
        }
    }
}

values Min_Matching_Memoizado(vector <double> A, vector <double> B, int p) {
    vector <pair<double, double>> M1, M2;
    //Find and save all blocks
    int n1 = -1;
    int m1 = -1;
    int _i = 0;
    int _j = 0;
    for (int i = 0; i < p; i++) {
        if (A[i] == 1 && n1 == -1) {
            n1 = i;
        }
        else if (A[i] == 0 && n1 != -1) {
            M1.push_back(make_pair(n1, i - 1));
            n1 = -1;
            _i++;
        }
        if (i == p - 1 && A[i] == 1 && n1 != -1) {
            M1.push_back(make_pair(n1, i - 1));
            _i++;
        }
        else if (i == p - 1 && A[i] == 1 && n1 == -1) {
            M1.push_back(make_pair(i - 1, i - 1));
            _i++;
        }
        if (B[i] == 1 && m1 == -1) {
            m1 = i;
        }
        else if (B[i] == 0 && m1 != -1) {
            M2.push_back(make_pair(m1, i - 1));
            m1 = -1;
            _j++;
        }
        if (i == p - 1 && B[i] == 1 && m1 != -1) {
            M2.push_back(make_pair(m1, i - 1));
            _j++;
        }
        else if (i == p - 1 && B[i] == 1 && m1 == -1) {
            M2.push_back(make_pair(i - 1, i - 1));
            _j++;
        }
    }
    //Inicitializing M matrix
    M.resize(_i);
    for (int i = 0; i < _i; i++) {
        M[i].resize(_j);
    }
    for (int i = 0; i < M.size(); i++) {
        for (int j = 0; j < M[i].size(); j++) {
            M[i][j].w = numeric_limits<double>::max();
        }
    }
    return Min_Matching_Memoizado(M1, M2, _i - 1, _j - 1);
}

int main() {
    // TEST VECTORS
    //vector <double> A = { 0,1,1,1,0,0,1,0,1,1,0,1,1,0,1,1,1,0,1,0 };
    //vector <double> B = { 0,0,1,1,0,1,1,0,0,0,1,1,1,1,1,0,0,1,1,0 };
    //vector <double> A = { 1,1,1,0,0,1,0,1,1,0,0,0,1,1,1,0,0,1,0,1 };
    //vector <double> B = { 0,0,0,1,1,0,1,0,1,0,1,0,1,1,0,0,0,0,0,0 };
    
    std::string s1;
    std::string s2;

    cout << "Insert the array A with spaces between the elements: " << endl;
    std::getline(cin, s1);
    cout << "Insert the array B with spaces between the elements: " << endl;
    std::getline(cin, s2);

    std::istringstream is_1(s1);
    std::istringstream is_2(s2);

    std::vector<double> A((std::istream_iterator<int>(is_1)), std::istream_iterator<int>());
    std::vector<double> B((std::istream_iterator<int>(is_2)), std::istream_iterator<int>());
    
    values Matching = Min_Matching_Memoizado(A, B, A.size());
    vector <pair<pair<int, int>, pair<int, int>>> S = Matching.match;
    cout << "Matching : " << endl;
    for (int i = 0; i < S.size(); i++) {
        cout << "(" << S[i].first.first << "," << S[i].first.second << ")-(" << S[i].second.first << "," << S[i].second.second << ") ";
    }
    cout << endl;
    cout << "Peso = " << Matching.w << endl;
    cout << "Matris : " << endl;
    for (int i = 0; i < M.size(); i++) {
        for (int j = 0; j < M[i].size(); j++) {
            cout << right << setw(20) << M[i][j].w;
        }
        cout << endl;
    }
    
}
