#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

Mat src, dst_erosion, dst_dilation, element;
int opt_size=1;

// find out whether a point in the origin image is the same to the point in the element.
// note that the area beyond the origin image will be treated as match.
bool match(int src_x, int src_y, int elem_x, int elem_y) {
    if (src_x<0 || src_y<0 || src_x>=src.cols || src_y>=src.rows) return true;
    else if (src.at<uchar>(src_x, src_y)==element.at<uchar>(elem_x, elem_y)) return true;
    else return false;
}

// find out whether a point in the origin image is black
bool elem_is_black(int x, int y) {
    if (element.at<uchar>(x, y)==0) return true;
    else return false;
}

// translate the mat to binary image
void binary_image(Mat& mat) {
    for (int i=0; i<mat.rows;i++) {
        for (int j=0; j<mat.cols; j++) {
            if (mat.at<uchar>(i, j)<128) mat.at<uchar>(i, j)=0;
            else mat.at<uchar>(i, j)=255;
        }
    }
}

// do one step of erosion, compare the element to a block in the origin image,
// and decide the color of the point of the image after erosion
void do_erosion_one_step(int x, int y) {
    // scan and compare each point in the structuring element to a group of point in the origin image
    for (int i=x-opt_size, e_i=0; i<=x+opt_size; i++, e_i++) {
        for (int j=y-opt_size, e_j=0; j<=y+opt_size; j++, e_j++) {
            if (elem_is_black(e_i, e_j) && !match(i, j, e_i, e_j)) {
                dst_erosion.at<uchar>(x, y)=255;
                return;
            }
        }
    }
    dst_erosion.at<uchar>(x, y)=0;
}

// do one step of dilation, compare the element to a block in the origin image,
// and decide the color of the point of the image after erosion
void do_dilation_one_step(int x, int y) {
    // scan and compare each point in the structuring element to a group of point in the origin image
    for (int i=x-opt_size, e_i=0; i<=x+opt_size; i++, e_i++) {
        for (int j=y-opt_size, e_j=0; j<=y+opt_size; j++, e_j++) {
            if (elem_is_black(e_i, e_j) && match(i, j, e_i, e_j)) {
                dst_dilation.at<uchar>(x, y)=0;
                return;
            }
        }
    }
    dst_dilation.at<uchar>(x, y)=255;
}

int main() {
    // read the file
    string filename;
    cout << "filename: ";
    cin >> filename;
    src = imread(filename.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
    binary_image(src);
    dst_erosion = src.clone();
    dst_dilation = src.clone();

    // read the type of structuring element
    int ty;
    cout << "structuring element type: 1.rectangle  2.circle  3.cross: ";
    cin >> ty;

    if (ty==1) {
        element = getStructuringElement(MORPH_RECT, Size(2 * opt_size + 1, 2 * opt_size + 1),
                                        Point(opt_size, opt_size));
    }
    else if (ty==2) {
        element = getStructuringElement(MORPH_ELLIPSE, Size(2 * opt_size + 1, 2 * opt_size + 1),
                                        Point(opt_size, opt_size));
    }
    else {
        element = getStructuringElement(MORPH_CROSS, Size(2 * opt_size + 1, 2 * opt_size + 1),
                                        Point(opt_size, opt_size));
    }
    binary_image(element);

    // read the size of the structuring element
    cout << "size of structuring element: ";
    cin >> opt_size;

    // do erosion operation step by step
    for (int i=0; i<src.rows;i++) {
        for (int j=0; j<src.cols; j++) {
            do_erosion_one_step(i, j);
        }
    }

    // do dilation operation step by step
    for (int i=0; i<src.rows;i++) {
        for (int j=0; j<src.cols; j++) {
            do_dilation_one_step(i, j);
        }
    }

    // draw pictures
    imshow("origin", src);
    imshow("erosion", dst_erosion);
    imshow("dilation", dst_dilation);

    waitKey(0);
    destroyAllWindows();

    return 0;
}