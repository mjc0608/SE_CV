#include <iostream>
#include <map>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int main() {
    string filename;
    cout << "file name: ";
    cin >> filename;

    // load image
    IplImage *img = cvLoadImage(filename.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
    int color_cnt[256]; // color count
    int color_cnt_new[256];
    int color_map[256]; // the old color maps to the new color
    int width = img->width;
    int height = img->height;
    memset(color_cnt, 0, 256 * sizeof(int));
    memset(color_cnt_new, 0, 256 * sizeof(int));
    memset(color_map, 0, 256 * sizeof(int));

    // scan the image and count each color
    for (int row = 0; row < height; row++) {
        uchar *ptr = (uchar *) img->imageData + row * img->width;
        for (size_t cols = 0; cols < width; cols++) {
            int intensity = ptr[cols];
            color_cnt[intensity]++;
        }
    }

    int all_pixel = 0;
    for (int i=0; i<256; i++) {
        all_pixel += color_cnt[i];
    }

    int curr_pixel = 0;
    for (int i=0; i<256; i++) {
        curr_pixel += color_cnt[i];
        double color_perc=(double)curr_pixel/(double)all_pixel;
        color_map[i]=color_perc*255;
        color_cnt_new[color_map[i]]=color_cnt[i];
    }

    IplImage *img_post = cvCloneImage(img);
    IplImage *pHistImage_pre = cvCreateImage(cvSize(256*2, 312), IPL_DEPTH_8U, true);
    IplImage *pHistImage_post = cvCreateImage(cvSize(256*2, 312), IPL_DEPTH_8U, true);

    // take away the background color
    for (int row = 0; row < height; row++) {
        uchar *ptr = (uchar *) img_post->imageData + row * img->width;
        for (size_t cols = 0; cols < width; cols++) {
            ptr[cols] = color_map[ptr[cols]];
        }
    }

    // find the most frequent color
    int max_cnt = color_cnt[0];
    for (int i = 1; i < 256; i++) {
        if (max_cnt < color_cnt[i]) {
            max_cnt = color_cnt[i];
        }
    }

    // draw the histogram
    for (int i = 0; i < 256; i++) {
        cvRectangle(pHistImage_pre,
                    cvPoint(2*i, 312),
                    cvPoint(2*(i + 1), 312 - color_cnt[i] / (double) max_cnt * 300),
                    cvScalar(255, 0, 0, 0),
                    CV_FILLED);
    }

    // draw the histogram after operation
    for (int i = 0; i < 256; i++) {
        cvRectangle(pHistImage_post,
                    cvPoint(2*i, 312),
                    cvPoint(2*(i + 1), 312 - color_cnt_new[i] / (double) max_cnt * 300),
                    cvScalar(255, 0, 0, 0),
                    CV_FILLED);
    }


    Mat mat = Mat::Mat(img, false);
    Mat mat_post = Mat::Mat(img_post, false);
    Mat pHist_pre = Mat::Mat(pHistImage_pre, false);
    Mat pHist_post = Mat::Mat(pHistImage_post, false);

    // draw pictures
    imshow("phist origin", pHist_pre);
    imshow("phist post", pHist_post);
    imshow("origin", mat);
    imshow("post operation", mat_post);

    waitKey(0);
    destroyAllWindows();

    return 0;
}