#pragma once
#include<iostream>
#include<sstream>
#include<vector>
#include<stack>
using namespace std;

/* Character classes */
#define EOF -1
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */
#define CONSTANT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define SEMI 27

typedef struct data {
    string name;
    int value;
}varData;

class symbol_table {	// <ident>�� ���簪�� �����ϱ� ���� symbol table, sentences ����
public:
    vector<varData> datum;

    bool isThere(const string& target) {	// identifier�� ������ �ִ� ������ Ȯ��.
        for (varData& cur : datum) {
            if (cur.name == target) {
                return true;
            }
        }

        return false;
    }

    int getValue(string target) {
        for (varData data : datum) {
            if (data.name == target) return data.value;
        }
    }
};

// �� ���忡 ���� ������ �����ϴ� class
class Sentence {
public:
	int ID;							// �� ������ identifier ����
	int CONST;						// �� ������ constant ����
	int OP;							// �� ������ operator ����
	vector<string> lexemes;			// ������ lexemes

	Sentence(string sentence) {	// constructor
		this->lexemes = tokenize(sentence);
		this->ID = 0;
		this->CONST = 0;
		this->OP = 0;
	}

	Sentence() {
		this->ID = 0;
		this->CONST = 0;
		this->OP = 0;
	}

	void initialize() {
		this->lexemes.clear();
		this->ID = 0;
		this->CONST = 0;
		this->OP = 0;
	}

    int calculateExpression(const vector<string>& lexemes, symbol_table table) {
        std::stack<int> operandStack;
        std::stack<std::string> operatorStack;

        for (const std::string& token : lexemes) {
            if (isdigit(token[0])) {  // ��ū�� ������ ���
                operandStack.push(std::stoi(token));
            }
            else if (isOperator(token)) {  // ��ū�� �������� ���
                while (!operatorStack.empty() && hasHigherPrecedence(operatorStack.top(), token)) {
                    processOperator(operandStack, operatorStack);
                }
                operatorStack.push(token);
            }
            else if (token == "(") {
                operatorStack.push(token);
            }
            else if (token == ")") {
                while (!operatorStack.empty() && operatorStack.top() != "(") {
                    processOperator(operandStack, operatorStack);
                }
                operatorStack.pop();  // '(' ����
            }
            else {
                if (table.isThere(token)) {
                    operandStack.push(table.getValue(token));
                }
                else {
                    //operandStack.push(UNKNOWN);
                }
            }
        }

        while (!operatorStack.empty()) {
            processOperator(operandStack, operatorStack);
        }

        return operandStack.top();  // ���� ��� ��ȯ
    }

    bool isOperator(const std::string& token) {
        return token == "+" || token == "-" || token == "*" || token == "/";
    }

    bool hasHigherPrecedence(const std::string& op1, const std::string& op2) {
        return (op1 == "*" || op1 == "/") && (op2 == "+" || op2 == "-");
    }

    void processOperator(std::stack<int>& operandStack, std::stack<std::string>& operatorStack) {
        int operand2 = operandStack.top();
        operandStack.pop();

        int operand1 = operandStack.top();
        operandStack.pop();

        std::string op = operatorStack.top();
        operatorStack.pop();

        if (op == "+") {
            operandStack.push(operand1 + operand2);
        }
        else if (op == "-") {
            operandStack.push(operand1 - operand2);
        }
        else if (op == "*") {
            operandStack.push(operand1 * operand2);
        }
        else if (op == "/") {
            operandStack.push(operand1 / operand2);
        }
    }

private:
	vector<string> tokenize(string sentence) {						// tokenize�ϴ� �Լ�
		stringstream ss(sentence);
		string token;												// lexeme�� �����ϱ� ���� �ӽ� ����

		vector<string> result_lexemes;
		while (ss >> token) result_lexemes.push_back(token);		// lexeme ����

		return result_lexemes;
	}

	/*
	�߻��� �� �ִ� ����

	1. ���ǵ��� ���� ������ ���					-> ���� �Ұ���
	2. �ߺ� ������(+, *)							-> ���� ����
	3. identifier �ڿ� identifier�� ������ ���.	-> ���� �Ұ���
	4. keyword�� ����� ���						-> ���� �Ұ���
	*/
};

void lexical();
bool isNumber(const string& str);