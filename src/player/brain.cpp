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
#include "eye.hpp"
#include "brain.hpp"

void Brain::play(){
  vector<KeyPoint> key = eye.analyze_screen();
  cv::Mat pic = eye.img_1;
  vector<Point2f> points;
  for(auto i = key.begin(); i != key.end(); ++i){
    
  }

}
