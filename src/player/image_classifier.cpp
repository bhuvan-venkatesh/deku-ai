#include "image_classifier.hpp"

Image_Classifier::Image_Classifier(int blocks_): blocks(blocks_) {
  cluster_centers = cc;
  max_iterations = iters;
  epsilon = eps;
  attempts = att;
};

vector<int32_t> Image_Classifier::block_classify(const Mat& image, const vector<KeyPoint>& keypoints){
  vector<cv::Point2f> points;
  for(auto i = keypoints.begin(); i != keypoints.end(); ++i){
    points.push_back(i->pt);
  }
  Mat labels, centers;
  double mean = kmeans(points,
                  cluster_centers,
                  labels, //Output labels
                  TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, //Stop either when iterations or epsilon has been hit
                    max_iterations, epsilon),
                  attempts,
                  KMEANS_PP_CENTERS, //Use chosen centers
                  centers); //output center matrix
}
