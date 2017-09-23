#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

// return the sum of the variance of the background and foreground
double cacuate_variance_sum(int *color_cnt, int mid) {
    double w1, w2, u1 = 0, u2 = 0;
    w1 = ((double) mid) / 256;
    w2 = 1 - w1;
    int left = 0, right = 0;

    for (int i = 0; i < mid; i++) {
        u1 += color_cnt[i] * i;
        left += color_cnt[i];
    }
    u1 /= left;

    for (int i = mid; i < 256; i++) {
        u2 += color_cnt[i] * i;
        right += color_cnt[i];
    }
    u2 /= right;

    return w1 * w2 * (u1 - u2) * (u1 - u2);
}

int main() {
    string filename;
    cout << "file name: ";
    cin >> filename;

    // load image
    IplImage *img = cvLoadImage(filename.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
    int color_cnt[256]; // color count
    int width = img->width;
    int height = img->height;
    memset(color_cnt, 0, 256 * sizeof(int));

    // scan the image and count each color
    for (int row = 0; row < height; row++) {
        uchar *ptr = (uchar *) img->imageData + row * img->width;
        for (size_t cols = 0; cols < width; cols++) {
            int intensity = ptr[cols];
            color_cnt[intensity]++;
        }
    }

    // find the max of the sum of the variance of the background and foreground
    double max_va = cacuate_variance_sum(color_cnt, 1);
    int max_c = 1;
    for (int i = 1; i < 256; i++) {
        double va = cacuate_variance_sum(color_cnt, i);
        if (va > max_va) {
            max_va = va;
            max_c = i;
        }
    }

    IplImage *img_background = cvCloneImage(img);
    IplImage *pHistImage = cvCreateImage(cvSize(256 * 2, 256), IPL_DEPTH_8U, true);

    // find the most frequent color
    int max_cnt = color_cnt[0];
    for (int i = 1; i < 256; i++) {
        if (max_cnt < color_cnt[i]) {
            max_cnt = color_cnt[i];
        }
    }

    // draw the histogram
    for (int i = 0; i < 256; i++) {
        cvRectangle(pHistImage,
                    cvPoint(2 * i, 256),
                    cvPoint(2 * (i + 1), 256 - color_cnt[i] / (double) max_cnt * 256),
                    cvScalar(i, 0, 0, 0),
                    CV_FILLED);
    }

    // take away the background color
    for (int row = 0; row < height; row++) {
        uchar *ptr = (uchar *) img->imageData + row * img->width;
        for (size_t cols = 0; cols < width; cols++) {
            if (ptr[cols] < max_c) {
                ptr[cols] = 0;
            }
        }
    }

    // take away the foreground color
    for (int row = 0; row < height; row++) {
        uchar *ptr = (uchar *) img_background->imageData + row * img_background->width;
        for (size_t cols = 0; cols < width; cols++) {
            if (ptr[cols] > max_c) {
                ptr[cols] = 0;
            }
        }
    }

    Mat mat = Mat::Mat(img, false);
    Mat mat_background = Mat::Mat(img_background, false);
    Mat pHist = Mat::Mat(pHistImage, false);

    // draw pictures
    imshow("phist", pHist);
    imshow("foreground", mat);
    imshow("background", mat_background);

    waitKey(0);
    destroyAllWindows();

    return 0;
}