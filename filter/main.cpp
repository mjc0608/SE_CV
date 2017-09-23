#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <math.h>
using namespace std;
using namespace cv;

Mat src, tmp, dst_gauss, dst_mean, dst_median, element;
int opt_size=3;

#define G_SIGMA 2

// the gauss function
double gauss(double x, double y) {
    return (1/(2*M_PI*G_SIGMA*G_SIGMA))*exp(-0.5*((x*x+y*y)/(G_SIGMA*G_SIGMA)));
}

// guass filter function, input the coordinate, output the value after the filter operation
int gauss_filter(int x, int y, Mat& s) {
    double sum = 0;
    double g_sum = 0;
    for (int i=-opt_size; i<=opt_size; i++) {
        for (int j=-opt_size; j<=opt_size; j++) {
            double g = gauss(i, j);
            sum += g*s.at<uchar>(x+i, y+j);
            g_sum += g;
        }
    }
    return (int)(sum/g_sum);
}

// mean filter function, input the coordinate, output the value after the filter operation
int mean_filter(int x, int y, Mat& s) {
    int sum = 0;
    for (int i=-opt_size; i<=opt_size; i++) {
        for (int j=-opt_size; j<=opt_size; j++) {
            sum+=s.at<uchar>(x+i, y+j);
        }
    }
    return sum/((opt_size*2+1)*(opt_size*2+1));
}

// median filter function, input the coordinate, output the value after the filter operation
int median_filter(int x, int y, Mat& s) {
    vector<int> vec;
    for (int i=-opt_size; i<=opt_size; i++) {
        for (int j=-opt_size; j<=opt_size; j++) {
            vec.push_back(s.at<uchar>(x+i, y+j));
        }
    }
    sort(vec.begin(), vec.end());
    return vec[vec.size()/2];
}

int main() {
    // read the file
    string filename;
    cout << "filename: ";
    cin >> filename;

    // set filter size
    int t;
    cout << "filter size (odd): ";
    cin >> t;
    opt_size = t/2;

    src = imread(filename.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
    dst_gauss = src.clone();
    dst_mean = src.clone();
    dst_median = src.clone();
    tmp = src.clone();

    int top = opt_size, bottom = opt_size, left = opt_size, right = opt_size;

    // extend the border to satisfy the filter
    copyMakeBorder(src, tmp, top, bottom, left, right, BORDER_REPLICATE);

    // do gauss filter operation
    for (int i = opt_size; i<=tmp.rows-opt_size; i++) {
        for (int j = opt_size; j<=tmp.cols-opt_size; j++) {
            dst_gauss.at<uchar>(i-opt_size, j-opt_size)=(uchar)gauss_filter(i, j, tmp);
        }
    }

    // do mean filter operation
    for (int i = opt_size; i<=tmp.rows-opt_size; i++) {
        for (int j = opt_size; j<=tmp.cols-opt_size; j++) {
            dst_mean.at<uchar>(i-opt_size, j-opt_size)=(uchar)mean_filter(i, j, tmp);
        }
    }

    // do median filter operation
    for (int i = opt_size; i<=tmp.rows-opt_size; i++) {
        for (int j = opt_size; j<=tmp.cols-opt_size; j++) {
            dst_median.at<uchar>(i-opt_size, j-opt_size)=(uchar)median_filter(i, j, tmp);
        }
    }

    // draw pictures
    imshow("origin", src);
    imshow("gauss filter", dst_gauss);
    imshow("mean filter", dst_mean);
    imshow("median filter", dst_median);

    waitKey(0);
    destroyAllWindows();

    return 0;
}