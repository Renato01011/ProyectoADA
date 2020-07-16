#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <iterator>
#include <string>

#include <chrono>
#include <thread>

using namespace cv;
using namespace std;

#define NUM_INT_IMG 10

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                   IMAGE PROCESSING                                                              //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

vector<vector<double>> GetImage(string filename, double c1, double c2, double c3, double umbral) {
    Mat img = imread(filename);
    //namedWindow("Input", WINDOW_AUTOSIZE);
    //namedWindow("Black Image", WINDOW_AUTOSIZE);
    //imshow("Input", img);
    vector<vector<double>> M;
    M.resize(img.rows);
    for (int i = 0; i < M.size(); i++) {
        M[i].resize(img.cols);
    }
    for (int i = 0; i < img.rows; ++i) {
        for (int j = 0; j < img.cols; ++j) {
            int B = img.at<Vec3b>(i, j)[0];
            int G = img.at<Vec3b>(i, j)[1];
            int R = img.at<Vec3b>(i, j)[2];
            int temp = (c3 * B) + (c2 * G) + (c1 * R);

            if (temp <= umbral) {
                img.at<Vec3b>(i, j)[0] = 0;
                img.at<Vec3b>(i, j)[1] = 0;
                img.at<Vec3b>(i, j)[2] = 0;
                /*img.at<Vec3b>(i, j)[0] = 255;
                img.at<Vec3b>(i, j)[1] = 255;
                img.at<Vec3b>(i, j)[2] = 255;*/
                M[i][j] = 0;
            }
            else {
                /*img.at<Vec3b>(i, j)[0] = 0;
                img.at<Vec3b>(i, j)[1] = 0;
                img.at<Vec3b>(i, j)[2] = 0;*/
                img.at<Vec3b>(i, j)[0] = 255;
                img.at<Vec3b>(i, j)[1] = 255;
                img.at<Vec3b>(i, j)[2] = 255;
                M[i][j] = 1;
            }
        }
    }
    //imshow("Black Image", img);
    waitKey();

    return M;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                      TRANSFORMACION Y MIN MATCHING VORAZ                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

pair<vector <pair<pair<int, int>, pair<int, int>>>, double> Min_matching(vector<double> A, vector<double> B, int p) {
	//Declare final matching vector S and block vectors M1 and M2;
    vector <pair<pair<int, int>, pair<int, int>>> S = {};
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

    if (_i == 0 || _j == 0) {
        return make_pair(S, 0);
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
                if (j == M2.size()) { break; }
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
                if (i == M1.size()) { break; }
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
    }

    return make_pair(M, peso_total);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                TRANSFORMACION Y MIN MATCHING PROGRAMACION DINAMICA                                              //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct values {
    double w = 0;
    vector<pair<pair<int, int>, pair<int, int>>> match = {};
};

//Matrix for storing already calculated values
vector<vector<values>> M;
vector<vector<int>> M_1;
vector<vector<int>> M_2;

values Min_Matching_Prog_Din(vector <pair<double, double>>& A, vector <pair<double, double>>& B, int i, int j) {
    //Case : i == 0 && j > 0
    for (int _j = 0; _j < j; _j++) {
        double temp = 0;
        for (int a = 0; a <= _j; a++) {
            temp += B[a].second - B[a].first + 1;
            M[0][_j].match.push_back(make_pair(A[0], B[a]));
        }
        M[0][_j].w = (A[0].second - A[0].first + 1) / temp;
    }
    //Case : i > 0 && j == 0
    for (int _i = 0; _i < i; _i++) {
        double temp = 0;
        for (int a = 0; a <= _i; a++) {
            temp += A[a].second - A[a].first + 1;
            M[_i][0].match.push_back(make_pair(A[a], B[0]));
        }
        M[_i][0].w = temp / (B[0].second - B[0].first + 1);
    }

    //Case : i == 0 && j == 0
    M[0][0].match.push_back(make_pair(A[0], B[0]));
    M[0][0].w = (A[0].second - A[0].first + 1) / (B[0].second - B[0].first + 1);

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

                temp = M[a - 1][_j - 1].w + (temp_1 / (B[_j].second - B[_j].first + 1));
                if (temp < min_1) {
                    min_1 = temp;
                    a_1 = a;
                }
            }

            for (int a = _j - 1; a > 0; a--) {
                temp_1 = 0;
                temp = 0;

                temp_1 = M_2[_j][a];

                temp = M[_i - 1][a - 1].w + ((A[_i].second - A[_i].first + 1) / temp_1);
                if (temp < min_2) {
                    min_2 = temp;
                    a_2 = a;
                }
            }

            min_3 = M[_i - 1][_j - 1].w + ((A[_i].second - A[_i].first + 1) / (B[_j].second - B[_j].first + 1));

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

values Min_Matching_Prog_Din(vector <double> A, vector <double> B, int p) {
    if (M.size() != 0) { M.clear(); }
    if (M_1.size() != 0) { M_1.clear(); }
    if (M_2.size() != 0) { M_2.clear(); }

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
//  cout << "_i = " << _i << ", _j = " << _j << endl;
    if (_i == 0 || _j == 0) { values M;  return M; }
    return Min_Matching_Prog_Din(M1, M2, _i - 1, _j - 1);
};

pair < vector< vector<pair<pair<int, int>, pair<int, int>>>>, double> Transformacion_Prog_Din(vector <vector<double>> &A, vector <vector<double>> &B) {
    vector< vector<pair<pair<int, int>, pair<int, int>>> > M;
    double peso_total = 0.0;
    values min_matching;

    for (int i = 0; i < A.size(); i++) {
        min_matching = Min_Matching_Prog_Din(A[i], B[i], A[i].size());
        M.push_back(min_matching.match);
        peso_total += min_matching.w;
    }

    return make_pair(M, peso_total);

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           TRANSFORMACION Y MIN MATCHING PROMEDIO PROGRAMACION DINAMICA                                          //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

values Min_Matching_Promedio_Prog_Din(vector <pair<double, double>>& A, vector <pair<double, double>>& B, int i, int j, double mu) {
    //Case : i == 0 && j > 0
    for (int _j = 0; _j < j; _j++) {
        double temp = 0;
        for (int a = 0; a <= _j; a++) {
            temp += B[a].second - B[a].first + 1;
            M[0][_j].match.push_back(make_pair(A[0], B[a]));
        }
        M[0][_j].w = abs(((A[0].second - A[0].first + 1) / temp) - mu);
    }
    //Case : i > 0 && j == 0
    for (int _i = 0; _i < i; _i++) {
        double temp = 0;
        for (int a = 0; a <= _i; a++) {
            temp += A[a].second - A[a].first + 1;
            M[_i][0].match.push_back(make_pair(A[a], B[0]));
        }
        M[_i][0].w = abs((temp / (B[0].second - B[0].first + 1)) - mu);
    }

    //Case : i == 0 && j == 0
    M[0][0].match.push_back(make_pair(A[0], B[0]));
    M[0][0].w = abs(((A[0].second - A[0].first + 1) / (B[0].second - B[0].first + 1)) - mu);

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

                temp = M[a - 1][_j - 1].w + abs((temp_1 / (B[_j].second - B[_j].first + 1)) - mu);
                if (temp < min_1) {

                    min_1 = temp;
                    a_1 = a;
                }
            }

            for (int a = _j - 1; a > 0; a--) {
                temp_1 = 0;
                temp = 0;

                temp_1 = M_2[_j][a];

                temp = M[_i - 1][a - 1].w + abs(((A[_i].second - A[_i].first + 1) / temp_1) - mu);
                if (temp < min_2) {
                    min_2 = temp;
                    a_2 = a;
                }
            }

            min_3 = M[_i - 1][_j - 1].w + abs(((A[_i].second - A[_i].first + 1) / (B[_j].second - B[_j].first + 1)) - mu);

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
    if (M.size() != 0) { M.clear(); }
    if (M_1.size() != 0) { M_1.clear(); }
    if (M_2.size() != 0) { M_2.clear(); }

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
    
    if (_i == 0 || _j == 0) { values M;  return M; }
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
    }

    return make_pair(M, peso_total);

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                     ANIMACION                                                                   //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Animacion(string filename_1, string filename_2, int Tipo_De_Transformacion) {
    //Umbral = 255 / 5

    //Y_709 == Y
    //vector<vector<double>> M_1 = GetImage(filename_1, 0.2126, 0.7152, 0.0722, 255 / 5);
    //vector<vector<double>> M_2 = GetImage(filename_2, 0.2126, 0.7152, 0.0722, 255 / 5);

    //Y_601
    //vector<vector<double>> M_1 = GetImage(filename_1, 0.299, 0.587, 0.114, 255 / 5);
    //vector<vector<double>> M_2 = GetImage(filename_2, 0.299, 0.587, 0.114, 255 / 5);

    //Y_240
    vector<vector<double>> M_1 = GetImage(filename_1, 0.212, 0.701, 0.087, 255 / 5);
    vector<vector<double>> M_2 = GetImage(filename_2, 0.212, 0.701, 0.087, 255 / 5);

    namedWindow("Input", WINDOW_AUTOSIZE);
    namedWindow("Output", WINDOW_AUTOSIZE);

    Mat img_1 = imread(filename_1);
    vector<Mat> int_img;
    for (int i = 0; i < NUM_INT_IMG; i++) {
        int_img.push_back(imread(filename_1));
    }
    Mat img_2 = imread(filename_2);

    pair< vector< vector< pair <pair<int, int>, pair<int, int>>>>, double> TV;
    
    switch (Tipo_De_Transformacion) {
    case 1:
        TV = Transformacion_Voraz(M_1, M_2);
        break;
    case 2:
        TV = Transformacion_Prog_Din(M_1, M_2);
        break;
    case 3:
        TV = Transformacion_Promedio_Prog_Din(M_1, M_2);
        break;
    default:
        TV = Transformacion_Prog_Din(M_1, M_2);
        break;
    }
    
    vector< vector< pair <pair<int, int>, pair<int, int>>>> M_T = TV.first;
    
    for (int i = 0; i < M_T.size(); i++) {
        if (M_T[i].size() == 0) { continue; }
        int _j = 0; 
        int max_j = M_T[i].size();
        while (_j < max_j - 2) {
            if (M_T[i][_j].first == M_T[i][_j + 1].first) {
                pair<int, int> curr = M_T[i][_j].first;
                int index_i_begin = M_T[i][_j].first.first;
                int index_i_end = M_T[i][_j].first.second;
                while (curr == M_T[i][_j].first) {
                    int index_j_begin = M_T[i][_j].second.first;
                    int index_j_end = M_T[i][_j].second.second;
                    while (index_j_begin < index_j_end && index_i_begin < index_i_end) {
                        double B_1 = img_1.at<Vec3b>(i, index_i_begin)[0];
                        double G_1 = img_1.at<Vec3b>(i, index_i_begin)[1];
                        double R_1 = img_1.at<Vec3b>(i, index_i_begin)[2];

                        double B_2 = img_2.at<Vec3b>(i, index_i_begin)[0];
                        double G_2 = img_2.at<Vec3b>(i, index_i_begin)[1];
                        double R_2 = img_2.at<Vec3b>(i, index_i_begin)[2];

                        double diff_B = (B_2 - B_1) / NUM_INT_IMG;
                        double diff_G = (G_2 - G_1) / NUM_INT_IMG;
                        double diff_R = (R_2 - R_1) / NUM_INT_IMG;

                        for (int k = 0; k < NUM_INT_IMG; k++) {
                            int_img[k].at<Vec3b>(i, index_i_begin)[0] = B_1 + (diff_B * (k + 1));
                            int_img[k].at<Vec3b>(i, index_i_begin)[1] = G_1 + (diff_G * (k + 1));
                            int_img[k].at<Vec3b>(i, index_i_begin)[2] = R_1 + (diff_R * (k + 1));
                        }
                        
                        B_1 = img_1.at<Vec3b>(i, index_j_begin)[0];
                        G_1 = img_1.at<Vec3b>(i, index_j_begin)[1];
                        R_1 = img_1.at<Vec3b>(i, index_j_begin)[2];

                        B_2 = img_2.at<Vec3b>(i, index_j_begin)[0];
                        G_2 = img_2.at<Vec3b>(i, index_j_begin)[1];
                        R_2 = img_2.at<Vec3b>(i, index_j_begin)[2];

                        diff_B = (B_2 - B_1) / NUM_INT_IMG;
                        diff_G = (G_2 - G_1) / NUM_INT_IMG;
                        diff_R = (R_2 - R_1) / NUM_INT_IMG;

                        for (int k = 0; k < NUM_INT_IMG; k++) {
                            int_img[k].at<Vec3b>(i, index_j_begin)[0] = B_1 + (diff_B * (k + 1));
                            int_img[k].at<Vec3b>(i, index_j_begin)[1] = G_1 + (diff_G * (k + 1));
                            int_img[k].at<Vec3b>(i, index_j_begin)[2] = R_1 + (diff_R * (k + 1));
                        }
                        
                        index_j_begin++;
                        index_i_begin++;
                    }
                    _j++;
                    if (_j == max_j) { break; }
                }
            }
            else if (M_T[i][_j].second == M_T[i][_j + 1].second) {
                pair<int, int> curr = M_T[i][_j].second;
                int index_j_begin = M_T[i][_j].second.first;
                int index_j_end = M_T[i][_j].second.second;
                while (curr == M_T[i][_j].second) {
                    int index_i_begin = M_T[i][_j].first.first;
                    int index_i_end = M_T[i][_j].first.second;
                    while (index_j_begin < index_j_end && index_i_begin < index_i_end) {
                        double B_1 = img_1.at<Vec3b>(i, index_i_begin)[0];
                        double G_1 = img_1.at<Vec3b>(i, index_i_begin)[1];
                        double R_1 = img_1.at<Vec3b>(i, index_i_begin)[2];

                        double B_2 = img_2.at<Vec3b>(i, index_i_begin)[0];
                        double G_2 = img_2.at<Vec3b>(i, index_i_begin)[1];
                        double R_2 = img_2.at<Vec3b>(i, index_i_begin)[2];

                        double diff_B = (B_2 - B_1) / NUM_INT_IMG;
                        double diff_G = (G_2 - G_1) / NUM_INT_IMG;
                        double diff_R = (R_2 - R_1) / NUM_INT_IMG;

                        for (int k = 0; k < NUM_INT_IMG; k++) {
                            int_img[k].at<Vec3b>(i, index_i_begin)[0] = B_1 + (diff_B * (k + 1));
                            int_img[k].at<Vec3b>(i, index_i_begin)[1] = G_1 + (diff_G * (k + 1));
                            int_img[k].at<Vec3b>(i, index_i_begin)[2] = R_1 + (diff_R * (k + 1));
                        }

                        B_1 = img_1.at<Vec3b>(i, index_j_begin)[0];
                        G_1 = img_1.at<Vec3b>(i, index_j_begin)[1];
                        R_1 = img_1.at<Vec3b>(i, index_j_begin)[2];

                        B_2 = img_2.at<Vec3b>(i, index_j_begin)[0];
                        G_2 = img_2.at<Vec3b>(i, index_j_begin)[1];
                        R_2 = img_2.at<Vec3b>(i, index_j_begin)[2];

                        diff_B = (B_2 - B_1) / NUM_INT_IMG;
                        diff_G = (G_2 - G_1) / NUM_INT_IMG;
                        diff_R = (R_2 - R_1) / NUM_INT_IMG;

                        for (int k = 0; k < NUM_INT_IMG; k++) {
                            int_img[k].at<Vec3b>(i, index_j_begin)[0] = B_1 + (diff_B * (k + 1));
                            int_img[k].at<Vec3b>(i, index_j_begin)[1] = G_1 + (diff_G * (k + 1));
                            int_img[k].at<Vec3b>(i, index_j_begin)[2] = R_1 + (diff_R * (k + 1));
                        }

                        index_j_begin++;
                        index_i_begin++;
                    }
                    _j++;
                    if (_j == max_j) { break; }
                }
            }
            else {
                int index_j_begin = M_T[i][_j].second.first;
                int index_j_end = M_T[i][_j].second.second;
                int index_i_begin = M_T[i][_j].first.first;
                int index_i_end = M_T[i][_j].first.second;
                while (index_i_begin < index_i_end && index_j_begin < index_j_end) {
                    double B_1 = img_1.at<Vec3b>(i, index_i_begin)[0];
                    double G_1 = img_1.at<Vec3b>(i, index_i_begin)[1];
                    double R_1 = img_1.at<Vec3b>(i, index_i_begin)[2];

                    double B_2 = img_2.at<Vec3b>(i, index_i_begin)[0];
                    double G_2 = img_2.at<Vec3b>(i, index_i_begin)[1];
                    double R_2 = img_2.at<Vec3b>(i, index_i_begin)[2];

                    double diff_B = (B_2 - B_1) / NUM_INT_IMG;
                    double diff_G = (G_2 - G_1) / NUM_INT_IMG;
                    double diff_R = (R_2 - R_1) / NUM_INT_IMG;

                    for (int k = 0; k < NUM_INT_IMG; k++) {
                        int_img[k].at<Vec3b>(i, index_i_begin)[0] = B_1 + (diff_B * (k + 1));
                        int_img[k].at<Vec3b>(i, index_i_begin)[1] = G_1 + (diff_G * (k + 1));
                        int_img[k].at<Vec3b>(i, index_i_begin)[2] = R_1 + (diff_R * (k + 1));
                    }

                    B_1 = img_1.at<Vec3b>(i, index_j_begin)[0];
                    G_1 = img_1.at<Vec3b>(i, index_j_begin)[1];
                    R_1 = img_1.at<Vec3b>(i, index_j_begin)[2];

                    B_2 = img_2.at<Vec3b>(i, index_j_begin)[0];
                    G_2 = img_2.at<Vec3b>(i, index_j_begin)[1];
                    R_2 = img_2.at<Vec3b>(i, index_j_begin)[2];

                    diff_B = (B_2 - B_1) / NUM_INT_IMG;
                    diff_G = (G_2 - G_1) / NUM_INT_IMG;
                    diff_R = (R_2 - R_1) / NUM_INT_IMG;

                    for (int k = 0; k < NUM_INT_IMG; k++) {
                        int_img[k].at<Vec3b>(i, index_j_begin)[0] = B_1 + (diff_B * (k + 1));
                        int_img[k].at<Vec3b>(i, index_j_begin)[1] = G_1 + (diff_G * (k + 1));
                        int_img[k].at<Vec3b>(i, index_j_begin)[2] = R_1 + (diff_R * (k + 1));
                    }

                    index_j_begin++;
                    index_i_begin++;
                }
                _j++;
                if (_j == max_j) { break; }
            }
        }
    }

    imshow("Input", img_1);
    string s_temp = "Int_0";
    for (int i = 0; i < NUM_INT_IMG; i++) {
        imshow("Input", int_img[i]);
        //this_thread::sleep_for(chrono::seconds(1));
        waitKey();
        //img_1.setTo(Scalar(0, 0, 0));
        /*imshow(s_temp, int_img[i]);
        s_temp.replace(4, 1, to_string(i+1));*/
    }
    //imshow("Output", img_2);
    imshow("Input", img_2);
    waitKey();
}

int main() {

    string filename_1, filename_2;
    int Tipo_De_Transformacion;
    cout << "Insert image 1 name = ";
    cin >> filename_1;
    cout << "Insert image 2 name = ";
    cin >> filename_2;
    cout << "Elegir tipo de transformacion (1 == Voraz, 2 == Prog Din, 3 == Prog Din Promedio) // If another number is inserted then Prog Din will be choosen" << endl;
    cout << "Tipo = ";
    cin >> Tipo_De_Transformacion;

    Animacion(filename_1, filename_2, Tipo_De_Transformacion);
    
    return 0;
}
