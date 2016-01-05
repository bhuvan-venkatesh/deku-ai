/*The brain will receive the keypoints
If kmeans is not initialized through a boolean flag,
  we will initialize the grouping and remember that each
  of the data set will get a grouping based on where it is.
else
  If kmeans is too big eject some of the older data that
  is not relevant -- through some generational data structure
  perscribed forgetting
  Then kmeans cluster the sample into a new clustering based on
  the old one to a blended degree, fuzzy for example. This is to
  have a simple sense of tracking --calling this evolutionary kmeans

_____
|1|2|
|3|4|

Then we take the image and divide it up into an n by n grid.
Take each chunk and see which data lies in the chunk and vote
on which kmeans grouping is the most dominant in that chunk
Whichever number wins that input will have the weight that
memory variable stores (It will be the cluster number and a random
assignment of positive or negative value). That will be the outputs
vector I believe as of now. The network is going to have to get a little
smarter on the input nodes -- communicating with the brain.

It is also going to have to be highly randomized but highly
sticky in order to latch on to right solutions we are going to need a metric
To measure this somehow*/

#include "brain.hpp"
#define inputs 256
#define outputs 8

Brain::Brain():
  pool(Pool(inputs, outputs)),
  cluster_centers(15),
  previous((time_t)0)
{

}

void Brain::play(){
  if(time(NULL)-previous >= 3600*5){ //Save every 5 minutes
    ofstream stream("NEAT.dat");
    pool.save(stream);
  }
  cv::Mat pic, labels, centers;
  vector<KeyPoint> key = eye.analyze_screen(pic);
  group(key, labels, centers);
  int rows = mat.rows;
  int cols = mat.cols;

}

void Brain::group(const vector<KeyPoint>& key, Mat& labels, Mat& centers){
  vector<Point2f> points;
  for(auto i = key.begin(); i != key.end(); ++i){
    points.push_back(key.pt);
  }
  int max_iterations = 100;
  double epsilon = .01;
  int attempts = 2;
  double mean = kmeans(points,
                  cluster_centers,
                  labels, //Output labels
                  TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, //Stop either when iterations or epsilon has been hit
                    max_iterations, epsilon),
                  attempts,
                  KMEANS_PP_CENTERS, //Use chosen centers
                  centers); //output center matrix
}
