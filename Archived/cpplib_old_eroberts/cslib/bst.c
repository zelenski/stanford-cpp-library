/*
 * File: bst.c
 * Last modified on Mon Dec 10 20:52:46 2012 by eroberts
 * -----------------------------------------------------
 * This file implements the bst.h interface, which provides a general
 * implementation of binary search trees.  It is used in the
 * implementations of the Map and Set types.
 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "bst.h"
#include "cmpfn.h"
#include "cslib.h"
#include "exception.h"
#include "foreach.h"
#include "generic.h"
#include "iterator.h"
#include "itertype.h"
#include "strlib.h"
#include "unittest.h"

/*
 * Type: BSTCDT
 * ------------
 * This type is the concrete type used to represent the BST.
 */

struct BSTCDT {
   IteratorHeader header;      /* Header to enable iteration            */
   string baseType;            /* The name of the base type             */
   int baseTypeSize;           /* Size of the base type in bytes        */
   CompareFn cmpFn;            /* Function to compare two keys          */
   FetchFn fetchFn;            /* Function to fetch an argument         */
   StoreFn storeFn;            /* Function to store a value             */
   ToStringFn toStringFn;      /* Function to convert key to a string   */
   BSTNode root;               /* Root of the tree                      */
   int count;                  /* Number of entries in the BST          */
   FILE *debugLog;             /* Debugging log                         */
};

/*
 * Type: BSTNode
 * -------------
 * This type defines the structure of a BST node.
 */

struct BSTNodeCDT {
   GenericType key;            /* Space for the key                     */
   void *value;                /* Value field for maps                  */
   struct BSTNodeCDT *left;    /* Left child                            */
   struct BSTNodeCDT *right;   /* Right child                           */
   int bf;                     /* Balance factor for node (-1, 0, +1)   */
   BST bst;                    /* Back pointer to the bst               */
};

/* Private function prototypes */

static BSTNode copyTree(BST newbst, BSTNode t);
static BSTNode findTreeNode(BST bst, BSTNode t, void *kp);
static int insertTreeNode(BST bst, BSTNode *tp, void *kp, BSTNode *rp);
static int removeTreeNode(BST bst, BSTNode *tp, void *kp);
static int removeTargetNode(BST bst, BSTNode *tp);
static void freeTree(BSTNode t);
static void freeNode(BSTNode node);
static void adjustBF(BST bst, BSTNode *tp, int delta);
static void fixLeftImbalance(BST bst, BSTNode *tp);
static void fixRightImbalance(BST bst, BSTNode *tp);
static void rotateLeft(BST bst, BSTNode *tp);
static void rotateRight(BST bst, BSTNode *tp);
static void mapTree(BSTNode t, proc fn, TraversalOrder order, void *data);
static void addNodeToIterator(BSTNode node, void *data);
static Iterator newForeachIterator(void *collection);

/* Exported entries */

BST newBSTFromType(string baseType) {
   BST bst;

   bst = newBlock(BST);
   enableIteration(bst, newForeachIterator);
   bst->baseType = baseType;
   bst->baseTypeSize = getTypeSizeForType(baseType);
   bst->fetchFn = getFetchFnForType(baseType);
   bst->storeFn = getStoreFnForType(baseType);
   bst->cmpFn = getCompareFnForType(baseType);
   bst->toStringFn = getToStringFn(baseType);
   bst->count = 0;
   bst->root = NULL;
   bst->debugLog = NULL;
   return bst;
}

void freeBST(BST bst) {
   clearBST(bst);
   freeBlock(bst);
}

int sizeBST(BST bst) {
   return bst->count;
}

bool isEmptyBST(BST bst) {
   return bst->count == 0;
}

void clearBST(BST bst) {
   freeTree(bst->root);
   bst->count = 0;
}

BST cloneBST(BST bst) {
   BST newbst;

   newbst = newBlock(BST);
   enableIteration(newbst, newForeachIterator);
   newbst->baseType = bst->baseType;
   newbst->baseTypeSize = bst->baseTypeSize;
   newbst->fetchFn = bst->fetchFn;
   newbst->storeFn = bst->storeFn;
   newbst->cmpFn = bst->cmpFn;
   newbst->toStringFn = bst->toStringFn;
   newbst->count = bst->count;
   newbst->root = copyTree(newbst, bst->root);
   newbst->debugLog = NULL;
   return newbst;
}

