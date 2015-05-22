# include "include/Cosmetics.hpp"

Cosmetics::Cosmetics(const std::vector<Hist>& h, const std::vector<int>& colours, const std::vector<double>& legend_pos, std::string ytitle):h(h), colours(colours), legend_pos(legend_pos){

  makeUpHists(ytitle);
  
}

void Cosmetics::adaptColour(unsigned k){
  
  for(unsigned p = 0; p<colours.size(); ++p) if(h[k].GetLineColor() == colours[p]) colours.erase(colours.begin()+p);

}

void Cosmetics::setColours(){
    
  if(h.size()>1) //if there is only one element, you don't need to change its colour!
    for(unsigned k = 0; k<h.size()-1; ++k){
      
      for(unsigned p = k+1; p<h.size(); ++p) 

	if(h[k].GetLineColor() == h[p].GetLineColor()){
	
	if(colours.size() != 0){//if there is at least the first color to pick, color[0]
	  
	  h[k].SetLineColor(colours.front());
	  colours.erase(colours.begin());
	  
	}
	else h[k].SetLineColor(1);//otherwise 'Paint It Black'
	
      }
      
    }
    
  
}

void Cosmetics::makeUpHists(std::string ytitle){
  
  for(unsigned k = 0; k<h.size(); ++k){
    
    h[k].SetStats(0);
    h[k].SetLineWidth(2);
    adaptColour(k);
    
    auto binwidth = std::to_string(1.e3*h[k].GetBinWidth(1));
    binwidth.erase(binwidth.begin() + binwidth.find_first_of("."), binwidth.end());
    boost::replace_all(ytitle, "binwidth", binwidth);
    
    h[k].SetYTitle(ytitle.c_str());
    h[k].GetYaxis()->SetTitleOffset(1.3);    
    
  }
  
  setColours();
  
}

void Cosmetics::setLegend(TCanvas& can){
  
  if(legend_pos.size()>3){
    
    TLegend* legend = can.BuildLegend(legend_pos[0], legend_pos[1], legend_pos[2], legend_pos[3]);
    legend->SetFillColor(0);
  
  }
  
  can.SetObjectStat(0);
  
}


unsigned Cosmetics::getHighestHistogram(){//to know which Histogram in the vector is the tallest
  
  unsigned tallest = 0;
  for(unsigned k = 0; k<h.size(); ++k) if(h[k].GetMaximum()>h[tallest].GetMaximum()) tallest = k;
  return tallest;
  
}

void Cosmetics::fileWriter(const std::string& outname){
  
  if(h.size()!=0){
    
    unsigned tallest = getHighestHistogram();//to know which Histogram in the vector is the tallest    
    TCanvas can("can");
    can.cd();
    h[tallest].Draw(); //draw the tallest Histogram first
    for(unsigned k = 0; k<h.size(); ++k) if(k != tallest) h[k].Draw("Same");
    
    setLegend(can);
    
    TFile fileout(outname.c_str(), "recreate");
    can.Write();
  
  }
  
}
  
  