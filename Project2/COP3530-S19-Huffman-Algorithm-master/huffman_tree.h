#ifndef _HUFFMAN_TREE_H_
#define _HUFFMAN_TREE_H_

#include <iostream>
#include <map>

class Node {
public:
	int freq = 0;
	char character;
	int code = 0;
	bool leaf = false; // this means that the node does not hold a valid character
	Node* left = nullptr;
	Node* right = nullptr;
	Node();
	~Node();
};

class comparator {
public:
	bool operator() (const Node* n1, const Node* n2) {
		return n1->freq > n2->freq;
	}
};

class huffman_tree {
public:
	Node* root = nullptr;
	std::map<char, std::string> codes;
	int totChars = 0;

	huffman_tree(const std::string &file_name);						// Constructor
	~huffman_tree();												// Destructor

	void storeCodes(Node* node, std::string);

	std::string get_character_code(char character) const;
	std::string encode(const std::string &file_name) const;
	std::string decode(const std::string &string_to_decode) const;
};

#endif