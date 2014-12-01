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

#include "DogTree.h"

/*
Height of AVL Tree
*/
int avlTree::height(avl_node *temp)
{
	int h = 0;
	if (temp != NULL)
	{
		int l_height = height(temp->left);
		int r_height = height(temp->right);
		int max_height = max(l_height, r_height);	//max requires #include <algorithm>
		h = max_height + 1;
	}
	return h;
}

/*
Height Difference
*/
int avlTree::diff(avl_node *temp)
{
	int l_height = height(temp->left);
	int r_height = height(temp->right);
	int b_factor = l_height - r_height;
	return b_factor;
}

/*
Right- Right Rotation
*/
avl_node *avlTree::rr_rotation(avl_node *parent)
{
	avl_node *temp;
	temp = parent->right;
	parent->right = temp->left;
	temp->left = parent;
	return temp;
}

/*
Left- Left Rotation
*/
avl_node *avlTree::ll_rotation(avl_node *parent)
{
	avl_node *temp;
	temp = parent->left;
	parent->left = temp->right;
	temp->right = parent;
	return temp;
}

/*
Left- Right Rotation
*/
avl_node *avlTree::lr_rotation(avl_node *parent)
{
	avl_node *temp;
	temp = parent->left;
	parent->left = rr_rotation(temp);
	return ll_rotation(parent);
}

/*
Right- Left Rotation
*/
avl_node *avlTree::rl_rotation(avl_node *parent)
{
	avl_node *temp;
	temp = parent->right;
	parent->right = ll_rotation(temp);
	return rr_rotation(parent);
}

/*
Balancing AVL Tree
*/
avl_node *avlTree::balance(avl_node *temp)
{
	int bal_factor = diff(temp);
	if (bal_factor > 1)
	{
		if (diff(temp->left) > 0)
			temp = ll_rotation(temp);
		else
			temp = lr_rotation(temp);
	}
	else if (bal_factor < -1)
	{
		if (diff(temp->right) > 0)
			temp = rl_rotation(temp);
		else
			temp = rr_rotation(temp);
	}
	return temp;
}

/*
Insert Element into the tree
*/
avl_node *avlTree::insert(avl_node *root, Dog *dog)
{
	if (root == NULL)
	{
		root = new avl_node;
		root->data = dog;
		root->left = NULL;
		root->right = NULL;
		return root;
	}
	else if (dog->getID() < root->data->getID())
	{
		root->left = insert(root->left, dog);
		root = balance(root);
	}
	else if (dog->getID() >= root->data->getID())
	{
		root->right = insert(root->right, dog);
		root = balance(root);
	}
	return root;
}

/*
Delete an Element from the tree
*/
avl_node *avlTree::deleteNode(avl_node* root, string dogId, bool *ptr_removed)
{
	//This pointer keeps track of whether a removal actually occurs
	*ptr_removed = false;

	if (root == NULL)
		return root;

	// If the key to be deleted is smaller than the root's key,
	// then it lies in left subtree
	if (dogId < root->data->getID())
		root->left = deleteNode(root->left, dogId, ptr_removed);

	// If the key to be deleted is greater than the root's key,
	// then it lies in right subtree
	else if (dogId > root->data->getID())
		root->right = deleteNode(root->right, dogId, ptr_removed);

	// if key is same as root's key, then this is the node to be deleted
	else
	{
		// node with only one child or no child
		if ((root->left == NULL) || (root->right == NULL))
		{
			avl_node *temp;

			if (root->left)
				temp = root->left;
			else
				temp = root->right;

			//avl_node *temp = root->left ? root->left : root->right;

			// No child case
			if (temp == NULL)
			{
				temp = root;
				root = NULL;
			}
			else // One child case
				*root = *temp; // Copy the contents of the non-empty child

			//The removal occured, set ptr_removed to true
			*ptr_removed = true;
			delete temp;
		}
		else
		{
			// node with two children: Get the inorder successor (smallest
			// in the right subtree)
			avl_node* temp = minValueNode(root->right);

			// Copy the inorder successor's data to this node
			root->data = temp->data;

			// Delete the inorder successor
			root->right = deleteNode(root->right, temp->data->getID(), ptr_removed);
		}
	}

	// If the tree had only one node then return
	if (root == NULL)
		return root;

	root = balance(root);
	return root;
}

/*
Find the leftmost node in the subtree
*/
avl_node *avlTree::minValueNode(avl_node* node)
{
	avl_node* current = node;

	/* loop down to find the leftmost leaf */
	while (current->left != NULL)
		current = current->left;

	return current;
}

/*
Recursively destroy the Tree
*/
void avlTree::destroySubTree(avl_node* temp)
{
	if (temp == NULL)
		return;

	//Recursively call destroySubTree on the left subtree
	destroySubTree(temp->left);

	//Recursively call destroySubTree on the right subtree
	destroySubTree(temp->right);

	//Delete the node
	delete temp;
}

/*
Display AVL Tree
*/
void avlTree::display(avl_node *ptr, int level)
{
	int i;
	if (ptr != NULL)
	{
		display(ptr->right, level + 1);
		printf("\n");
		if (ptr == root)
			cout << "Root -> ";
		for (i = 0; i < level && ptr != root; i++)
			cout << "        ";
		cout << ptr->data->getID();
		display(ptr->left, level + 1);
	}
}

/*
* Inorder Traversal of AVL Tree
*/
void avlTree::inorder(avl_node *tree)
{
	if (tree == NULL)
		return;
	inorder(tree->left);
	cout << tree->data->toString() << endl;
	inorder(tree->right);
}
/*
* Preorder Traversal of AVL Tree
*/
void avlTree::preorder(avl_node *tree)
{
	if (tree == NULL)
		return;
	cout << tree->data->toString() << endl;
	preorder(tree->left);
	preorder(tree->right);
}

/*
* Postorder Traversal of AVL Tree
*/
void avlTree::postorder(avl_node *tree)
{
	if (tree == NULL)
		return;
	postorder(tree->left);
	postorder(tree->right);
	cout << tree->data->toString() << endl;
}