BSTNode findBSTNode(BST bst, ...) {
   va_list args;
   GenericType any;

   va_start(args, bst);
   bst->fetchFn(args, &any);
   va_end(args);
   return findBSTNodeFromArg(bst, any);
}

BSTNode findBSTNodeFromArg(BST bst, GenericType any) {
   return findTreeNode(bst, bst->root, &any);
}

BSTNode insertBSTNode(BST bst, ...) {
   va_list args;
   GenericType any;

   va_start(args, bst);
   bst->fetchFn(args, &any);
   va_end(args);
   return insertBSTNodeFromArg(bst, any);
}

BSTNode insertBSTNodeFromArg(BST bst, GenericType any) {
   BSTNode node;

   insertTreeNode(bst, &bst->root, &any, &node);
   return node;
}

void removeBSTNode(BST bst, ...) {
   va_list args;
   GenericType any;

   va_start(args, bst);
   bst->fetchFn(args, &any);
   va_end(args);
   removeBSTNodeFromArg(bst, any);
}

void removeBSTNodeFromArg(BST bst, GenericType any) {
   removeTreeNode(bst, &bst->root, &any);
}

void mapBST(BST bst, proc fn, TraversalOrder order, void *data) {
   mapTree(bst->root, fn, order, data);
}

/*
 * Implementation notes: newNodeIterator
 * -------------------------------------
 * This function creates an iterator that maps over the nodes in the
 * binary search tree using the specified iteration order.  Using the bst
 * value itself in a <code>foreach</code> construct creates a key iterator
 * with an INORDER traversal.  For details on the general iterator strategy,
 * see the comments in the <code>itertype.h</code> interface.
 */

Iterator newNodeIterator(BST bst, TraversalOrder order) {
   Iterator iterator;

   iterator = newListIterator(sizeof(void *), NULL);
   mapBST(bst, addNodeToIterator, order, iterator);
   return iterator;
}

BSTNode getRootBST(BST bst) {
   return bst->root;
}

BSTNode getLeftChild(BSTNode node) {
   return node->left;
}

BSTNode getRightChild(BSTNode node) {
   return node->right;
}

GenericType getKey(BSTNode node) {
   return node->key;
}

string getKeyString(BSTNode node) {
   return node->bst->toStringFn(node->key);
}

int getBF(BSTNode node) {
   return node->bf;
}

void setNodeValue(BSTNode node, void *value) {
   node->value = value;
}

void *getNodeValue(BSTNode node) {
   return node->value;
}

string getBaseTypeBST(BST bst) {
   return bst->baseType;
}

int getBaseTypeSizeBST(BST bst) {
   return bst->baseTypeSize;
}

void setCompareFnBST(BST bst, CompareFn cmpFn) {
   bst->cmpFn = cmpFn;
}

CompareFn getCompareFnBST(BST bst) {
   return bst->cmpFn;
}

void setDebugLog(BST bst, FILE *outfile) {
   bst->debugLog = outfile;
}

/* Private functions */

/*
 * Implementation notes: copyTree
 * ------------------------------
 * This function implements a recursive walk on the tree to preserve the
 * structure.
 */

static BSTNode copyTree(BST newbst, BSTNode t) {
   BSTNode node;

   if (t == NULL) return NULL;
   node = newBlock(BSTNode);
   node->key = t->key;
   node->value = t->value;
   node->left = copyTree(newbst, t->left);
   node->right = copyTree(newbst, t->right);
   node->bf = t->bf;
   node->bst = newbst;
   return node;
}

/*
 * Implementation notes: findTreeNode
 * ----------------------------------
 * Finds the specified key by walking recursively over the BST.
 */

static BSTNode findTreeNode(BST bst, BSTNode t, void *kp) {
   int sign;

   if (t == NULL) return NULL;
   sign = bst->cmpFn(kp, &t->key);
   if (sign == 0) return t;
   if (sign < 0) {
      return findTreeNode(bst, t->left, kp);
   } else {
      return findTreeNode(bst, t->right, kp);
   }
}

/*
 * Implementation notes: insertTreeNode
 * ------------------------------------
 * Enters the key into the binary search tree, starting the recursive
 * search at the tree whose address is passed as the tp parameter.
 * The rp parameter specifies the location in which the node (which may
 * be either an existing node or a newly allocated one) is stored.  The
 * return value is an integer that indicates the change in the height of
 * the subtree, which is then used to correct the balance factors in
 * ancestor nodes.
 */

