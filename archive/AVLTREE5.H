#ifndef avltree_h
#define avltree_h

const unsigned char Equal = 0 ;
const unsigned char LeftHigh = 1 ;
const unsigned char RightHigh = 2 ;
const unsigned char NoBranch = 0 ;
const unsigned char LeftBranch = 1 ;
const unsigned char RightBranch = 2 ;

typedef char KeyType ;

// ============== //
// AVL Node class //
// ============== //

class AVLNode
{
    friend class AVLTree ;
    
    AVLNode *parent ;
    AVLNode *left ;
    AVLNode *right ;
    unsigned char balance : 2 ;
    unsigned char branch : 2 ;
    KeyType info ;
    
    AVLNode(KeyType i)
    {
        info = i ;
        parent = 0 ;
        left = right = 0 ;
        balance = Equal ;
        branch = NoBranch ;
    }
    void SetRight(AVLNode *r)
    {
        right = r ;
        if (r) r->parent = this ;
    }
    void SetLeft(AVLNode *l)
    {
        left = l ;
        if (l) l->parent = this ;
    }
    AVLNode *RotateLeft() ;
    AVLNode *RotateRight() ;
    AVLNode *DoubleRotateRight() ;
    AVLNode *BalanceLeft() ;
    AVLNode *DoubleRotateLeft() ;
    AVLNode *BalanceRight() ;
    void     Draw() ;
    void     DrawLevel(AVLNode *root, int now, int cur_level, int gap) ;
    int      Height() ;
} ;

typedef void (*EvalFuncPtr) (KeyType i) ;

// ============== //
// AVL Tree class //
// ============== //

class AVLTree
{
    AVLNode * root ;

    AVLNode * _Insert(AVLNode *root, KeyType i) ;
    AVLNode * _Delete(AVLNode *root, KeyType i) ;
    void      _EvalInorder(AVLNode *root, EvalFuncPtr eval) ;

public:
    AVLTree()  { root = 0; }
    ~AVLTree() { while (root != 0) root = _Delete(root, root->info) ; }

    void Insert(KeyType i) { root = _Insert(root, i) ; }
    void Delete(KeyType i) { root = _Delete(root, i) ; }
    void EvalInorder(EvalFuncPtr eval) { _EvalInorder(root, eval) ; }
    int  Height() { if (root) return root->Height() ; else return 0 ; }
    void Draw() { if (root) root->Draw() ; }
} ;

#endif
