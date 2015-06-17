#include "boost/program_options.hpp"
#include "PathGrabber.hpp"
#include "Storer.hpp"
#include "Rebinner.hpp"
#include "Cosmetics.hpp"

namespace bpo = boost::program_options;

void CompareData(const boost::filesystem::path& directory, const std::string& outname, bool normaliseData, bool rename, unsigned verbose){
  
  Data data;
  
  PathGrabber pathGrabber;
  pathGrabber.pushPathsFrom(directory);
  Storer storer(pathGrabber.getFilePaths());
  storer.fill(data);
  if(verbose > 0) std::cout<<storer<<std::endl;
  if(verbose > 2) std::cout<<data<<std::endl;

  Rebinner rebinner(data);
  if(verbose > 1) std::cout<<rebinner<<std::endl;
  rebinner.rebin(data);
  if(normaliseData) normalise(data);

  if(rename) data.renameHistograms(pathGrabber.getFileNames());
  std::vector<int> colours {2, 600+2, 1, 416+2, 880-3, 800+7, 6, 632+1, 800, 416+4, 820+10, 600-7, 432, 840+2, 632-9, 800+4, 920+2, 400+1, 900-2, 400-4, 600-1, 616-5, 416-4, 616+4};
  std::vector<double> legend_position {0.70, 0.68, 0.95, 0.95};
  Cosmetics Beautiful(data.getHistograms(), colours, legend_position, "Events [per binwidth keV]");
  Beautiful.fileWriter(outname); //write the histograms into a canvas into the same file
  if(verbose > 0) std::cout<<"Canvas successfully saved in: \""<<outname<<"\"\n";
  
}

int main (int argc, char* argv[]){

  boost::filesystem::path target;
  std::string output;
  unsigned verbose;
  
  bpo::options_description optionDescription("Compare Data usage");
  optionDescription.add_options()
  ("help,h", "Display this help message")
  ("target,t", bpo::value<boost::filesystem::path>(&target)->required(), "Path of the files to compare")
  ("output,o", bpo::value<std::string>(&output)->required(), "Output file where to save the canvas")
  ("normalise,n", "Normalise data")
  ("rename,r", "Rename objects according to filenames")
  ("verbose,v", bpo::value<unsigned>(&verbose)->default_value(1),"Display filenames at 1, add rebin at 2 and objects at 3");

  bpo::positional_options_description positionalOptions;//to use arguments without "--"
  positionalOptions.add("target", 1);
  
  bpo::variables_map arguments;
  try{
    
    bpo::store(bpo::command_line_parser(argc, argv).options(optionDescription).positional(positionalOptions).run(), arguments);
    
    if(arguments.count("help")){
      
      std::cout<<optionDescription<<std::endl;
      return 0;
      
    }
      
    bpo::notify(arguments);//the arguments are ready to be used
    
  }
  catch(bpo::error& e){
    
    std::cout<<e.what()<<std::endl;
    return 1;
    
  }
  
  if(boost::filesystem::is_directory(target)) CompareData(target, output, arguments.count("normalise"), arguments.count("rename"), verbose);
  else std::cout<<target<<" is not a directory (see help)"<<std::endl;
  
  return 0;
  
}
