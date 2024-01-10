// --- avltree.cpp (version 5.0)
// --- started october 04 1994
// --- finished october 05 1994 at 11:30 am
// --- doggy

#include "avltree5.h"
#include <iostream.h>

// #define DO_TRACE
#ifdef DO_TRACE
#define TRACE(x)    x
#define NO_TRACE(x)
#else
#define TRACE(x)
#define NO_TRACE(x) x
#endif

// --- rotate sub-tree left, return new root (do not change balance)
AVLNode * AVLNode::RotateLeft()
{
    AVLNode *newroot = right ;
    if (parent)
    {
        AVLNode *p = parent ;
        if (this == p->left)
            p->SetLeft(newroot) ;
        else
            p->SetRight(newroot) ;
    }
    else
        newroot->parent = 0 ;
    SetRight(newroot->left);
    newroot->SetLeft(this);
    return newroot ;
}

// --- rotate sub-tree right, return new root (do not change balance)
AVLNode * AVLNode::RotateRight()
{
    AVLNode *newroot = left ;
    if (parent)
    {
        AVLNode *p = parent ;
        if (this == p->left)
            p->SetLeft(newroot) ;
        else
            p->SetRight(newroot) ;
    }
    else
        newroot->parent = 0 ;
    SetLeft(newroot->right);
    newroot->SetRight(this);
    return newroot ;
}

// --- doubly rotate sub-tree and return new root (change balance as needed)
AVLNode * AVLNode::DoubleRotateRight()
{
    AVLNode *lf = left ;
    AVLNode *rt = lf->right ;

    switch (rt->balance)
    {
        case RightHigh:
            TRACE( cout << "Setting balance of " << info << " to Equal and\n"; )
            TRACE( cout << "balance of " << lf->info << " to LeftHigh and\n"; )
            balance = Equal ;
            lf->balance = LeftHigh ;
            break ;

        case Equal:
            TRACE( cout << "Setting balance of " << info << " and " << lf->info << " to Equal and\n"; )
            balance = lf->balance = Equal ;
            break ;

        case LeftHigh:
            TRACE( cout << "Setting balance of " << info << " to RightHigh and\n"; )
            TRACE( cout << "balance of " << lf->info << " to Equal and\n"; )
            balance = RightHigh ;
            lf->balance = Equal ;
            break ;
    }
    TRACE( cout << "balance of " << rt->info << " to Equal\n\n"; )
    rt->balance = Equal ;
    SetLeft(lf->RotateLeft()) ;
    AVLNode *root = RotateRight() ;
    return root ;
}

// --- balance sub-tree whose left sub-tree has become too heavy
AVLNode * AVLNode::BalanceLeft()
{
    AVLNode *lf = left ;
    AVLNode *root = this ;
    
    switch (lf->balance)
    {
        case LeftHigh:
            TRACE( cout << "Setting balance of " << info << " and " << lf->info << " to Equal and\n"; )
            TRACE( cout << "Rotating tree right at " << root->info << "\n\n"; )
            balance = Equal ;
            lf->balance = Equal ;
            root = RotateRight() ;
            break;

        case RightHigh:
            TRACE( cout << "Doubly rotating tree right at " << root->info << "\n"; )
            root = DoubleRotateRight() ;
            break ;
    }
    return root ;
}

// --- doubly rotate sub-tree and return new root (change balance as needed)
AVLNode * AVLNode::DoubleRotateLeft()
{
    AVLNode *rt = right ;
    AVLNode *lf = rt->left ;

    switch (lf->balance)
    {
        case RightHigh:
            TRACE( cout << "Setting balance of " << info << " to LeftHigh and\n"; )
            TRACE( cout << "balance of " << rt->info << " to Equal and\n"; )
            balance = LeftHigh ;
            rt->balance = Equal ;
            break ;

        case Equal:
            TRACE( cout << "Setting balance of " << info << " and " << rt->info << " to Equal and\n"; )
            balance = rt->balance = Equal ;
            break ;

        case LeftHigh:
            TRACE( cout << "Setting balance of " << info << " to Equal and\n"; )
            TRACE( cout << "balance of " << rt->info << " to RightHigh and\n"; )
            balance = Equal ;
            rt->balance = RightHigh ;
            break ;
    }
    TRACE( cout << "balance of " << lf->info << " to Equal\n\n"; )
    lf->balance = Equal ;
    SetRight(rt->RotateRight()) ;
    AVLNode *root = RotateLeft() ;
    return root ;
}

