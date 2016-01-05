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
  previous((time_t)0),
  classifier(Image_Classifier(inputs))
{
}

//This is the heart of the program!
void Brain::play(){
  //TODO change to a lower priority thread
  if(time(NULL)-previous >= 3600*5){ //Save every 5 minutes
    ofstream stream("NEAT.dat");
    pool.save(stream);
  }
  cv::Mat pic;
  vector<KeyPoint> keys = eye.analyze_screen(pic);
  vector<int32_t> block = classifier.block_classify(pic, keys);
  vector<int32_t> inputs_ = demonize_blocks(block);
  vector<bool> should_press = pool.evaluate(inputs_);
  send_signals(should_press);
}

vector<int32_t> Brain::demonize_blocks(const vector<int32_t>& block_classes){
    vector<int32_t> ret;
    for(auto i = block_classes.begin(); i != block_classes.end(); ++i){
      if(memory.find(*i) == memory.end()){
        memory[*i] = rand()%2 ? 1 : -1;
      }
      ret.push_back(memory[*i] * (*i));
    }
    return ret;
}

//TODO: Change this more programatically
void Brain::send_signals(const vector<bool>& buttons){
  for(size_t i = 0; i < buttons.size(); ++i){
    if(!buttons[i]){
      continue;
    }
    switch(i){
      case 0:
        controller.a();
        break;
      case 1:
        controller.b();
        break;
      case 2:
        controller.x();
        break;
      case 3:
        controller.y();
        break;
      case 4:
        controller.up();
        break;
      case 5:
        controller.down();
        break;
      case 6:
        controller.left();
        break;
      case 7:
        controller.right();
        break;
    }
  }
  controller.execute();
}
