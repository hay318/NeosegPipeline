#ifndef DEF_Atlas
#define DEF_Atlas


#include <string>

struct Atlas { 

  std::string name;
  std::string image;
  
  bool probabilistic;

  std::string seg;

  std::string white;
  std::string gray; 
  std::string csf;

}; 

#endif
