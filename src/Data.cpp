#include "Data.hpp"

std::ostream& operator<<(std::ostream& output, const Data& data){
  
  output<<"Histograms:\n";
  for(const TH1D& Hist : data.getHistograms()) output<<Hist<<"\n";
  return output;
  
}

Data operator+(Data d1, const Data& d2){
  
  d1 += d2;
  return d1;

}

Data operator*(Data data, double a){
  
  data *= a;
  return data;

}

Data operator*(double a, Data data){
  
  return data * a;

}

Data join(Data d1, const Data& d2){

  for(const auto& h : d2.getHistograms()) d1.pushHist(h);
  return d1;
  
}


void normalise(Data& data, double newArea){
  
  for(auto& h : data.getHistograms()) normalise(h, newArea);

}

Data::Data(const std::vector<TH1D>& histograms):histograms(histograms.begin(),histograms.end()){
  
}

Data& Data::operator+=(const Data& other){
  
  for(auto itPair = std::make_pair(histograms.begin(), other.histograms.begin()); itPair.first != histograms.end() && itPair.second != other.histograms.end(); ++itPair.first, ++itPair.second)
    *itPair.first += *itPair.second;
  
  return *this;

}

Data& Data::operator*=(double a){
  
  for(auto& h : histograms) h *= a;
  
  return *this;

}

void Data::pushHist(const Hist& hist){
  
  histograms.push_back(hist);

}

const std::vector<Hist>& Data::getHistograms() const{
  
  return histograms;
  
}

std::vector<Hist>& Data::getHistograms(){
  
  return histograms;
  
}

const std::vector<TH1D> Data::getTH1DCopies() const{
  
  return std::vector<TH1D>(histograms.begin(),histograms.end());
  
}

unsigned int Data::getNumberOfBins() const{
  
  return (*std::min_element(histograms.begin(), histograms.end(), [](const Hist& h1, const Hist& h2){return h1.getNumberOfBins() < h2.getNumberOfBins();})).getNumberOfBins();

}

unsigned int Data::getSize() const{

  return histograms.size();
  
}

void Data::renameHistograms(const std::vector<std::string>& names){

  for(auto itPair = std::make_pair(histograms.begin(), names.begin()); itPair.first != histograms.end() && itPair.second != names.end(); ++itPair.first, ++itPair.second)
    (*itPair.first).SetTitle((*itPair.second).c_str());
  
}

void Data::clear(){
  
  histograms.clear();

}
