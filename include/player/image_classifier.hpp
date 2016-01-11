#ifndef IMAGE_CLASSIFIER_H
#define IMAGE_CLASSIFIER_H

#include "eye.hpp"
#define cc 30
#define iters 10
#define eps .01
#define att 2

class Image_Classifier{
public:
  Image_Classifier(int blocks_);

  void load_templates();
  void detect_mario(const Mat& img_scene, vector<int32_t>& ret, const int& width, const int& height);
  vector<int32_t> block_classify(const Mat& image);

  float rightmost;

private:
  int blocks; //Has to be a square number
  int side;
  vector<Point2f> prev;
  vector<Mat> templates;

  Ptr<FeatureDetector> detector;
  Ptr<FeatureDetector> harris;
  SiftDescriptorExtractor extractor;
  FlannBasedMatcher matcher;
};

#endif
