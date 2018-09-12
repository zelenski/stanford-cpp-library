/*
 * CS 106B, Marty Stepp
 * This file contains the declaration of the HashTableMap class, which implements
 * a map using a hash table.
 * The hash table uses separate chaining (a linked list of values in each
 * hash bucket) to resolve hash collisions.
 *
 * @version 2015/07/21
 */

#ifndef _hashtablemap_h
#define _hashtablemap_h

#include <iomanip>
#include <iostream>
#include <string>

/*
 * A HashNode stores a single integer of data and a link to another node.
 */
template <typename K, typename V>
struct HashTableMapNode {
    K key;
    V value;
    HashTableMapNode* next;
    HashTableMapNode(K key, V value, HashTableMapNode* next = nullptr) {
        this->key = key;
        this->value = value;
        this->next = next;
    }
};

template <typename K, typename V>
class HashTableMap {
public:
    HashTableMap(int capacity = 10, double rehashLoadFactor = 0.5);
    void put(K key, V value);
    bool containsKey(K key) const;
    V get(K key) const;
    double loadFactor() const;
    void printStructure() const;
    void remove(K key);
    void setRehashLoadFactor(double loadFactor);

private:
    HashTableMapNode<K, V>** elements;
    int mysize;
    int capacity;
    double rehashLoadFactor;
    int __hashCode(int n) const;
    int __hashCode(double d) const;
    int __hashCode(string s) const;
    int _hashCode(K key) const;

    void rehash();
};

template <typename K, typename V>
HashTableMap<K, V>::HashTableMap(int capacity, double rehashLoadFactor) {
    mysize = 0;
    this->capacity = capacity;
    elements = new HashTableMapNode<K, V>*[capacity]();
    this->rehashLoadFactor = rehashLoadFactor;
}

template <typename K, typename V>
void HashTableMap<K, V>::put(K key, V value) {
    int index = _hashCode(key);
    if (containsKey(key)) {
        // already has a pair for this key; replace the value
        HashTableMapNode<K, V>* current = elements[index];
        while (current) {
            if (current->key == key) {
                current->value = value;
                break;
            }
            current = current->next;
        }
    } else {
        // add a new pair to the front of the chain
        elements[index] = new HashTableMapNode<K, V>(key, value, elements[index]);
        mysize++;
        if (loadFactor() >= rehashLoadFactor) {
            rehash();
        }
    }
}

template <typename K, typename V>
bool HashTableMap<K, V>::containsKey(K key) const {
    int index = _hashCode(key);
    HashTableMapNode<K, V>* current = elements[index];
    while (current) {
        if (current->key == key) {
            return true;
        }
        current = current->next;
    }
    return false;
}

template <typename K, typename V>
V HashTableMap<K, V>::get(K key) const {
    int index = _hashCode(key);
    HashTableMapNode<K, V>* current = elements[index];
    while (current) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }
    return V();
}

template <typename K, typename V>
int HashTableMap<K, V>::__hashCode(int n) const {
    return n;
}

template <typename K, typename V>
int HashTableMap<K, V>::__hashCode(double d) const {
    return hashCode(d);
}

template <typename K, typename V>
int HashTableMap<K, V>::__hashCode(string s) const {
    return hashCode(s);
}

template <typename K, typename V>
int HashTableMap<K, V>::_hashCode(K key) const {
    return abs(__hashCode(key)) % capacity;
}

template <typename K, typename V>
double HashTableMap<K, V>::loadFactor() const {
    return (double) mysize / capacity;
}

template <typename K, typename V>
void HashTableMap<K, V>::printStructure() const {
    for (int i = 0; i < capacity; i++) {
        cout << "[" << setw(2) << i << "]:";
        HashTableMapNode<K, V>* curr = elements[i];
        while (curr) {
            cout << " -> " << setw(2) << curr->key << ":" << curr->value;
            curr = curr->next;
        }
        cout << endl;
    }
    cout << "size        = " << mysize << endl;
    cout << "capacity    = " << capacity << endl;
    cout << "load factor = " << loadFactor() << endl;
}

template <typename K, typename V>
void HashTableMap<K, V>::rehash() {
    HashTableMapNode<K, V>** oldElements = elements;
    elements = new HashTableMapNode<K, V>*[capacity * 2]();
    capacity *= 2;
    mysize = 0;
    
    for (int i = 0; i < capacity/2; i++) {
        HashTableMapNode<K, V>* curr = oldElements[i];
        while (curr) {
            put(curr->key, curr->value);
            HashTableMapNode<K, V>* trash = curr;
            curr = curr->next;
            delete trash;
        }
    }
    
    delete oldElements;
}

template <typename K, typename V>
void HashTableMap<K, V>::remove(K key) {
    int index = _hashCode(key);
    if (!elements[index]) {
        return;
    } else if (elements[index]->key == key) {
        // remove from front
        HashTableMapNode<K, V>* trash = elements[index];
        elements[index] = elements[index]->next;
        delete trash;
        mysize--;
    } else {
        // remove from later in the chain
        HashTableMapNode<K, V>* current = elements[index];
        while (current->next) {
            if (current->next->key == key) {
                HashTableMapNode<K, V>* trash = current->next;
                current->next = current->next->next;
                delete trash;
                mysize--;
                break;
            }
            current = current->next;
        }
    }
}

template <typename K, typename V>
void HashTableMap<K, V>::setRehashLoadFactor(double loadFactor) {
    this->rehashLoadFactor = loadFactor;
}

#endif
