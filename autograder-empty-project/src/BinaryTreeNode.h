/*
 * This file contains the declaration of the BinaryTreeNode structure.
 * A BinaryTreeNode stores one integer piece of data in a binary tree of integers.
 *
 * @version 2015/11/10
 * - added nodes for char, double, string
 * @version 2015/07/21
 * - initial version
 */

#ifndef _binarytreenode_h
#define _binarytreenode_h

#include <iostream>
#include <string>
#include "queue.h"
#include "strlib.h"

template <typename T>
class BinaryTreeNodeGen {
public:
    static int s_allocated;
    static int s_freed;
    T data;
    BinaryTreeNodeGen<T>* left;
    BinaryTreeNodeGen<T>* right;

    /*
     * Constructs a new tree node with the given data and left/right links.
     */
    BinaryTreeNodeGen<T>(const T& data = T(), BinaryTreeNodeGen* left = nullptr, BinaryTreeNodeGen* right = nullptr);
    ~BinaryTreeNodeGen<T>();
    bool isLeaf() const;
};

// define various node types so that some students need not learn C++ templates
typedef BinaryTreeNodeGen<int> BinaryTreeNode;
typedef BinaryTreeNodeGen<char> BinaryTreeNodeChar;
typedef BinaryTreeNodeGen<double> BinaryTreeNodeDouble;
typedef BinaryTreeNodeGen<std::string> BinaryTreeNodeString;

template <typename T>
std::ostream& operator <<(std::ostream& out, BinaryTreeNodeGen<T>* node);

template <typename T>
std::istream& operator >>(std::istream& input, BinaryTreeNodeGen<T>*& node);


// begin ".cpp" section

template <typename T>
int BinaryTreeNodeGen<T>::s_allocated = 0;

template <typename T>
int BinaryTreeNodeGen<T>::s_freed = 0;

/*
 * Constructs a new tree node with the given data and left/right links.
 */
template <typename T>
BinaryTreeNodeGen<T>::BinaryTreeNodeGen(const T& data, BinaryTreeNodeGen* left, BinaryTreeNodeGen* right) {
    this->data = data;
    this->left = left;
    this->right = right;
    s_allocated++;
}

template <typename T>
BinaryTreeNodeGen<T>::~BinaryTreeNodeGen() {
    s_freed++;
    this->left = nullptr;
    this->right = nullptr;
}

template <typename T>
bool BinaryTreeNodeGen<T>::isLeaf() const {
    return left == nullptr && right == nullptr;
}

template <typename T>
void parseBinaryTreeNodeFromQueue(Queue<std::string>& tokenQueue, BinaryTreeNodeGen<T>*& node) {
    if (tokenQueue.isEmpty()) {
        node = nullptr;
    } else if (tokenQueue.peek() == "(") {
        // start of a new node
        tokenQueue.dequeue();   // the "(" token
        T data;
        std::istringstream isstr(tokenQueue.dequeue());
        readGenericValue(isstr, data);
        node = new BinaryTreeNodeGen<T>(data);
        parseBinaryTreeNodeFromQueue(tokenQueue, node->left);
        parseBinaryTreeNodeFromQueue(tokenQueue, node->right);
        tokenQueue.dequeue();   // the ")" token
    } else if (tokenQueue.peek() == "NULL" || tokenQueue.peek() == "null"
               || tokenQueue.peek() == "nullptr" || tokenQueue.peek() == "/") {
        tokenQueue.dequeue();
        node = nullptr;
    } else {
        node = nullptr;
    }
}

template <typename T>
void makeTreeFromString(std::string s, BinaryTreeNodeGen<T>*& node) {
    // make easier for tokenizing
    s = stringReplace(s, "(", "( ");
    s = stringReplace(s, "[", "( ");
    s = stringReplace(s, ")", " ) ");
    s = stringReplace(s, "]", " ) ");
    s = stringReplace(s, ", ", " , ");
    s = stringReplace(s, "  ", " ");
    s = stringReplace(s, "  ", " ");
    s = stringReplace(s, "  ", " ");
    s = stringReplace(s, "  ", " ");
    Vector<std::string> tokens = stringSplit(s, " ");
    Queue<std::string> tokenQueue;
    for (const std::string& token : tokens) {
        tokenQueue.enqueue(token);
    }

    parseBinaryTreeNodeFromQueue(tokenQueue, node);
}

template <typename T>
std::ostream& binaryTreeOpLtLtHelper(std::ostream& out, BinaryTreeNodeGen<T>* node) {
    if (!node) {
        out << "/";
    } else {
        out << "(";
        writeGenericValue(out, node->data, /* forceQuotes */ true);
        if (!node->isLeaf()) {
            out << " ";
            binaryTreeOpLtLtHelper(out, node->left);
            if (node->right) {
                out << " ";
                binaryTreeOpLtLtHelper(out, node->right);
            }
        }
        out << ")";
    }
    return out;
}

template <typename T>
std::ostream& operator <<(std::ostream& out, BinaryTreeNodeGen<T>* node) {
    binaryTreeOpLtLtHelper(out, node);
    return out;
}

template <typename T>
std::istream& operator >>(std::istream& input, BinaryTreeNodeGen<T>*& node) {
    std::string line;
    if (getline(input, line)) {
        makeTreeFromString(line, node);
    } else {
        node = nullptr;
    }
    return input;
}

#endif // _binarytreenode_h