// --- balance sub-tree whose right sub-tree has become too heavy
AVLNode * AVLNode::BalanceRight()
{
    AVLNode *rt = right ;
    AVLNode *root = this ;

    switch (rt->balance)
    {
        case RightHigh:
            TRACE( cout << "Setting balance of " << info << " and " << rt->info << " to Equal and\n"; )
            TRACE( cout << "Rotating tree left at " << root->info << "\n\n"; )            
            balance = Equal ;
            rt->balance = Equal ;
            root = RotateLeft() ;
            break;

        case LeftHigh:
            TRACE( cout << "Doubly rotating tree left at " << root->info << "\n"; )
            root = DoubleRotateLeft() ;
            break ;
    }
    return root ;
}

// --- calculate height of sub-tree
int AVLNode::Height()
{
    if (this == 0) return 0 ;
    int height = 0 ;
    int lf_height = 0, rt_height = 0 ;
    if (left) lf_height = left->Height() ;
    if (right) rt_height = right->Height() ;
    if (lf_height > rt_height)
        height += lf_height + 1 ;
    else
        height += rt_height + 1 ;
    return height ;
}

// --- draw all nodes at a given level
static char balance_types[] = { '-', '/', '\\' } ;
void AVLNode::DrawLevel(AVLNode *root, int now, int cur_level, int gap)
{
    if (cur_level == now)
    {
        if (root == 0)
            cout << "**" ;
        else
            cout << root->info << balance_types[root->balance] ;
        while (--gap) cout << "  " ;
    }
    else if (root == 0)
    {
        DrawLevel(root, now+1, cur_level, gap) ;
        DrawLevel(root, now+1, cur_level, gap) ;
    }
    else
    {
        DrawLevel(root->left, now+1, cur_level, gap) ;
        DrawLevel(root->right, now+1, cur_level, gap) ;
    }
}

// --- draw a sub-tree
void AVLNode::Draw()
{
    if (this == 0) return ;

    int cur_level, num_levels, gap ;
    num_levels = Height() ;
    gap = 1 ;
    for (cur_level = 1; cur_level <= num_levels; cur_level++)
        gap += gap ;
    for (cur_level = 1; cur_level <= num_levels; cur_level++)
    {
        int num_gaps = (gap/2)+1 ;
        while (num_gaps--) cout << "  " ;
        DrawLevel(this, 1, cur_level, gap) ;
        gap /= 2 ;
        cout << endl << endl ;
    }
}

