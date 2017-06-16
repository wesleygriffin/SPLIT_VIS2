/*connected-component labeling */

#include <fstream>
#include <algorithm>
#include "svCCL.h"
#include "svUtil.h"

using namespace std;

namespace __svl_lib {

svConnect::svConnect()
{

}

svConnect::~svConnect()
{

}

vector<int> svConnect::TwoPass(char *inf, char *outf)
{
   vector<btree*> linked;
   svIntArray labels;
   svIntArray *neighbors;
   svIntArray count;

//input file
   ifstream infile(inf); 
   int n;
   infile>>n;//cerr<<n<<endl;
   neighbors = new svIntArray[n];
   for(int i=0;i<n;i++)
   {
       int m;
       infile>>m;//cerr<<m<<" ";
       for(int j=0;j<m;j++)
       {
            int nei;
            infile>>nei;//cerr<<nei<<" ";
            neighbors[i].add(nei);
       }//cerr<<endl;
       count.add(m);
       labels.add(-1);
   }
   infile.close();

cerr<<"first pass "<<n<<endl;
   int current = 0;

   for(int i=0;i<n;i++)
   {//cerr<<i<<" ";
      int percentage = i*100/n;
      ProgressBar(percentage);

       if(count[i] == 0 )
       {
           btree *l = new btree();
           l->insert(current);
           linked.push_back(l);
           labels[i] = current;//cerr<<i<<" "<<linked.size()<<endl;
           current ++;
       }
       else if(count[i]>0)
       {
           int m = 90000;
           for(int j=0;j<count[i];j++)
           {
               if(labels[neighbors[i][j]]<m)
               {
                    m = labels[neighbors[i][j]];
               }
           }
           labels[i] = m;//cerr<<"label"<<m<<endl;
/*           for(int j=0;j<count[i];j++)
           {
               linked[labels[neighbors[i][j]]].insert(m);
           }
*/
           for(int j=0;j<count[i];j++)
           {
               bnode *root1 = linked[m]->getRoot();
               bnode *root2 = linked[labels[neighbors[i][j]]]->getRoot();
    //   cerr<<m<<" "<<root1->key_value<<" "<<neighbors[i][j]<<" "<<root2->key_value<<endl; 
               btree *tmp = new btree();
          //     tmp->print(linked[m]->getRoot());
          //     tmp->print(linked[labels[neighbors[i][j]]]->getRoot());
               tmp->merge(root1, root2);
           } 
       }
//cerr<<"size"<<linked.size()<<endl;
//   for(int ii=0;ii<linked.size();ii++)
//         linked[ii]->print(linked[ii]->getRoot());
//cerr<<"==="<<endl;
   }
//cerr<<"size"<<linked.size()<<endl;
//   for(int i=0;i<linked.size();i++)
//         linked[i]->print(linked[i]->getRoot());

cerr<<"second pass"<<endl;
   vector<int> unique_label;
   for(int i=0;i<n;i++)
   {
       int l = labels[i];//cerr<<l<<endl;
       if(l>=0)
          labels[i] = linked[l]->getMinKey();

       bool flag = true;
       for(int j=0;j<unique_label.size();j++)
       {
          if(labels[i] == unique_label[j])
          {flag = false;break;}
       }
       if(flag)
       {
           unique_label.push_back(labels[i]);
       }
    //   cerr<<labels[i]<<endl;
   }

   sort(unique_label.begin(), unique_label.end());   
   for(int i=0;i<n;i++)
   {
      int l;
       for(int j=0;j<unique_label.size();j++)
       {
          if(labels[i] == unique_label[j])
          {
             l = j;
              break;
          }
       }
       if(labels[i]>=0)
           labels[i] = l;
   }
  
   for(int i=0;i<unique_label.size();i++)
   {
      if(unique_label[i] >=0)  
             unique_label[i] = i; 
   }

//output
   ofstream outfile(outf);
   outfile<<n<<endl;
   for(int i=0;i<n;i++)
      outfile<<labels[i]<<endl;
   outfile.close();


//release memory
   labels.free();
   linked.clear();
   for(int i=0;i<n;i++)
        neighbors[i].free();
   delete [] neighbors;

   return unique_label;
}



}

