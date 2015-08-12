/*
 * CS 106B, Marty Stepp
 * This file contains the declaration of the BinaryTreeNode structure.
 * A BinaryTreeNode stores one integer piece of data in a binary tree of integers.
 *
 * @version 2015/07/21
 */

#ifndef _binarytreenode_h
#define _binarytreenode_h

#include <cstdlib>

struct BinaryTreeNode {
public:
    static int s_allocated;
    static int s_freed;
    int data;
    BinaryTreeNode* left;
    BinaryTreeNode* right;

    /*
     * Constructs a new tree node with the given data and left/right links.
     */
    BinaryTreeNode(int data = 0, BinaryTreeNode* left = NULL, BinaryTreeNode* right = NULL);
    ~BinaryTreeNode();
    bool isLeaf();
};

#endif
