// AVLSet.hpp
//
// ICS 46 Winter 2019
// Project #3: Set the Controls for the Heart of the Sun
//
// An AVLSet is an implementation of a Set that is an AVL tree, which uses
// the algorithms we discussed in lecture to maintain balance every time a
// new element is added to the set.  The balancing is actually optional,
// with a bool parameter able to be passed to the constructor to explicitly
// turn the balancing on or off (on is default).  If the balancing is off,
// the AVL tree acts like a binary search tree (e.g., it will become
// degenerate if elements are added in ascending order).
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to implement your AVL tree
// using your own dynamically-allocated nodes, with pointers connecting them,
// and with your own balancing algorithms used.

#ifndef AVLSET_HPP
#define AVLSET_HPP

#include <functional>
#include "Set.hpp"




template <typename ElementType>
class AVLSet : public Set<ElementType>
{
public:
    // A VisitFunction is a function that takes a reference to a const
    // ElementType and returns no value.
    using VisitFunction = std::function<void(const ElementType&)>;

public:
    // Initializes an AVLSet to be empty, with or without balancing.
    explicit AVLSet(bool shouldBalance = true);

    // Cleans up the AVLSet so that it leaks no memory.
    virtual ~AVLSet() noexcept;

    // Initializes a new AVLSet to be a copy of an existing one.
    AVLSet(const AVLSet& s);

    // Initializes a new AVLSet whose contents are moved from an
    // expiring one.
    AVLSet(AVLSet&& s) noexcept;

    // Assigns an existing AVLSet into another.
    AVLSet& operator=(const AVLSet& s);

