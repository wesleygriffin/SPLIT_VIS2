//http://www.sanfoundry.com/cpp-program-implement-single-linked-list/

#include "svList.h"

using namespace std;


namespace __svl_lib {

void svList::new_list(int seed)
{
//cerr<<"new_list"<<endl;
     start.resize(seed);
     for(int i=0;i<seed;i++)
     {
//cerr<<i<<endl;
      struct node s;
      start[i] = &s;
      start[i] = NULL;
     }
}
int svList::remove_list(int index)
{
    struct node *s, *ptr;
    int count = 0;
    s= start[index];
    while(s != NULL)
    {
          s = s->next;
          count++;
    }
    if(count == 0) return 0;

    s = start[index];
    if(count == 1)
    {
        start[index] = s->next;
    }
    else
    {
      if(count>0)
      {
        for(int i=1;i<count;i++)
        {
             ptr = s;
             s = s->next;
        }
        ptr->next = s->next;
      }
      free(s);
    }

    return count;
}
void svList::destroy()
{
    for(int i=0;i<start.size();i++)
    {
          while( remove_list(i)>=1)
          {

          }
          start[i] = NULL;
    }

    start.clear();   
}
node *  svList::create_node(int value1, int value2)
{

    struct node  *temp;

    temp = new (struct node);

           temp->data1 = value1;
           temp->data2 = value2;
           temp->next = NULL;
           return temp;
}
void svList::add_first(int index, int value1, int value2)
{
//cerr<<"add_first"<<endl;
     struct node *temp;
     temp = create_node(value1, value2);
//cerr<<"create node"<<endl;
     if(start[index] == NULL)
     {
        start[index] = temp;
        start[index]->next = NULL;
     }
      
}
void svList::add_next(int index, vector<int> value)
{
   add_first(index, value[0], value[1]);

   for(int i=1;i<value.size()/2;i++)
   {
//cerr<<index<<" "<<value[i]<<" "<<value[i+1]<<endl;
     struct node *temp, *s;
     temp = create_node(value[i*2],value[i*2+1]);

     s = start[index];

        while(s->next!=NULL)
        {
            s = s->next;
        }
        temp->next = NULL;
        s->next = temp;
   } 
}
/*
void svList::add_down(int value)
{
     struct node *temp, *s;
     temp = create_node(value[i]);

     s = start;

     while(s->next!=NULL)
     {
          s = s->down;
     }
     temp->down = NULL;
     s->down = temp;
}
*/
vector<int> svList::search_list(int index)
{
      vector<int> value;
      struct node *s;

      s = start[index];

      while(s!=NULL)
      {
         value.push_back(s->data1);
         value.push_back(s->data2);
         s = s -> next;
      }

      return value;
}
void svList::getData(int index, int *data1, int *data2)
{
      vector<int> data;
      int count = 0;
      struct node *s;
      s = start[index];
      while(s!=NULL)
     {
          data1[count] = s->data1;
          data2[count] = s->data2;
          count++;
          s = s->next;
     }
}
int svList::getSize(int index)
{
      struct node *s;
      s = start[index];
      int count = 0;
      while(s!=NULL)
      {
           count++;
           s = s->next;
      }
      return count;
}
void svList::display_list()
{
    struct node *temp;
    for(int i=0;i<start.size();i++)
    {
         if(start[i] == NULL)
         {
              cout<<"empty list"<<endl;
              return;
         }
         temp = start[i];
         while(temp!=NULL)
         {
             cout<<temp->data1<<" "<<temp->data2<<"->";
             temp = temp->next;
         }
         cout<<endl;
         cout<<"|"<<endl;
    }
}

}