// --- insert a key into an AVL tree and return new root
AVLNode * AVLTree::_Insert(AVLNode *root, KeyType i)
{
    TRACE( static int ctr = 0 ; )
    TRACE( cout << "INSERTION: Example # " << ++ctr << endl ; )
    TRACE( cout << "Initial tree is " << (root ? "as below" : "NULL") << "\n\n"; )
    TRACE( if (root) root->Draw(); )
    TRACE( cout << "Inserting " << i << "\n" ; )
    
    // --- is the tree empty ?
    if (root == 0)
    {
        root = new AVLNode(i) ;
        TRACE( cout << "Done ! Final tree is as below\n\n"; )
        TRACE( root->Draw(); )
        return root ;
    }

    // --- find insertion point
    AVLNode *p, *n = root ;
    do
    {
        p = n ;
        if (i < p->info)
        {
            n = p->left ;
            p->branch = LeftBranch ;
        }
        else if (i > p->info)
        {
            n = p->right ;
            p->branch = RightBranch ;
        }
        else
            // duplicate
            return root ;
    } while (n != 0) ;

    // --- insert
    if (p->branch == LeftBranch)
    {
        p->SetLeft(new AVLNode(i)) ;

        TRACE( cout << "After insertion tree looks like below\n\n"; )
        TRACE( root->Draw(); )
        TRACE( cout << "Rebalancing tree\n"; )

        switch (p->balance)
        {
            case Equal:
                TRACE( cout << "Setting balance of " << p->info << " to LeftHigh\n\n"; )
                p->balance = LeftHigh ;
                break ;

            case RightHigh:
                TRACE( cout << "Setting balance of " << p->info << " to Equal\n\n"; )
                p->balance = Equal ;
                break ;
        }
    }
    else
    {
        p->SetRight(new AVLNode(i)) ;
        
        TRACE( cout << "After insertion tree looks like below\n\n"; )
        TRACE( root->Draw(); )
        TRACE( cout << "Rebalancing tree\n"; )        

        switch (p->balance)
        {
            case Equal:
                TRACE( cout << "Setting balance of " << p->info << " to RightHigh\n\n"; )
                p->balance = RightHigh ;
                break ;

            case LeftHigh:
                TRACE( cout << "Setting balance of " << p->info << " to Equal\n\n"; )
                p->balance = Equal ;
                break ;
        }
    }

    TRACE( p->Draw() ; )

    // --- rebalance tree
    n = p ;
    p = p->parent ;
    while (p != 0 && n->balance != Equal)
    {
        if (p->branch == LeftBranch)
        {
            if (p->balance == Equal)
            {
                TRACE( cout << "Setting balance of " << p->info << " to LeftHigh\n\n"; )
                p->balance = LeftHigh ;
            }
            else
            {
                if (p->balance == RightHigh)
                {
                    TRACE( cout << "Setting balance of " << p->info << " to Equal\n\n"; )
                    p->balance = Equal ;
                }
                else
                    p = p->BalanceLeft() ;
            }
        }
        else
        {
            if (p->balance == Equal)
            {
                TRACE( cout << "Setting balance of " << p->info << " to RightHigh\n\n"; )
                p->balance = RightHigh ;
            }
            else
            {
                if (p->balance == LeftHigh)
                {
                    TRACE( cout << "Setting balance of " << p->info << " to Equal\n\n"; )
                    p->balance = Equal ;
                }
                else
                    p = p->BalanceRight() ;
            }
        }
        TRACE( p->Draw(); )
        n = p ;
        p = p->parent ;
    }
    if (p == 0)
    {
        root = n ;
        TRACE( cout << "Done ! Final tree is as above\n\n" ; )
    }
    else
    {
        TRACE( cout << "Done ! Final tree is as below\n\n" ; )
        TRACE( root->Draw() ; )
    }
    return root ;
}

