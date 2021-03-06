# include "Rebinner.hpp"

using namespace std;

ostream& operator<<(ostream& output, const Rebinner& rebinner){

  output<<"Bin edges: \n";
  for(const double& binEdge : rebinner.getRebin()) output<<binEdge<<"\n";
  return output;
  
}

Rebinner::Rebinner(const Data& data){
  
  buildFrom(data);

}

Rebinner::Rebinner(const vector<double>& edge):edge(edge){
  
}

void Rebinner::Normalise(Hist& h, const int ref){

  if(ref>0 && ref<h.GetNbinsX())
    for(int k = 0; k<h.GetNbinsX(); ++k) h.SetBinContent(k+1, h.GetBinContent(k+1)*h.GetBinWidth(ref)/h.GetBinWidth(k+1));
  
}

vector<double> Rebinner::getCommonElements(const vector<double>& v1, const vector<double>& v2, double epsilon){
  
  vector<double> common;
  set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(common), [&](double a, double b){if(abs(a-b)<epsilon) return false; else return a < b;});//if the numbers are too close to each other, decrete that none is strictly greater than the other

  return common;
  
}

vector<unsigned> Rebinner::getCommonIndices(const vector<double>& v1, const vector<double>& v2, double epsilon) {

  vector<unsigned> commonIndices;
  
  for(unsigned k = 0; k<v1.size(); ++k)
    for(unsigned p = 0; p<v2.size(); ++p) if(abs(v1[k] - v2[p])<epsilon) commonIndices.push_back(k);
  
  return commonIndices;
  
}

void Rebinner::buildFrom(const Data& data){
  
  if(!data.isEmpty()){
    
    edge = data.getHistograms().front().getEdge();//overwrite the old edge
    for(const Hist& h : data.getHistograms()) edge = getCommonElements(edge, h.getEdge());
    
  }
  
}

void Rebinner::excludeBinsAbove(const double& newUpEdge){

  auto itGarbage = remove_if(edge.begin(), edge.end(), [&](const double& currentEdge){return currentEdge > newUpEdge;});
  edge.erase(itGarbage, edge.end());

}

void Rebinner::excludeBinsBelow(const double& newLowEdge){

  auto itGarbage = remove_if(edge.begin(), edge.end(), [&](const double& currentEdge){return currentEdge < newLowEdge;});//remove_if stores the garbage at the end of the vector and returns an iterator to the start of the garbage
  edge.erase(itGarbage, edge.end());//the garbage must then be removed with 'erase'
  
}

void Rebinner::squeezeBinning(unsigned factor){

  int counter = -1;
  auto itGarbage = remove_if(edge.begin(), edge.end(), [&](double){++counter; return counter % factor != 0;});//remove all the indices that are not multiples from factor
  edge.erase(itGarbage, edge.end());
  
}

void Rebinner::rebin(Data& data) const{
  
  for(auto& h : data.getHistograms()) h = *dynamic_cast<TH1D*>(h.Rebin(edge.size()-1, h.GetName(), edge.data()));//rebin the Histogram
  
}

bool Rebinner::admissibleRebinFor(Data& data) const{

  vector<double> intersection(edge.size());
  
  for(const Hist& h : data.getHistograms())
    if(intersection.begin() == set_intersection(edge.begin(), edge.end(), h.getEdge().begin(), h.getEdge().end(), intersection.begin())) return false;//if there is no intersection with the proposed bining, return false

  return true;
  
}

bool Rebinner::hasValidRebin() const{
  
  return !edge.empty();

}

const vector<double>& Rebinner::getRebin() const{
  
  return edge;
  
}

unsigned int Rebinner::getNumberOfBins() const{
  
  if(!edge.empty()) return edge.size() -1;
  else return 0;
  
}