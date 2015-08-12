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
    // TODO
    root = NULL;
}

bool BinaryTree::contains(int value) {
    return contains(root, value);
}

bool BinaryTree::contains(BinaryTreeNode* node, int value) {
    if (node == NULL) {
        return false;
    } else if (node->data == value) {
        return true;
    } else {
        return contains(node->left, value) ||
               contains(node->right, value);
    }
}

static void binaryTreeClearHelper(BinaryTreeNode*& node) {
    if (node != NULL) {
        binaryTreeClearHelper(node->left);
        binaryTreeClearHelper(node->right);
        delete node;
        node = NULL;
    }
}

void BinaryTree::clear() {
    binaryTreeClearHelper(root);
}

void BinaryTree::print() {
    print(root, 1);
}

void BinaryTree::print(BinaryTreeNode* node, int level) {
    if (node != NULL) {
        print(node->left, level+1);
        cout << node->data << " (" << level << ")" << endl;
        print(node->right, level+1);
    }
}

void BinaryTree::printSideways() {
    printSideways(root, "");
}

void BinaryTree::printSideways(BinaryTreeNode* node, string indent) {
    if (node != NULL) {
        printSideways(node->right, indent + "  ");
        cout << indent << node->data << endl;
        printSideways(node->left, indent + "  ");
    }
}

int BinaryTree::size() {
    return size(root);
}

int BinaryTree::size(BinaryTreeNode* node) {
    if (node == NULL) {
        return 0;
    } else {
        return 1 + size(node->left) + size(node->right);
    }
}

//int BinaryTree::height() {
//    return height(root);
//}
//int BinaryTree::height(BinaryTreeNode* node) {
//    if (node == NULL) {
//        return 0;
//    } else {
//        return 1 + max(height(node->left), height(node->right));
//    }
//}

string BinaryTree::toString() {
    return toString(root);
}
string BinaryTree::toString(BinaryTreeNode* node) {
    if (node == NULL) {
        return "/";
    } else if (node->left == NULL && node->right == NULL) {
        return integerToString(node->data);
    } else {
        return "(" + integerToString(node->data) + ", "
             + toString(node->left) + ", " + toString(node->right) + ")";
    }
}

void BinaryTree::deleteTree(BinaryTreeNode* node) {
    if (node != NULL) {
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
}

BinaryTreeNode* parseBinaryTreeNodeFromQueue(Queue<string>& tokenQueue) {
    if (tokenQueue.isEmpty()) {
        return NULL;
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
        return NULL;
    } else {
        return NULL;
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
    if (node == NULL) {
        out << "/";
    } else {
        out << "(";
        out << node->data;
        if (!node->isLeaf()) {
            out << " ";
            binaryTreeOpLtLtHelper(out, node->left);
            if (node->right != NULL) {
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
