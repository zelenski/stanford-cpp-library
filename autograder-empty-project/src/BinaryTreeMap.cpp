/*
 * CS 106B, Marty Stepp
 * This file contains the implementation of the BinaryTreeMap class,
 * which defines a map from string->integer using a binary search tree.
 * See BinaryTreeMap.h for a description of each member.
 *
 * @version 2015/07/21
 */

#define INTERNAL_INCLUDE 1
#include <string>
#include "BinaryTreeMap.h"
#include "strlib.h"
#undef INTERNAL_INCLUDE

BinaryTreeMap::BinaryTreeMap() {
    m_root = nullptr;
}

BinaryTreeMap::~BinaryTreeMap() {
    // TODO: implement

}

bool BinaryTreeMap::containsKey(string key) const {
    return containsKey(m_root, key);
}

bool BinaryTreeMap::containsKey(BinaryTreeMapNode* node, string key) const {
    if (!node) {
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
    if (!node) {
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
    if (!node) {
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
    if (!node) {
        // not here, nothing to do
    } else if (key == node->key) {
        // remove THIS node
        BinaryTreeMapNode* trash = nullptr;
        if (node->isLeaf()) {
            trash = node;
            node = nullptr;
        } else if (!node->right) {
            // left child only; replace me with left
            trash = node;
            node = node->left;
        } else if (!node->left) {
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
        if (trash) {
            delete trash;
        }
    } else if (key < node->key) {
        remove(node->left, key);
    } else {  // key > node->key
        remove(node->right, key);
    }
}

BinaryTreeMapNode* BinaryTreeMap::getMin(BinaryTreeMapNode* node) const {
    if (!node || !node->left) {
        return node;
    } else {
        return getMin(node->left);
    }
}

int BinaryTreeMap::size() const {
    return m_size;
}
