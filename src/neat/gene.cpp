#include "gene.hpp"

Gene::Gene() : into(0), out(0), weight(0), innovation(0), enabled(true) {
  // Left blank intentionally
}
Gene::Gene(const int32_t &into_, const int32_t &out_)
    : into(into_), out(out_), weight(0), innovation(0), enabled(true) {}

void Gene::copy(const Gene &swap) {
  into = swap.into;
  out = swap.out;
  weight = swap.weight;
  innovation = swap.innovation;
  enabled = swap.innovation;
}

void Gene::swap(Gene &other) {
  using std::swap;
  swap(into, other.into);
  swap(out, other.out);
  swap(weight, other.weight);
  swap(innovation, other.innovation);
  swap(enabled, other.enabled);
}

Gene::Gene(const Gene &swap) { copy(swap); }

Gene::Gene(Gene &&swap_) { swap(swap_); }

Gene &Gene::operator=(Gene swap_) {
  swap(swap_);
  return *this;
}

bool Gene::operator<(const Gene &rhs) const { return out < rhs.out; }
bool Gene::operator>(const Gene &rhs) const { return out > rhs.out; }

bool Gene::operator==(const Gene &rhs) const {
  return innovation == rhs.innovation;
}

bool Gene::same_link(const Gene &other) const {
  return into == other.into && out == other.out;
}

bool Gene::save(ofstream &ofs) const {
  /*Flush once at the end of the
  stream to make use of buffering
  efficency while preserving parse
  efficency and readibility*/

  ofs << into << "\n";
  ofs << out << "\n";
  ofs << weight << "\n";
  ofs << innovation << "\n";
  ofs << enabled << "\n";
  return true;
}

bool Gene::load(ifstream &ifs) {
  ifs >> into >> out >> weight >> innovation >> enabled;
  return true;
}
