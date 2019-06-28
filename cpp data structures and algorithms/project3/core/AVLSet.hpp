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
#include <memory>
#include <algorithm>

// #include <iostream>


template <typename ElementType>
class AVLSet : public Set<ElementType>
{
public:
    // A VisitFunction is a function that takes a reference to a const
    // ElementType and returns no value.
    using VisitFunction = std::function<void(const ElementType&)>;

private:
    struct Node{
        ElementType value;
        int height;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
    };

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

    

    void printer(const std::unique_ptr<Node>& node) const;

    void print_inorder() const;


private:
    // You'll no doubt want to add member variables and "helper" member
    // functions here.
    bool balance;
    unsigned int _size;
    std::unique_ptr<Node> head;

    int unbalance_factor(const std::unique_ptr<Node>& node) const;
    void just_add(const ElementType& element, std::unique_ptr<Node>& node);
    bool recurse_contain(const ElementType& element, const std::unique_ptr<Node>& node) const;
    void left_rotate(std::unique_ptr<Node>& z);
    void right_rotate(std::unique_ptr<Node>& z);
    void set_height(const std::unique_ptr<Node>& node);
    int get_height(const std::unique_ptr<Node>& node) const;

    void nodeCopy(std::unique_ptr<Node>& target, const std::unique_ptr<Node>& node);

    void r_preorder(const std::unique_ptr<Node>& node, VisitFunction visit) const;

    void r_postorder(const std::unique_ptr<Node>& node, VisitFunction visit) const;

    void r_inorder(const std::unique_ptr<Node>& node, VisitFunction visit) const;

    void r_inorder(const std::unique_ptr<Node>& node) const;


};


template <typename ElementType> //constructor
AVLSet<ElementType>::AVLSet(bool shouldBalance)
:balance{shouldBalance}, _size{0}, head{nullptr}
{
}


template <typename ElementType> //destructor
AVLSet<ElementType>::~AVLSet() noexcept
{
}


template <typename ElementType> //copy constructor
AVLSet<ElementType>::AVLSet(const AVLSet& s)
:balance{s.balance}, _size{s.size()}, head{nullptr}
{
    if (s.head != nullptr){
        nodeCopy(head, s.head);
    }
}


template <typename ElementType> //move constructor
AVLSet<ElementType>::AVLSet(AVLSet&& s) noexcept
:balance{s.balance}, _size{s._size}, head{nullptr}
{
    std::swap(head, s.head);
}


template <typename ElementType> //assignment operator
AVLSet<ElementType>& AVLSet<ElementType>::operator=(const AVLSet& s)
{
    if (&s != this){
        _size = s.size();
        balance = s.balance;
        if (s.head == nullptr){
            head = nullptr;
        }
        else{
            nodeCopy(head, s.head);
        }
        
    }
    return *this;
}


