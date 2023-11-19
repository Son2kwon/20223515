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

/*Print type*/
#define typeA 71
#define typeB 72

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

typedef struct data {   // identifier의 정보를 저장하는 구조체
    string name;        // identifier의 이름  
    int value;          // identifier의 값
}varData;

class symbol_table {	// <ident>의 현재값을 저장하기 위한 symbol table, sentences 단위
public:
    vector<varData> datum;                  // data 구조체를 이용한 vector, data들을 저장
    bool checked = false;                   // 이미 예외 처리가 된 identifier인지를 저장

    bool isThere(const string& target) {	// identifier가 기존에 있던 것인지 확인
        for (varData& cur : datum) {
            if (cur.name == target) {
                return true;
            }
        }

        return false;
    }

    int getValue(string target) {           // identifier의 값을 받아옴
        for (varData data : datum) {
            if (data.name == target) return data.value;
        }
    }

    void setChecked(const string& target) {
        for (varData& cur : datum) {
            if (cur.name == target) {
                this->checked = true;
            }
        }
    }

    bool isChecked(const string& target) {
        for (varData& cur : datum) {
            if (cur.name == target) {
                return this->checked;
            }
        }
    }
};

// 한 문장에 대해 정보를 저장하는 class
class Sentence {
public:
    int ID;							// 그 문장의 identifier 개수
    int CONST;						// 그 문장의 constant 개수
    int OP;							// 그 문장의 operator 개수
    vector<string> lexemes;			// 문장의 lexemes
    bool warning;
    string warning_sign;

    Sentence(string sentence) { 	// constructor
        this->lexemes = tokenize(sentence);
        this->ID = 0;
        this->CONST = 0;
        this->OP = 0;
        this->warning = false;
    }

    Sentence() {                    // 또 다른 constructor
        this->ID = 0;
        this->CONST = 0;
        this->OP = 0;
        this->warning = false;
    }

    void initialize() {             // 초기화
        this->lexemes.clear();
        this->ID = 0;
        this->CONST = 0;
        this->OP = 0;
        this->warning = false;
    }


    // sentence를 계산하기 위한 코드
    int calculateExpression(symbol_table table) {
        std::stack<int> operandStack;
        std::stack<std::string> operatorStack;

        for (const std::string& token : this->lexemes) {
            if (isdigit(token[0])) {  // 토큰이 숫자인 경우
                operandStack.push(std::stoi(token));
            }
            else if (isOperator(token)) {  // 토큰이 연산자인 경우
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
                operatorStack.pop();  // '(' 제거
            }

            else if (token == ";") continue;
            else {
                if (table.isThere(token)) { // 토큰이 identifier인 경우 값을 받아 stack에 push
                    operandStack.push(table.getValue(token));
                }
                else {                      // 없다면 Error 반환
                    return UNKNOWN;
                }
            }
        }

        while (!operatorStack.empty()) {
            processOperator(operandStack, operatorStack);
        }

        return operandStack.top();  // 최종 결과 반환
    }

    bool isOperator(const std::string& token) {
        return token == "+" || token == "-" || token == "*" || token == "/";
    }

    bool hasHigherPrecedence(const std::string& op1, const std::string& op2) {                       // 연산자 우선순위를 판단해주는 함수
        return (op1 == "*" || op1 == "/") && (op2 == "+" || op2 == "-");
    }

    void processOperator(std::stack<int>& operandStack, std::stack<std::string>& operatorStack) {   // 계산하는 함수
        int operand2 = operandStack.top();
        operandStack.pop();

        int operand1 = operandStack.top();
        operandStack.pop();

        std::string op = operatorStack.top();
        operatorStack.pop();

        if (op == "+") {
            if (operand1 == UNKNOWN || operand2 == UNKNOWN) operandStack.push(UNKNOWN);
            else operandStack.push(operand1 + operand2);
        }
        else if (op == "-") {
            if (operand1 == UNKNOWN || operand2 == UNKNOWN) operandStack.push(UNKNOWN);
            else operandStack.push(operand1 - operand2);
        }
        else if (op == "*") {
            if (operand1 == UNKNOWN || operand2 == UNKNOWN) operandStack.push(UNKNOWN);
            else operandStack.push(operand1 * operand2);
        }
        else if (op == "/") {
            if (operand1 == UNKNOWN || operand2 == UNKNOWN) operandStack.push(UNKNOWN);
            else operandStack.push(operand1 / operand2);
        }
    }



private:
    vector<string> tokenize(string sentence) {						// tokenize하는 함수
        stringstream ss(sentence);
        string token;												// lexeme을 저장하기 위한 임시 변수

        vector<string> result_lexemes;
        while (ss >> token) result_lexemes.push_back(token);		// lexeme 저장

        return result_lexemes;
    }
};

void lexical();
bool isNumber(const string& str);