#include <iostream>
#include <string>

class Node
{
public:
	int name;
	Node* left = NULL;
	Node* right = NULL;
};

Node* insert(Node* root, int key)
{
	if (root == NULL)
	{
		Node* temp = new Node();
		temp->name = key;
		return temp;
	}
	if (key < root->name)
		root->left = insert(root->left, key);
	else if (key > root->name)
		root->right = insert(root->right, key);
	return root;
}

std::string traverse(Node* head)
{
	std::string string1, string2, string3;
	if (head == NULL)
		return "";
	string1 = traverse(head->left);
	string2 = std::to_string(head->name);
	string3 = traverse(head->right);
	return string1 + string2 + string3;
}

/*********************************************************/
Node* rotateLeft(Node *node)
{
	Node* grandchild = node->right->left;
	Node* newParent = node->right;
	newParent->left = node;
	node->right = grandchild;
	return newParent;
}
Node* rotateRight(Node *node)
 {
	Node* grandchild = node->left->right;
	Node* newParent = node->left;
	newParent->right = node;
	node->left = grandchild;
	return newParent;
}

Node* rotateRightLeft(Node *node)
{
	// "node" is the ROOT of the  tree
	Node* newRoot = node;
	// Do Right Rotation on right child of "node"
	newRoot->right = rotateRight(newRoot->right);
	// Do Left Rotation on right child of "node"
	newRoot = rotateLeft(newRoot);
	return newRoot;
}

int main()
{
	Node* root = NULL;
	int x;
	int num;
	std::cin >> num;
	for (int i = 0;i < num;i++)
	{
		std::cin >> x;
		root = insert(root, x);
	}
	root = rotateRightLeft(root);
	std::cout << traverse(root);
}