#include "PathGrabber.hpp"

std::ostream& operator<<(std::ostream& output, const PathGrabber& pathGrabber){
  
  output<<"Grabbed the files:\n";
  for(const boost::filesystem::path& filep : pathGrabber.getFilePaths()) output<<filep<<"\n";
  return output;  
  
}

bool PathGrabber::pathMatches(const boost::filesystem::directory_iterator& it, const std::string& fileSorter){
  
  return it->path().filename().string().find(fileSorter) != std::string::npos;
  
}

void PathGrabber::pushIfRoot(const boost::filesystem::directory_iterator& it){//fills 'found_paths' with the path pointed to by 'it' if 'it' refers to a file
  
  if(boost::filesystem::is_directory(it->status()) == 0 && pathMatches(it, ".root")) filePaths.push_back(it->path()); ; //find returns the position of 'name' in the path, so if it returned a position indeed(namely not npos), we found something
  
}

void PathGrabber::pushIfRootAnd(const boost::filesystem::directory_iterator& it, const std::string& fileSorter){//fills 'found_paths' with the path pointed to by 'it' if 'it' refers to a file
  
  if(boost::filesystem::is_directory(it->status()) == 0 && pathMatches(it, ".root") && pathMatches(it, fileSorter)) filePaths.push_back(it->path()); ; //find returns the position of 'name' in the path, so if it returned a position indeed(namely not npos), we found something
  
}

void PathGrabber::pushPathsFrom(const boost::filesystem::path& searchPath){ //retrieves the path of all files in a directory

  boost::filesystem::directory_iterator end; //the default constructor creates an end iterator which cannot be reached unless nothing was found
  for(boost::filesystem::directory_iterator it(searchPath); it!= end; ++it) pushIfRoot(it);
  
}

void PathGrabber::pushPathsFrom(const boost::filesystem::path& searchPath, const std::string& fileSorter){ //retrieves the path of all the files in a directory that match the name fileSorter

  boost::filesystem::directory_iterator end; //the default constructor creates an end iterator which cannot be reached unless nothing was found
  for(boost::filesystem::directory_iterator it(searchPath); it!= end; ++it) pushIfRootAnd(it, fileSorter);
  
}

void PathGrabber::sortPaths(){

  std::sort(filePaths.begin(), filePaths.end());

}

void PathGrabber::clear(){
  
  filePaths.clear();

}

const std::vector<boost::filesystem::path>& PathGrabber::getFilePaths() const{
  
  return filePaths;

}

std::vector<std::string> PathGrabber::getFileNames() const{

  std::vector<std::string> fileNames;
  for(const auto& p : filePaths) fileNames.emplace_back(p.stem().string());
  return fileNames;
  
}