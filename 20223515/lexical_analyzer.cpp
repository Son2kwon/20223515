#include"lexical_analyzer.h"
#include<sstream>
#include<iostream>
#include<vector>
using namespace std;

int next_token;
string token_string;
size_t current_index;
extern vector<string> tokenStream;

// 숫자인지 판단하는 함수
bool isNumber(const string& str) {
	try {
		size_t pos;
		std::stoi(str, &pos);
		return pos == str.length();  // If pos is equal to the string length, the entire string was converted
	}
	catch (const std::invalid_argument& e) {
		return false;  // Conversion failed
	}
	catch (const std::out_of_range& e) {
		return false;  // Value is out of the range of representable values for the int type
	}
}

void lexical() {
	if (current_index < tokenStream.size()) {
		token_string = tokenStream[current_index];

		if (token_string == "(") {
			next_token = LEFT_PAREN;
		}

		else if (token_string == ")") {
			next_token = RIGHT_PAREN;
		}

		else if (token_string == ":=") {
			next_token = ASSIGN_OP;
		}

		else if (token_string == "+") {
			next_token = ADD_OP;
		}

		else if (token_string == "-") {
			next_token = SUB_OP;
		}

		else if (token_string == "*") {
			next_token = MULT_OP;
		}

		else if (token_string == "/") {
			next_token = DIV_OP;
		}

		else if (token_string == ":=") {
			next_token = ASSIGN_OP;
		}

		else if (isNumber(token_string)) {
			next_token = CONSTANT;
		}

		else if (token_string == ";") {
			next_token = SEMI;
		}

		else {
			next_token = IDENT;
		}

		++current_index;
	}

	else {
		next_token = EOF;
		token_string = "EOF";
	}
}