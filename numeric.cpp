/*
 * eval.cpp
 *
 *  Created on: Sep 18, 2017
 *      Author: andrewratz
 */

#include <stdio.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <list>
#include "eval.h"

using namespace std;

int main() {
  //User input string
	string all;
  //List of string tokens
	list<string> tokens;
  //If input was not piped in using the '<' key in the command line
 	if (isatty(fileno(stdin))) {
 		cout << "Enter a sexp or type \"done\" to quit" << endl;
	}
  //Read input
 	getline(cin, all);
  //If user types done, quit
 	if (all.find("done") != string::npos) {
 		exit(0);
 	}
  //Max length an array would need to be
  int length = all.length();
  //Stack of the tokens
  Stack *stack = new Stack(length);
  //Tokenize the input string
  tokenize( all, tokens );
  int i = 0;
  for (auto token:tokens) {
    stack->push(token);
  }
  tokens.clear();
  //Keeps track of the child count that gets passed in to operator nodes 
 	int children = 0;
  //Helps keep count of parentheses
 	int rightmost = 0;
  //Operator nodes
 	AdditionNode* add;
 	SubtractionNode* sub;
 	MultiplicationNode* mult;
 	DivisionNode* div;
 	NumericNode num[length];
 	NumericNode final_array[length];
  //Last_operator keeps track of which operator is up
 	string last_operator = "";
 	int index = 0;
  int final_index = 0;
  //While there are still tokens
 	while (stack->is_empty() == false) {
    //Pop the most recent token (rightmost) and sets it as the current token
 		string current = stack->pop();
  		if (current == "(" || current == ")") {
  			rightmost++;
      //Perform add operation
 			if (last_operator == "+") {
        if (children == 0) {
        add = new AdditionNode(index, final_array);
      }
      else {
        add = new AdditionNode(children, num);
      }
 				num[children].setValue(add->evaluate());
 				delete(add);
  				final_array[index] = num[children];
 				index++;
 			}
      //Perform subtradct operation
 			else if (last_operator == "-") {
        if (children == 0) {
        sub = new SubtractionNode(index, final_array);
      }
      else {
          sub = new SubtractionNode(children, num);
      }
 				num[children].setValue(sub->evaluate());
 				delete(sub);
   				final_array[index] = num[children];
 				index++;
 			}
      //Perform multiply operation
 			else if (last_operator == "*") {
        if (children == 0) {
        mult = new MultiplicationNode(index, final_array);
      }
      else {
        mult = new MultiplicationNode(children, num);
      }
 				num[children].setValue(mult->evaluate());
 				delete(mult);
   				final_array[index] = num[children];
 				index++;
 			}
      //Perform divide operation
 			else if (last_operator == "/") {
        if (children == 0) {
        div = new DivisionNode(index, final_array);
      }
      else {
        div = new DivisionNode(children, num);
      }
 				num[children].setValue(div->evaluate());
 				delete(div);
   				final_array[index] = num[children];
 				index++;
 			}
      //Reset variables
 			last_operator = ")";
      final_index = children;
      children = 0;
    }
    //Set the most recent operator to the current
 		else if (current == "+") {
 			last_operator = current;
 		}
 		else if (current == "-") {
 			last_operator = current;
 		}
 		else if (current == "*") {
      last_operator = current;
 		}
 		else if (current == "/") {
      last_operator = current;
 		}
    //If not a parentheses or an operator, then it must be an operand (number)
 		else {
      //Convert the string to a double
 			num[children].setValue(strtod(current.c_str(), NULL));
      //Deals with double right parentheses without an operator
 			if (rightmost <= 1) {
 				final_array[index] = num[children];
 				index++;
 			}
      //Increments the child count
 			children++;
 		}
 	}
  //Outputs the solution
 	cout << num[final_index].evaluate() << endl;
}
