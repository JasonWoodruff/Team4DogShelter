/*
CIS22C Team 4 Project:	Dog Shelter Management Program

Purpose:	The function of our software is to store, organize and
manage data about dogs in a shelter.

@author
Jason Woodruff	-	Project Coordinator
Stacy Wheeler	-	Data Record
Bryson Li		-	I/O
James Juchau	-	Hashed Table
Kevin Chen		-	Binary Search Tree

@version	1.5
@since		11/30/2014
*/

#include <iostream>
#include <cstdio>
#include <sstream>
#include <algorithm>
#include "Dog.h"
using namespace std;

//node structure
struct avl_node
{
	Dog *data;					//pointer to a Dog
	struct avl_node *left;		//pointer to the left subtree
	struct avl_node *right;		//pointer to the right subtree
};	

/*
	AVL Tree code adapted from the following sources:
	http://www.sanfoundry.com/cpp-program-implement-avl-trees/
	http://www.geeksforgeeks.org/avl-tree-set-2-deletion/
*/
class avlTree
{
private:
	avl_node *root;		//root node
	
	/*
		height recursively finds the height of the Tree
		@return	-	h is the height of the Tree
	*/
	int height(avl_node* temp);

	/*
		diff returns the balance factor between a node's left and right subtrees
		@param	-	temp is the node to be evaluated
		@return	-	b_factor is the height of the right subtree subtracted from the height of the left subtree
	*/
	int diff(avl_node* temp);

	/*
		perform a Right-Right rotation
		@param	-	parent is the unbalanced node (root of the subtree to be rotated)
		@post	-	the subtree has been rotated
		@return	-	temp is the new parent after the rotation
	*/
	avl_node *rr_rotation(avl_node* parent);

	/*
		perform a Left-Left rotation
		@param	-	parent is the unbalanced node (root of the subtree to be rotated)
		@post	-	the subtree has been rotated
		@return	-	temp is the new parent after the rotation
	*/
	avl_node *ll_rotation(avl_node* parent);

	/*
		perform a Left-Right rotation
		@param	-	parent is the unbalanced node (root of the subtree to be rotated)
		@post	-	the subtree has been rotated
		@return	-	the new parent after a the rotation
	*/
	avl_node *lr_rotation(avl_node* parent);

	/*
		perform a Right-Left rotation
		@param	-	parent is the unbalanced node (root of the subtree to be rotated)
		@post	-	the subtree has been rotated
		@return	-	the new parent after a the rotation
	*/
	avl_node *rl_rotation(avl_node* parent);

	/*
		balance the Tree using the necessary rotation (root of the subtree to be balanced)
		@param	-	temp is the node to be balanced
		@post	-	the subtree has been balanced
		@return	-	temp is the new node at the root of the subtree
	*/
	avl_node *balance(avl_node* temp);

	/*
		insert a pointer to a Dog into the Tree by traversing the Tree, starting at root, and finding a suitable location for the pointer
		the Tree is then balanced to ensure it is of minimum height
		@param	-	root is the root of the Tree
		@param	-	value is the pointer to a Dog to be inserted
		@post	-	the pointer is inserted into the Tree and the Tree balances itself
		@return	-	the new root of the Tree
	*/
	avl_node *insert(avl_node* root, Dog* dog);	//root, done

	/*
		search the Tree, starting at root, for a Dog with the same ID as dogId, delete the node from the Tree if found, then balance the Tree
		@param	-	root is the root of the Tree
		@param	-	dogId is the Dog ID of the Dog to be removed
		@param	-	ptr_removed is a pointer to a bool that indicates whether a deletion occured (used in other functions)
		@post	-	the node is deleted and the Tree balances itself
		@return	-	the new root of the Tree
	*/
	avl_node* deleteNode(avl_node* root, string dogId, bool* ptr_removed);

	/*
		recursively scan a subtree and return the minimum value (leftmost) node, starting at its root
		@param	-	node is the root of the subtree
		@return	-	the minimum value node of the subtree
	*/
	avl_node *minValueNode(avl_node* node);

	/*
		recursively delete the Tree by postorder traversal
		@param	-	temp is the root of the subtree
		@post	-	the Tree is deleted
	*/
	void destroySubTree(avl_node* temp);

	/*
		display the indented search Tree
		@param	-	is the root of the Tree
		@param	-	n is the Tree level to start at
	*/
	void display(avl_node* ptr, int n);

	/*
		inorder displays the IDs of the Tree's Dogs by inorder traversal
		@param	-	tree is the root of the Tree
	*/
	void inorder(avl_node* tree);

	/*
		preorder displays the IDs of the Tree's Dogs by preorder traversal
		@param	-	tree is the root of the Tree
	*/
	void preorder(avl_node* tree);
	
	/*
		postorder displays the IDs of the Tree's Dogs by postorder traversal
		@param	-	tree is the root of the Tree
	*/
	void postorder(avl_node* tree);

public:
	/*
		avlTree constructor sets root to null
	*/
	avlTree()
	{
		root = NULL;
	}

	/*
		~avlTree destructor resets keyNumGenerator to 1 and calls destroySubTree to destroy the Tree
	*/
	~avlTree()
	{
		Dog::setKeyNumGenerator(1);
		destroySubTree(root);
	}

	/*
		getHeight returns the height of the Tree
		@return	-	height is the height of the Tree
	*/
	int getHeight()
	{
		return height(root);
	}

	/*
		insert adds a pointer to a Dog to the Tree in its correct sorted position, while keeping the Tree at minimum height
		@param	-	dog is the pointer to a Dog to be added to the Tree
		@post	-	root is determined through rotations to form the minimum height Tree
		@post	-	dog has been added to the Tree in its correct sorted position and the Tree has been balanced accordingly
	*/
	void insert(Dog *dog)
	{
		root = insert(root, dog);
	}

	/*
		deleteNode deletes a pointer to a Dog from the Tree if found, and recycles the Dog's ID into the nextAvailable queue for later use
		@param	-	dogId is the ID of the Dog to be removed
		@post	-	root is determined through rotations to form the minimum height Tree
		@post	-	the Dog with the ID dogId has been removed from the Tree, dogId has been pushed into the nextAvailable queue, and the Tree has been balanced accordingly
	*/
	void deleteNode(string dogId)
	{
		bool removed = false;
		bool *ptr_removed = &removed;
		root = deleteNode(root, dogId, ptr_removed);
		
		//If a Dog was found and removed, put its Id in the Queue
		if (removed == true)
			Dog::setNextAvailable(dogId);
	}

	/*
		display displays the indented search Tree
		@param	-	n is the Tree level to start at
	*/
	void display(int n)
	{
		if (root == NULL)
			cout << "Tree is Empty" << endl;
		else
		{
			cout << "Balanced AVL Tree:" << endl;
			display(root, n);
		}
	}

	/*
		inorder displays the IDs of the Tree's Dogs by inorder traversal
	*/
	void inorder()
	{
		inorder(root);
	}

	/*
		preorder displays the IDs of the Tree's Dogs by preorder traversal
	*/
	void preorder()
	{
		preorder(root);
	}

	/*
		postorder displays the IDs of the Tree's Dogs by postorder traversal
	*/
	void postorder()
	{
		postorder(root);
	}
};
