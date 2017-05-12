
/* rendering routine of a vector field using glyphs */



#include "svReadConfig.h"

namespace __svl_lib {

svReadConfig::svReadConfig(char *file)
{
   std::istringstream is_file(config);

   std::string line;
   while( std::getline(is_file, line) )
  {
        std::istringstream is_line(line);
        std::string key;
        if( std::getline(is_line, key, '=') )
        {
              std::string value;
              if( std::getline(is_line, value) ) 
                   store_line(key, value);
        }
  }
}

class svReadConfig {
  public:
    svReadConfig(char *file);
   
    char *LookupString(char *name);
    bool LookupBool(char *name);
    int LookupBool(char *name);
    double LookupDouble(char *name);
    svVector3 LookupsvVector3(char *name);
    int *LookupIntArray(char *name);
    double *LookupDoubleArray(char *name);
};

}


