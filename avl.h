#ifndef avl_h
#define avl_h

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum BalanceType { Equal=0, LeftHigh=-1, RightHigh=1 } BalanceType;

struct AVLNode {
	struct AVLNode *parent, *left, *right;
	BalanceType balance;
};
typedef struct AVLNode AVLNode;

typedef struct AVLAllocator {
	void *userArg;

	void *(*alloc)(void *userArg, size_t size);

	void (*release)(void *userArg, void *ptr);
} AVLAllocator;

typedef struct AVLTree {
	AVLNode *root;
	AVLAllocator allocator;
} AVLTree;

AVLNode *AVLTree_FindFirst(AVLTree *tree);

AVLNode *AVLTree_FindLast(AVLTree *tree);

AVLNode *AVLTree_FindNext(AVLTree *tree, AVLNode *current);

AVLNode *AVLTree_FindPrevious(AVLTree *tree, AVLNode *current);

AVLNode *AVL_RebalanceHeavierLeft(AVLNode *self, int *height_changed);

AVLNode *AVL_RebalanceHeavierRight(AVLNode *self, int *height_changed);

AVLNode *AVL_RebalanceShorterLeft(AVLNode *self, int *height_changed);

AVLNode *AVL_RebalanceShorterRight(AVLNode *self, int *height_changed);

int AVLTree_Height(AVLTree *tree);

/* set right child */
static inline void AVL_SetRight(AVLNode *self, AVLNode *r)
{
	self->right = r;
	if (r)
		r->parent = self;
}

/* set left child */
static inline void AVL_SetLeft(AVLNode *self, AVLNode *l)
{
	self->left = l;
	if (l)
		l->parent = self;
}