static int insertTreeNode(BST bst, BSTNode *tp, void *kp, BSTNode *rp) {
   BSTNode t;
   int sign;

   t = *tp;
   if (t == NULL) {
      t = newBlock(BSTNode);
      memcpy(&t->key, kp, bst->baseTypeSize);
      t->bst = bst;
      t->bf = 0;
      t->left = t->right = NULL;
      t->value = NULL;
      *tp = t;
      *rp = t;
      bst->count++;
      return +1;
   }
   sign = bst->cmpFn(kp, &t->key);
   if (sign == 0) {
      *rp = t;
      return 0;
   }
   if (sign < 0) {
      if (insertTreeNode(bst, &t->left, kp, rp) > 0) {
         switch (t->bf) {
           case +1: t->bf =  0; return 0;
           case  0: t->bf = -1; return +1;
           case -1: fixLeftImbalance(bst, tp); return 0;
         }
      }
   } else {
      if (insertTreeNode(bst, &t->right, kp, rp) > 0) {
         switch (t->bf) {
           case -1: t->bf =  0; return 0;
           case  0: t->bf = +1; return +1;
           case +1: fixRightImbalance(bst, tp); return 0;
         }
      }
   }
   return 0;
}

/*
 * Implementation notes: removeTreeNode
 * ------------------------------------
 * Removes the node with the specified key from the tree
 * whose address is passed as the <code>tp</code> parameter.
 * The return value is the change in height of the tree, which
 * is either 0 if the height is unchanged or -1 if the height
 * decreases by one.  This value is then used to correct the
 * balance factors in ancestor nodes.
 */

static int removeTreeNode(BST bst, BSTNode *tp, void *kp) {
   BSTNode t;
   int sign, hDelta, bfDelta;

   t = *tp;
   if (t == NULL) return 0;
   bfDelta = 0;
   sign = bst->cmpFn(kp, &t->key);
   if (sign == 0) return removeTargetNode(bst, tp);
   if (sign < 0) {
      hDelta = removeTreeNode(bst, &t->left, kp);
      if (hDelta < 0) bfDelta = +1;
   } else {
      hDelta = removeTreeNode(bst, &t->right, kp);
      if (hDelta < 0) bfDelta = -1;
   }
   adjustBF(bst, tp, bfDelta);
   return ((bfDelta != 0 && t->bf == 0) ? -1 : 0);
}

/*
 * Implementation notes: removeTargetNode
 * --------------------------------------
 * Removes the node whose address is passed as the <code>tp</code>
 * parameter.  This method returns the change in height of the tree
 * rooted at that node, which is either 0 if the height is unchanged
 * or -1 if the height decreases by one.  This value is then used to
 * correct the balance factors in ancestor nodes.
 *
 * This implementation is divided into several cases.  The easy case
 * occurs when either of the children is <code>NULL</code>; in that case,
 * all you need to do is replace the node with its non-<code>NULL</code>
 * child.  If both children are non-<code>NULL</code>, this code
 * finds the rightmost descendent of the left child; this node
 * may not be a leaf, but will have no right child.  Its left
 * child replaces it in the tree, after which the replacement
 * data is moved to the position occupied by the target node.
 */

static int removeTargetNode(BST bst, BSTNode *tp) {
   BSTNode t, np;

   t = *tp;
   if (t->left == NULL) {
      *tp = t->right;
      freeNode(t);
      bst->count--;
      return -1;
   } else if (t->right == NULL) {
      *tp = t->left;
      freeNode(t);
      bst->count--;
      return -1;
   } else {
      np = t->left;
      while (np->right != NULL) {
         np = np->right;
      }
      t->key = np->key;
      t->value = np->value;
      if (removeTreeNode(bst, &t->left, &t->key) < 0) {
         adjustBF(bst, tp, +1);
         return (t->bf == 0) ? -1 : 0;
      } else {
         return 0;
      }
   }
}

/*
 * Implementation notes: freeTree and freeNode
 * -------------------------------------------
 * The freeTree function frees all nodes by conducting a postorder walk.
 * The freeNode function makes a special-case check to free the key string.
 */

