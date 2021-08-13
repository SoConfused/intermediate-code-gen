#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <stdexcept>

using namespace std;

//State table is 22 wide
//Accept = 0, Starting = 1, Letter = 2, Digit = 3, Separators = 4, Operators = 5, Comment = 6
//  L, D, (, ), {, }, [, ], ,, ., :, ;, *, +, -, =, /, >, <, %, !, \s\n\t, $, _
const int state_table[][24] = { { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },		//Accepting (skip spaces)
								{ 2, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 6, 1, 999, 999 },		//Starting state (ignore spaces)
								{ 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2 },		//Letters
								{ 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 999, 999 },		//Digits
								{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 999, 999 },		//Separators
								{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 999, 999 },		//Operators
								{ 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 6, 999, 999 }		//Comment
};

//====ARITHMETIC====
const string syn_table[16][11] = {
	/*		  i,   +,   -,    *,     /,   (,     ),   $,  ||  E,   T,   F */
	/*0*/	{"s5", "0", "0",  "0",  "0", "s4",  "0",  "0",   "1", "2", "3"},
	/*1*/	{"0", "s6", "s7", "0",  "0",  "0",  "0",  "99",  "0", "0", "0"},
	/*2*/	{"0", "r3", "r3", "s8", "s9", "0",  "r3", "r3",  "0", "0", "0"},
	/*3*/	{"0", "r6", "r6", "r6", "r6", "0",  "r6", "r6",  "0", "0", "0"},
	/*4*/	{"s5", "0", "0",  "0",  "0",  "s4", "0",  "0",   "10", "2", "3"},
	/*5*/	{"0", "r8", "r8", "r8", "r8", "0", "r8", "r8",   "0", "0", "0"},
	/*6*/	{"s5", "0", "0",  "0",  "0",  "s4", "0",  "0",   "0", "11", "3"},
	/*7*/	{"s5", "0", "0",  "0",  "0",  "s4", "0",  "0",   "0", "12", "3"},
	/*8*/	{"s5", "0", "0",  "0",  "0",  "s4", "0",  "0",   "0", "0",  "13"},
	/*9*/	{"s5", "0", "0",  "0",  "0",  "s4", "0",  "0",   "0", "0",  "14"},
	/*10*/	{"0", "s6", "s7", "0",  "0",  "0", "s15", "0",   "0", "0", "0"},
	/*11*/	{"0", "r1", "r1", "s8", "s9", "0",  "r1", "r1",  "0", "0", "0"},
	/*12*/	{"0", "r2", "r2", "s8", "s9", "0",  "r2", "r2",  "0", "0", "0"},
	/*13*/	{"0", "r4", "r4", "r4", "r4", "0",  "r4", "r4",  "0", "0", "0"},
	/*14*/	{"0", "r5", "r5", "r5", "r5", "0",  "r5", "r5",  "0", "0", "0"},
	/*15*/	{"0", "r7", "r7", "r7", "r7", "0",  "r7", "r7",  "0", "0", "0"}
};

//const char char_table[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
const string KEYW[] = { "int", "float", "bool", "True", "False", "if", "else", "then", "endif", "endelse", "while", "whileend", "do", "enddo", "for", "endfor", "STDinput", "STDoutput", "and", "or", "not" };
const char SEPARATORS[] = { ';', '(', ')', '{' , '}', '[' , ']', ',', '.', ':' };
const char OPS[] = { '*', '+', '-', '/', '=', '>', '<', '%' };

bool is_keyword(string tok);

bool is_number(string tok);

bool is_operator(char tok);

bool is_separator(char tok);

int update_col(char t);

string open_file();

vector<vector<string>> lexer(ifstream &fin);


//ADD and SUB
//<Expression> -> <Expression> + <Term> | <Expression> - <Term> | <Term>
//MULT and DIV
//<Term> -> <Term>* <Factor> | <Term> / <Factor> | <Factor>
//FACTOR
//<Factor> -> ( <Expression> ) | <ID> | <Num> 
//ID
//<ID>->id

//====ASSIGNMENT====
//<Statement> -> <Assign>
//<Assign> -> <ID> = <Expression>;

//====SINGLE DECLARATIVE====
//<Statement> -> <Declarative> 
//<Declarative> -> <Type> <ID>
//<Type> -> bool | int



//R1: E -> E + T
//R2: E -> E - T
//R3: E -> T
//R4: T -> T * F
//R5: T -> T / F
//R6: T -> F
//R7: T -> (E)
//R8: F -> i

bool id_search(string& id);

int id_index(string& id);

int convert_char(char c);

void F(vector<vector<string>>& ltok, int& i);

void Tprime(vector<vector<string>>& ltok, int& i);

void T(vector<vector<string>>& ltok, int& i);

void Eprime(vector<vector<string>>& ltok, int& i);

void E(vector<vector<string>>& ltok, int& i);

bool A(vector<vector<string>>& ltok, int& i);