// --- delete a key from an AVL tree and return new root
AVLNode * AVLTree::_Delete(AVLNode *root, KeyType i)
{
    // --- search
    AVLNode *p = root ;
    while (p != 0)
    {
        if (i < p->info)
        {
            p->branch = LeftBranch ;
            p = p->left ;
        }
        else if (i > p->info)
        {
            p->branch = RightBranch ;
            p = p->right ;
        }
        else
            break ;
    }
    if (p == 0)
        // not found
        return root ;

    TRACE( static int ctr = 0 ; )
    TRACE( cout << "DELETION: Example # " << ++ctr << endl ; )
    TRACE( cout << "Initial tree is as below\n\n"; )
    TRACE( root->Draw(); )
    TRACE( cout << "Deleting " << i << "\n" ; )

    // --- find predecessor
    p->branch = LeftBranch ;
    AVLNode *n = p->left ;
    while (n != 0 && n->right)
    {
        n->branch = RightBranch ;
        n = n->right ;
    }

    if (n != 0)
    {
        // --- found predecessor, right subtree is guaranteed to be NULL
        p->info = n->info ;
        p = n->parent ;
        if (p->branch == LeftBranch)
            p->SetLeft(n->left) ;
        else
            p->SetRight(n->left) ;
        delete n ;
    }
    else
    {
        // --- no predecessor, left subtree is guaranteed to be NULL
        n = p ;
        p = n->parent ;
        if (p != 0)
        {
            if (p->branch == LeftBranch)
                p->SetLeft(n->right) ;
            else
                p->SetRight(n->right) ;
            delete n ;
        }
        else
        {
            // --- root is being deleted
            p = n->right ;
            if (p) p->parent = 0 ;
            delete n ;
            if (p)
            {
                TRACE( cout << "Done ! Final tree is as below\n\n"; )
                TRACE( p->Draw(); )
            }
            else
                TRACE( cout << "Done ! Final tree is NULL\n\n"; )
            return p ;
        }
    }

    TRACE( cout << "After deletion tree looks like below\n\n"; )
    TRACE( root->Draw(); )
    TRACE( cout << "Rebalancing tree\n"; )

    // --- rebalance the tree
    int shorter = 1 ;
    n = p ;
    while (shorter && p != 0)
    {
        if (p->branch == LeftBranch)
        {
            // --- left subtree has become shorter
            switch (p->balance)
            {
                case Equal:
                    TRACE( cout << "Setting balance of " << p->info << " to RightHigh\n\n"; )
                    p->balance = RightHigh ;
                    shorter = 0 ;
                    break ;

                case LeftHigh:
                    // --- taller (left) subtree was shortened
                    TRACE( cout << "Setting balance of " << p->info << " to Equal\n\n"; )
                    p->balance = Equal ;
                    break ;

                case RightHigh:
                {
                    // --- shorter (left) subtree was shortened,
                    //     violating AVL rules
                    AVLNode * q ;
                    q = p->right ;

                    switch (q->balance)
                    {
                        case Equal:
                            shorter = 0 ;
                            TRACE( cout << "Setting balance of " << q->info << " to LeftHigh\n"; )
                            TRACE( cout << "And Rotating tree left at " << p->info << "\n\n"; )
                            q->balance = LeftHigh ;
                            p = p->RotateLeft() ;
                            break ;

                        case RightHigh:
                            TRACE( cout << "Setting balance of " << p->info << " and " << q->info << " to Equal\n"; )
                            TRACE( cout << "And Rotating tree left at " << p->info << "\n\n"; )
                            p->balance = q->balance = Equal ;
                            p = p->RotateLeft() ;
                            break ;

                        case LeftHigh:
                            TRACE( cout << "Doubly rotating tree left at " << p->info << "\n"; )
                            p = p->DoubleRotateLeft() ;
                            break ;
                    }
                    break ;
                }
            }
        }
        else
        {
            // --- right subtree has become shorter
            switch (p->balance)
            {
                case Equal:
                    TRACE( cout << "Setting balance of " << p->info << " to LeftHigh\n\n"; )
                    p->balance = LeftHigh ;
                    shorter = 0 ;
                    break ;

                case RightHigh:
                    // --- taller (right) subtree was shortened
                    TRACE( cout << "Setting balance of " << p->info << " to Equal\n\n"; )
                    p->balance = Equal ;
                    break ;

                case LeftHigh:
                {
                    // --- shorter (right) sub-tree was shortened,
                    //     violating AVL rules
                    AVLNode * q ;
                    q = p->left ;

                    switch (q->balance)
                    {
                        case Equal:
                            shorter = 0 ;
                            TRACE( cout << "Setting balance of " << q->info << " to RightHigh\n"; )
                            TRACE( cout << "And Rotating tree right at " << p->info << "\n\n"; )
                            q->balance = RightHigh ;
                            p = p->RotateRight() ;
                            break ;

                        case LeftHigh:
                            TRACE( cout << "Setting balance of " << p->info << " and " << q->info << " to Equal\n"; )
                            TRACE( cout << "And Rotating tree right at " << p->info << "\n\n"; )
                            p->balance = q->balance = Equal ;
                            p = p->RotateRight() ;
                            break ;

                        case RightHigh:
                            TRACE( cout << "Doubly rotating tree right at " << p->info << "\n"; )
                            p = p->DoubleRotateRight() ;
                            break ;
                    }
                    break ;
                }
            }
        }
        TRACE( p->Draw(); )
        n = p ;
        p = p->parent ;
    }
    if (p == 0)
    {
        root = n ;
        TRACE( cout << "Done ! Final tree is as above\n\n" ; )
    }
    else
    {
        TRACE( cout << "Done ! Final tree is as below\n\n" ; )
        TRACE( root->Draw() ; )
    }
    return root ;
}