static void freeTree(BSTNode t) {
   if (t != NULL) {
      freeTree(t->left);
      freeTree(t->right);
      freeNode(t);
   }
}

static void freeNode(BSTNode node) {
   freeBlock(node);
}

/*
 * Implementation notes: adjustBF
 * ------------------------------
 * Adjusts the balance factors in a node by the specified delta,
 * rebalancing the tree as needed.
 */

static void adjustBF(BST bst, BSTNode *tp, int delta) {
   BSTNode t;

   t = *tp;
   t->bf += delta;
   if (t->bf < -1) {
      fixLeftImbalance(bst, tp);
   } else if (t->bf > +1) {
      fixRightImbalance(bst, tp);
   }
}

/*
 * Implementation notes: fixLeftImbalance
 * --------------------------------------
 * Repairs the balance factor when a node has been found that
 * is out of balance with the longer subtree on the left.
 * Depending on the balance factor of the left child, the
 * code performs a single or double rotation.
 */

static void fixLeftImbalance(BST bst, BSTNode *tp) {
   BSTNode t, parent, child, *cp;
   int oldBF;

   parent = *tp;
   cp = &parent->left;
   child = *cp;
   if (child->bf == +1) {
      oldBF = child->right->bf;
      rotateLeft(bst, cp);
      rotateRight(bst, tp);
      t = *tp;
      t->bf = 0;
      switch (oldBF) {
        case -1: t->left->bf = 0; t->right->bf = +1; break;
        case  0: t->left->bf = t->right->bf = 0; break;
        case +1: t->left->bf = -1; t->right->bf = 0; break;
      }
   } else if (child->bf == 0) {
      rotateRight(bst, tp);
      t = *tp;
      t->bf = +1;
      t->right->bf = -1;
   } else {
      rotateRight(bst, tp);
      t = *tp;
      t->right->bf = t->bf = 0;
   }
}

/*
 * Implementation notes: fixRightImbalance
 * ---------------------------------------
 * Repairs the balance factor when a node has been found that
 * is out of balance with the longer subtree on the right.
 * Depending on the balance factor of the right child, the
 * code performs a single or double rotation.
 */

static void fixRightImbalance(BST bst, BSTNode *tp) {
   BSTNode t, parent, child, *cp;
   int oldBF;

   parent = *tp;
   cp = &parent->right;
   child = *cp;
   if (child->bf == -1) {
      oldBF = child->left->bf;
      rotateRight(bst, cp);
      rotateLeft(bst, tp);
      t = *tp;
      t->bf = 0;
      switch (oldBF) {
        case -1: t->left->bf = 0; t->right->bf = +1; break;
        case  0: t->left->bf = t->right->bf = 0; break;
        case +1: t->left->bf = -1; t->right->bf = 0; break;
      }
   } else if (child->bf == 0) {
      rotateLeft(bst, tp);
      t = *tp;
      t->bf = -1;
      t->left->bf = +1;
   } else {
      rotateLeft(bst, tp);
      t = *tp;
      t->left->bf = t->bf = 0;
   }
}

/*
 * Implementation notes: rotateLeft
 * --------------------------------
 * Performs a single left rotation of the tree whose address is
 * passed as an argument.  The balance factors are unchanged by this
 * function and must be corrected at a higher level of the algorithm.
 */

static void rotateLeft(BST bst, BSTNode *tp) {
   BSTNode parent, child;
   string key;

   parent = *tp;
   if (bst->debugLog != NULL) {
      key = bst->toStringFn(parent->key);
      fprintf(bst->debugLog, "rotateLeft around %s\n", key);
      freeBlock(key);
   }
   child = parent->right;
   parent->right = child->left;
   child->left = parent;
   *tp = child;
}

/*
 * Implementation notes: rotateRight
 * ---------------------------------
 * Performs a single right rotation of the tree whose address is
 * passed as an argument.  The balance factors are unchanged by this
 * function and must be corrected at a higher level of the algorithm.
 */

static void rotateRight(BST bst, BSTNode *tp) {
   BSTNode parent, child;
   string key;

   parent = *tp;
   if (bst->debugLog != NULL) {
      key = bst->toStringFn(parent->key);
      fprintf(bst->debugLog, "rotateRight around %s\n", key);
      freeBlock(key);
   }
   child = parent->left;
   parent->left = child->right;
   child->right = parent;
   *tp = child;
}

