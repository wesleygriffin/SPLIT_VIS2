/*list */

#ifndef __SV_LIST_H
#define __SV_LIST_H

#include <vector>
#include "svArray.h"
#include "svVectorField.h"
#include "svParticle.h"
#include "svPrimitive.h"
#include "svLut.h"


namespace __svl_lib {

struct node
{
   int data1;
   int data2;
   node *next;
};

class svList {
  public:
    svList(){};
	
    virtual ~svList(){};

    void new_list(int seed);
    node * create_node(int value1, int value2);
    void add_first(int index, int value1, int value2);
    void add_next(int index, vector<int> value);
    int remove_list(int index);
    int getSize(int index);
    void getData(int index, int *data1, int *data2);
    void destroy();
    vector<int>  search_list(int index);
    void display_list();

  private:
     vector< struct node *> start;
};

}

#endif // __SV_LIST_H
