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
extern int type;

// 연산자인지 판단하는 함수
bool isOperator(const std::string& token) {
	return token == "+" || token == "-" || token == "*" || token == "/";
}

// 결과를 출력하는 함수
void printResult() {																	// 각 문장의 결과를 출력하는 함수
	for (string token : cur_sentence.lexemes) {											// 각 문장을 출력하기 위해 token들을 출력
		cout << token << " ";
	}

	cout << endl;

	cout << "ID: " << cur_sentence.ID << "; ";											// 이 문장의 Identifier의 개수
	cout << "CONST: " << cur_sentence.CONST << "; ";									// 이 문장의 constant 개수
	cout << "OP: " << cur_sentence.OP << ";" << endl;									// 이 문장의 operator 개수

	vector<string> temp(cur_sentence.lexemes.begin() + 2, cur_sentence.lexemes.end());	// 이 문장의 연산 결과를 위해 temp라는 변수에 := 이후를 저장
	Sentence temp_sentence; temp_sentence.lexemes = temp;								// Sentence에 있는 연산 함수를 쓰기 위한 임시 변수
	int result = temp_sentence.calculateExpression(table);								// 연산 결과를 저장

	if (result == UNKNOWN) {															// 정의되지 않은 변수 찾기
		string errorString;	

		for (string str : temp) {
			if (isNumber(str) || isOperator(str) || str == "(" || str == ")" || str == ";") continue;

			else {
				if (!table.isThere(str)) {
					errorString = str;

					varData tempDatum; tempDatum.name = errorString; tempDatum.value = UNKNOWN;
					table.datum.push_back(tempDatum);

					cout << "(Error) \" 정의되지 않은 변수(" << errorString << ") 가 참조됨\"" << endl;
					break;
				}

				else if (table.isChecked(str)) {
					continue;
				}
			}
		}
		
		if (table.isChecked(errorString)) {
			cout << "(OK)" << endl;
			table.setChecked(errorString);
		}

		

		varData datum; datum.name = cur_sentence.lexemes[0]; datum.value = result;			// 새로운 구조체를 만들어 변수 이름과 value를 저장
		table.setChecked(cur_sentence.lexemes[0]);
		table.datum.push_back(datum);														// table에 저장

		cur_sentence.initialize();															// 다음 문장을 위해 초기화
	}

	else if (cur_sentence.warning) {
		cout << "(Warning) \"중복 연산자(" << cur_sentence.warning_sign << ") 제거\"" << endl;

		varData datum; datum.name = cur_sentence.lexemes[0]; datum.value = result;			// 새로운 구조체를 만들어 변수 이름과 value를 저장
		table.datum.push_back(datum);														// table에 저장
		table.setChecked(cur_sentence.lexemes[0]);
		cur_sentence.initialize();															// 다음 문장을 위해 초기화
	}

	else {
		cout << "(OK)" << endl;

		varData datum; datum.name = cur_sentence.lexemes[0]; datum.value = result;			// 새로운 구조체를 만들어 변수 이름과 value를 저장
		table.datum.push_back(datum);														// table에 저장
		table.setChecked(cur_sentence.lexemes[0]);
		cur_sentence.initialize();															// 다음 문장을 위해 초기화
	}
}

// 최종 결과를 출력하는 함수
void printTotalResult() {
	cout << "Result => ";

	for (varData data : table.datum) {
		if (data.value == UNKNOWN) {	// data_value가 unknown이면 Unknown 출력
			cout << data.name << ": " << "Unknown" << "; ";
		}
		else cout << data.name << ": " << data.value << "; ";
	}

	cout << endl;
}

// tokenStream에 token_string(현재 token)이 있는지 확인하는 함수
bool isThere(const string target) {
	for (string token : tokenStream) {
		if (token == target) return true;
	}

	return false;
}

// <program> → <statements>
void program() {
	lexical();
	if (type == typeB) {
		cout << next_token << endl;
	}
	statements();

	if (type == typeA) 
		printResult();
	
	
	if (type == typeA)
		printTotalResult();
}

// <statements> → <statement> | <statement><semi_colon><statements>
void statements() {
	/*

	*/
	statement();

	if (isThere(";") && next_token != EOF) {												// semicolon이 없다면 마지막 문장이므로 끝 /  있다면 parsing 계속
		semi_colon();
		statements();
	}

}

void semi_colon() {
	//cout << "Semi Colon: " << token_string << endl;
	cur_sentence.lexemes.push_back(token_string);
	printResult();
	if(next_token != EOF) lexical();	// 끝나지 않았다면 update
}

// <statement> → <ident><assignment_op><expression>
void statement() {
	/*

	*/
	if (next_token == IDENT) {
		ident();
		assignment_op();
		expression();
	}
	
	else {
		
	}
}

// <ident> → any names conforming to C identifier rules
void ident() {
	//cout << "Identifier: " << token_string << endl;
	cur_sentence.ID++;
	cur_sentence.lexemes.push_back(token_string);
	lexical();
	if (type == typeB) cout << next_token << endl;
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
		if (next_token == ADD_OP) {
			cur_sentence.warning = true;
			cur_sentence.warning_sign = token_string;
			lexical();
		}
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
		if (next_token == LEFT_PAREN) {
			cur_sentence.warning = true;
			cur_sentence.warning_sign = token_string;
			lexical();
		}
		expression();
		right_paren();
		if (next_token == RIGHT_PAREN) {
			cur_sentence.warning = true;
			cur_sentence.warning_sign = token_string;
			lexical();
		}
	}

	else if (next_token == IDENT) {
		ident();
	}

	else if (isNumber(token_string)) {
		constant();
	}
}

// <factor_tail> → <mult_op><factor><factor_tail> | ε
void factor_tail() {
	if (next_token == MULT_OP) {
		mult_operator();
		if (next_token == MULT_OP) {
			cur_sentence.warning = true;
			cur_sentence.warning_sign = token_string;
			lexical();
		}
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
	if (type == typeB) cout << next_token << endl;
}

void mult_operator() {
	//cout << "Mul operator: " << token_string << endl;
	cur_sentence.OP++;
	cur_sentence.lexemes.push_back(token_string);
	if (next_token != EOF) lexical();
	if (type == typeB) cout << next_token << endl;
}

void left_paren() {
	//cout << "Left paren: " << token_string << endl;
	cur_sentence.lexemes.push_back(token_string);
	if (next_token != EOF) lexical();
	if (type == typeB) cout << next_token << endl;
}

void right_paren() {
	//cout << "Right paren: " << token_string << endl;
	cur_sentence.lexemes.push_back(token_string);
	if (next_token != EOF) lexical();
	if (type == typeB) cout << next_token << endl;
}

void constant() {
	//cout << "Constant: " << token_string << endl;
	cur_sentence.CONST++;
	cur_sentence.lexemes.push_back(token_string);
	if (next_token != EOF) lexical();
	if (type == typeB) cout << next_token << endl;
}