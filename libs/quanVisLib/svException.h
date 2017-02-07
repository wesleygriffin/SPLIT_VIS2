/* This file defines exceptions for flow volume computation
 */

#ifndef __SV_EXCEPTION_H
#define __SV_EXCEPTION_H

#include <iostream>
#define SV_EXP_INVALID_SEED        -5000
#define SV_EXP_STATIC_REGION       -5001
#define SV_EXP_TOO_LARGE_STEP      -5002
#define SV_EXP_INIT_FIELD          -5003
#define SV_EXP_OUT_OF_RANGE_TIME   -5004
#define SV_EXP_OUT_OF_RANGE_SPACE  -5005
#define SV_EXP_INVALID_OPERATION   -9000
#define SV_EXP_GENERIC_ERROR       -9001

using namespace std;
namespace  __svl_lib {
class svException
{
  public:
     char *msg;
     svException(char *m) {msg=m;}
     void Report(){cerr << "EXCEPTION: " << msg << endl;}
};
}

#endif // __SV_EXCEPTION_H
