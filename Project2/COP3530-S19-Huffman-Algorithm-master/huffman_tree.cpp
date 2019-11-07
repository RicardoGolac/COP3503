#include "huffman_tree.h"
#include <string>
#include <queue>
#include <sstream>
#include <iostream>
#include <fstream>
#include <map>

Node::Node() {

}

Node::~Node() {

}

/*
Preconditions: file_name is the name of (and possibly path to) a text file
Postconditions: Reads the contents of file_name and constructs a
				huffman tree based on the character frequencies of the file contents
*/

huffman_tree::huffman_tree(const std::string &file_name) {
	// open the file given
	// read file contents and create queue of characters in the txt file
	std::ifstream file(file_name);
	std::priority_queue<Node*, std::vector<Node*>, comparator> q;
	char c;

	// plug character frequencies into array of ascii values
	int asciiTab[128];
	for (int i = 0; i < 128; i++)
		asciiTab[i] = 0;

	if (file.is_open()) 
	{
		while (file.good()) 
		{
			c = file.get();
			if (c == -1)
				break;
			int asVal = (int)c;
			asciiTab[asVal]++;
		}
		file.close();
	}
	
	// Build priority queue with frequency array
	for (int i = 0; i < 128; i++)
	{
		if (asciiTab[i] > 0)
		{
			int frequency = asciiTab[i];
			char cha = (char)i;
			Node* temp = new Node;
			temp->leaf = true;
			temp->freq = frequency;
			temp->character = cha;
			q.push(temp);
			totChars++; // keep track of total unique characters
		}
	}

	//std::cout << "Priority queue created" << std::endl;
	// form a huffman tree
	// unload from queue and build tree, node by node
	// build up from smallest freq nodes

	if (q.size() == 0) {
		Node* temp = q.top();
		root = temp;
		q.pop();
	}
	while (q.size() != 0)
	{
		if (q.size() >= 2)
		{
			Node* first = q.top();
			q.pop();
			Node *second = q.top();
			q.pop();
			if (first->freq > second->freq)
			{
				Node* temp = new Node;
				temp->leaf = false;
				temp->freq = first->freq + second->freq;
				temp->right = first;
				temp->right->code = 1;
				temp->left = second;
				temp->left->code = 0;
				q.push(temp);
			}
			else if (first->freq < second->freq)
			{
				Node* temp = new Node;
				temp->leaf = false;
				temp->freq = first->freq + second->freq;
				temp->left = first;
				temp->left->code = 0;
				temp->right = second;
				temp->right->code = 1;
				q.push(temp);
			}
			else if (first->freq == second->freq)
			{
				Node* temp = new Node;
				temp->leaf = false;
				temp->freq = first->freq + second->freq;
				temp->left = first;
				temp->left->code = 0;
				temp->right = second;
				temp->right->code = 1;
				q.push(temp);
			}
		}
		// set root of huffman tree to top node;
		else if (q.size() == 1) {
			//std::cout << "Root Node created!" << std::endl;
			root = q.top();
			q.pop();
		}
	}
	std::string tempstr = "";
	storeCodes(root, tempstr);
}

// Destructor
huffman_tree::~huffman_tree() {

}

// Recursive Helper Function
void huffman_tree::storeCodes(Node* node, std::string str)
{
	//std::map<int, std::string> tempcodes;
	if (node == nullptr)
		return;
	if (node->leaf == true)
	{
		char cha = node->character;
		if (totChars == 1)
			codes.emplace(node->character, "0");
		else
			codes.emplace(cha, str);
	}

	storeCodes(node->left, str + '0');
	storeCodes(node->right, str + '1');
}

/*
Preconditions: Character is a character with an ASCII value
				between 0 and 127 (inclusive).
Postconditions: Returns the Huffman code for character if character is in the tree
				and an empty string otherwise.
*/
std::string huffman_tree::get_character_code(char character) const {
	Node* temp = root;

	// Tree of size 1 will return 0;
	if (temp->left == nullptr && temp->right == nullptr)
		return "0";
	std::map<char, std::string> copyMap = codes;

	for (std::map<char, std::string>::iterator it = copyMap.begin(); it != copyMap.end(); it++)
	{
		if (it->first == character)
			return it->second;
	}
	
	// return an empty string if char wasn't found in tree
	return "";
}

/*
Preconditions: file_name is the name of (and possibly path to) a text file
Postconditions: Returns the Huffman encoding for the contents of file_name
				if file name exists and an empty string otherwise.
				If the file contains letters not present in the huffman_tree,
				return an empty string
*/
std::string huffman_tree::encode(const std::string &file_name) const {
	std::ifstream file(file_name);
	std::string str = "";
	std::string text;
	std::string add;

	if (file.is_open()) {
		
		file.seekg(0, std::ios::end);
		file.seekg(0, std::ios::beg);
		text.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		for (unsigned int i = 0; i < text.length(); i++) {
			add = get_character_code(text.at(i));
			if (add != "") {
				str += add;
			}
			else {
				return "";
			}
		}
		file.close();
	}
	
	return str;	
}

/*
Preconditions: string_to_decode is a string containing Huffman-encoded text
Postconditions: Returns the plaintext represented by the string if the string
				is a valid Huffman encoding and an empty string otherwise
*/
std::string huffman_tree::decode(const std::string &string_to_decode) const {
	// first check that string has only 1s and 0s
	for (unsigned int i = 0; i < string_to_decode.length(); i++) {
		char temp = string_to_decode[i];
		if (temp != 48 && temp != 49)
			return "";
	}

	// deal with a string of one number
	if (string_to_decode.length() == 1)
	{
		std::string temp = "";
		temp += root->character;
		return temp;
	}

	std::string str = "";
	Node* temp = root;

	for (unsigned int i = 0; i < string_to_decode.length(); i++)
	{
		int num = (int)string_to_decode[i];
		
		if (num == 48)
			temp = temp->left;
		else
			temp = temp->right;

		if (temp->left == nullptr && temp->right == nullptr)
		{
			char cha = temp->character;
			str += cha;
			temp = root;
		}
	}
	return str;
}

