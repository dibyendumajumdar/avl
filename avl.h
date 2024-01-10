#ifndef avl_h
#define avl_h

#include <stdint.h>

typedef int64_t ValueType;

typedef enum BalanceType {
   Equal, LeftHigh, RightHigh
} BalanceType;

struct AVLNode {
   struct AVLNode *parent, *left, *right;
   BalanceType balance;
   ValueType value;
};
typedef struct AVLNode AVLNode;

typedef int (*CompFunc)(ValueType a, ValueType b);

typedef struct AVLAllocator {
   void *userArg;

   void *(*alloc)(void *userArg, size_t size);

   void (*release)(void *userArg, void *ptr);
} AVLAllocator;

typedef struct AVLTree {
   AVLNode *root;
   int n;
   CompFunc comp;
   AVLAllocator allocator;
} AVLTree;

AVLTree *AVLTree_New(AVLAllocator allocator, CompFunc compFunc);

AVLNode *AVLTree_Search(AVLTree *tree, ValueType key);

void AVLTree_Insert(AVLTree *tree, ValueType key);

int AVLTree_Delete(AVLTree *tree, ValueType key);

int AVLTree_DeleteObject(AVLTree *tree, AVLNode *object);

AVLNode *AVLTree_FindFirst(AVLTree *tree);

AVLNode *AVLTree_FindLast(AVLTree *tree);

AVLNode *AVLTree_FindNext(AVLTree *tree, AVLNode *current);

AVLNode *AVLTree_FindPrevious(AVLTree *tree, AVLNode *current);

int AVLTree_Height(AVLTree *tree);

void AVLTree_Foreach(AVLTree *tree, void (*funcptr)(ValueType v));

void AVLTree_Backeach(AVLTree *tree, void (*funcptr)(ValueType v));


#endif