/*
 * Implementation notes: mapTree
 * -----------------------------
 * Implements a recursive walk over the tree.
 */

static void mapTree(BSTNode t, proc fn, TraversalOrder order, void *data) {
   if (t != NULL) {
      if (order == PREORDER) fn(t, data);
      mapTree(t->left, fn, order, data);
      if (order == INORDER) fn(t, data);
      mapTree(t->right, fn, order, data);
      if (order == POSTORDER) fn(t, data);
   }
}

/*
 * Implementation notes: addNodeToIterator
 * ---------------------------------------
 * Adds the specified node to the iterator, which is passed as the
 * <code>data</code> parameter.
 */

static void addNodeToIterator(BSTNode node, void *data) {
   addToIteratorList((Iterator) data, &node);
}

/*
 * Implementation notes: newForeachIterator
 * ----------------------------------------
 * The foreach iterator always uses an INORDER walk.
 */

static Iterator newForeachIterator(void *collection) {
   return newNodeIterator((BST) collection, INORDER);
}

/**********************************************************************/
/* Unit test for the bst module                                       */
/**********************************************************************/

#ifndef _NOTEST_

/* Constant tables */

static string ELEMENTS[] = {
   "H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al",
   "Si", "P", "S", "Cl", "Ar", "K", "Ca", "Sc", "Ti", "V", "Cr", "Mn",
   "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb",
   "Sr", "Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In",
   "Sn", "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd", "Pm",
   "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Hf", "Ta",
   "W", "Re", "Os", "Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi", "Po", "At",
   "Rn", "Fr", "Ra", "Ac", "Th", "Pa", "U", "Np", "Pu", "Am", "Cm", "Bk",
   "Cf", "Es", "Fm", "Md", "No", "Lr", "Rf", "Db", "Sg", "Bh", "Hs", "Mt"
};
static int N_ELEMENTS = sizeof ELEMENTS / sizeof ELEMENTS[0];

static string ELEMENTS_PREORDER[] = {
   "P", "H", "Cl", "Be", "Ar", "Ag", "Ac", "Al", "Am", "B", "At", "As",
   "Au", "Ba", "C", "Bk", "Bi", "Bh", "Br", "Cd", "Ca", "Ce", "Cf", "F",
   "Cu", "Cr", "Co", "Cm", "Cs", "Er", "Dy", "Db", "Eu", "Es", "Ga", "Fm",
   "Fe", "Fr", "Ge", "Gd", "Li", "I", "Hf", "He", "Ho", "Hg", "Hs", "K",
   "In", "Ir", "Kr", "La", "N", "Md", "Lu", "Lr", "Mn", "Mg", "Mo", "Mt",
   "Ne", "Nb", "Na", "Nd", "O", "No", "Ni", "Np", "Os", "Sc", "Rb", "Pm",
   "Pb", "Pa", "Pd", "Pr", "Po", "Pu", "Pt", "Ra", "Ru", "Rh", "Re", "Rf",
   "Rn", "S", "Sb", "Ti", "Sr", "Si", "Se", "Sg", "Sn", "Sm", "Tc", "Tb",
   "Ta", "Te", "Th", "Y", "V", "Tm", "Tl", "U", "Xe", "W", "Zn", "Yb", "Zr"
};

static string ELEMENTS_INORDER[] = {
   "Ac", "Ag", "Al", "Am", "Ar", "As", "At", "Au", "B", "Ba", "Be", "Bh",
   "Bi", "Bk", "Br", "C", "Ca", "Cd", "Ce", "Cf", "Cl", "Cm", "Co", "Cr",
   "Cs", "Cu", "Db", "Dy", "Er", "Es", "Eu", "F", "Fe", "Fm", "Fr", "Ga",
   "Gd", "Ge", "H", "He", "Hf", "Hg", "Ho", "Hs", "I", "In", "Ir", "K",
   "Kr", "La", "Li", "Lr", "Lu", "Md", "Mg", "Mn", "Mo", "Mt", "N", "Na",
   "Nb", "Nd", "Ne", "Ni", "No", "Np", "O", "Os", "P", "Pa", "Pb", "Pd",
   "Pm", "Po", "Pr", "Pt", "Pu", "Ra", "Rb", "Re", "Rf", "Rh", "Rn", "Ru",
   "S", "Sb", "Sc", "Se", "Sg", "Si", "Sm", "Sn", "Sr", "Ta", "Tb", "Tc",
   "Te", "Th", "Ti", "Tl", "Tm", "U", "V", "W", "Xe", "Y", "Yb", "Zn", "Zr"
};

