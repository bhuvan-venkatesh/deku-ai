#include "brain.hpp"
#define inputs 256
#define side 16
#define outputs 8

Brain::Brain()
    : pool(Pool(inputs, outputs)), classifier(Image_Classifier(side)) {}

void Brain::initialize_run() {
  controller.load_state();
  rightmost = 0;
  pool.reset_frame();
  timeout = timeout_constant;
  controller.execute();

  pool.generate_top_network();
  // play();
}

// This is the heart of the program!
void Brain::play() {
  // TODO change to a lower priority thread
  cv::Mat pic = eye.analyze_screen();
  vector<int32_t> block = classifier.block_classify(pic);
  pic.release();
  cout << classifier.prev_x << "\t" << classifier.prev_y << endl;
  /*
    if (classifier.prev_x > rightmost) {
      rightmost = classifier.prev_x;
      timeout = timeout_constant;
    }
    timeout--;
    int timeout_bonus = pool.get_frame() / 4;
    if (timeout + timeout_bonus <= 0) {
      int32_t fitness = rightmost - pool.get_frame() / 2;
      if (rightmost > 600) {
        fitness += 1000;
      }
      if (fitness == 0) {
        fitness = -1;
      }
      pool.update_fitness(fitness);
      initialize_run();
    }
    pool.advance_frame();
    vector<bool> should_press = pool.evaluate(block);
    send_signals(should_press);*/
}

// TODO: Change this more programatically
void Brain::send_signals(const vector<bool> &buttons) {
  for (size_t i = 0; i < buttons.size(); ++i) {
    if (!buttons[i]) {
      continue;
    }
    switch (i) {
    case 0:
      controller.a();
      std::cout << "Pressed\n";
      break;
    case 1:
      controller.b();
      std::cout << "Pressed\n";
      break;
    case 2:
      controller.x();
      std::cout << "Pressed\n";
      break;
    case 3:
      controller.y();
      std::cout << "Pressed\n";
      break;
    case 4:
      controller.up();
      std::cout << "Pressed\n";
      break;
    case 5:
      controller.down();
      std::cout << "Pressed\n";
      break;
    case 6:
      controller.left();
      std::cout << "Pressed\n";
      break;
    case 7:
      controller.right();
      std::cout << "Pressed\n";
      break;
    }
  }
  controller.execute();
}
