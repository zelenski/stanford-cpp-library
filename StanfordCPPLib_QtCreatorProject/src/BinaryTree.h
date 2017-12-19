/*
 * This file contains the declaration of the BinaryTree class,
 * which defines a binary tree of integers.
 * See BinaryTree.cpp for implementation of each member.
 *
 * @version 2017/10/20
 * - replaced nulls with nullptr
 * @version 2015/07/21
 */

#ifndef _binarytree_h
#define _binarytree_h

#include <iostream>
#include <string>
#include "BinaryTreeNode.h"
#include "queue.h"
using namespace std;

class BinaryTree {
public:
    // public just for exam testing
    BinaryTreeNode* root;   // topmost node in the tree (null if empty)

    BinaryTree(BinaryTreeNode* root = nullptr);
    virtual ~BinaryTree();
    
    void clear();
    // int height();
    string toString();
    
    // lecture problems
    void print() const;
    int size() const;
    void printSideways() const;
    bool contains(int value) const;

    // section problems
    int height() const;
    int countLeftNodes() const;
    bool isBalanced() const;
    bool isBST() const;
    void removeLeaves();
    void completeToLevel(int k);
    void tighten();
    void limitPathSum(int max);
    
    // exam problems
    bool hasPath(int start, int end) const;
    bool isConsecutive() const;
    void limitLeaves(int n);
    int makeFull();
    int range(int min, int max);
    void removeMatchingLeaves(const BinaryTree& other);
    void swapChildrenAtLevel(int level);

    // operators for reading/writing trees based on a root pointer
    // (these are used by the CodeStepByStep tool)
    friend ostream& operator <<(ostream& out, BinaryTreeNode* node);
    friend istream& operator >>(istream& input, BinaryTreeNode*& node);

private:
    int size(BinaryTreeNode* node);
    string toString(BinaryTreeNode* node);
    void deleteTree(BinaryTreeNode* node);
};

ostream& operator <<(ostream& out, const BinaryTree& tree);
istream& operator >>(istream& input, BinaryTree& tree);

enum Order {PRE_ORDER, IN_ORDER, POST_ORDER};

template <typename T>
class TreeSet {
public:
    TreeSet(BinaryTreeNodeGen<T>* root = nullptr);
    virtual ~TreeSet();
    
    void add(T value);
    bool contains(T value);
    T getMin() const;
    int height() const;
    bool isBalanced() const;
    void print(int order = IN_ORDER);
    void printSideways();
    void remove(T value);
    
private:
    BinaryTreeNodeGen<T>* root;   // topmost node in the tree (nullptr if empty)

    void add(BinaryTreeNodeGen<T>*& node, T value);
    bool contains(BinaryTreeNodeGen<T>* node, T value);
    T getMin(BinaryTreeNodeGen<T>* node) const;
    int height(BinaryTreeNodeGen<T>* node) const;
    bool isBalanced(BinaryTreeNodeGen<T>* node) const;
    void print(BinaryTreeNodeGen<T>* node, int order = IN_ORDER);
    void printSideways(BinaryTreeNodeGen<T>* node, string indent);
    void remove(BinaryTreeNodeGen<T>*& node, T value);
};

// method bodies must be in .h file for template classes

template <typename T>
TreeSet<T>::TreeSet(BinaryTreeNodeGen<T>* root) {
    this->root = root;
}

template <typename T>
TreeSet<T>::~TreeSet() {
    // TODO
    root = nullptr;
}

template <typename T>
void TreeSet<T>::add(T value) {
    add(root, value);
}

template <typename T>
void TreeSet<T>::add(BinaryTreeNodeGen<T>*& node, T value) {
    if (!node) {
        node = new BinaryTreeNodeGen<T>(value);
    } else if (value < node->data) {
        add(node->left, value);
    } else if (value > node->data) {
        add(node->right, value);
    }
}

template <typename T>
bool TreeSet<T>::contains(T value) {
    return contains(root, value);
}

template <typename T>
bool TreeSet<T>::contains(BinaryTreeNodeGen<T>* node, T value) {
    if (!node) {
        return false;
    } else if (value == node->data) {
        return true;
    } else if (value < node->data) {
        return contains(node->left, value);
    } else {
        return contains(node->right, value);
    }
}

template <typename T>
T TreeSet<T>::getMin() const {
    return getMin(root);
}

template <typename T>
T TreeSet<T>::getMin(BinaryTreeNodeGen<T>* node) const {
    if (!node) {
        throw ":-(";
    } else if (!node->left) {
        return node->data;
    } else {
        return getMin(node->left);
    }
}

template <typename T>
int TreeSet<T>::height() const {
    return height(root);
}

template <typename T>
int TreeSet<T>::height(BinaryTreeNodeGen<T>* node) const {
    if (!node) {
        return 0;
    } else {
        int l = height(node->left);
        int r = height(node->right);
        return max(l, r) + 1;
    }
}

template <typename T>
bool TreeSet<T>::isBalanced() const {
    return isBalanced(root);
}

template <typename T>
bool TreeSet<T>::isBalanced(BinaryTreeNodeGen<T>* node) const {
    if (!node) {
        return true;
    } else {
        int l = height(node->left);
        int r = height(node->right);
        int bf = r - l;
        if (abs(bf) > 1) {
            cout << "imbalanced: " << node->data << " (balance factor: " << bf << ")" << endl;
        }
        // inefficient because we want to print ALL imbalanced nodes
        bool balL = isBalanced(node->left);
        bool balR = isBalanced(node->right);
        return abs(l - r) <= 1 && balL && balR;
    }
}

template <typename T>
void TreeSet<T>::print(int order) {
    print(root, order);
    cout << endl;
}

template <typename T>
void TreeSet<T>::print(BinaryTreeNodeGen<T>* node, int order) {
    if (node) {
        if (order == PRE_ORDER) {
            cout << node->data << " ";
            cout.flush();
        }
        print(node->left, order);
        if (order == IN_ORDER) {
            cout << node->data << " ";
            cout.flush();
        }
        print(node->right, order);
        if (order == POST_ORDER) {
            cout << node->data << " ";
            cout.flush();
        }
    }
}

template <typename T>
void TreeSet<T>::printSideways() {
    printSideways(root, "");
}

template <typename T>
void TreeSet<T>::printSideways(BinaryTreeNodeGen<T>* node, string indent) {
    if (node) {
        printSideways(node->right, indent + "  ");
        cout << indent << node->data << endl;
        printSideways(node->left, indent + "  ");
    }
}

template <typename T>
void TreeSet<T>::remove(T value) {
    remove(root, value);
}

template <typename T>
void TreeSet<T>::remove(BinaryTreeNodeGen<T>*& node, T value) {
    if (!node) {
        // do nothing
    } else if (value < node->data) {
        remove(node->left, value);
    } else if (value > node->data) {
        remove(node->right, value);
    } else {
        // remove this node
        if (!node->left && !node->right) {
            delete node;
            node = nullptr;
        } else if (!node->left) {
            BinaryTreeNodeGen<T>* trash = node;
            node = node->right;
            delete trash;
        } else if (!node->right) {
            BinaryTreeNodeGen<T>* trash = node;
            node = node->left;
            delete trash;
        } else {
            T min = getMin(node->right);
            remove(node->right, min);
            node->data = min;
        }
    }
}

#endif // _binarytree_h
