#ifndef DATA_H
#define DATA_H

#include <algorithm>
#include "Hist.hpp"

class Data{
  std::vector<Hist> histograms;

public:
  Data() = default;
  Data(const std::vector<TH1D>& histograms);
  Data& operator+=(const Data& other);//add the bin contents and the matrices coefficient wise
  Data& operator*=(double a);//multiplies the histograms and the matrices by 'a'
  void pushHist(const Hist& hist);//pushes hist into 'histograms'
  const std::vector<Hist>& getHistograms() const;
  std::vector<Hist>& getHistograms();
  const std::vector<TH1D> getTH1DCopies() const;
  unsigned getNumberOfBins() const;//returns the minimum of the Hist::getNumberOfBins
  unsigned getSize() const;//returns the largest size of the std::vectors in Data
  bool isEmpty() const;//checks the size of 'histograms'
  void renameHistograms(const std::vector<std::string>& names);
  void clear();//resizes all vectors to zero

};

std::ostream& operator<<(std::ostream& output, const Data& data);
Data operator+(Data d1, const Data& d2);
Data operator*(Data data, double a);
Data operator*(double a, Data data);
Data join(Data d1, const Data& d2);//utilitary function to join Data objects and return the joined data
void normalise(Data& data, double newArea = 1);//normalise 'data' to 'newArea'

#endif