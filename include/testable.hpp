#pragma once
#define DEBUG TRUE

class Testable {
public:
  virtual void check_rep() const = 0;
  virtual ~Testable() {}
};
