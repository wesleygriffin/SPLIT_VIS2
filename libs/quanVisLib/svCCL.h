/*connected-component labeling */

#ifndef __SV_CCL_H
#define __SV_CCL_H

#include "svBST.h"

namespace __svl_lib {

class svConnect 
{
public:
    svConnect();
    ~svConnect();

    vector<int> TwoPass(char *inf, char *outf);    

//private:
        
};


}

#endif // __SV_CCL_H