// --- evaluate a function for all keys in ascending order
void AVLTree::_EvalInorder(AVLNode *root, EvalFuncPtr eval)
{
    if (root)
    {
        _EvalInorder(root->left, eval) ;
        (*eval) (root->info);
        _EvalInorder(root->right, eval) ;
    }
}

#define TEST
#ifdef TEST

#include <iostream.h>
#include <string.h>

// --- dump info to standard output
void DumpKey(KeyType i)
{
    cout << i ;
}

void main()
{
    char * unsorted_items = "mepcjnsbdhkoruagiltf" ;
    char * sorted_items = "abcdefghijklmnoprstu" ;
    int i = 0 ;
    AVLTree Tree ;

    TRACE( char *items = sorted_items ; )
    NO_TRACE( char *items = unsorted_items ; )
    // --- create a tree from unsorted_items
    while (items[i])
    {
        Tree.Insert(items[i]) ;
        i++ ;
    }
    NO_TRACE( Tree.EvalInorder(DumpKey) ; )
    NO_TRACE( cout << endl ; )
    NO_TRACE( cout << " Height = " << Tree.Height() << endl ; )
    NO_TRACE( Tree.Draw() ; )

    // --- delete all items in ascending order
    for (i = 0; sorted_items[i+1]; i++)
    {
        Tree.Delete(sorted_items[i]) ;
        NO_TRACE( Tree.EvalInorder(DumpKey) ; )
        NO_TRACE( cout << endl ; )
    }
    Tree.Delete(sorted_items[i]) ;
    TRACE( return ; )

    // --- insert all items in ascending order
    for (i = 0; sorted_items[i]; i++)
        Tree.Insert(unsorted_items[i]) ;
    Tree.EvalInorder(DumpKey) ;
    cout << endl ;

    // --- delete all items in descending order
    for (i = strlen(sorted_items)-1 ; i > 0 ; i--)
    {
        Tree.Delete(sorted_items[i]) ;
        Tree.EvalInorder(DumpKey) ;
        cout << endl ;
    }
    Tree.Delete(sorted_items[i]) ;

    // --- insert all items in descending order
    for (i = strlen(sorted_items)-1; i >= 0; i--)
        Tree.Insert(sorted_items[i]) ;
    Tree.EvalInorder(DumpKey) ;
    cout << endl ;

    // --- insert all items in descending order in UPPER case
    strupr(sorted_items) ;
    for (i = strlen(sorted_items)-1; i >= 0; i--)
        Tree.Insert(sorted_items[i]) ;
    Tree.EvalInorder(DumpKey) ;
    cout << endl ;

    // --- delete all lower case items in unsorted order
    for (i = 0; unsorted_items[i+1]; i++)
    {
        Tree.Delete(unsorted_items[i]) ;
        Tree.EvalInorder(DumpKey) ;
        cout << endl ;
    }
    Tree.Delete(unsorted_items[i]) ;
    Tree.EvalInorder(DumpKey) ;
    cout << endl ;
}

#endif
