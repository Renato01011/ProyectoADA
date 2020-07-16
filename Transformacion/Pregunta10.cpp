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
vector<vector<int>> M_1;
vector<vector<int>> M_2;


values Min_Matching_Promedio_Prog_Din(vector <pair<double, double>>& A, vector <pair<double, double>>& B, int i, int j, double mu) {
    //Case : i == 0 && j > 0
    for (int _j = 0; _j < j; _j++) {
        double temp = 0;
        for (int a = 0; a <= _j; a++) {
            temp += B[a].second - B[a].first + 1;
            M[0][_j].match.push_back(make_pair(A[0], B[a]));
        }
        M[0][_j].w = abs( ((A[0].second - A[0].first + 1) / temp) - mu );
    }
    //Case : i > 0 && j == 0
    for (int _i = 0; _i < i; _i++) {
        double temp = 0;
        for (int a = 0; a <= _i; a++) {
            temp += A[a].second - A[a].first + 1;
            M[_i][0].match.push_back(make_pair(A[a], B[0]));
        }
        M[_i][0].w = abs( (temp / (B[0].second - B[0].first + 1)) - mu );
    }

    //Case : i == 0 && j == 0
    M[0][0].match.push_back(make_pair(A[0], B[0]));
    M[0][0].w = abs( ((A[0].second - A[0].first + 1) / (B[0].second - B[0].first + 1)) - mu ) ;

    //Fill Other Two Matrixes
    for (int _i = 0; _i <= i; _i++) {
        bool first_iter = false;
        for (int a = _i; a >= 0; a--) {
            if (first_iter == false) {
                M_1[_i][a] = (A[a].second - A[a].first + 1);
                first_iter = true;
            }
            else {
                M_1[_i][a] = M_1[_i][a + 1] + (A[a].second - A[a].first + 1);
            }
        }
    }
    for (int _j = 0; _j <= j; _j++) {
        bool first_iter = false;
        for (int a = _j - 1; a >= 0; a--) {
            if (first_iter == false) {
                M_2[_j][a] = (B[a].second - B[a].first + 1);
                first_iter = true;
            }
            else {
                M_2[_j][a] = M_2[_j][a + 1] + (B[a].second - B[a].first + 1);
            }
        }
    }

    //Case : i > 0 && j > 0
    for (int _i = 1; _i <= i; _i++) {
        for (int _j = 1; _j <= j; _j++) {
            double min_1 = numeric_limits<double>::max();
            double min_2 = numeric_limits<double>::max();
            double min_3 = numeric_limits<double>::max();
            double temp = 0;
            double temp_1 = 0;
            int a_1 = 0; int a_2 = 0;

            for (int a = _i - 1; a > 0; a--) {
                temp_1 = 0;
                temp = 0;

                temp_1 = M_1[_i][a];

                temp = M[a - 1][_j - 1].w + abs( (temp_1 / (B[_j].second - B[_j].first + 1)) - mu );
                if (temp < min_1) {

                    min_1 = temp;
                    a_1 = a;
                }
            }

            for (int a = _j - 1; a > 0; a--) {
                temp_1 = 0;
                temp = 0;

                temp_1 = M_2[_j][a];

                temp = M[_i - 1][a - 1].w + abs( ((A[_i].second - A[_i].first + 1) / temp_1) - mu );
                if (temp < min_2) {
                    min_2 = temp;
                    a_2 = a;
                }
            }

            min_3 = M[_i - 1][_j - 1].w + abs( ((A[_i].second - A[_i].first + 1) / (B[_j].second - B[_j].first + 1)) - mu );

            int cond = 0;
            double min = min_1;
            if (min > min_2) { min = min_2; cond = 1; }
            else if (min > min_3) { min = min_3; cond = 2; }
            switch (cond) {
            case 0:
                M[_i][_j].match = M[a_1 - 1][_j - 1].match;
                for (int a = a_1; a <= _i; a++) {
                    M[_i][_j].match.push_back(make_pair(A[a], B[_j]));
                }
                break;
            case 1:
                M[_i][_j].match = M[_i - 1][a_2 - 1].match;
                for (int a = a_2; a <= _j; a++) {
                    M[_i][_j].match.push_back(make_pair(A[_i], B[a]));
                }
                break;
            case 2:
                M[_i][_j].match = M[_i - 1][_j - 1].match;
                M[_i][_j].match.push_back(make_pair(A[_i], B[_j]));
                break;
            }
            M[_i][_j].w = min;
        }
    }
    return M[i][j];
}

