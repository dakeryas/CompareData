#ifndef COSMETICS_H
#define COSMETICS_H

#include <string>
#include "boost/algorithm/string.hpp"
#include "TFile.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLegend.h"
#include "Hist.hpp"

class Cosmetics {

  std::vector<Hist> h;
  std::vector<int> colours;//admissible colours
  std::vector<double> legend_pos;
  void setLegend(TCanvas& can);//set a blank legend at legend_pos in can
  void adaptColour(unsigned k);//remove the colour of the histogram k in the allowed colours
  void setColours();//keep the colours of the hist if they don't appear twice, otherwise pick them from "colours"
  void makeUpHists(std::string ytitle);

public:
  Cosmetics(const std::vector<Hist>& h, const std::vector<int>& colours, const std::vector<double>& legend_pos, std::string ytitle);
  unsigned getHighestHistogram();//returns the index of the tallest histogram of vecth
  void fileWriter(const std::string& rootfile);

};

#endif