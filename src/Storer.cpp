#include "Storer.hpp"

std::ostream& operator<<(std::ostream& output, const Storer& data){
  
  output<<"Files to store from:\n";
  for(const auto& filep : data.getFilePaths()) output<<filep<<"\n";
  return output;  
  
}

Storer::Storer(const boost::filesystem::path& filePath):filePaths({filePath}){

}

Storer::Storer(const std::vector<boost::filesystem::path>& filePaths):filePaths(filePaths){

}

bool Storer::itemMatches(TObject* obj, const char* className){

  return std::string(obj->ClassName()).find(std::string(className)) != std::string::npos;
  
}

void Storer::pushAsHist(Data& data, TObject* obj) const{

  if(itemMatches(obj, "TH1D")) data.pushHist(*dynamic_cast<TH1D*>(obj)); //if "GetClassName" contains TH1, the function finds shouldn't return "string::npos"
  else if(itemMatches(obj, "TH1F")){
    
    TH1D htemp;
    dynamic_cast<TH1F*>(obj)->Copy(htemp);//copy the TH1F into a TH1D
    data.pushHist(htemp);
    
  }

}

void Storer::pushFromCan(Data& data, TObject* readObject) const{//reads a key to retrieve a TH1 from a canvas and store it in h

  TCanvas* can = dynamic_cast<TCanvas*>(readObject);//if one of the 'items' is a canvas, we store the pointer to it in can
  TObject* obj; //then we can loop over the objects of this canvas
  
  TIter objit(can->GetListOfPrimitives()); //iterator over the contents of the canvas
  while((obj = objit())) pushAsHist(data, obj);
  
}

void Storer::pushPath(const boost::filesystem::path& filePath){
  
  filePaths.push_back(filePath);

}

void Storer::clear(){
  
  filePaths.clear();

}

void Storer::fill(Data& data) const{
  
  TKey* currentKey; //to point to the 'items' in a root file
  TObject* readObject;//to flush the key into a TObject
  TDirectory* CurrentDir = gDirectory->GetDirectory("");//shameful trick 
  
  for(const auto& p : filePaths){
  
    TFile file(p.string().c_str());
    CurrentDir->cd(); //shameful trick, second part
    
    TIter keyit(file.GetListOfKeys()); //iterator over the contents of filePaths
    while((currentKey = dynamic_cast<TKey*>(keyit()))){ //returns the pointer and increment it, when the pointer is not allocated it returns zero so the loop ends
      
      readObject = currentKey->ReadObj();//this is silly to cast a TObject to a TKey to eventually return a TObject with the ReadObj method, but heh, this is ROOT !
      if(itemMatches(readObject, "TH1")) pushAsHist(data, readObject);//reads a key to retrieve a TH1 and store it in h
      else if(itemMatches(readObject, "TCanvas")) pushFromCan(data, readObject);//reads a key to retrieve a TH1 from a canvas and store it in h
      
    }
    
  }
  
}

void Storer::setFilePaths(const std::vector<boost::filesystem::path>& filePaths){
  
  this->filePaths = filePaths;

}

const std::vector<boost::filesystem::path>& Storer::getFilePaths() const{

  return filePaths;
  
}