/*binary tree */

#include "svBST.h"

namespace __svl_lib {

btree::btree()
{
   root = NULL;
   treesize = 0;
}

btree::~btree()
{
    destroy_tree();
    treesize = 0;
}

bnode * newNode(int key)
{
    bnode *leaf = new bnode;
    leaf->key_value = key;
    leaf->left = NULL;
    leaf->right = NULL;
    leaf->parent = NULL;
    return leaf;
}

int btree::getMinKey()
{
   bnode *leaf = root;
   while(leaf->parent!=NULL)
     {
         leaf = leaf->parent;
     }

   root = leaf;
   int min  = leaf->key_value;
   while(leaf!=NULL)
   {
         min = leaf->key_value;
         leaf = leaf->left;
   }
   return min;
}

void btree::insert(int key)
{
      if(root!=NULL)
         insert(key, root);
      else
      {
           root = new bnode;
           root->key_value = key;
           root->left = NULL;
           root->right = NULL;
           root->parent = NULL;
      }
}

bnode *btree::search(int key)
{
    return search(key, root);
}

void btree::destroy_tree()
{
     destroy_tree(root);
}

void btree::print(bnode *leaf)
{
   bnode *r = new bnode;
   
   r = leaf;
   while(r->parent!=NULL)
   {
       r = r->parent;
   }
//cerr<<r->key_value<<" ";
   printInorder(r);cerr<<endl;
}

void btree::merge(bnode *leaf1, bnode *leaf2)
{
     bnode *test1;
     test1 = leaf1;
     int count = 0;
     while(test1->parent!=NULL)
     {
         test1 = test1->parent;
     }
     bnode *test2;
     test2 = leaf2;
     while(test2->parent!=NULL)
     {
         test2 = test2->parent;
     }
     if(test1 == test2) return;

     insertTree(test1, test2);
/*
//cerr<<"merge"<<endl;
     tnode<bnode*> *inorder = new tnode<bnode*>;
     inorder->next = NULL;
     tnode<bnode*> *start = new tnode<bnode*>;
     start = inorder;
     
     int i = 0;
     storeInorder(test2, inorder, &i);
//cerr<<"merge"<<endl;
     while(start!=NULL)
     {cerr<<start->data1->key_value<<" ";
           insert(test1, start->data1);
           start = start->next; 
     }//cerr<<"merge"<<endl;
*/
}

void btree::insert(bnode *node,  bnode *leaf)
{
    if(node->key_value < leaf->key_value)
    {
          if(leaf->left!=NULL)
          {
             insert(node, leaf->left); 
          } 
          else
          {
              node->parent = leaf;
              node->left = NULL;
              node->right = NULL;
              leaf->left = node;
          }
    }
    else 
    {
        if(leaf->right != NULL)
        {
            insert(node, leaf->right);
        }
        else
        {
             node->parent =leaf;
             node->left = NULL;
             node->right = NULL;
             leaf->right = node;
        }
    }
}
bnode* btree::merge(bnode *r, int size)
{
    if(root == r)
    {
       return NULL;
    }
    else
    {
       int *arr1 = new int[size];
       int i =0;
       storeInorder(r, arr1, &i);
       
/*       int *arr2 = new int [treesize];
       int j=0;
       storeInorder(root, arr2, &j);

       int *arr = new int[size + treesize];
       int count1 = 0;
       int count2 = 0;
       for(int i=0;i<size+treesize;i++)
       {
          if(arr1[count1] < arr2[count2])
          {
               arr[i] = arr1[count1];
               count1++;
          }
          else if(arr1[count1] > arr2[count2])
          {
               arr[i] = arr2[count2];
               count2++;
          }
          else
          {
               arr[i] = arr1[count1];
               count1++;
               count2++;
          }
       }

       bnode *resultr = build_tree(arr, 0,size+treesize-1);
       destroy_tree();
       
       root = resultr;
*/
       for(int ii=0;ii<size;ii++)
             insert(arr1[ii]);
       return root;
    }
}

void btree::buildfromTree(btree tree)
{
   root = tree.getRoot();
   treesize = tree.getSize();
}

bnode* btree::build_tree(int inorder[], int start, int end)
{
    if(start > end) return NULL;

    int mid = (start + end)/2;
    bnode *r  = new bnode;

    r->key_value = inorder[mid];
    r->left = NULL;
    r->right = NULL;
    treesize++;  
 
    r->left = build_tree(inorder, start, mid-1);
    r->right = build_tree(inorder, mid+1, end); 

    return r;
}

void btree::destroy_tree(bnode *leaf)
{
    if(leaf!=NULL)
    {
        destroy_tree(leaf->left);
        destroy_tree(leaf->right);
        delete leaf;
        treesize--;
    }
}

//there is no replicated keys
void btree::insert(int key, bnode *leaf)
{
    if(key < leaf->key_value)
    {
          if(leaf->left!=NULL)
          {
              insert(key, leaf->left);
          }
          else
          {
              leaf->left = new bnode;
              leaf->left->key_value = key;
              leaf->left->left = NULL;
              leaf->left->right = NULL;
              leaf->left->parent = leaf;
              treesize++;
          }
    }
    else if(key > leaf->key_value)
    {
        if(leaf->right != NULL)
        {
            insert(key, leaf->right);
        }
        else
        {
             leaf->right = new bnode;
             leaf->right->key_value = key;
             leaf->right->left = NULL;
             leaf->right->right = NULL;
             leaf->right->parent = leaf;
             treesize++;
        }
    }
}

bnode *btree::search(int key, bnode *leaf)
{
       if(leaf != NULL)
       {
           if(key == leaf->key_value)
                 return leaf;
           if(key < leaf->key_value)
                 return search(key, leaf->left);
           else 
                 return search(key, leaf->right);
       }
       else 
             return NULL;
}

void btree::storeInorder(bnode *leaf, int inorder[], int *index_ptr)
{
    if(leaf == NULL) return ;

    storeInorder(leaf->left, inorder, index_ptr);

    inorder[*index_ptr] = leaf->key_value;
    (*index_ptr)++;

    storeInorder(leaf->right, inorder, index_ptr);
}
void btree::printInorder(bnode *leaf)
{
   if(leaf == NULL) return ;

    printInorder(leaf->left);

   cerr<<leaf->key_value<<" ";

    printInorder(leaf->right);

}
//post order
void btree::insertTree(bnode *root1, bnode *root2)
{//todo
   if(root1 == NULL) return;

   insertTree(root1->left, root2);

   insertTree(root1->right, root2);

   insert(root1, root2);
}
void btree::storeInorder(bnode *leaf, tnode<bnode*> *inorder, int *index_ptr)
{//wrong!!!!!!!!!!
    if(leaf == NULL) return ;

    struct tnode<bnode*> *tmp = new (struct tnode<bnode*>);
    inorder->next = tmp;
    storeInorder(leaf->left, tmp, index_ptr);
       
    inorder->data1 = leaf;
    cerr<<*index_ptr<<" "<<inorder->data1->key_value<<endl;
    (*index_ptr)++;

    storeInorder(leaf->right, tmp, index_ptr); 
}

}

