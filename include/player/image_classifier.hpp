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

  vector<int32_t> block_classify(const Mat& image, const vector<KeyPoint>& points);

private:
  int blocks; //Has to be a square number
  vector<Point2f> prev;
	int cluster_centers;
	int max_iterations;
  double epsilon;
  int attempts;
};

#endif