static string ELEMENTS_POSTORDER[] = {
   "Ac", "Am", "Al", "Ag", "As", "Au", "At", "Ba", "B", "Ar", "Bh", "Bi",
   "Br", "Bk", "Ca", "Cf", "Ce", "Cd", "C", "Be", "Cm", "Co", "Cs", "Cr",
   "Db", "Dy", "Es", "Eu", "Er", "Cu", "Fe", "Fr", "Fm", "Gd", "Ge", "Ga",
   "F", "Cl", "He", "Hg", "Hs", "Ho", "Hf", "Ir", "In", "La", "Kr", "K",
   "I", "Lr", "Lu", "Mg", "Mt", "Mo", "Mn", "Md", "Na", "Nd", "Nb", "Ni",
   "Np", "No", "Os", "O", "Ne", "N", "Li", "H", "Pa", "Pd", "Pb", "Po",
   "Pt", "Ra", "Pu", "Pr", "Pm", "Rf", "Re", "Rn", "Rh", "Sb", "S", "Ru",
   "Rb", "Sg", "Se", "Sm", "Sn", "Si", "Ta", "Tb", "Th", "Te", "Tc", "Sr",
   "Tl", "U", "Tm", "W", "Xe", "V", "Yb", "Zr", "Zn", "Y", "Ti", "Sc", "P"
};

static int PRIMES[] = {
     2,   3,   5,   7,  11,  13,  17,  19,  23,  29,  31,  37,  41,
    43,  47,  53,  59,  61,  67,  71,  73,  79,  83,  89,  97, 101,
   103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167,
   173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239,
   241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313,
   317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397,
   401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467,
   479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557, 563, 569,
   571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643,
   647, 653, 659, 661, 673, 677, 683, 691, 701, 709, 719, 727, 733,
   739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 811, 821, 823,
   827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911,
   919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997
};
static int N_PRIMES = sizeof PRIMES / sizeof PRIMES[0];

static int PRIMES_PREORDER[] = {
   311, 131,  53,  19,   7,   3,   2,   5,  13,  11,  17,  37,  29,
    23,  31,  43,  41,  47,  89,  71,  61,  59,  67,  79,  73,  83,
   107, 101,  97, 103, 113, 109, 127, 223, 173, 151, 139, 137, 149,
   163, 157, 167, 193, 181, 179, 191, 199, 197, 211, 263, 239, 229,
   227, 233, 251, 241, 257, 281, 271, 269, 277, 293, 283, 307, 719,
   503, 409, 359, 337, 317, 313, 331, 349, 347, 353, 383, 373, 367,
   379, 397, 389, 401, 457, 433, 421, 419, 431, 443, 439, 449, 479,
   463, 461, 467, 491, 487, 499, 613, 569, 541, 521, 509, 523, 557,
   547, 563, 593, 577, 571, 587, 601, 599, 607, 659, 641, 619, 617,
   631, 647, 643, 653, 683, 673, 661, 677, 701, 691, 709, 827, 769,
   743, 733, 727, 739, 757, 751, 761, 809, 787, 773, 797, 821, 811,
   823, 941, 881, 857, 839, 829, 853, 863, 859, 877, 911, 887, 883,
   907, 929, 919, 937, 971, 953, 947, 967, 983, 977, 991, 997
};

/* Private function prototypes */

static void testStringBST(void);
static void testIntBST(void);
static void insertArray(BST bst, void *array, int n);
static void checkIterator(BST bst, TraversalOrder order, void *array);
static void checkOrdered(BST bst);
static void checkBalanceFactors(BST bst);
static void checkNodeBalance(BST bst, BSTNode t);
static int treeHeight(BSTNode t);
static string bfToString(int bf);

/* Unit test */

void testBSTModule(void) {
   testStringBST();
   testIntBST();
}

/* Private functions */