#define AVLTREE(V)                                                                                                     \
	typedef int (*CompFunc_##V)(V a, V b);                                                                         \
	typedef struct AVLNode_##V AVLNode_##V;                                                                        \
	typedef struct AVLTree_##V {                                                                                   \
		AVLNode_##V *root;                                                                                     \
		AVLAllocator allocator;                                                                                \
		CompFunc_##V comp;                                                                                     \
	} AVLTree_##V;                                                                                                 \
	struct AVLNode_##V {                                                                                           \
		struct AVLNode_##V *parent, *left, *right;                                                             \
		BalanceType balance;                                                                                   \
		V value;                                                                                               \
	};                                                                                                             \
	static int AVLTree_DeleteObject_##V(AVLTree_##V *tree, AVLNode_##V *n);                                        \
                                                                                                                       \
	static AVLTree_##V *AVLTree_New_##V(AVLAllocator allocator, CompFunc_##V compFunc)                             \
	{                                                                                                              \
		AVLTree_##V *tree = allocator.alloc(allocator.userArg, sizeof(AVLTree_##V));                           \
		if (tree != NULL) {                                                                                    \
			tree->root = NULL;                                                                             \
			tree->allocator = allocator;                                                                   \
			tree->comp = compFunc;                                                                         \
		}                                                                                                      \
		return tree;                                                                                           \
	}                                                                                                              \
                                                                                                                       \
	static AVLNode_##V *AVL_new_##V(AVLTree_##V *tree, V key)                                                      \
	{                                                                                                              \
		AVLNode_##V *n = (AVLNode_##V *)tree->allocator.alloc(tree->allocator.userArg, sizeof(AVLNode_##V));   \
                                                                                                                       \
		if (n == NULL) {                                                                                       \
			fprintf(stderr, "Error [AVL_new]: Out of memory\n");                                           \
			exit(-1);                                                                                      \
		}                                                                                                      \
		n->left = n->right = n->parent = NULL;                                                                 \
		n->balance = Equal;                                                                                    \
		n->value = key;                                                                                        \
		return n;                                                                                              \
	}                                                                                                              \
                                                                                                                       \
	/* insert a key into an AVL tree and return new object inserted */                                             \
	static void AVLTree_Insert_##V(AVLTree_##V *tree, V k)                                                         \
	{                                                                                                              \
		AVLNode_##V *root = tree->root;                                                                        \
		AVLNode_##V *found = NULL;                                                                             \
                                                                                                                       \
		/* is the tree empty ? */                                                                              \
		if (root == NULL) {                                                                                    \
			found = root = AVL_new_##V(tree, k);                                                           \
		} else {                                                                                               \
			/* find insertion point */                                                                     \
			int height_changed = 0;                                                                        \
			AVLNode_##V *p = NULL, *n = NULL;                                                              \
                                                                                                                       \
			n = root;                                                                                      \
			do {                                                                                           \
				int compresult = 0;                                                                    \
                                                                                                                       \
				p = n;                                                                                 \
				compresult = tree->comp(k, n->value);                                                  \
				if (compresult < 0)                                                                    \
					n = (AVLNode_##V *)p->left;                                                    \
				else if (compresult > 0)                                                               \
					n = (AVLNode_##V *)p->right;                                                   \
				else                                                                                   \
					return;                                                                        \
			} while (n != 0);                                                                              \
			found = n = AVL_new_##V(tree, k);                                                              \
                                                                                                                       \
			/* insert */                                                                                   \
			if (tree->comp(k, p->value) < 0)                                                               \
				AVL_SetLeft((AVLNode *)p, (AVLNode *)n);                                               \
			else                                                                                           \
				AVL_SetRight((AVLNode *)p, (AVLNode *)n);                                              \
                                                                                                                       \
			/* rebalance */                                                                                \
			height_changed = 1;                                                                            \
			while (p != 0 && height_changed) {                                                             \
				if (n == p->left)                                                                      \
					p = (AVLNode_##V *)AVL_RebalanceHeavierLeft((AVLNode *)p, &height_changed);    \
				else                                                                                   \
					p = (AVLNode_##V *)AVL_RebalanceHeavierRight((AVLNode *)p, &height_changed);   \
				n = p;                                                                                 \
				p = (AVLNode_##V *)p->parent;                                                          \
			}                                                                                              \
			if (p == 0)                                                                                    \
				root = n;                                                                              \
		}                                                                                                      \
		tree->root = root;                                                                                     \
	}                                                                                                              \
                                                                                                                       \
	/* delete a key from an AVL tree */                                                                            \
	static int AVLTree_Delete_##V(AVLTree_##V *tree, V k)                                                          \
	{                                                                                                              \
		AVLNode_##V *root = tree->root;                                                                        \
		AVLNode_##V *n = NULL;                                                                                 \
                                                                                                                       \
		if (root == NULL)                                                                                      \
			return -1;                                                                                     \
                                                                                                                       \
		/* search */                                                                                           \
		n = root;                                                                                              \
		while (n != NULL) {                                                                                    \
			int compresult = tree->comp(k, n->value);                                                      \
			if (compresult < 0)                                                                            \
				n = (AVLNode_##V *)n->left;                                                            \
			else if (compresult > 0)                                                                       \
				n = (AVLNode_##V *)n->right;                                                           \
			else                                                                                           \
				break;                                                                                 \
		}                                                                                                      \
		if (n == NULL)                                                                                         \
			return -1;                                                                                     \
		AVLTree_DeleteObject_##V(tree, n);                                                                     \
		return 0;                                                                                              \
	}                                                                                                              \
                                                                                                                       \
	/* delete an object from an AVL tree */                                                                        \
	static int AVLTree_DeleteObject_##V(AVLTree_##V *tree, AVLNode_##V *n)                                         \
	{                                                                                                              \
		AVLNode_##V *root = tree->root;                                                                        \
		AVLNode_##V *discard = NULL, *p = NULL;                                                                \
		int height_changed = 0;                                                                                \
		int flag = 0;                                                                                          \
                                                                                                                       \
		if (n == NULL)                                                                                         \
			return -1;                                                                                     \
                                                                                                                       \
		if (n->left != NULL) {                                                                                 \
			/* find predecessor */                                                                         \
			p = n;                                                                                         \
			for (n = (AVLNode_##V *)n->left; n->right != NULL; n = (AVLNode_##V *)n->right)                \
				;                                                                                      \
			p->value = n->value;                                                                           \
                                                                                                                       \
			p = (AVLNode_##V *)n->parent;                                                                  \
			if (n == (AVLNode_##V *)p->left) {                                                             \
				flag = 1;                                                                              \
				AVL_SetLeft((AVLNode *)p, (AVLNode *)n->left);                                         \
			} else                                                                                         \
				AVL_SetRight((AVLNode *)p, (AVLNode *)n->left);                                        \
			discard = n;                                                                                   \
		} else {                                                                                               \
			/* no predecessor, left subtree is guaranteed to be NULL */                                    \
			p = (AVLNode_##V *)n->parent;                                                                  \
			if (p != NULL) {                                                                               \
				if (n == (AVLNode_##V *)p->left) {                                                     \
					AVL_SetLeft((AVLNode *)p, (AVLNode *)n->right);                                \
					flag = 1;                                                                      \
				} else                                                                                 \
					AVL_SetRight((AVLNode *)p, (AVLNode *)n->right);                               \
				discard = n;                                                                           \
			} else {                                                                                       \
				/* root is being deleted */                                                            \
				p = (AVLNode_##V *)n->right;                                                           \
				if (p != NULL)                                                                         \
					p->parent = NULL;                                                              \
				tree->allocator.release(tree->allocator.userArg, n);                                   \
				tree->root = p;                                                                        \
				return 0;                                                                              \
			}                                                                                              \
		}                                                                                                      \
                                                                                                                       \
		/* rebalance */                                                                                        \
		height_changed = 1;                                                                                    \
		while (p != NULL && height_changed) {                                                                  \
			if (flag || n == (AVLNode_##V *)p->left) {                                                     \
				flag = 0;                                                                              \
				p = (AVLNode_##V *)AVL_RebalanceShorterLeft((AVLNode *)p, &height_changed);            \
			} else                                                                                         \
				p = (AVLNode_##V *)AVL_RebalanceShorterRight((AVLNode *)p, &height_changed);           \
			n = p;                                                                                         \
			p = (AVLNode_##V *)p->parent;                                                                  \
		}                                                                                                      \
		if (p == NULL)                                                                                         \
			tree->root = n;                                                                                \
		tree->allocator.release(tree->allocator.userArg, discard);                                             \
		return 0;                                                                                              \
	}                                                                                                              \
                                                                                                                       \
	/* search for an object */                                                                                     \
	static AVLNode_##V *AVLTree_Search_##V(AVLTree_##V *tree, V k)                                                 \
	{                                                                                                              \
		AVLNode_##V *root = tree->root;                                                                        \
                                                                                                                       \
		if (root) {                                                                                            \
			AVLNode_##V *current = root;                                                                   \
                                                                                                                       \
			while (current) {                                                                              \
				int compresult = tree->comp(k, current->value);                                        \
				if (compresult == 0)                                                                   \
					return current;                                                                \
				else if (compresult < 0)                                                               \
					current = (AVLNode_##V *)current->left;                                        \
				else                                                                                   \
					current = (AVLNode_##V *)current->right;                                       \
			}                                                                                              \
		}                                                                                                      \
		return NULL;                                                                                           \
	}                                                                                                              \
	static AVLNode_##V *AVLTree_FindFirst_##V(AVLTree_##V *tree)                                                   \
	{                                                                                                              \
		return (AVLNode_##V *)AVLTree_FindFirst((AVLTree *)tree);                                              \
	}                                                                                                              \
                                                                                                                       \
	static AVLNode_##V *AVLTree_FindLast_##V(AVLTree_##V *tree)                                                    \
	{                                                                                                              \
		return (AVLNode_##V *)AVLTree_FindLast((AVLTree *)tree);                                               \
	}                                                                                                              \
                                                                                                                       \
	static AVLNode_##V *AVLTree_FindNext_##V(AVLTree_##V *tree, AVLNode_##V *current)                              \
	{                                                                                                              \
		return (AVLNode_##V *)AVLTree_FindNext((AVLTree *)tree, (AVLNode *)current);                           \
	}                                                                                                              \
                                                                                                                       \
	static AVLNode_##V *AVLTree_FindPrevious_##V(AVLTree_##V *tree, AVLNode_##V *current)                          \
	{                                                                                                              \
		return (AVLNode_##V *)AVLTree_FindPrevious((AVLTree *)tree, (AVLNode *)current);                       \
	}

#endif
