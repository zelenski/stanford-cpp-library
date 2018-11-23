/*
 * CS 106B, Marty Stepp
 * This file contains the declaration of the HashTableSet class, which implements
 * a set of integers using a hash table.
 * The hash table uses separate chaining (a linked list of values in each
 * hash bucket) to resolve hash collisions.
 * See HashTableSet.cpp for the implementation of each member.
 *
 * @version 2015/07/21
 */

#ifndef _hashtableset_h
#define _hashtableset_h

#include <iostream>
#include <string>

/*
 * A HashNode stores a single integer of data and a link to another node.
 */
struct HashTableNode {
    int data;
    HashTableNode* next;
    HashTableNode(int data = 0, HashTableNode* next = nullptr) {
        this->data = data;
        this->next = next;
    }
};

class HashTableSet {
public:
    HashTableSet();
    void add(int value);
    bool contains(int value) const;
    void remove(int value);
    void printStructure() const;

    void trimChains(int k);

private:
    HashTableNode** m_elements;
    int m_size;
    int m_capacity;
    int hashCode(int value) const;
};

#endif





