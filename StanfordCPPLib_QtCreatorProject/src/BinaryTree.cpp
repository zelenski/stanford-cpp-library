/*
 * This file contains the implementation of members of the BinaryTree class,
 * which defines a binary tree of integers.
 * See BinaryTree.h for a description of each member.
 *
 * @version 2015/07/21
 */

#include "BinaryTree.h"
#include "random.h"
#include "strlib.h"

BinaryTree::BinaryTree(BinaryTreeNode* root) {
    this->root = root;
}

BinaryTree::~BinaryTree() {
    // TODO: free memory
    root = nullptr;
}

static void binaryTreeClearHelper(BinaryTreeNode*& node) {
    if (node) {
        binaryTreeClearHelper(node->left);
        binaryTreeClearHelper(node->right);
        delete node;
        node = nullptr;
    }
}

void BinaryTree::clear() {
    binaryTreeClearHelper(root);
}

string BinaryTree::toString() {
    return toString(root);
}
string BinaryTree::toString(BinaryTreeNode* node) {
    if (!node) {
        return "/";
    } else if (node->left == nullptr && node->right == nullptr) {
        return integerToString(node->data);
    } else {
        return "(" + integerToString(node->data) + ", "
             + toString(node->left) + ", " + toString(node->right) + ")";
    }
}

void BinaryTree::deleteTree(BinaryTreeNode* node) {
    if (node) {
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
}

BinaryTreeNode* parseBinaryTreeNodeFromQueue(Queue<string>& tokenQueue) {
    if (tokenQueue.isEmpty()) {
        return nullptr;
    } else if (tokenQueue.peek() == "(") {
        // start of a new node
        tokenQueue.dequeue();   // the "(" token
        int data = stringToInteger(tokenQueue.dequeue());
        BinaryTreeNode* node = new BinaryTreeNode(data);
        node->left = parseBinaryTreeNodeFromQueue(tokenQueue);
        node->right = parseBinaryTreeNodeFromQueue(tokenQueue);
        tokenQueue.dequeue();   // the ")" token
        return node;
    } else if (tokenQueue.peek() == "NULL" || tokenQueue.peek() == "null" || tokenQueue.peek() == "/") {
        tokenQueue.dequeue();
        return nullptr;
    } else {
        return nullptr;
    }
}

void makeTreeFromString(BinaryTree& tree, string s) {
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
    Vector<string> tokens = stringSplit(s, " ");
    Queue<string> tokenQueue;
    for (string token : tokens) {
        tokenQueue.enqueue(token);
    }
    tree.root = parseBinaryTreeNodeFromQueue(tokenQueue);
}

static void binaryTreeOpLtLtHelper(ostream& out, BinaryTreeNode* node) {
    if (!node) {
        out << "/";
    } else {
        out << "(";
        out << node->data;
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
}

ostream& operator <<(ostream& out, const BinaryTree& tree) {
    binaryTreeOpLtLtHelper(out, tree.root);
    return out;
}

istream& operator >>(istream& input, BinaryTree& tree) {
    tree.clear();
    string line;
    if (getline(input, line)) {
        makeTreeFromString(tree, line);
    }
    return input;
}

ostream& operator <<(ostream& out, BinaryTreeNode* node) {
    BinaryTree* tree = new BinaryTree();
    tree->root = node;
    out << *tree;
    tree->root = nullptr;   // avoid double-free
    return out;
}

istream& operator >>(istream& input, BinaryTreeNode*& node) {
    BinaryTree* tree = new BinaryTree();
    input >> *tree;
    if (input.fail()) {
        node = nullptr;
    } else {
        node = tree->root;
    }
    tree->root = nullptr;   // avoid double-free
    return input;
}
