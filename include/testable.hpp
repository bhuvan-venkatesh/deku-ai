#pragma once
#define DEBUG true

class Testable {
public:
  virtual void check_rep() const = 0;
  virtual ~Testable() {}
};
