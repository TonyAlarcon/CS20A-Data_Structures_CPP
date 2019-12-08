#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#ifdef ENABLE_DEBUG_MEM
#include"debugmem.h"
#endif

#include<functional>

class BinarySearchTree {

public:

	// Public Interface Functions: All of these are implemented
	BinarySearchTree();
	~BinarySearchTree();

	void	printInorder();
	void	printPostorder();
	void	printPreorder();
	void	insert(int value);

	void	remove(int value);

	int		min() const;
	int		max() const;
	int		height() const;
	bool	search(int value) const;

	bool	isEmpty() const;

	void	printTree() const;
#ifndef MAKE_MEMBERS_PUBLIC
private:
#endif
	// Forward declare Node
	struct Node;

	// Member variable root
	Node* m_root;



	//Auxillary recursive bst functions
	//public interface functions call these
	//You will implement these functions.

	// Tree traversal, second parameter is a "function" with 
	// return type void and parameter Node*, i.e: 
	//					void process(Node* )
	void inorder(Node* node, std::function<void(Node*)> process);
	void preorder(Node* node, std::function<void(Node*)> process);
	void postorder(Node* node, std::function<void(Node*)> process);

	Node*	minNode(Node *node) const;
	Node*	maxNode(Node *node) const;
	int 	height(Node* node) const;

	Node*	insert(Node *node, int value);
	Node*	search(Node *node, int value) const;
	
	Node*	deleteNode(Node *node, int value);
	
	void	printTree(Node *node, int space) const;	
};

#endif//BINARYSEARCHTREE_H