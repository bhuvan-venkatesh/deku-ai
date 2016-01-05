#include "image_classifier.hpp"

vector<int32_t> Image_Classifier::block_classify(const Mat& image, const vector<KeyPoint>& points){
  vector<Point2f> points;
  for(auto i = key.begin(); i != key.end(); ++i){
    points.push_back(key.pt);
  }
  Mat lables, centers;
  double mean = kmeans(points,
                  cluster_centers,
                  labels, //Output labels
                  TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, //Stop either when iterations or epsilon has been hit
                    max_iterations, epsilon),
                  attempts,
                  KMEANS_PP_CENTERS, //Use chosen centers
                  centers); //output center matrix
}
