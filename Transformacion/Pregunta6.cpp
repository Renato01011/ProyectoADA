#include <iostream>
#include <limits>
#include <vector>
#include <cmath>
#include <sstream>
#include <iterator>
#include <string>
using namespace std;

pair<vector <pair<pair<int, int>, pair<int, int>>>, double> Min_matching(vector<double> A, vector<double> B, int p) {
	//Declare final matching vector S and block vectors M1 and M2;
	vector <pair<pair<int, int>, pair<int, int>>> S;
	vector <pair<double, double>> M1, M2;
	//Find and save all blocks
	//Find and save all blocks
	int n1 = -1;
	int m1 = -1;
	for (int i = 0; i < p; i++) {
		if (A[i] == 1 && n1 == -1) {
			n1 = i;
		}
		else if (A[i] == 0 && n1 != -1) {
			M1.push_back(make_pair(n1, i - 1));
			n1 = -1;
		}
		if (i == p - 1 && A[i] == 1 && n1 != -1) {
			//M1.push_back(make_pair(n1, i - 1));
			M1.push_back(make_pair(n1, i));
		}
		else if (i == p - 1 && A[i] == 1 && n1 == -1) {
			//M1.push_back(make_pair(i - 1, i - 1));
			M1.push_back(make_pair(i, i));
		}
		if (B[i] == 1 && m1 == -1) {
			m1 = i;
		}
		else if (B[i] == 0 && m1 != -1) {
			M2.push_back(make_pair(m1, i - 1));
			m1 = -1;
		}
		if (i == p - 1 && B[i] == 1 && m1 != -1) {
			//M2.push_back(make_pair(m1, i - 1));
			M2.push_back(make_pair(m1, i));
		}
		else if (i == p - 1 && B[i] == 1 && m1 == -1) {
			//M2.push_back(make_pair(i - 1, i - 1));
			M2.push_back(make_pair(i, i));
		}
	}
	//Matching process
	int i = 0;
	int j = 0;
	double temp = 0.0;
	double peso = 0.0;
	double num = 0.0;

	while (i < M1.size() && j < M2.size()) {
		if (j == M2.size() - 1) {
			num = 0.0;
			while (i < M1.size()) {
				S.push_back(make_pair(M1[i], M2[j]));
				num += M1[i].second - M1[i].first + 1;
				i++;
			}
			peso += num / (M2[j].second - M2[j].first + 1);
		}
		else if (i == M1.size() - 1) {
			num = 0.0;
			while (j < M2.size()) {
				S.push_back(make_pair(M1[i], M2[j]));
				num += M2[j].second - M2[j].first + 1;
				j++;
			}
			peso += (M1[i].second - M1[i].first + 1) / num;
		}
		else if ((M1[i].second - M1[i].first + 1) > (M2[j].second - M2[j].first + 1)) {
			temp = (M1[i].second - M1[i].first + 1);
			num = 0.0;
			while (temp > 0) {
				S.push_back(make_pair(M1[i], M2[j]));
				temp = temp - (M2[j].second - M2[j].first + 1);
				num += M2[j].second - M2[j].first + 1;
				j++;
			}
			peso += (M1[i].second - M1[i].first + 1) / num;
			i++;
		}
		else if ((M1[i].second - M1[i].first + 1) < (M2[j].second - M2[j].first + 1)) {
			temp = (M2[j].second - M2[j].first + 1);
			num = 0.0;
			while (temp > 0) {
				S.push_back(make_pair(M1[i], M2[j]));
				temp = temp - (M1[i].second - M1[i].first + 1);
				num += M1[i].second - M1[i].first + 1;
				i++;
			}
			peso += num / (M2[j].second - M2[j].first + 1);
			j++;
		}
		else if ((M1[i].second - M1[i].first + 1) == (M2[j].second - M2[j].first + 1)) {
			S.push_back(make_pair(M1[i], M2[j]));
			peso += (M1[i].second - M1[i].first + 1) / (M2[j].second - M2[j].first + 1);
			i++;
			j++;
		}
	}
	return make_pair(S, peso);
}

pair< vector< vector< pair <pair<int, int>, pair<int, int>>>>, double> Transformacion_Voraz(vector <vector<double>> A, vector <vector<double>> B) {
	vector< vector< pair <pair<int, int>, pair<int, int>>>> M;
	double peso_total = 0.0;
	pair<vector <pair<pair<int, int>, pair<int, int>>>, double> min_matching;

	for (int i = 0; i < A.size(); i++) {
		min_matching = Min_matching(A[i], B[i], A[i].size());
		M.push_back(min_matching.first);
		peso_total += min_matching.second;
		cout << "Peso total = " << peso_total << endl;
	}

	return make_pair(M, peso_total);
}

int main() {
	vector <vector<double>> A = {{ 0,1,1,1,0,0,1,0,1,1,0,1,1,0,1,1,1,0,1,0 },
								 { 1,1,1,0,0,1,0,1,1,0,0,0,1,1,1,0,0,1,0,1 },
								 { 1,1,0,1,0,0,1,0,1,0,0,1,1,1,0,1,1,0,1,1 },
								 { 0,1,1,1,0,0,1,0,1,1,0,1,1,0,1,1,1,0,1,0 }};

	vector <vector<double>> B = {{ 1,1,0,1,1,1,1,0,1,0,1,1,1,1,1,0,1,1,1,1 },
								 { 0,0,0,1,1,0,1,0,1,0,1,0,1,1,0,0,0,0,0,0 },
								 { 0,0,1,1,0,1,1,0,0,0,1,1,1,1,1,0,0,1,1,0 },
								 { 0,0,1,1,0,1,1,0,0,0,1,1,1,1,1,0,0,1,1,0 }};

	pair< vector< vector< pair <pair<int, int>, pair<int, int>>>>, double> TV = Transformacion_Voraz(A, B);
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
