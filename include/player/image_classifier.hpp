#pragma once

#include "eye.hpp"

using std::vector;
using std::string;
using cv::Mat;
using cv::KeyPoint;
using cv::FeatureDetector;
using cv::FlannBasedMatcher;
using cv::SiftDescriptorExtractor;
using cv::Ptr;
using cv::Point2f;
using cv::DMatch;
using cv::imread;

class Image_Classifier{
public:
  Image_Classifier(int blocks_);

  void load_templates();
  void detect_mario(const Mat& img_scene, vector<int32_t>& ret,
    const int& width, const int& height);
  vector<int32_t> block_classify(const Mat& image);
  vector<KeyPoint> get_keypoints(const Mat& image) const;
  Mat get_descriptors(const Mat& image,
    vector<KeyPoint>& keypoints_object) const;

  double min_distance(const vector< DMatch >& matches) const;
  vector< DMatch > matches_within_tolerance(const vector< DMatch >& matches, double tolerance);
  Mat get_homogeny(const vector< DMatch >& matches, const vector<KeyPoint>& keypoints_scene,
              const vector<KeyPoint>& keypoints_object);

  int32_t prev_x;
  int32_t prev_y;
private:
  int32_t blocks; //Has to be a square number
  int32_t side;

  vector<Point2f> prev;
  vector<Mat> templates;
  vector<vector<KeyPoint>> template_keypoints;
  vector<Mat> template_descriptors;

  Ptr<FeatureDetector> detector;
  Ptr<FeatureDetector> harris;
  SiftDescriptorExtractor extractor;
  FlannBasedMatcher matcher;
};
