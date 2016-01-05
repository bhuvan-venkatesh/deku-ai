#ifndef IMAGE_CLASSIFIER_H
#define IMAGE_CLASSIFIER_H

#include "eye.hpp"

class Image_Classifier{
public:
  Image_Classifier(int blocks_): blocks(blocks_) {};

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
