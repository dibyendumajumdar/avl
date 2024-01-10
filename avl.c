#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "avl.h"

int AVL_Height(AVLNode *self);

//void AVLTree_BackwardApply(AVLNode *root, void (*funcptr)(ValueType));
//
//void AVLTree_ForwardApply(AVLNode *root, void (*funcptr)(ValueType));

void AVLTree_Destroy(AVLTree *tree)
{
	//	if (tree->vptr->destroyobject != NULL) {
	//		AVLTree_ForwardApply((struct AVLNode_st *)tree->root, tree->vptr->destroyobject);
	//	}
	//	ys_destroy_allocator(tree->a);
	//	free(tree);             /* jul 6 1996 */
}

/* rotate sub-tree left, return new root (do not change balance) */
AVLNode *AVL_RotateLeft(AVLNode *self)
{
	AVLNode *newroot = self->right;
	if (self->parent != 0) {
		if (self == self->parent->left)
			AVL_SetLeft(self->parent, newroot);
		else
			AVL_SetRight(self->parent, newroot);
	} else
		newroot->parent = 0;
	AVL_SetRight(self, newroot->left);
	AVL_SetLeft(newroot, self);
	return newroot;
}

/* rotate sub-tree right, return new root (do not change balance) */
AVLNode *AVL_RotateRight(AVLNode *self)
{
	AVLNode *newroot = self->left;
	if (self->parent != 0) {
		if (self == self->parent->left)
			AVL_SetLeft(self->parent, newroot);
		else
			AVL_SetRight(self->parent, newroot);
	} else
		newroot->parent = 0;
	AVL_SetLeft(self, newroot->right);
	AVL_SetRight(newroot, self);
	return newroot;
}

/* doubly rotate sub-tree and return new root (change balance as needed) */
AVLNode *AVL_DoubleRotateRight(AVLNode *self)
{
	AVLNode *lf = self->left;
	AVLNode *rt = lf->right;
	AVLNode *root;

	switch (rt->balance) {
	case RightHigh:
		self->balance = Equal;
		lf->balance = LeftHigh;
		break;

	case Equal:
		self->balance = lf->balance = Equal;
		break;

	case LeftHigh:
		self->balance = RightHigh;
		lf->balance = Equal;
		break;
	}
	rt->balance = Equal;
	AVL_SetLeft(self, AVL_RotateLeft(lf));
	root = AVL_RotateRight(self);
	return root;
}

/* doubly rotate sub-tree and return new root (change balance as needed) */
AVLNode *AVL_DoubleRotateLeft(AVLNode *self)
{
	AVLNode *rt = self->right;
	AVLNode *lf = rt->left;
	AVLNode *root;

	switch (lf->balance) {
	case RightHigh:
		self->balance = LeftHigh;
		rt->balance = Equal;
		break;

	case Equal:
		self->balance = rt->balance = Equal;
		break;

	case LeftHigh:
		self->balance = Equal;
		rt->balance = RightHigh;
		break;
	}
	lf->balance = Equal;
	AVL_SetRight(self, AVL_RotateRight(rt));
	root = AVL_RotateLeft(self);
	return root;
}

/*
 * Rebalance sub-tree whose left branch has become heavier because of
 * insertion
 */
AVLNode *AVL_RebalanceHeavierLeft(AVLNode *self, int *height_changed)
{
	AVLNode *root = self;

	/* left-subtree has grown by one level */
	switch (root->balance) {
	case RightHigh:
		root->balance = Equal;
		*height_changed = 0;
		break;

	case Equal:
		root->balance = LeftHigh;
		break;

	case LeftHigh: {
		AVLNode *lf = self->left;
		switch (lf->balance) {
		case LeftHigh:
			self->balance = Equal;
			lf->balance = Equal;
			root = AVL_RotateRight(self);
			break;

		case RightHigh:
			root = AVL_DoubleRotateRight(self);
			break;

		default:
			break;
		}
		*height_changed = 0;
		break;
	}
	}
	return root;
}

/*
 * Rebalance sub-tree whose right branch has become heavier because of
 * insertion
 */
AVLNode *AVL_RebalanceHeavierRight(AVLNode *self, int *height_changed)
{
	AVLNode *root = self;
	/* right-subtree has grown by one level */
	switch (root->balance) {
	case LeftHigh:
		root->balance = Equal;
		*height_changed = 0;
		break;

	case Equal:
		root->balance = RightHigh;
		break;

	case RightHigh: {
		AVLNode *rt = self->right;
		switch (rt->balance) {
		case RightHigh:
			self->balance = Equal;
			rt->balance = Equal;
			root = AVL_RotateLeft(self);
			break;

		case LeftHigh:
			root = AVL_DoubleRotateLeft(self);
			break;

		default:
			break;
		}
		*height_changed = 0;
		break;
	}
	}
	return root;
}

