/*
 * eval.h
 *
 *  Created on: Sep 18, 2017
 *      Author: andrewratz
 */

#ifndef EVAL_H_
#define EVAL_H_

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <list>
using namespace std;

//Virtual class Node
struct Node {
	//Constructor
	Node() {

	}
	//Destructor
	virtual ~Node() {

	}
	virtual double evaluate() = 0;
};

//NumericNode holds a double
struct NumericNode: public Node {
	double value;
	NumericNode() {
		value = 0;
	}
	NumericNode(double value) {
		this->value = value;
	}
	double evaluate() {
		return value;
	}
	void setValue(double value) {
		this->value = value;
	}
};

//Addition
struct AdditionNode: public Node {
	//Number of children
	int child_count;
	//Array of numeric nodes
	NumericNode* child;
	//Sets the values
	AdditionNode(int child_count, NumericNode child[]) {
		this->child_count = child_count;
		this->child = child;
	}
	//Evaluates the addition
	double evaluate() {
		if (child_count < 2) {
			cerr << ("invalid sexp") << endl;
			exit(0);
		}
		double accumulated_value = child[0].evaluate();
		for (size_t i = 1; i < child_count; ++i) {
			accumulated_value += child[i].evaluate();
		}
		return accumulated_value;
	}
};

//Subtraction
struct SubtractionNode: public Node {
	//Number of children
	int child_count;
	//Array of numeric nodes
	NumericNode* child;
	//Sets the values
	SubtractionNode(int child_count, NumericNode child[]) {
		this->child_count = child_count;
		this->child = child;
	}
	//Evaluates the subtraction
	double evaluate() {
		if (child_count < 2) {
			cerr << ("invalid sexp") << endl;
			exit(0);
		}
		double accumulated_value = child[child_count - 1].evaluate();
		for (size_t i = 0; i != child_count - 1; ++i) {
			accumulated_value -= child[i].evaluate();
		}
		return accumulated_value;
	}
};

//Multiplication
struct MultiplicationNode: public Node {
	//Number of children
	int child_count;
	//Array of numeric nodes
	NumericNode* child;
	//Sets the values
	MultiplicationNode(int child_count, NumericNode child[]) {
		this->child_count = child_count;
		this->child = child;
	}
	//Evaluates the multiplication
	double evaluate() {
		if (child_count < 2) {
			cerr << ("invalid sexp") << endl;
			exit(0);
		}
		double accumulated_value = child[0].evaluate();
		for (size_t i = 1; i != child_count; ++i) {
			accumulated_value *= child[i].evaluate();
		}
		return accumulated_value;
	}
};

//Division
struct DivisionNode: public Node {
	//Number of children
	int child_count;
	//Array of numeric nodes
	NumericNode* child;
	//Sets the values
	DivisionNode(int child_count, NumericNode child[]) {
		this->child_count = child_count;
		this->child = child;
	}
	//Evaluates the division
	double evaluate() {
		if (child_count < 2) {
			cerr << ("invalid sexp") << endl;
			exit(0);
		}
		double accumulated_value = child[child_count - 1].evaluate();
		for (size_t i = 0; i != child_count - 1; ++i) {
			accumulated_value /= child[i].evaluate();
		}
		return accumulated_value;
	}
};

//Stack for the tokens
class Stack {
public:
	string *backing_array;
	int array_size;
	Stack() {

	}
	Stack(int array_size) {
		backing_array = new string[array_size];
		for (int i = 0; i < array_size; i++) {
			backing_array[i] = "";
		}
		this->array_size = array_size;
	}
	void push(string token) {
		for (int i = 0; i < array_size; i++) {
			if (backing_array[i] == "") {
				backing_array[i] = token;
				return;
			}
		}
		cerr << "Full" << endl;
		return;
	}
	string pop() {
		for (int i = array_size-1; i >= 0; i--) {
			if (backing_array[i] != "") {
				string temp = backing_array[i];
				backing_array[i] = "";
				return temp;
			}
		}
		cerr << "Empty" << endl;
		return "";
	}
	void print() {
		for (int i = 0; i < array_size; i++) {
			cout << backing_array[i] << " ";
		}
	}
	bool is_empty() {
		for (int i = 0; i < array_size; i++) {
			if (backing_array[i] != "") {
				return false;
			}
		}
		return true;
	}
};

//Tokenizing the input (from lecture notes)
template <typename ContainerT>
void tokenize(const std::string& str, ContainerT& tokens,
 const std::string& delimiters = " ", bool trimEmpty = false)
{
 std::string::size_type pos, lastPos = 0, length = str.length();
 using value_type = typename ContainerT::value_type;
 using size_type = typename ContainerT::size_type;
 while(lastPos < length + 1)
 {
 pos = str.find_first_of(delimiters, lastPos);
 if(pos == std::string::npos)
 {
 pos = length;
 }
 if(pos != lastPos || !trimEmpty)
 tokens.push_back(value_type(str.data()+lastPos,
 (size_type)pos-lastPos ));
 lastPos = pos + 1;
 }
}

#endif /* EVAL_H_ */