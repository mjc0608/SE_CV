#include <opencv2/opencv.hpp>
using namespace std;
IplImage *g_src, *g_canny;
const char *canny_title = "Canny";
const char *src_file = "/Users/jiacheng/Downloads/20.jpg";
const char *src_title = "Origin";
int width, height;

int nwhite_col(IplImage *img, int col) {
    int cnt = 0;
    for (int i=0; i<height; i++) {
        double c = cvGet2D(img, i, col).val[0];
        if ((int)c==255) cnt++;
    }
    return cnt;
}

int main()
{
    g_src = cvLoadImage(src_file, CV_LOAD_IMAGE_GRAYSCALE);
    g_canny = cvCreateImage(cvGetSize(g_src), IPL_DEPTH_8U, 1);

    width = g_src->width;
    height = g_src->height;

    int *white_arr = new int[width];

    cvNamedWindow(src_title, CV_WINDOW_AUTOSIZE);
    cvNamedWindow(canny_title, CV_WINDOW_AUTOSIZE);
    cvShowImage(src_title, g_src);

    //canny
    int threshold = 200;
    cvCanny(g_src, g_canny, threshold, threshold * 3, 3);

    int white_sum = 0;
    for (int i=0; i<width; i++) {
        white_arr[i] = nwhite_col(g_canny, i);
        white_sum += white_arr[i];
        cout << white_arr[i] << " ";
    }
    cout << endl;



//    for(int i=0;i<g_canny->height;i++)
//        for(int j=0;j<g_canny->width;j++) {
//            int tmp = cvGet2D(g_canny, i, j).val[0];
//            cvSet2D(g_canny, i, j, cvScalar(255-tmp));
//        }

    cvShowImage(canny_title, g_canny);

    cvWaitKey();

    cvDestroyWindow(src_title);
    cvDestroyWindow(canny_title);
    cvReleaseImage(&g_src);
    cvReleaseImage(&g_canny);
    return 0;
}