/* Rebalance sub-tree whose left branch has become short because of deletion */
AVLNode *AVL_RebalanceShorterLeft(AVLNode *self, int *height_changed)
{
	AVLNode *p = self;
	switch (p->balance) {
	case Equal:
		p->balance = RightHigh;
		*height_changed = 0;
		break;

	case LeftHigh:
		/* taller (left) subtree was shortened */
		p->balance = Equal;
		break;

	case RightHigh: {
		/*
		 * shorter (left) subtree was shortened, violating
		 * AVL rules
		 */
		AVLNode *q;
		q = p->right;

		switch (q->balance) {
		case Equal:
			*height_changed = 0;
			q->balance = LeftHigh;
			p = AVL_RotateLeft(p);
			break;

		case RightHigh:
			p->balance = q->balance = Equal;
			p = AVL_RotateLeft(p);
			break;

		case LeftHigh:
			p = AVL_DoubleRotateLeft(p);
			break;
		}
		break;
	}
	}
	return p;
}

/*
 * rebalance a sub-tree whose right branch has been reduced by one level
 * because of deletion
 */
AVLNode *AVL_RebalanceShorterRight(AVLNode *self, int *height_changed)
{
	AVLNode *p = self;
	switch (p->balance) {
	case Equal:
		p->balance = LeftHigh;
		*height_changed = 0;
		break;

	case RightHigh:
		/* taller (right) subtree was shortened */
		p->balance = Equal;
		break;

	case LeftHigh: {
		/*
		 * shorter (right) sub-tree was shortened, violating
		 * AVL rules
		 */
		AVLNode *q;
		q = p->left;

		switch (q->balance) {
		case Equal:
			*height_changed = 0;
			q->balance = RightHigh;
			p = AVL_RotateRight(p);
			break;

		case LeftHigh:
			p->balance = q->balance = Equal;
			p = AVL_RotateRight(p);
			break;

		case RightHigh:
			p = AVL_DoubleRotateRight(p);
			break;
		}
		break;
	}
	}
	return p;
}

int AVL_Height(AVLNode *self)
{
	int height = 0;
	int lf_height = 0, rt_height = 0;

	if (self == 0)
		return 0;
	if (self->left)
		lf_height = AVL_Height(self->left);
	if (self->right)
		rt_height = AVL_Height(self->right);
	if (lf_height > rt_height)
		height += lf_height + 1;
	else
		height += rt_height + 1;
	return height;
}

/* find the first object in the tree */
AVLNode *AVLTree_FindFirst(AVLTree *tree)
{
	void *root = tree->root;
	if (root) {
		AVLNode *current = root;
		while (current->left != NULL)
			current = current->left;
		return current;
	}
	return NULL;
}

/* find the last object in the tree */
AVLNode *AVLTree_FindLast(AVLTree *tree)
{
	void *root = tree->root;
	if (root) {
		AVLNode *current = root;
		while (current->right != NULL)
			current = current->right;
		return current;
	}
	return NULL;
}

/* find the next object in the tree */
AVLNode *AVLTree_FindNext(AVLTree *tree, AVLNode *current)
{
	if (current) {
		if (current->right) {
			current = current->right;
			while (current->left != NULL)
				current = current->left;
		} else {
			AVLNode *p = current;
			current = p->parent;
			while (current) {
				if (current->right == p) {
					p = current;
					current = current->parent;
				} else
					break;
			}
		}
	}
	if (current)
		return current;
	else
		return NULL;
}

/* find the previous object in the tree */
AVLNode *AVLTree_FindPrevious(AVLTree *tree, AVLNode *current)
{
	if (current) {
		if (current->left) {
			current = current->left;
			while (current->right != NULL)
				current = current->right;
		} else {
			AVLNode *p = current;
			current = p->parent;
			while (current) {
				if (current->left == p) {
					p = current;
					current = current->parent;
				} else
					break;
			}
		}
	}
	if (current)
		return current;
	else
		return NULL;
}

int AVLTree_Height(AVLTree *tree) { return AVL_Height((AVLNode *)tree->root); }

// void
// AVLTree_BackwardApply(AVLNode *root, void (*funcptr)(ValueType v)) {
//    if (root == NULL)
//       return;
//    AVLTree_BackwardApply(root->right, funcptr);
//    (*funcptr)(root->value);
//    AVLTree_BackwardApply(root->left, funcptr);
// }

// void
// AVLTree_Backeach(AVLTree *tree, void (*funcptr)(ValueType)) {
//    AVLTree_BackwardApply((AVLNode *) tree->root, funcptr);
// }

// void
// AVLTree_ForwardApply(AVLNode *root, void (*funcptr)(ValueType)) {
//    if (root == NULL)
//       return;
//    AVLTree_ForwardApply(root->left, funcptr);
//    (*funcptr)(root->value);
//    AVLTree_ForwardApply(root->right, funcptr);
// }

// void
// AVLTree_Foreach(AVLTree *tree, void (*funcptr)(ValueType)) {
//    AVLTree_ForwardApply((AVLNode *) tree->root, funcptr);
// }