values Min_Matching_Promedio_Prog_Din(vector <double> A, vector <double> B, int p) {
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
            //M1.push_back(make_pair(n1, i - 1));
            M1.push_back(make_pair(n1, i));
            _i++;
        }
        else if (i == p - 1 && A[i] == 1 && n1 == -1) {
            //M1.push_back(make_pair(i - 1, i - 1));
            M1.push_back(make_pair(i, i));
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
            //M2.push_back(make_pair(m1, i - 1));
            M2.push_back(make_pair(m1, i));
            _j++;
        }
        else if (i == p - 1 && B[i] == 1 && m1 == -1) {
            //M2.push_back(make_pair(i - 1, i - 1));
            M2.push_back(make_pair(i, i));
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

    M_1.resize(_i);
    for (int i = 0; i < _i; i++) {
        M_1[i].resize(_i);
    }
    for (int i = 0; i < M_1.size(); i++) {
        for (int j = 0; j < M_1[i].size(); j++) {
            M_1[i][j] = numeric_limits<int>::max();
        }
    }

    M_2.resize(_j);
    for (int i = 0; i < _j; i++) {
        M_2[i].resize(_j);
    }
    for (int i = 0; i < M_2.size(); i++) {
        for (int j = 0; j < M_2[i].size(); j++) {
            M_2[i][j] = numeric_limits<int>::max();
        }
    }

    //Calcular mu

    double sum_M1 = 0;
    double sum_M2 = 0;
    for (int i = 0; i < M1.size(); i++) {
        sum_M1 += M1[i].second - M1[i].second + 1;
    }
    for (int i = 0; i < M2.size(); i++) {
        sum_M2 += M2[i].second - M2[i].second + 1;
    }
    double mu = sum_M1 / sum_M2;
    
    return Min_Matching_Promedio_Prog_Din(M1, M2, _i - 1, _j - 1, mu);
};

pair < vector< vector<pair<pair<int, int>, pair<int, int>>>>, double> Transformacion_Promedio_Prog_Din(vector <vector<double>> A, vector <vector<double>> B) {
    vector< vector<pair<pair<int, int>, pair<int, int>>> > M;
    double peso_total = 0.0;
    values min_matching;

    for (int i = 0; i < A.size(); i++) {
        min_matching = Min_Matching_Promedio_Prog_Din(A[i], B[i], A[i].size());
        M.push_back(min_matching.match);
        peso_total += min_matching.w;
        cout << "Peso total = " << peso_total << endl;
    }

    return make_pair(M, peso_total);

}

int main() {

    vector <vector<double>> A = { { 0,1,1,1,0,0,1,0,1,1,0,1,1,0,1,1,1,0,1,0 },
                                 { 1,1,1,0,0,1,0,1,1,0,0,0,1,1,1,0,0,1,0,1 },
                                 { 1,1,0,1,0,0,1,0,1,0,0,1,1,1,0,1,1,0,1,1 },
                                 { 0,1,1,1,0,0,1,0,1,1,0,1,1,0,1,1,1,0,1,0 } };

    vector <vector<double>> B = { { 1,1,0,1,1,1,1,0,1,0,1,1,1,1,1,0,1,1,1,1 },
                                 { 0,0,0,1,1,0,1,0,1,0,1,0,1,1,0,0,0,0,0,0 },
                                 { 0,0,1,1,0,1,1,0,0,0,1,1,1,1,1,0,0,1,1,0 },
                                 { 0,0,1,1,0,1,1,0,0,0,1,1,1,1,1,0,0,1,1,0 } };

    pair < vector< vector<pair<pair<int, int>, pair<int, int>>>>, double> TV = Transformacion_Promedio_Prog_Din(A, B);
    vector< vector< pair <pair<int, int>, pair<int, int>>>> M = TV.first;
    cout << "Matchings = " << endl;
    for (int i = 0; i < M.size(); i++) {
        for (int j = 0; j < M[i].size(); j++) {
            cout << "(" << M[i][j].first.first << "," << M[i][j].first.second << ")-(" << M[i][j].second.first << "," << M[i][j].second.second << ") ";
        }
        cout << endl;
    }
    cout << endl;
    cout << "Peso = " << TV.second << endl;

    return 0;
}
