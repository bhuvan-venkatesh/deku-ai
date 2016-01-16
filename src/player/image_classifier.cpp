#include "image_classifier.hpp"
#include <sys/types.h>
#include <dirent.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#define key_threshold 3
#define tolerance_factor 3
#define mario_tolerance 100

Image_Classifier::Image_Classifier(int side_)
    : blocks(side_ * side_), detector(FeatureDetector::create("SIFT")),
      harris(FeatureDetector::create("HARRIS")), side(side_), prev_x(-1),
      prev_y(-1) {
  load_templates();
}

bool has_ending(std::string const &fullString, std::string const &ending) {
  if (fullString.length() >= ending.length()) {
    return (0 ==
            fullString.compare(fullString.length() - ending.length(),
                               ending.length(), ending));
  } else {
    return false;
  }
}

void Image_Classifier::load_templates() {

  const static vector<string> endings = {"jpg", "png", "jpeg", "jpg",
                                         "jpe", "bmp", "tiff", "tif"};

  DIR *d = opendir("."); // Should not be error unless dire
  struct dirent *de = NULL;
  while (de = readdir(d)) {
    string file(de->d_name);
    for (auto i = endings.begin(); i != endings.end(); ++i) {
      if (has_ending(file, *i)) {
        Mat templat = imread(de->d_name, CV_LOAD_IMAGE_GRAYSCALE);
        templates.push_back(templat);

        vector<KeyPoint> keypoints_object = get_keypoints(templat);
        template_keypoints.push_back(keypoints_object);

        Mat descriptors_object = get_descriptors(templat, keypoints_object);
        template_descriptors.push_back(descriptors_object);
      }
    }
  }
  closedir(d);
}

vector<KeyPoint> Image_Classifier::get_keypoints(const Mat &image) const {
  vector<KeyPoint> keypoints_object;
  detector->detect(image, keypoints_object);
  return keypoints_object;
}

Mat Image_Classifier::get_descriptors(
    const Mat &image, vector<KeyPoint> &keypoints_object) const {

  Mat descriptors_object;
  extractor.compute(image, keypoints_object, descriptors_object);
  return descriptors_object;
}

double Image_Classifier::min_distance(const vector<DMatch> &matches) const {
  double min_dist = 100;

  //-- Quick calculation of max and min distances between keypoints
  for (auto i = matches.begin(); i < matches.end(); i++) {
    double dist = i->distance;
    if (dist < min_dist)
      min_dist = dist;
  }
  return min_dist;
}

vector<DMatch>
Image_Classifier::matches_within_tolerance(const vector<DMatch> &matches,
                                           double tolerance) {
  vector<DMatch> good_matches;
  for (auto i = matches.begin(); i != matches.end(); i++) {
    if (i->distance < tolerance) {
      good_matches.push_back(*i);
    }
  }
  return good_matches;
}

Mat Image_Classifier::get_homogeny(const vector<DMatch> &matches,
                                   const vector<KeyPoint> &keypoints_scene,
                                   const vector<KeyPoint> &keypoints_object) {
  //-- Localize the object
  vector<Point2f> obj;
  vector<Point2f> scene;

  for (auto i = 0; i < matches.size(); i++) {
    //-- Get the keypoints from the good matches
    obj.push_back(keypoints_object[matches[i].queryIdx].pt);
    scene.push_back(keypoints_scene[matches[i].trainIdx].pt);
  }

  return findHomography(obj, scene, CV_RANSAC);
}

vector<int32_t> Image_Classifier::block_classify(const Mat &image) {
  vector<int32_t> ret(blocks, 0);
  vector<cv::KeyPoint> keypoints;

  harris->detect(image, keypoints);

  int height = image.rows / side;
  int width = image.cols / side;

  for (auto i = keypoints.begin(); i != keypoints.end(); ++i) {
    cv::Point2f color = i->pt;
    ret[(int)(color.x / width * side + color.y / height) % blocks] += 1;
  } /*

vector<string> files = {"1.png", "2.jpg", "3.png"};

for (auto i = files.begin(); i != files.end(); ++i) {
 Mat templat = imread("1.png", CV_LOAD_IMAGE_GRAYSCALE);
 imshow("1", templat);
 cv::waitKey(0);
 templates.push_back(templat);

 vector<KeyPoint> keypoints_object = get_keypoints(templat);
 template_keypoints.push_back(keypoints_object);

 Mat descriptors_object = get_descriptors(templat, keypoints_object);
 template_descriptors.push_back(descriptors_object);
}*/

  for (size_t i = 0; i < ret.size(); ++i) {
    if (ret[i] > key_threshold)
      ret[i] = -1;
  }
  detect_mario(image, ret, width, height);

  return ret;
}

void Image_Classifier::detect_mario(const Mat &img_scene, vector<int32_t> &ret,
                                    const int &block_width,
                                    const int &block_height) {

  for (size_t i = 0; i != templates.size(); ++i) {
    auto &descriptors_object = template_descriptors[i];
    auto &keypoints_object = template_keypoints[i];
    const auto &cols = templates[i].cols;
    const auto &rows = templates[i].rows;

    vector<KeyPoint> keypoints_scene = get_keypoints(img_scene);
    Mat descriptors_scene = get_descriptors(img_scene, keypoints_scene);

    vector<DMatch> matches;
    matcher.match(descriptors_object, descriptors_scene, matches);

    double min_dist = min_distance(matches);
    vector<DMatch> good_matches =
        matches_within_tolerance(matches, tolerance_factor * min_dist);

    Mat H;
    try {
      H = get_homogeny(good_matches, keypoints_scene, keypoints_object);
    } catch (...) {
      continue;
    }

    //-- Get the corners from the image_1 ( the object to be "detected" )
    std::vector<Point2f> obj_corners(4);
    obj_corners[0] = cvPoint(0, 0);
    obj_corners[1] = cvPoint(cols, 0);
    obj_corners[2] = cvPoint(cols, rows);
    obj_corners[3] = cvPoint(0, rows);
    std::vector<Point2f> scene_corners(4);

    perspectiveTransform(obj_corners, scene_corners, H);

    // Mario Rectangle Identified!
    const auto width = abs(scene_corners[0].x - scene_corners[1].x);
    auto height = abs(scene_corners[0].y - scene_corners[3].y);
    Point2f mario = scene_corners[0] + Point2f(cols, 0);

    // TODO: Check if it is within reason, if it is then colors those blocks
    // good
    if (abs(prev_y - mario.y) < mario_tolerance &&
            abs(prev_x - mario.x) < mario_tolerance ||
        prev_y == -1 && prev_x == -1) {
      prev_y = mario.y;
      prev_x = mario.y;
      for (int32_t cell = (int)(mario.x / block_width * side +
                                mario.y / block_height);
           height > 0; height -= block_height, cell += side) {
        int32_t prev = cell;
        for (auto temp_width = width; temp_width > 0;
             temp_width -= block_width, cell++) {
          ret[cell] = 1;
        }
        cell = prev;
      }
      break;
    }
    descriptors_scene.release();
    H.release();
  }
}
