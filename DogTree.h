#include <iostream>
#include <cstdio>
#include <sstream>
#include <algorithm>
#include "Dog.h"
using namespace std;

//node structure
struct avl_node
{
	Dog *data;
	struct avl_node *left;
	struct avl_node *right;
};

class avlTree
{
private:
	//node
	avl_node *root;

	//private member functions
	int height(avl_node *);
	int diff(avl_node *);
	avl_node *rr_rotation(avl_node *);
	avl_node *ll_rotation(avl_node *);
	avl_node *lr_rotation(avl_node *);
	avl_node *rl_rotation(avl_node *);
	avl_node *balance(avl_node *);
	avl_node *insert(avl_node *, Dog *);	//root, done
	avl_node* deleteNode(avl_node*, string, bool*);
	avl_node *minValueNode(avl_node*);
	void destroySubTree(avl_node * temp);
	void display(avl_node *, int);		//root, done
	void inorder(avl_node *);
	void preorder(avl_node *);
	void postorder(avl_node *);

public:
	//constructor
	avlTree()
	{
		root = NULL;
	}

	~avlTree()
	{
		destroySubTree(root);
	}

	void clear()
	{
		destroySubTree(root);
	}

	//for the Efficiency Report
	int getHeight()
	{
		return height(root);
	}

	//insert
	void insert(Dog *dog)
	{
		root = insert(root, dog);
	}

	//delete
	void deleteNode(string dogId)
	{
		bool removed = false;
		bool *ptr_removed = &removed;
		root = deleteNode(root, dogId, ptr_removed);
		
		//If a Dog was found and removed, put its Id in the Queue
		if (removed == true)
			Dog::nextAvailable.push(dogId);
	}

	//display
	void display(int n)
	{
		if (root == NULL)
		{
			cout << "Tree is Empty" << endl;
		}
		else
		{
			cout << "Balanced AVL Tree:" << endl;
			display(root, n);
		}
	}

	//inorder
	void inorder()
	{
		inorder(root);
	}

	//preorder
	void preorder()
	{
		preorder(root);
	}

	//postorder
	void postorder()
	{
		postorder(root);
	}
};
