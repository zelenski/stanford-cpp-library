/*
 * This file contains the implementation of members of the BinaryTree class,
 * which defines a binary tree of integers.
 * See BinaryTree.h for a description of each member.
 *
 * @version 2015/07/21
 */

#define INTERNAL_INCLUDE 1
#include "BinaryTree.h"
#undef INTERNAL_INCLUDE

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

std::string BinaryTree::toString() {
    return toString(root);
}
std::string BinaryTree::toString(BinaryTreeNode* node) {
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

std::ostream& operator <<(std::ostream& out, const BinaryTree& tree) {
    return binaryTreeOpLtLtHelper(out, tree.root);
}

std::istream& operator >>(std::istream& input, BinaryTree& tree) {
    tree.clear();
    std::string line;
    if (getline(input, line)) {
        makeTreeFromString(line, tree.root);
    }
    return input;
}
