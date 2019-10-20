/*
 * File: bst.h
 * -----------
 * This interface supports a general abstraction for binary search trees.
 * A <b><i>binary search tree</i></b> (or <b><i>BST</i></b> for short) is
 * a binary tree in which the key in each node is greater than the keys
 * descending to the left and less than the keys descending to the right.
 *
 * <p>Although the <code>BST</code> and <code>BSTNode</code> types are
 * available to clients, most applications will find the <code>Set</code>
 * and <code>HashMap</code> types instead, which use the <code>BST</code>
 * type in their implementation.
 */

#ifndef _bst_h
#define _bst_h

#include <stdarg.h>
#include "cmpfn.h"
#include "cslib.h"
#include "generic.h"
#include "iterator.h"

/*
 * Type: BST
 * ---------
 * The abstract type for a binary search tree.
 */

typedef struct BSTCDT *BST;

/*
 * Type: TraversalOrder
 * --------------------
 * This type allows clients to control the order of iteration.  Specifying
 * <code>PREORDER</code> means that the root node is processed before its
 * children, <code>INORDER</code> means that the root is processed between
 * the processing of the left and right children, and <code>POSTORDER</code>
 * means that the root is processed after its children.
 */

typedef enum {PREORDER, INORDER, POSTORDER} TraversalOrder;

/*
 * Type: BSTNode
 * -------------
 * The abstract type for a tree node.
 */

typedef struct BSTNodeCDT *BSTNode;

/*
 * Function: newBST
 * Usage: bst = newBST(type);
 * --------------------------
 * Creates a new empty binary search tree for keys with the specified base
 * type.  The <code>type</code> parameter must be an explicit type name
 * like <code>int</code> or <code>string</code>.
 */

#define newBST(type) newBSTFromType(#type)

/*
 * Function: newBSTFromType
 * Usage: bst = newBSTFromType(baseType);
 * --------------------------------------
 * Creates a new empty binary search tree for keys with the specified base
 * type expressed as a string.
 */

BST newBSTFromType(string baseType);

/*
 * Function: freeBST
 * Usage: freeBST(bst);
 * --------------------
 * Frees the storage for a binary search tree.  If nodes contain
 * data fields with allocated storage, the client must free this storage
 * by traversing the tree prior to calling <code>freeBST</code>.
 */

void freeBST(BST bst);

/*
 * Function: size
 * Usage: n = size(bst);
 * ---------------------
 * Returns the number of elements in the binary search tree.
 */

int sizeBST(BST bst);

/*
 * Function: isEmpty
 * Usage: if (isEmpty(bst)) . . .
 * ------------------------------
 * Returns <code>true</code> if the binary search tree has no entries.
 */

bool isEmptyBST(BST bst);

/*
 * Function: clearBST
 * Usage: clearBST(bst);
 * ---------------------
 * Removes all nodes from the BST.
 */

void clearBST(BST bst);

/*
 * Function: clone
 * Usage: newbst = clone(bst);
 * ---------------------------
 * Creates a copy of the BST.  The <code>clone</code> function copies
 * only the first level of the structure and does not copy the individual
 * elements.
 */

BST cloneBST(BST bst);

/*
 * Function: findBSTNode
 * Usage: node = findBSTNode(bst, key);
 * ------------------------------------
 * Applies the binary search algorithm to find a particular key
 * in the tree represented by <code>bst</code>.  If the key appears
 * in the tree, <code>findBSTNode</code> returns that node; if not,
 * <code>findBSTNode</code> returns <code>NULL</code>.
 */

BSTNode findBSTNode(BST bst, ...);

/*
 * Friend function: findBSTNodeFromArg
 * Usage: node = findBSTNodeFromArg(bst, any);
 * -------------------------------------------
 * Identical to <code>findBSTNode</code> except that the key is taken
 * from the generic argument.
 */

BSTNode findBSTNodeFromArg(BST bst, GenericType any);

/*
 * Function: insertBSTNode
 * Usage: node = insertBSTNode(bst, key);
 * --------------------------------------
 * Inserts a new node into a binary search tree, if it does not already
 * exist.  If a node already exists in the tree, <code>insertBSTNode</code>
 * returns that node; if not, <code>insertBSTNode</code> creates a new
 * node, copies the key value, and returns the newly created node.
 */

