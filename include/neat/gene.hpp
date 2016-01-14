#pragma once

#include <cstdint>
#include "serial.hpp"

using std::int32_t;

struct Gene : public Serial {
public:
  Gene();
  Gene(const int32_t &into_, const int32_t &out_);
  Gene(const Gene &swap);
  Gene(Gene &&rhs);
  Gene &operator=(Gene rhs);

  bool operator<(const Gene &rhs) const;

  bool operator==(const Gene &rhs) const;

  bool same_link(const Gene &other) const;

  bool save(ofstream &ofs) const;
  bool load(ifstream &ifs);
  virtual ~Gene() {}

  void copy(const Gene &swap);
  void swap(Gene &other);

  /* With alignment -
   20 bytes*/

  int32_t into;
  int32_t out;
  int32_t weight;
  int32_t innovation;
  bool enabled;

private:
};
