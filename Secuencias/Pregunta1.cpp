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
	vector <pair<double, double>> M1,M2;
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
			M1.push_back(make_pair(n1, i - 1));
		}
		else if (i == p - 1 && A[i] == 1 && n1 == -1) {
			M1.push_back(make_pair(i - 1, i - 1));
		}
		if (B[i] == 1 && m1 == -1) {
			m1 = i;
		}
		else if (B[i] == 0 && m1 != -1) {
			M2.push_back(make_pair(m1, i - 1));
			m1 = -1;
		}
		if (i == p - 1 && B[i] == 1 && m1 != -1) {
			M2.push_back(make_pair(m1, i - 1));
		}
		else if (i == p - 1 && B[i] == 1 && m1 == -1) {
			M2.push_back(make_pair(i - 1, i - 1));
		}
	}
	//Matching process
	int i = 0;
	int j = 0; 
	double temp = 0.0;
	double peso = 0.0;
	double num = 0.0;

	while (i < M1.size() && j < M2.size()) {
		if (j == M2.size()-1) {
			num = 0.0;
			while (i < M1.size()) {
				S.push_back(make_pair(M1[i], M2[j]));
				num += M1[i].second - M1[i].first + 1;
				i++;
			}
			peso += num / (M2[j].second - M2[j].first + 1);
		}
		else if (i == M1.size()-1) {
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
	return make_pair(S,peso);
}

int main() {
	//vector <double> A = { 0,1,1,1,0,0,1,0,1,1,0,1,1,0,1,1,1,0,1,0 };
    //vector <double> B = { 0,0,1,1,0,1,1,0,0,0,1,1,1,1,1,0,0,1,1,0 };

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

	pair< vector <pair<pair<int, int>, pair<int, int>>>, double > Matching = Min_matching(A, B, A.size());
	vector <pair<pair<int, int>, pair<int, int>>> S = Matching.first;
	cout << "Matching : " << endl;
	for (int i = 0; i < S.size(); i++) {
		cout << "(" << S[i].first.first << "," << S[i].first.second << ")-(" << S[i].second.first << "," << S[i].second.second << ") ";
	}
	cout << endl;
	cout << "Peso = " << Matching.second;
}