BSTNode insertBSTNode(BST bst, ...);

/*
 * Friend function: insertBSTNodeFromArg
 * Usage: node = insertBSTNodeFromArg(bst, any);
 * ---------------------------------------------
 * Identical to <code>insertBSTNode</code> except that the key is taken
 * from the generic argument.
 */

BSTNode insertBSTNodeFromArg(BST bst, GenericType any);

/*
 * Function: removeBSTNode
 * Usage: removeBSTNode(bst, key);
 * -------------------------------
 * Removes a node in the tree that matches the specified key.
 */

void removeBSTNode(BST bst, ...);

/*
 * Friend function: removeBSTNodeFromArg
 * Usage: node = removeBSTNodeFromArg(bst, any);
 * ---------------------------------------------
 * Identical to <code>removeBSTNode</code> except that the key is taken
 * from the generic argument.
 */

void removeBSTNodeFromArg(BST bst, GenericType any);

/*
 * Function: mapBST
 * Usage: mapBST(bst, fn, order, data);
 * ------------------------------------
 * Calls a function on every node in the binary search tree using the
 * specified iteration order.  The arguments to the callback function
 * are a pointer to the node and the <code>data</code> pointer.
 */

void mapBST(BST bst, proc fn, TraversalOrder order, void *data);

/*
 * Function: newNodeIterator
 * Usage: iterator = newNodeIterator(bst, order);
 * ----------------------------------------------
 * Returns an iterator for traversing the nodes in a binary search tree
 * in the specified order.  The <code>foreach</code> statement
 * automatically uses an <code>INORDER</code> traversal.
 */

Iterator newNodeIterator(BST bst, TraversalOrder order);

/*
 * Function: getRootBST
 * Usage: root = getRootBST(bst);
 * ------------------------------
 * Returns the root node of the binary search tree.
 */

BSTNode getRootBST(BST bst);

/*
 * Function: getLeftChild
 * Usage: child = getLeftChild(node);
 * ----------------------------------
 * Returns the left child of the specified node in a binary search tree.
 */

BSTNode getLeftChild(BSTNode node);

/*
 * Function: getRightChild
 * Usage: child = getRightChild(node);
 * -----------------------------------
 * Returns the right child of the specified node in a binary search tree.
 */

BSTNode getRightChild(BSTNode node);

/*
 * Function: getKey
 * Usage: any = getKey(node);
 * --------------------------
 * Returns the key as a generic object
 */

GenericType getKey(BSTNode node);

/*
 * Function: getKeyString
 * Usage: str = getKeyString(node);
 * --------------------------------
 * Returns a string representation of the key.
 */

string getKeyString(BSTNode node);

/*
 * Function: setNodeValue
 * Usage: setNodeValue(node, value);
 * ---------------------------------
 * Sets the value pointer associated with a <code>BSTNode</code>.
 */

void setNodeValue(BSTNode node, void *value);

/*
 * Function: getNodeValue
 * Usage: value = getNodeValue(node);
 * ----------------------------------
 * Returns the value pointer associated with a <code>BSTNode</code>.
 */

void *getNodeValue(BSTNode node);

/*
 * Function: getBaseTypeBST
 * Usage: baseType = getBaseTypeBST(bst);
 * --------------------------------------
 * Returns the name of the key type.
 */

string getBaseTypeBST(BST bst);

/*
 * Function: getBaseTypeSizeBST
 * Usage: size = getBaseTypeSizeBST(bst);
 * --------------------------------------
 * Returns the size of the base type in bytes.
 */

int getBaseTypeSizeBST(BST bst);

/*
 * Function: setCompareFnBST
 * Usage: setCompareFnBST(bst, cmpFn);
 * -----------------------------------
 * Sets the comparison function for keys.  This method need not be
 * called for any of the standard types.
 */

void setCompareFnBST(BST bst, CompareFn cmpFn);

/*
 * Function: getCompareFnBST
 * Usage: cmpFn = getCompareFnBST(bst);
 * ------------------------------------
 * Returns the comparison function for keys.
 */

CompareFn getCompareFnBST(BST bst);

#endif
