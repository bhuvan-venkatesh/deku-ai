#include <iostream>
#include <ctime>
#include "brain.hpp"

using cv::KeyPoint;

int main() {
  Brain brain;
  brain.initialize_run();

  const int SKIP_TICKS = 1000 / FRAMES_PER_SECOND;

  clock_t next_game_tick = clock();

  int sleep_time = 0;

  while (true) {
    /*
    brain.controller.right();
    brain.controller.execute();
    */
    brain.play();

    next_game_tick += SKIP_TICKS;
    sleep_time = next_game_tick - clock();
    if (sleep_time >= 0) {
      sleep(sleep_time);
    }
  }
  return 0;
}