static void testStringBST(void) {
   BST bst, bst2;
   string symbol;

   trace(bst = newBST(string));
   trace(insertArray(bst, ELEMENTS, N_ELEMENTS));
   trace(checkIterator(bst, PREORDER, ELEMENTS_PREORDER));
   trace(checkIterator(bst, INORDER, ELEMENTS_INORDER));
   trace(checkIterator(bst, POSTORDER, ELEMENTS_POSTORDER));
   trace(checkBalanceFactors(bst));
   trace(bst2 = cloneBST(bst));
   trace(removeBSTNode(bst, "Sc"));           /* Rotates right around Ru */
   trace(removeBSTNode(bst, "He"));           /* Rotates left around Hf  */
   trace(removeBSTNode(bst, "I"));            /* Double around Hf, Ho    */
   trace(checkBalanceFactors(bst));
   trace(checkOrdered(bst));
   trace(checkIterator(bst2, INORDER, ELEMENTS_INORDER));
}

static void testIntBST(void) {
   BST bst, bst2;
   int value;

   trace(bst = newBST(int));
   trace(insertArray(bst, PRIMES, N_PRIMES));
   trace(checkIterator(bst, INORDER, PRIMES));
   trace(checkIterator(bst, PREORDER, PRIMES_PREORDER));
   trace(checkOrdered(bst));
   trace(checkBalanceFactors(bst));
   trace(bst2 = cloneBST(bst));
   trace(removeBSTNode(bst, 977));            /* Rotates left around 983 */
   trace(removeBSTNode(bst, 311));            /* Deletes the root        */
   trace(checkOrdered(bst));
   trace(checkBalanceFactors(bst));
   trace(checkIterator(bst2, INORDER, PRIMES));
}

static void insertArray(BST bst, void *array, int n) {
   char *cptr;
   string typeName;
   GenericType key;
   int size, i;

   cptr = (char *) array;
   typeName = getBaseTypeBST(bst);
   size = getTypeSizeForType(typeName);
   for (i = 0; i < n; i++) {
      memcpy(&key, cptr, size);
      insertBSTNode(bst, key);
      cptr += size;
   }
}

static void checkIterator(BST bst, TraversalOrder order, void *array) {
   char *cptr;
   CompareFn cmpFn;
   ToStringFn toStringFn;
   BSTNode node;
   int size;

   cptr = (char *) array;
   cmpFn = getCompareFnBST(bst);
   toStringFn = getToStringFn(getBaseTypeBST(bst));
   size = getTypeSizeForType(getBaseTypeBST(bst));
   foreach (node in newNodeIterator(bst, order)) {
      if (cmpFn(&node->key, cptr) != 0) {
         reportError("Incorrect key: %s", toStringFn(node->key));
      }
      cptr += size;
   }
}

static void checkOrdered(BST bst) {
   CompareFn cmpFn;
   BSTNode prev, node;
   GenericType key1, key2;
   int size;

   size = getTypeSizeForType(getBaseTypeBST(bst));
   cmpFn = getCompareFnBST(bst);
   prev = NULL;
   foreach (node in newNodeIterator(bst, INORDER)) {
      if (prev != NULL) {
         key1 = prev->key;
         key2 = node->key;
         if (cmpFn(&key1, &key2) >= 0) {
            reportError("%s >= %s", getKeyString(prev), getKeyString(node));
         }
      }
      prev = node;
   }
}

static void checkBalanceFactors(BST bst) {
   BSTNode node;

   foreach (node in newNodeIterator(bst, INORDER)) {
      checkNodeBalance(bst, node);
   }
}

static void checkNodeBalance(BST bst, BSTNode t) {
   int actualBF, storedBF;

   actualBF = treeHeight(getRightChild(t)) - treeHeight(getLeftChild(t));
   storedBF = getBF(t);
   if (abs(actualBF) > 1) {
      reportError("Unbalanced tree at %s (%s)", getKeyString(t),
                  bfToString(actualBF));
   }
   if (storedBF != actualBF) {
      reportError("bf incorrect for %s (%s should be %s)", getKeyString(t),
                  bfToString(storedBF), bfToString(actualBF));
   }
}

static int treeHeight(BSTNode t) {
   int lh, rh;

   if (t == NULL) return 0;
   lh = treeHeight(t->left);
   rh = treeHeight(t->right);
   return (lh > rh) ? lh + 1 : rh + 1;
}

static string bfToString(int bf) {
   if (bf > 0) return concat("+", integerToString(bf));
   return integerToString(bf);
}

#endif
