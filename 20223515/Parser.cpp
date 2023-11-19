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

// ���������� �Ǵ��ϴ� �Լ�
bool isOperator(const std::string& token) {
	return token == "+" || token == "-" || token == "*" || token == "/";
}

// ����� ����ϴ� �Լ�
void printResult() {																	// �� ������ ����� ����ϴ� �Լ�
	for (string token : cur_sentence.lexemes) {											// �� ������ ����ϱ� ���� token���� ���
		cout << token << " ";
	}

	cout << endl;

	cout << "ID: " << cur_sentence.ID << "; ";											// �� ������ Identifier�� ����
	cout << "CONST: " << cur_sentence.CONST << "; ";									// �� ������ constant ����
	cout << "OP: " << cur_sentence.OP << ";" << endl;									// �� ������ operator ����

	vector<string> temp(cur_sentence.lexemes.begin() + 2, cur_sentence.lexemes.end());	// �� ������ ���� ����� ���� temp��� ������ := ���ĸ� ����
	Sentence temp_sentence; temp_sentence.lexemes = temp;								// Sentence�� �ִ� ���� �Լ��� ���� ���� �ӽ� ����
	int result = temp_sentence.calculateExpression(table);								// ���� ����� ����

	if (result == UNKNOWN) {															// ���ǵ��� ���� ���� ã��
		string errorString;	

		for (string str : temp) {
			if (isNumber(str) || isOperator(str) || str == "(" || str == ")" || str == ";") continue;

			else {
				if (!table.isThere(str)) {
					errorString = str;

					varData tempDatum; tempDatum.name = errorString; tempDatum.value = UNKNOWN;
					table.datum.push_back(tempDatum);

					cout << "(Error) \" ���ǵ��� ���� ����(" << errorString << ") �� ������\"" << endl;
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

		

		varData datum; datum.name = cur_sentence.lexemes[0]; datum.value = result;			// ���ο� ����ü�� ����� ���� �̸��� value�� ����
		table.setChecked(cur_sentence.lexemes[0]);
		table.datum.push_back(datum);														// table�� ����

		cur_sentence.initialize();															// ���� ������ ���� �ʱ�ȭ
	}

	else if (cur_sentence.warning) {
		cout << "(Warning) \"�ߺ� ������(" << cur_sentence.warning_sign << ") ����\"" << endl;

		varData datum; datum.name = cur_sentence.lexemes[0]; datum.value = result;			// ���ο� ����ü�� ����� ���� �̸��� value�� ����
		table.datum.push_back(datum);														// table�� ����
		table.setChecked(cur_sentence.lexemes[0]);
		cur_sentence.initialize();															// ���� ������ ���� �ʱ�ȭ
	}

	else {
		cout << "(OK)" << endl;

		varData datum; datum.name = cur_sentence.lexemes[0]; datum.value = result;			// ���ο� ����ü�� ����� ���� �̸��� value�� ����
		table.datum.push_back(datum);														// table�� ����
		table.setChecked(cur_sentence.lexemes[0]);
		cur_sentence.initialize();															// ���� ������ ���� �ʱ�ȭ
	}
}

// ���� ����� ����ϴ� �Լ�
void printTotalResult() {
	cout << "Result => ";

	for (varData data : table.datum) {
		if (data.value == UNKNOWN) {	// data_value�� unknown�̸� Unknown ���
			cout << data.name << ": " << "Unknown" << "; ";
		}
		else cout << data.name << ": " << data.value << "; ";
	}

	cout << endl;
}

// tokenStream�� token_string(���� token)�� �ִ��� Ȯ���ϴ� �Լ�
bool isThere(const string target) {
	for (string token : tokenStream) {
		if (token == target) return true;
	}

	return false;
}

// <program> �� <statements>
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

// <statements> �� <statement> | <statement><semi_colon><statements>
void statements() {
	/*

	*/
	statement();

	if (isThere(";") && next_token != EOF) {												// semicolon�� ���ٸ� ������ �����̹Ƿ� �� /  �ִٸ� parsing ���
		semi_colon();
		statements();
	}

}

void semi_colon() {
	//cout << "Semi Colon: " << token_string << endl;
	cur_sentence.lexemes.push_back(token_string);
	printResult();
	if(next_token != EOF) lexical();	// ������ �ʾҴٸ� update
}

// <statement> �� <ident><assignment_op><expression>
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

// <ident> �� any names conforming to C identifier rules
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

// <expression> �� <term><term_tail>
void expression() {
	/*

	*/
	term();
	term_tail();
}

// <term> �� <factor> <factor_tail>
void term() {
	/*

	*/

	factor();
	factor_tail();
}

// <term_tail> �� <add_op><term><term_tail> | ��
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

// <factor> �� <left_paren><expression><right_paren> | <ident> | <const>
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

// <factor_tail> �� <mult_op><factor><factor_tail> | ��
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