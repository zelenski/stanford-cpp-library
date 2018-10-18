/*
 * CS 106B, Marty Stepp
 * This file contains the implementation of the HashTableSet class, which implements
 * a set of integers using a hash table.
 * See HashTableSet.h for the declarations of each member.
 *
 * @version 2015/07/21
 */

#define INTERNAL_INCLUDE 1
#include <iostream>
#include <iomanip>
#include "HashTableSet.h"
#undef INTERNAL_INCLUDE

HashTableSet::HashTableSet() {
    m_size = 0;
    m_capacity = 10;
    m_elements = new HashTableNode*[m_capacity]();   // all null
}

void HashTableSet::add(int value) {
    if (!contains(value)) {
        int index = hashCode(value);
        HashTableNode* node = new HashTableNode(value);
        node->next = m_elements[index];
        m_elements[index] = node;
        m_size++;
    }
}

bool HashTableSet::contains(int value) const {
    int index = hashCode(value);
    HashTableNode* current = m_elements[index];
    while (current) {
        if (current->data == value) {
            return true;
        }
        current = current->next;
    }
    return false;
}

int HashTableSet::hashCode(int value) const {
    return abs(value) % m_capacity;
}

void HashTableSet::remove(int /*value*/) {
    // not implemented
}

void HashTableSet::printStructure() const {
    for (int i = 0; i < m_capacity; i++) {
        std::cout << "[" << std::setw(2) << i << "]:";
        HashTableNode* curr = m_elements[i];
        while (curr) {
            std::cout << " -> " << std::setw(2) << curr->data;
            curr = curr->next;
        }
        std::cout << " /" << std::endl;
    }
    // cout << "size = " << m_size << ", load factor = " << loadFactor() << endl;
}