    // Assigns an expiring AVLSet into another.
    AVLSet& operator=(AVLSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement an AVLSet, false otherwise.
    virtual bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function always runs in O(log n) time
    // when there are n elements in the AVL tree.
    virtual void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function always runs in O(log n) time when
    // there are n elements in the AVL tree.
    virtual bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const noexcept override;


    // height() returns the height of the AVL tree.  Note that, by definition,
    // the height of an empty tree is -1.
    int height() const;


    // preorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a preorder traversal of the AVL
    // tree.
    void preorder(VisitFunction visit) const;


    // inorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by an inorder traversal of the AVL
    // tree.
    void inorder(VisitFunction visit) const;


    // postorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a postorder traversal of the AVL
    // tree.
    void postorder(VisitFunction visit) const;


private:
    // You'll no doubt want to add member variables and "helper" member
    // functions here.
    struct Node {
        ElementType value;
        struct Node *left;
        struct Node *right;
        int h;
        Node(ElementType element) { 
            value = element;
            left = NULL;
            right = NULL;
            h = 0;
        }
        Node(ElementType element, Node *l, Node *r) {
            value = element;
            left = l;
            right = r;
            h = 0;
        }
    };
    bool balancing;
    int sz;
    Node *root;
    void makeEmpty(Node *r);
    Node* add_helper(const ElementType& element, Node *r);
    bool contains_helper(const ElementType& element, Node *r) const;
    int max(int x, int y) const;
    Node* deepCopy(Node *r);
    void preorder_helper(Node* r, VisitFunction visit) const;
    void inorder_helper(Node* r, VisitFunction visit) const;
    void postorder_helper(Node* r, VisitFunction visit) const;
    Node* rr_rotation(Node* n);
    Node* lr_rotation(Node* n);
    int difference(Node* n);
    int hhh(Node* n);
};
typedef struct Node Node;

template <typename ElementType>
int AVLSet<ElementType>::difference(Node* n) {
    int l = -1;
    int r = -1;
    if(n == nullptr) {
        return 0;
    }
    if( n->left != nullptr) {
        l = n->left->h;
    }
    if( n->right != nullptr) {
        r = n->right->h;
    }
    return l - r;
}


template <typename ElementType>
typename AVLSet<ElementType>::Node* AVLSet<ElementType>::rr_rotation(Node* n) {
    Node* x = n->left;
    Node* t = x->right;
    x->right = n;
    n->left = t;

    n->h = max(hhh(n->left),hhh(n->right))+1;
    x->h = max(hhh(x->left),hhh(x->right))+1;

    return x;
}


template<typename ElementType>
int AVLSet<ElementType>::hhh(Node* n) {
    if(n == nullptr) {
        return -1;
    }
    else {
        return n->h;
    }
}

template <typename ElementType>
typename AVLSet<ElementType>::Node* AVLSet<ElementType>::lr_rotation(Node* n) {
    Node* y = n->right;
    Node* t = y->left;
    y->left = n;
    n->right = t;

    n->h = max(hhh(n->left),hhh(n->right))+1;
    y->h = max(hhh(y->left),hhh(y->right))+1;

    return y;
}


template <typename ElementType>
typename AVLSet<ElementType>::Node* AVLSet<ElementType>::deepCopy(Node *r) {
    if(r == nullptr) {
        return nullptr;
    }
    else {
        return new Node(r->value, deepCopy(r->left), deepCopy(r->right));
    }
}

template <typename ElementType>
void AVLSet<ElementType>::preorder_helper(Node* r, VisitFunction visit) const
{
    if(r != nullptr) {
        visit(r->value);
        preorder_helper(r->left, visit);
        preorder_helper(r->right, visit);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::inorder_helper(Node* r, VisitFunction visit) const
{
    if(r != nullptr) {
        inorder_helper(r->left, visit);
        visit(r->value);
        inorder_helper(r->right, visit);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::postorder_helper(Node* r, VisitFunction visit) const
{
    if(r != nullptr) {
        postorder_helper(r->left, visit);
        postorder_helper(r->right, visit);
        visit(r->value);
    }
}


template <typename ElementType>
void AVLSet<ElementType>::makeEmpty(Node *r) {
    if(r != nullptr) {
        makeEmpty(r->left);
        makeEmpty(r->right);
        delete r;
    }
    
}

template <typename ElementType>
typename AVLSet<ElementType>::Node* AVLSet<ElementType>::add_helper(const ElementType& element, Node *n) { 
    if(n == nullptr) {
        n = new Node(element);
        sz += 1;
    }
    else if(n->value > element) {
        n->left = add_helper(element, n->left);
    }
    else {
        n->right = add_helper(element, n->right);
    }
    
    int l = n->left? n->left->h : -1;
    int r = n->right? n->right->h : -1;
    n->h = max(l + 1, r + 1);
    

    if(balancing == true) {
        int b = difference(n);
        if(b > 1 && element < n->left->value) {
            return rr_rotation(n);
        }
        if(b < -1 && element > n->right->value) {
            return lr_rotation(n);
        }
        if(b > 1 && element > n->left->value) {
            n->left = lr_rotation(n->left);
            return rr_rotation(n);
        }
        if(b < -1 && element < n->right->value) {
            n->right = rr_rotation(n->right);
            return lr_rotation(n);
        }
    }
    return n;
}

template <typename ElementType>
bool AVLSet<ElementType>::contains_helper(const ElementType& element, Node *r) const {
    if(r == nullptr) {
        return false;
    }
    else if(r->value == element) {
        return true;
    }
    else if(element < r->value) {
        return contains_helper(element, r->left);
    }
    else {
        return contains_helper(element, r->right);
    }
}

template <typename ElementType>
int AVLSet<ElementType>::max(int x, int y) const {
    if(x < y) {
        return y;
    }
    else {
        return x;
    }
}


//
//
//

template <typename ElementType>
AVLSet<ElementType>::AVLSet(bool shouldBalance)
{
    root = nullptr;
    balancing = shouldBalance;
    sz = 0;
}


template <typename ElementType>
AVLSet<ElementType>::~AVLSet() noexcept
{
    makeEmpty(root);
    root = nullptr;
    sz = 0;
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(const AVLSet& s)
{   
    root = deepCopy(s.root);
    balancing = s.balancing;
    sz = s.sz;
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(AVLSet&& s) noexcept
{   
    root = nullptr;
    std::swap(root, s.root); 
    std::swap(balancing, s.balancing);
    std::swap(sz, s.sz);
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(const AVLSet& s)
{
    if(this != &s) {
        makeEmpty(root);
        root = deepCopy(s.root);
        sz = s.sz;
        balancing = s.balancing;
    }
    return *this;
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(AVLSet&& s) noexcept
{
    if(this != &s) {
        makeEmpty(root);
        std::swap(root, s.root);
        s.root = nullptr;
        sz = s.sz;
        balancing = s.balancing;
    }
    return *this;
}


template <typename ElementType>
bool AVLSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void AVLSet<ElementType>::add(const ElementType& element)
{   
    if(AVLSet<ElementType>::contains(element) == false) {
        root = add_helper(element, root);
    }
}


template <typename ElementType>
bool AVLSet<ElementType>::contains(const ElementType& element) const
{   
    return contains_helper(element, root);
}


template <typename ElementType>
unsigned int AVLSet<ElementType>::size() const noexcept
{
    return sz;
}


template <typename ElementType>
int AVLSet<ElementType>::height() const
{
    if(root == nullptr) {
        return -1;
    }
    else {
        return root->h;
    }
}


template <typename ElementType>
void AVLSet<ElementType>::preorder(VisitFunction visit) const
{
    preorder_helper(root, visit);
}


template <typename ElementType>
void AVLSet<ElementType>::inorder(VisitFunction visit) const
{
    inorder_helper(root, visit);
}


template <typename ElementType>
void AVLSet<ElementType>::postorder(VisitFunction visit) const
{
    postorder_helper(root, visit);
}



#endif // AVLSET_HPP

