#pragma once

#include <cstdint>
#include <unordered_map>

#include "pool.hpp"
#include "snes_controller.hpp"
#include "image_classifier.hpp"

#define FRAMES_PER_SECOND 60
#define timeout_constant FRAMES_PER_SECOND * 4

struct Brain {
public:
  Brain();
  void initialize_run();
  void play();
  Snes_Controller controller;

private:
  void send_signals(const vector<bool> &buttons);
  Pool pool;
  Eye eye;
  Image_Classifier classifier;
  int rightmost;
  int timeout;
};
