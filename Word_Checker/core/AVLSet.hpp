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

//#include <vector>
//#include <iostream>
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
//
//	void print_tree();

private:
	struct Node
	{
		ElementType val;
		Node* left;
		Node* right;
	};
	
	Node* root;
	int count;
	bool check_balance;
	void delete_all(Node* n);
	Node* copy_all(Node* n) const;
	bool need_check_balance = false;
	
	int h(Node* n) const;
	void pre(Node* n, VisitFunction visit) const;
	void in(Node* n, VisitFunction visit) const;
	void post(Node* n, VisitFunction visit) const;
	
	void a(Node* curr, Node* prev, const ElementType& element);
	bool is_balance(Node* n);
	
	Node* ll(Node* n);
	Node* rr(Node* n);
	Node* lr(Node* n);
	Node* rl(Node* n);
    // You'll no doubt want to add member variables and "helper" member
    // functions here.
};


template <typename ElementType>
AVLSet<ElementType>::AVLSet(bool shouldBalance)
{
	root = nullptr;
	check_balance = shouldBalance;
	count = 0;
}

template <typename ElementType>
AVLSet<ElementType>::~AVLSet() noexcept
{
	delete_all(root);
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(const AVLSet& s)
{
	root = copy_all(s.root);
	check_balance = s.check_balance;
	count = s.count;
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(AVLSet&& s) noexcept
{
	root = s.root;
	check_balance = s.check_balance;
	s.root = nullptr;
	s.count = 0;
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(const AVLSet& s)
{
	if (this != &s)
	{
		delete_all(root);
		root = copy_all(s.root);
		count = s.count;
		check_balance = s.check_balance;
	}
	
    return *this;
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(AVLSet&& s) noexcept
{
	if (this != &s)
	{
		delete_all(root);
		root = s.root;
		s.root = nullptr;
		count = s.count;
		check_balance = s.check_balance;
		
		s.count = 0;
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
	if (root == nullptr)
	{
		root = new Node{element, nullptr, nullptr};
		count ++;
	}
	else
	{
		a(root, nullptr ,element);
	}
}

template <typename ElementType>
void AVLSet<ElementType>::a(Node* curr, Node* prev, const ElementType& element)
{
	if (curr == nullptr)
	{
		if (element > prev->val)
		{prev->right = new Node{element, nullptr, nullptr};}
		else
		{prev->left = new Node{element, nullptr, nullptr};}
		count ++;
		need_check_balance = true;
		return;
	}
	else if (element > curr->val)
	{
		a(curr -> right, curr, element);
	}
	else if (element < curr->val)
	{
		a(curr -> left, curr, element);
	}
	if (check_balance and need_check_balance and (not is_balance(curr)))
	{
		if (prev == nullptr)
		{
			if (element > curr->val and element > (curr->right)->val)
			{root = rr(curr);}
			else if (element < curr->val and element < (curr->left)->val)
			{root = ll(curr);}
			else if (element < curr->val and element > (curr->left)->val)
			{root = lr(curr);}
			else if (element > curr->val and element < (curr->right)->val)
			{root = rl(curr);}
		}
		else if (prev->right == curr)
		{
			if (element > curr->val and element > (curr->right)->val)
			{prev->right = rr(curr);}
			else if (element < curr->val and element < (curr->left)->val)
			{prev->right = ll(curr);}
			else if (element < curr->val and element > (curr->left)->val)
			{prev->right = lr(curr);}
			else if (element > curr->val and element < (curr->right)->val)
			{prev->right = rl(curr);}
		}
		else
		{
			if (element > curr->val and element > (curr->right)->val)
			{prev->left = rr(curr);}
			else if (element < curr->val and element < (curr->left)->val)
			{prev->left = ll(curr);}
			else if (element < curr->val and element > (curr->left)->val)
			{prev->left = lr(curr);}
			else if (element > curr->val and element < (curr->right)->val)
			{prev->left = rl(curr);}
		}
		need_check_balance = false;
		return;
	}
}

template <typename ElementType>
typename AVLSet<ElementType>::Node* AVLSet<ElementType>:: ll(Node* n)
{
	Node* x = n->left;
	Node* y = (n->left)->right;
	(n->left)->right = n;
	n->left = y;
	return x;
}

template <typename ElementType>
typename AVLSet<ElementType>::Node* AVLSet<ElementType>:: rr(Node* n)
{
	Node* x = (n->right)->left;
	Node* y = n->right;
	(n->right)->left = n;
	n->right = x;
	return y;
}

template <typename ElementType>
typename AVLSet<ElementType>::Node* AVLSet<ElementType>:: lr(Node* n)
{
	Node* x = (n->left)->right;
	Node* y1 = ((n->left)->right)->left;
	Node* y2 = ((n->left)->right)->right;
	((n->left)->right)->left = n->left;
	((n->left)->right)->right = n;
	(n->left)->right = y1;
	n->left = y2;
	return x;
}

template <typename ElementType>
typename AVLSet<ElementType>::Node* AVLSet<ElementType>:: rl(Node* n)
{
	Node* x = (n->right)->left;
	Node* y1 = ((n->right)->left)->left;
	Node* y2 = ((n->right)->left)->right;
	((n->right)->left)->right = n->right;
	((n->right)->left)->left = n;
	(n->right)->left = y2;
	n->right = y1;
	return x;
}

template <typename ElementType>
bool AVLSet<ElementType>::is_balance(Node* n)
{
	if (n == nullptr)
	{return true;}
	int l = h(n->left);
	int r = h(n->right);
	if (l-r > 1)
	{return false;}
	else if (r-l > 1)
	{return false;}
	return true;
}


template <typename ElementType>
bool AVLSet<ElementType>::contains(const ElementType& element) const
{
	Node* curr = root;
	while(curr != nullptr)
	{
		if (curr->val == element)
		{return true;}
		else if (element > curr->val)
		{curr = curr -> right;}
		else
		{curr = curr -> left;}
	}
	return false;
}

template <typename ElementType>
unsigned int AVLSet<ElementType>::size() const noexcept
{
    return count;
}


template <typename ElementType>
int AVLSet<ElementType>::height() const
{
	if (root == nullptr)
	{return -1;}
	else
	{return h(root);}
}

template <typename ElementType>
int AVLSet<ElementType>::h(Node* n) const
{
	if (n == nullptr)
	{ return -1;}
	int lt = h(n->left);
	int rt = h(n->right);
	
	if (lt > rt)
	{return lt + 1;}
	else {return rt +1;}
}


template <typename ElementType>
void AVLSet<ElementType>::preorder(VisitFunction visit) const
{
	pre(root,visit);
}

template <typename ElementType>
void AVLSet<ElementType>::pre(Node* n, VisitFunction visit) const
{
	if (n != nullptr)
	{
		visit(n->val);
		pre(n->left,visit);
		pre(n->right,visit);
	}
}

template <typename ElementType>
void AVLSet<ElementType>::inorder(VisitFunction visit) const
{
	in(root,visit);
}

template <typename ElementType>
void AVLSet<ElementType>::in(Node* n, VisitFunction visit) const
{
	if (n != nullptr)
	{
		in(n->left,visit);
		visit(n->val);
		in(n->right,visit);
	}
}

template <typename ElementType>
void AVLSet<ElementType>::postorder(VisitFunction visit) const
{
	post(root,visit);
}

template <typename ElementType>
void AVLSet<ElementType>::post(Node* n, VisitFunction visit) const
{
	if (n != nullptr)
	{
		post(n->left,visit);
		post(n->right,visit);
		visit(n->val);
	}
}

template <typename ElementType>
void AVLSet<ElementType>::delete_all(Node* n)
{
	if (n != nullptr)
	{
		delete_all(n->left);
		delete_all(n->right);
		delete n;
	}
}


template <typename ElementType>
typename AVLSet<ElementType>::Node* AVLSet<ElementType>::copy_all(Node* n) const
{
	if (n == nullptr)
	{
		return nullptr;
	}
	Node* x = new Node{n->val,nullptr,nullptr};
	x->left = copy_all(n->left);
	x->right = copy_all(n->right);
	
	return x;
}

//template <typename ElementType>
//void AVLSet<ElementType>:: print_tree()
//{
//	std::vector<Node*> x;
//	x.push_back(root);
//
//	Node* curr;
//
//	while (x.size() != 0)
//	{
//		curr = x[0];
//		x.erase(x.begin());
//		std::cout << curr->val << ' ';
//		if (curr->left!=nullptr)
//		{x.push_back(curr->left);}
//		if (curr->right!= nullptr)
//		{x.push_back(curr->right);}
//	}
//	std::cout << std::endl;
//}


#endif // AVLSET_HPP

