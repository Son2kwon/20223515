#pragma once
#include<iostream>
#include<vector>
#include"lexical_analyzer.h"
using namespace std;

/*
<Grammars>

<program> �� <statements>
<statements> �� <statement> | <statement><semi_colon><statements>
<statement> �� <ident><assignment_op><expression>
<expression> �� <term><term_tail>
<term_tail> �� <add_op><term><term_tail> | ��
<term> �� <factor> <factor_tail>
<factor_tail> �� <mult_op><factor><factor_tail> | ��
<factor> �� <left_paren><expression><right_paren> | <ident> | <const>
<const> �� any decimal numbers
<ident> �� any names conforming to C identifier rules
<assignment_op> �� :=
<semi_colon> �� ;
<add_operator> �� + | -
<mult_operator> �� * | /
<left_paren> �� (
<right_paren> �� )

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