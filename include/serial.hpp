#pragma once

#include <fstream>
#include <iostream>

using std::ofstream;
using std::ifstream;
using std::endl;

class Serial{
public:
   virtual bool save(ofstream& ofs) const=0;
   virtual bool load(ifstream& ifs)=0;
   virtual ~Serial() {}
};
