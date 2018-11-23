/*
 * CS 106B, Marty Stepp
 * This file contains the declaration of the TreeMapNode structure.
 * A TreeMapNode stores one string/integer key/value pair in a tree map.
 *
 * @version 2015/07/21
 */

#ifndef _binarytreemapnode_h
#define _binarytreemapnode_h

#include <string>
using namespace std;

struct BinaryTreeMapNode {
    string key;
    int value;
    BinaryTreeMapNode* left;
    BinaryTreeMapNode* right;

    /*
     * Constructs a new tree node with the given key/value and left/right links.
     */
    BinaryTreeMapNode(string key, int value, BinaryTreeMapNode* left = nullptr, BinaryTreeMapNode* right = nullptr) {
        this->key = key;
        this->value = value;
        this->left = left;
        this->right = right;
    }
    
    /*
     * Returns true if this node has no children.
     */
    bool isLeaf() {
        return !left && !right;
    }
};

#endif
