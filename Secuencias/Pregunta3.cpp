#include <iostream>
#include <limits>
#include <vector>
#include <cmath>
#include <sstream>
#include <iterator>
#include <string>
using namespace std;

struct values {
    double w = -numeric_limits<double>::min();
    vector<pair<pair<int, int>, pair<int, int>>> match;
};

values Opt(vector <pair<double, double>> &A, vector <pair<double, double>> &B, int i, int j) {
    values v;
    if (i == 0 && j > 0) {
        double temp = 0;
        for (int a = 0; a <= j; a++) {
            temp += B[a].second - B[a].first + 1;
            v.match.push_back(make_pair(A[i],B[a]));
        }
        v.w = (A[i].second - A[i].first + 1) / temp;
        return v;
    }
    else if (i > 0 && j == 0) {
        double temp = 0;
        for (int a = 0; a <= i; a++) {
            temp += A[a].second - A[a].first + 1;
            v.match.push_back(make_pair(A[a], B[j]));
        }
        v.w = temp / (B[j].second - B[j].first + 1);
        return v;
    }
    else if (i == 0 && j == 0) {
        v.match.push_back(make_pair(A[i], B[j]));
        v.w = (A[i].second - A[i].first + 1) / (B[j].second - B[j].first + 1);
        return v;
    }
    else if (i > 0 && j > 0) {
        values min_1; min_1.w = numeric_limits<double>::max();
        values min_2; min_2.w = numeric_limits<double>::max();
        values min_3; min_3.w = numeric_limits<double>::max();
        double temp = 0;
        double temp_1 = 0;
        values temp_2, temp_3, temp_4;
        int a_1 = 0; int a_2 = 0;
        for (int a = i - 1; a > 0; a--) {
            temp_1 = 0;
            temp = 0;
            for (int b = a; b <= i ; b++) {
                temp_1 += A[b].second - A[b].first + 1;
            }
            temp_2 = Opt(A, B, a - 1, j - 1);
            temp = temp_2.w + (temp_1 / (B[j].second - B[j].first + 1));
            if (temp < min_1.w) {
                min_1.match = temp_2.match;
                min_1.w = temp;
                a_1 = a;
            }
        }

        for (int a = j - 1; a > 0; a--) {
            temp_1 = 0;
            temp = 0;
            for (int b = a; b <= j; b++) {
                temp_1 += B[b].second - B[b].first + 1;
            }
            temp_3 = Opt(A, B, i - 1, a - 1);
            temp = temp_3.w + ((A[i].second - A[i].first + 1) / temp_1);
            if (temp < min_2.w) {
                min_2.match = temp_3.match;
                min_2.w = temp;
                a_2 = a;
            }
        }

        temp_4 = Opt(A, B, i - 1, j - 1);
        min_3.w = temp_4.w + ((A[i].second - A[i].first + 1) / (B[j].second - B[j].first + 1));
        min_3.match = temp_4.match;

        values min; 
        min.w = min_1.w;
        min.match = min_1.match;
        for (int a = a_1; a <= i; a++) {
            min.match.push_back(make_pair(A[a], B[j]));
        }
        if (min.w > min_2.w) { 
            min.w = min_2.w;
            min.match = min_2.match;
            for (int a = a_2; a <= j; a++) {
                min.match.push_back(make_pair(A[i], B[a]));
            }
        }
        else if (min.w > min_3.w) { 
            min.w = min_3.w;
            min.match = min_3.match;
            min.match.push_back(make_pair(A[i], B[j]));
        }
        return min;
    }
}

values Opt(vector <double> A, vector <double> B, int p) {
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
    return Opt(M1, M2, _i - 1, _j - 1);
}

int main() {
    // TEST VECTORS
    //vector <double> A = { 0,1,1,1,0,0,1,0,1,1,0,1,1,0,1,1,1,0,1,0 };
    //vector <double> B = { 0,0,1,1,0,1,1,0,0,0,1,1,1,1,1,0,0,1,1,0 };
    //vector <double> A = { 1,1,0,1,0,0,1,0,1,0,0,1,1,1,0,1,1,0,1,1 };
    //vector <double> B = { 1,1,0,1,1,1,1,0,1,0,1,1,1,1,1,0,1,1,1,1 };
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

    values Matching = Opt(A, B, A.size());
    vector <pair<pair<int, int>, pair<int, int>>> S = Matching.match;
    cout << "Matching : " << endl;
    for (int i = 0; i < S.size(); i++) {
        cout << "(" << S[i].first.first << "," << S[i].first.second << ")-(" << S[i].second.first << "," << S[i].second.second << ") ";
    }
    cout << endl;
    cout << "Peso = " << Matching.w;
}
