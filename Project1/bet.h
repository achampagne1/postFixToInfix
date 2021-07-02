#ifndef _betH_
#define _betH_

#include <string>
#include <iostream>
#include <sstream>
#include <stack>

class BET {
private:

	class BinaryNode {
	public:
		BinaryNode* left = nullptr;
		BinaryNode* right = nullptr;
		std::string data = "";
		BinaryNode(BinaryNode* left, BinaryNode* right, std::string data) {
			this->left = left;
			this->right = right;
			this->data = data;
		}
	};

public:
	BET() {}
	BET(const std::string& postfix) {
		if (buildFromPostfix(postfix))
			std::cout << "build successful" << std::endl;
		else
			std::cout << "build failed" << std::endl;
	}
	BET(const BET& input) {
		root = clone(input.root);
	}
	~BET() {
		empty();
	}
	bool buildFromPostfix(const std::string& postfix) { 
		if (size() != 0)
			empty();

		std::stack<BET*> operands;
		std::istringstream buf(postfix);
		for (std::string word; buf >> word; ) { //goes through each character of string delimited by spaces
			BET* temp = new BET();
			temp->root = new BinaryNode{ nullptr,nullptr,word };
			if (isOperator(word)) {
				if (operands.size()<2) {
					std::cout << "too many operators" << std::endl;
					return 0;
				}
				BET* treePointer = operands.top();
				temp->root->left = clone(treePointer->root);
				operands.pop();
				treePointer = operands.top();
				temp->root->right = clone(treePointer->root);
				operands.pop();
			}
			operands.push(temp);
		}
		if (operands.size() > 1) {
			std::cout << "Too many operands" << std::endl;
			return 0;
		}
		BET* treePointer = operands.top();
		root = clone(treePointer->root);
		return 1;
	}
	const BET& operator= (const BET& input) {
		BET* temp = new BET;
		temp->root = clone(input.root);
		return *temp;
	}
	void printInfixExpression() {
		std::cout << "infix: ";
		printInfixExpression(root);
		std::cout<<'\n';
	}
	void printPostfixExpression() {
		std::cout << "postfix: ";
		printPostfixExpression(root);
		std::cout <<'\n';
	}
	size_t size() {
		return size(root);

	}
	size_t leaf_nodes() {
		return leaf_nodes(root)/2;
	}
	bool empty() {
		makeEmpty(root);
		root = nullptr;
		return 1;
	}

private:
	BinaryNode *root = nullptr;

	int precidence(std::string input) {
		if (input == "*" || input == "/")
			return 2;
		else if (input == "+" || input == "-")
			return 1;
		else
			return 3;
	}
	bool isOperator(std::string input) {
		if ((input == "*" || input == "/" || input == "+" || input == "-"))
			return 1;
		return 0;
	}
	void printInfixExpression(BinaryNode* n) {
		if (n == nullptr)
			return;
		else {
			if (isOperator(n->data) && (precidence(n->data) > precidence(n->right->data)) ) {
				std::cout << "( ";
				printInfixExpression(n->right);
				std::cout << ") ";
			}
			else
				printInfixExpression(n->right);

			std::cout << n->data << " ";

			if (isOperator(n->data) && precidence(n->data) >= precidence(n->left->data)) {
				std::cout << "( ";
				printInfixExpression(n->left);
				std::cout << ") ";
			}
			else
				printInfixExpression(n->left);
		}
	}
	void makeEmpty(BinaryNode*& t) {
		if (t == nullptr)
			return;
		else {
			makeEmpty(t->left);
			makeEmpty(t->right);
			delete t;
		}
	}
	BinaryNode* clone(BinaryNode* t) {
		if (t == nullptr)
			return nullptr;
		else
			return new BinaryNode{clone(t->left),clone(t->right),t->data };
	}
	void printPostfixExpression(BinaryNode* n) {
		if (n == nullptr)
			return;
		else {
			printPostfixExpression(n->right);
			printPostfixExpression(n->left);
			std::cout << n->data << " ";
		}
	}
	size_t size(BinaryNode* t) {
		size_t sizeOf = 0;
		if (t == nullptr) {
			return 0;
		}
		else {
			sizeOf++;
			sizeOf += size(t->right);
			sizeOf += size(t->left);
		}
	}
	size_t leaf_nodes(BinaryNode* t) {
		size_t sizeOf = 0;
		if (t == nullptr) {
			return 1;
		}
		else {
			sizeOf = leaf_nodes(t->left);
			sizeOf += leaf_nodes(t->right);
		}
	}
};

#endif