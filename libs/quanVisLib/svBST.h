/*binary tree */

#ifndef __SV_BST_H
#define __SV_BST_H

#include "svList.h"

namespace __svl_lib {

struct bnode
{
    int key_value;
    bnode *parent;
    bnode *left;
    bnode *right;
};

    bnode* newNode(int key);


class btree
{
public:
    btree();
    ~btree();

    int getSize(){return treesize;}
    bnode *getRoot(){return root;}
    int getMinKey();
 
    void print(bnode *leaf);

    void buildfromTree(btree tree);
    void merge(bnode *leaf1, bnode *leaf2);

    void insert(int key);
    bnode *search(int key);
    void destroy_tree();
    bnode* merge(bnode *r, int size);
private:
    bnode* build_tree(int inorder[], int start, int end);
    void destroy_tree(bnode *leaf);
    void insert(int key, bnode *leaf);
    void insert(bnode *node,  bnode *leaf);
    void insertTree(bnode *root1, bnode *root2);
    bnode *search(int key, bnode *leaf);
    void storeInorder(bnode *leaf, tnode<bnode*>* inorder, int *index_ptr);    
    void storeInorder(bnode *leaf, int inorder[], int *index_ptr);
    void printInorder(bnode *leaf);

    bnode *root;
    int treesize;
};


}

#endif // __SV_BST_H
