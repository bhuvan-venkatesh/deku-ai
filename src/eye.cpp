#include "eye.h"
extern "C"{
#include <xdo.h>
}
#define emulator_name "zsnes"
#define screen_default NULL
using std::cout;
using namespace cv;
typedef unsigned int uint;
//ASSUMES default dislay
Eye::Eye(){
    disp = XOpenDisplay(screen_default);
    scr = DefaultScreen(disp);
    get_window(emulator_name);
    screen = XDefaultScreenOfDisplay(disp);
    draw_keypoints = false;
}

void Eye::get_window(const char* name){

    xdo_t* xdo = xdo_new (screen_default);
    if(!xdo){
        std::cerr<<"Xdo Allocation Failed"<<std::endl;
        throw;
    }
    xdo_search_t search;
    search.max_depth = -1;
    search.require = xdo_search::SEARCH_ANY;

    search.searchmask |= SEARCH_NAME;
    search.winname = name;

    Window *ret;
    unsigned int len;
    xdo_search_windows(xdo, &search, &ret, &len );

    if(len != 1){
        std::cerr<<"More than one match, be more specific"<<std::endl;
        xdo_free(xdo);
        throw;
    }
    //Assignment happens at the end to provide strong exception guarentee
    xdo_get_window_size(xdo, ret[0], &width, &height);
    root = ret[0];
    xdo_free(xdo);
}

std::vector<KeyPoint> Eye::analyze_screen(){
    cairo_surface_t* x11_surf = cairo_xlib_surface_create(disp, root, DefaultVisual(disp, scr), width, height);
    cairo_surface_t* img_surf = convert_xlib_to_image_surface(x11_surf);
    img_1 = convert_image_surface_to_mat(img_surf);
    vector<KeyPoint> points = analyze_keypoints(img_1);
    if(draw_keypoints){
        cv::Mat img_keypoints_1;
        drawKeypoints( img_1, points, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
        imshow("Keypoints 1", img_keypoints_1 );
        waitKey(0);
    }

    cairo_surface_destroy(x11_surf);
    cairo_surface_destroy(img_surf);
    x11_surf = NULL;
    img_surf = NULL;
    return points;
}

cairo_surface_t* Eye::convert_xlib_to_image_surface( cairo_surface_t* x11_surf){
    cairo_surface_t* img_surf = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cairo_t* cr = cairo_create(img_surf);
    cairo_set_source_surface(cr, x11_surf, 0, 0);
    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
    cairo_paint(cr);
    cairo_destroy(cr);

    return img_surf;

}

cv::Mat Eye::convert_image_surface_to_mat(cairo_surface_t* img_surf){
    unsigned char* ptr = cairo_image_surface_get_data(img_surf);

    if(!ptr){
        cout<<"ERROR ptr is bad";
    };
    //cairo_surface_write_to_png(img_surf,"test.png");

    Mat img_1 = cv::Mat::zeros(height, width, CV_8U);

    for (int i = 0; i < img_1.rows; ++i)
    {
        for (int j = 0; j < img_1.cols; j++)
        {
            uint alpha = (uint)*ptr;
            ++ptr;
            uint red = (uint)*ptr;
            ++ptr;
            uint blue = (uint)*ptr;
            ++ptr;
            uint green = (uint)*ptr;
            ++ptr;
            unsigned char average = ((unsigned char)((red+blue+green)/3*alpha/255.0));
            img_1.at<unsigned char>(i, j)=average;

        }
    }

    return img_1;
}

std::vector<KeyPoint> Eye::analyze_keypoints(const cv::Mat& img_1){
    int minHessian = 400;
    SurfFeatureDetector detector( minHessian );
    std::vector<KeyPoint> keypoints_1;
    Mat img_keypoints_1;

    detector.detect( img_1, keypoints_1 );
    return keypoints_1;
}
