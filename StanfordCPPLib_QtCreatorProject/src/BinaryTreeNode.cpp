/*
 * This file implements the members declared in BinaryTreeNode.h.
 * 
 * @version 2015/07/21
 */

#include "BinaryTreeNode.h"

int BinaryTreeNode::s_allocated = 0;
int BinaryTreeNode::s_freed = 0;

/*
 * Constructs a new tree node with the given data and left/right links.
 */
BinaryTreeNode::BinaryTreeNode(int data, BinaryTreeNode* left, BinaryTreeNode* right) {
    this->data = data;
    this->left = left;
    this->right = right;
    s_allocated++;
}

BinaryTreeNode::~BinaryTreeNode() {
    s_freed++;
}

bool BinaryTreeNode::isLeaf() {
    return left == NULL && right == NULL;
}
