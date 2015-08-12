/*
 * CS 106B, Marty Stepp
 * This file contains the implementation of the BinaryTreeMap class,
 * which defines a map from string->integer using a binary search tree.
 * See BinaryTreeMap.h for a description of each member.
 *
 * @version 2015/07/21
 */

#include <string>
#include "BinaryTreeMap.h"
#include "strlib.h"
using namespace std;

BinaryTreeMap::BinaryTreeMap() {
    m_root = NULL;
}

BinaryTreeMap::~BinaryTreeMap() {
    // TODO: implement

}

bool BinaryTreeMap::containsKey(string key) const {
    return containsKey(m_root, key);
}

bool BinaryTreeMap::containsKey(BinaryTreeMapNode* node, string key) const {
    if (node == NULL) {
        return false;
    } else if (key == node->key) {
        return true;
    } else if (key < node->key) {
        return containsKey(node->left, key);
    } else {  // key > node->key
        return containsKey(node->right, key);
    }
}

int BinaryTreeMap::get(string key) const {
    return get(m_root, key);
}

int BinaryTreeMap::get(BinaryTreeMapNode* node, string key) const {
    if (node == NULL) {
        return 0;
    } else if (key == node->key) {
        return node->value;
    } else if (key < node->key) {
        return get(node->left, key);
    } else {  // key > node->key
        return get(node->right, key);
    }
}

bool BinaryTreeMap::isEmpty() const {
    return m_size == 0;
}

void BinaryTreeMap::put(string key, int value) {
    put(m_root, key, value);
}

void BinaryTreeMap::put(BinaryTreeMapNode*& node, string key, int value) {
    if (node == NULL) {
        node = new BinaryTreeMapNode(key, value);
        m_size++;
    } else if (node->key == key) {
        node->value = value;
    } else if (key < node->key) {
        put(node->left, key, value);
    } else {  // key > node->key
        put(node->right, key, value);
    }
}

void BinaryTreeMap::remove(string key) {
    remove(m_root, key);
}

void BinaryTreeMap::remove(BinaryTreeMapNode*& node, string key) {
    if (node == NULL) {
        // not here, nothing to do
    } else if (key == node->key) {
        // remove THIS node
        BinaryTreeMapNode* trash = NULL;
        if (node->isLeaf()) {
            trash = node;
            node = NULL;
        } else if (node->right == NULL) {
            // left child only; replace me with left
            trash = node;
            node = node->left;
        } else if (node->left == NULL) {
            // right child only; replace me with right
            trash = node;
            node = node->right;
        } else {
            // :-( both children alive
            BinaryTreeMapNode* rightMinNode = getMin(node->right);
            node->key = rightMinNode->key;
            node->value = rightMinNode->value;
            remove(node->right, rightMinNode->key);
        }

        m_size--;
        if (trash != NULL) {
            delete trash;
        }
    } else if (key < node->key) {
        remove(node->left, key);
    } else {  // key > node->key
        remove(node->right, key);
    }
}

BinaryTreeMapNode* BinaryTreeMap::getMin(BinaryTreeMapNode* node) const {
    if (node == NULL || node->left == NULL) {
        return node;
    } else {
        return getMin(node->left);
    }
}

int BinaryTreeMap::size() const {
    return m_size;
}