template <typename ElementType> //move assgignment operator
AVLSet<ElementType>& AVLSet<ElementType>::operator=(AVLSet&& s) noexcept
{
    if (&s != this){
        std::swap(s._size, _size);
        std::swap(s.balance, balance);
        std::swap(s.head, head);
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
    if (head == nullptr){
        _size++;
        head = std::unique_ptr<Node>{new Node{element, 0, nullptr, nullptr}};
    }
    else{
        just_add(element, head);
        
    }

}


template <typename ElementType>
bool AVLSet<ElementType>::contains(const ElementType& element) const
{
    return recurse_contain(element, head);
}


template <typename ElementType>
unsigned int AVLSet<ElementType>::size() const noexcept
{
    return _size;
}


template <typename ElementType>
int AVLSet<ElementType>::height() const
{
    if (head == nullptr){
        return -1;
    }
    return head->height;

}


template <typename ElementType>
void AVLSet<ElementType>::preorder(VisitFunction visit) const
{
    r_preorder(head, visit);
}

template <typename ElementType>
void AVLSet<ElementType>::r_preorder(const std::unique_ptr<Node>& node, VisitFunction visit) const{
    if (node != nullptr){
        visit(node->value);
        r_preorder(node->left, visit);
        r_preorder(node->right, visit);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::inorder(VisitFunction visit) const
{
    r_inorder(head, visit);
}

template <typename ElementType>
void AVLSet<ElementType>::r_inorder(const std::unique_ptr<Node>& node, VisitFunction visit) const{
    if (node != nullptr){
        r_inorder(node->left, visit);
        visit(node->value);
        r_inorder(node->right, visit);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::postorder(VisitFunction visit) const
{
    r_postorder(head, visit);
}

template <typename ElementType>
void AVLSet<ElementType>::r_postorder(const std::unique_ptr<Node>& node, VisitFunction visit) const{
    if (node != nullptr){
        r_postorder(node->left, visit);
        r_postorder(node->right, visit);
        visit(node->value);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::just_add(const ElementType& element, std::unique_ptr<Node>& node){

    if (element == node->value){
        return;
    }
    
    if (element > node->value){
        if (node->right == nullptr){
            _size++;
            node->right = std::unique_ptr<Node>{new Node{element, 0, nullptr, nullptr}};
        }
        else{

            just_add(element, node->right);
        }
    }
    else if (element < node->value){

        if (node->left == nullptr){
            _size++;
            node->left = std::unique_ptr<Node>{new Node{element, 0, nullptr, nullptr}};
        }
        else{

            just_add(element, node->left);
        }
    }

    set_height(node);
    if (balance){
        int pos_whenLeftTreeIsHigher = unbalance_factor(node);
        // std::cout << "height diff for " << node-> value << " is " << pos_whenLeftTreeIsHigher << std::endl;

        if ((pos_whenLeftTreeIsHigher) > 1 and (element < node->left->value)){ //left left

            right_rotate(node);
            
            set_height(node->left);
            set_height(node->right);
            set_height(node);
        }
        if ((pos_whenLeftTreeIsHigher) < -1 and (element > node->right->value)){ //right right

            left_rotate(node);
            
            set_height(node->left);
            set_height(node->right);
            set_height(node);

        }
        if ((pos_whenLeftTreeIsHigher) > 1 and (element > node->left->value)){ //left right

            
            left_rotate(node->left);
            // std::cout <<"here" <<std::endl;
            right_rotate(node);
            
            set_height(node->left);
            set_height(node->right);
            set_height(node);
        }
        if ((pos_whenLeftTreeIsHigher) < -1 and (element < node->right->value)){ //right left

            right_rotate(node->right);
            left_rotate(node);
            
            set_height(node->left);
            set_height(node->right);
            set_height(node);
        }

    }
    
}

template <typename ElementType>
bool AVLSet<ElementType>::recurse_contain(const ElementType& element, const std::unique_ptr<Node>& node) const{
    if (node == nullptr){
        return false;
    }
    if (node->value == element){
        return true;
    }

    if (element > node->value){
        return recurse_contain(element, node->right);
    }

    if (element < node->value){
        return recurse_contain(element, node->left);
    }
    else{
        return false;
    }

}


template <typename ElementType>
void AVLSet<ElementType>::set_height(const std::unique_ptr<Node>& node){
    if (node != nullptr){
        node->height = std::max(1+get_height(node->left), 1+get_height(node->right));
        // std::cout << std::max(1+get_height(node->left), 1+get_height(node->right)) << std::endl;
    }
}

template <typename ElementType>
int AVLSet<ElementType>::get_height(const std::unique_ptr<Node>& node) const{
    if (node == nullptr){
        return -1;
    }
    return node->height;
}


template <typename ElementType>
void AVLSet<ElementType>::printer(const std::unique_ptr<Node>& node) const{
    std::cout << std::string(node->height, '-') << node->value << "(" << unbalance_factor(node) << ")" << std::endl;
}



template <typename ElementType> //left sub-right sub
int AVLSet<ElementType>::unbalance_factor(const std::unique_ptr<Node>& node) const{
    return get_height(node->left) - get_height(node->right);
}

template <typename ElementType>
void AVLSet<ElementType>::left_rotate(std::unique_ptr<Node>& z){
    std::unique_ptr<Node> temp = std::move(z->right);
    //what is z-> left right now?
    std::swap(z, temp);
    std::swap(z->left, temp->right);
    std::swap(z->left, temp);

}


template <typename ElementType>
void AVLSet<ElementType>::right_rotate(std::unique_ptr<Node>& z){
    std::unique_ptr<Node> temp = std::move(z->left);
    //what is z-> left right now?
    std::swap(z, temp);
    std::swap(z->right, temp->left);
    std::swap(z->right, temp);

}

template <typename ElementType>
void AVLSet<ElementType>::nodeCopy(std::unique_ptr<Node>& target, const std::unique_ptr<Node>& node){
    std::unique_ptr<Node> recurse_ptr;
    std::unique_ptr<Node> recurse_ptr1;

    if (node->left == nullptr and node->right == nullptr){
        target = std::unique_ptr<Node>{new Node{node->value, 0, nullptr, nullptr}};
    }
    else if (node->left == nullptr and node->right != nullptr){
        nodeCopy(recurse_ptr, node->right);
        target = std::unique_ptr<Node>{new Node{node->value, node->height, nullptr, std::move(recurse_ptr)}};
    }
    else if (node->left != nullptr and node->right == nullptr){
        nodeCopy(recurse_ptr, node->left);
        target = std::unique_ptr<Node>{new Node{node->value, node-> height, std::move(recurse_ptr), nullptr}};
    }
    else{
        nodeCopy(recurse_ptr, node->left);
        nodeCopy(recurse_ptr1, node->right);
        target = std::unique_ptr<Node>{new Node{node->value, node->height, std::move(recurse_ptr), std::move(recurse_ptr1)}};
    }

}

template <typename ElementType>
void AVLSet<ElementType>::print_inorder() const{
    r_inorder(head);
}

template <typename ElementType>
void AVLSet<ElementType>::r_inorder(const std::unique_ptr<Node>& node) const{
    if (node != nullptr){
        r_inorder(node->left);
        printer(node);
        r_inorder(node->right);
    }
}

#endif // AVLSET_HPP

