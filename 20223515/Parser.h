#pragma once
#include<iostream>
#include<vector>
#include"lexical_analyzer.h"
using namespace std;

/*
<Grammars>

<program> ¡æ <statements>
<statements> ¡æ <statement> | <statement><semi_colon><statements>
<statement> ¡æ <ident><assignment_op><expression>
<expression> ¡æ <term><term_tail>
<term_tail> ¡æ <add_op><term><term_tail> | ¥å
<term> ¡æ <factor> <factor_tail>
<factor_tail> ¡æ <mult_op><factor><factor_tail> | ¥å
<factor> ¡æ <left_paren><expression><right_paren> | <ident> | <const>
<const> ¡æ any decimal numbers
<ident> ¡æ any names conforming to C identifier rules
<assignment_op> ¡æ :=
<semi_colon> ¡æ ;
<add_operator> ¡æ + | -
<mult_operator> ¡æ * | /
<left_paren> ¡æ (
<right_paren> ¡æ )

*/


void program();
void statements();
void statement();
void expression();
void term_tail();
void term();
void factor_tail();
void factor();
void constant();
void ident();
void semi_colon();

void assignment_op();
void add_operator();
void mult_operator();
void left_paren();
void right_paren();