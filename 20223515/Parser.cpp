#include"Parser.h"
#include"lexical_analyzer.h"
#include<iostream>
#include<string>
#include<vector>
#include<cmath>
#include<algorithm>
using namespace std;

extern int next_token;
extern string token_string;
vector<string> tokenStream;
extern size_t current_index;
Sentence cur_sentence;
symbol_table table;

// 결과를 출력하는 함수
void printResult() {
	for (string token : cur_sentence.lexemes) {
		cout << token << " ";
	}

	cout << endl;

	cout << "ID: " << cur_sentence.ID << "; ";
	cout << "CONST: " << cur_sentence.CONST << "; ";
	cout << "OP: " << cur_sentence.OP << ";" << endl;


	vector<string> temp(cur_sentence.lexemes.begin() + 2, cur_sentence.lexemes.end());
	Sentence temp_sentence;
	int result = temp_sentence.calculateExpression(temp, table);
	varData datum; datum.name = cur_sentence.lexemes[0]; datum.value = result;

	table.datum.push_back(datum);

	cur_sentence.initialize();
}

void printTotalResult() {
	for (varData data : table.datum) {
		cout << data.name << ": " << data.value << "; ";
	}

	cout << endl;
}

bool isThere(const string target) {
	for (string token : tokenStream) {
		if (token == target) return true;
	}

	return false;
}

//bool look_ahead() {
//	
//}

// <program> → <statements>
void program() {
	lexical();
	statements();

	printResult();

	printTotalResult();
}

// <statements> → <statement> | <statement><semi_colon><statements>
void statements() {
	/*
	statements부터 하나하나 구분해서 넣어야 함
	look_ahead를 통해 ; 이 있나 확인 후 parsing
	*/
	statement();

	if (isThere(";") && next_token != EOF) {
		semi_colon();
		statements();
	}

}

void semi_colon() {
	//cout << "Semi Colon: " << token_string << endl;
	cur_sentence.lexemes.push_back(token_string);
	printResult();
	if(next_token != EOF) lexical();
}

// <statement> → <ident><assignment_op><expression>
void statement() {
	/*

	*/

	ident();
	assignment_op();
	expression();
}

// <ident> → any names conforming to C identifier rules
void ident() {
	//cout << "Identifier: " << token_string << endl;
	cur_sentence.ID++;
	cur_sentence.lexemes.push_back(token_string);
	lexical();
}

void assignment_op() {
	//cout << "Assignment operator: " << token_string << endl;
	cur_sentence.lexemes.push_back(token_string);
	if (next_token != EOF) lexical();
}

// <expression> → <term><term_tail>
void expression() {
	/*

	*/
	term();
	term_tail();
}

// <term> → <factor> <factor_tail>
void term() {
	/*

	*/

	factor();
	factor_tail();
}


// <term_tail> → <add_op><term><term_tail> | ε
void term_tail() {
	/*

	*/
	if (next_token == ADD_OP) {
		add_operator();
		term();
		term_tail();
	}

	else {
		
	}
}

// <factor> → <left_paren><expression><right_paren> | <ident> | <const>
void factor() {
	if (next_token == LEFT_PAREN) {
		left_paren();
		expression();
		right_paren();
	}

	else if (next_token == IDENT) {
		ident();
	}

	else constant();
}

// <factor_tail> → <mult_op><factor><factor_tail> | ε
void factor_tail() {
	if (next_token == MULT_OP) {
		mult_operator();
		factor();
		factor_tail();
	}

	else {
		
	}
}

void add_operator() {
	//cout << "Add operator: " << token_string << endl;
	cur_sentence.OP++;
	cur_sentence.lexemes.push_back(token_string);
	if (next_token != EOF) lexical();
}

void mult_operator() {
	//cout << "Mul operator: " << token_string << endl;
	cur_sentence.OP++;
	cur_sentence.lexemes.push_back(token_string);
	if (next_token != EOF) lexical();
}

void left_paren() {
	//cout << "Left paren: " << token_string << endl;
	cur_sentence.lexemes.push_back(token_string);
	if (next_token != EOF) lexical();
}

void right_paren() {
	//cout << "Right paren: " << token_string << endl;
	cur_sentence.lexemes.push_back(token_string);
	if (next_token != EOF) lexical();
}

void constant() {
	//cout << "Constant: " << token_string << endl;
	cur_sentence.CONST++;
	cur_sentence.lexemes.push_back(token_string);
	if (next_token != EOF) lexical();
}