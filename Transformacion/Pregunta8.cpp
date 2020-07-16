#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

vector<vector<double>> GetImage(string filename, double c1, double c2, double c3, double umbral) {
	Mat img = imread(filename);
	namedWindow("Input", WINDOW_AUTOSIZE);
    namedWindow("Output", WINDOW_AUTOSIZE);
	imshow("Input", img);
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
            if (temp < umbral) {
                img.at<Vec3b>(i, j)[0] = 0;
                img.at<Vec3b>(i, j)[1] = 0;
                img.at<Vec3b>(i, j)[2] = 0;
                M[i][j] = 0;
            }
            else {
                img.at<Vec3b>(i, j)[0] = 255;
                img.at<Vec3b>(i, j)[1] = 255;
                img.at<Vec3b>(i, j)[2] = 255;
                M[i][j] = 1;
            }
        }
    }
    imshow("Output", img);
	waitKey();

    return M;
}

int main() {

    string filename;
    cout << "Insert filename = ";
    cin >> filename;
    //0.2126, 0.7152, 0722
    vector<vector<double>> M = GetImage(filename, 0.2126, 0.7152, 0.0722, 255 / 2);

    for (int i = 0; i < M.size(); i++) {
        for (int j = 0; j < M[i].size(); j++) {
            cout << M[i][j] << " ";
        }
        cout << endl;
    }

	return 0;

}
