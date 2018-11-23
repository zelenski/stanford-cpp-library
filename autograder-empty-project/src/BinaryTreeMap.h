/*
 * CS 106B, Marty Stepp
 * This file contains the declaration of the BinaryTreeMap class,
 * which defines a map from string->integer using a binary search tree.
 * See BinaryTreeMap.cpp for implementation of each member.
 *
 * @version 2015/07/21
 */

#ifndef _binarytreemap_h
#define _binarytreemap_h

#include <iostream>
#include <string>
#include "BinaryTreeMapNode.h"
using namespace std;

class BinaryTreeMap {
public:
    /*
     * Constructs a new tree map.
     */
    BinaryTreeMap();

    /*
     * Frees memory that was allocated by this tree map.
     */
    ~BinaryTreeMap();
    
    /*
     * Returns true if the given key is contained in this map.
     */
    bool containsKey(string key) const;

    /*
     * Returns the value associated with the given key in this map.
     * If the given key is not associated with any value, returns 0.
     */
    int get(string key) const;

    /*
     * Returns true if this map does not contain any key/value pairs.
     */
    bool isEmpty() const;

    /*
     * Adds the given key/value pair to this tree map, if not already present.
     */
    void put(string key, int value);

    /*
     * Removes the given key and its associated value from this tree map, if it was present.
     */
    void remove(string key);

    /*
     * Returns the number of key/value pairs in this map.
     */
    int size() const;
    
private:
    BinaryTreeMapNode* m_root;   // topmost node in the tree (null if empty)
    int m_size;            // # of k/v pairs in map (0 if empty)

    /*
     * Recursive helpers to implement the above public members.
     * Each helper accepts a pointer to the part of the tree to process.
     */
    bool containsKey(BinaryTreeMapNode* node, string key) const;
    int get(BinaryTreeMapNode* node, string key) const;
    BinaryTreeMapNode* getMin(BinaryTreeMapNode* node) const;
    void put(BinaryTreeMapNode*& node, string key, int value);
    void remove(BinaryTreeMapNode*& node, string key);
};

#endif
