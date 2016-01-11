#include "brain.hpp"
#define inputs 256
#define outputs 8

Brain::Brain():
  pool(Pool(inputs, outputs)),
  classifier(Image_Classifier(inputs))
{
}

void Brain::initialize_run(){
  	controller.load_state();
  	rightmost = 0;
  	pool.reset_frame();
  	timeout = timeout_constant;
    controller.execute();

  	auto& genome = pool.top_genome();
    genome.generate_network();
  	play();
}

//This is the heart of the program!
void Brain::play(){
  //TODO change to a lower priority thread
  /*
    ofstream stream("NEAT.dat");
    pool.save(stream);*/
  cv::Mat pic = eye.analyze_screen();
  vector<int32_t> block = classifier.block_classify(pic);
  rightmost = classifier.rightmost;
  vector<bool> should_press = pool.evaluate(block);
  send_signals(should_press);
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
