#include "eye.h"
using std::cout;
using namespace cv;
typedef unsigned int uint;
//ASSUMES default dislay
Eye::Eye(){
    window = Emulator_Window::get_emulator();
    draw_keypoints = false;
}

std::vector<KeyPoint> Eye::analyze_screen(){
    const unsigned int width = window->width, height = window->height;

    cairo_surface_t* x11_surf = cairo_xlib_surface_create(window->disp, window->root,
                                                          DefaultVisual(window->disp, window->scr),
                                                          width, height);
    cairo_surface_t* img_surf = convert_xlib_to_image_surface(x11_surf, width, height);
    img_1 = convert_image_surface_to_mat(img_surf, width, height);
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

cairo_surface_t* Eye::convert_xlib_to_image_surface( cairo_surface_t* x11_surf,
  const unsigned int& width, const unsigned int& height){
    cairo_surface_t* img_surf = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cairo_t* cr = cairo_create(img_surf);
    cairo_set_source_surface(cr, x11_surf, 0, 0);
    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
    cairo_paint(cr);
    cairo_destroy(cr);

    return img_surf;

}

cv::Mat Eye::convert_image_surface_to_mat(cairo_surface_t* img_surf,const unsigned int& width, const unsigned int& height){
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
