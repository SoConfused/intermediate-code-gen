#include "Header.h"

#define SIZE 1000
string holder[SIZE][4];
string ops[SIZE][3];
int table_size = 0, ops_size = 0;
bool str_complete = 0;
string tmpstring, statestring;


bool id_search (string& id) {
	bool yesno = 0;
	for (int i = 0; i < table_size; ++i) {
		if (id == holder[i][0]) {
			yesno = 1;
			break;
		}
	}
	return yesno;
}

int id_index(string& id) {
	int index = -1;
	for (int i = 0; i < table_size; ++i) {
		if (id == holder[i][0]) {
			index = i;
			break;
		}
	}
	return index;
}

int convert_char(char c) {
	int index = 999;

	if (c == 'i') {
		index = 0;
	}
	else if (c == '+') {
		index = 1;
	}
	else if (c == '-') {
		index = 2;
	}
	else if (c == '*') {
		index = 3;
	}
	else if (c == '/') {
		index = 4;
	}
	else if (c == '(') {
		index = 5;
	}
	else if (c == ')') {
		index = 6;
	}
	else if (c == '$') {
		index = 7;
	}
	else if (c == 'E') {
		index = 8;
	}
	else if (c == 'T') {
		index = 9;
	}
	else if (c == 'F') {
		index = 10;
	}

	return index;
}


void F(vector<vector<string>>& ltok, int& i) {
	int table_index = -1;

	if (ltok[i][0] == "Identifier") {					//a = b
		if (id_search(ltok[i][1]) && (table_index = id_index(ltok[i][1])) != -1) {
			tmpstring += ltok[i][1];
			statestring += "i";
			++i;

			ops[ops_size][0] = to_string(ops_size);

			if (holder[table_index][2] == "int") {
				ops[ops_size][1] = "PUSHI";
				ops[ops_size][2] = holder[table_index][3];
			}

			else if ((holder[table_index][2] == "bool")) {
				ops[ops_size][1] = "PUSHB";
				ops[ops_size][2] = holder[table_index][3];
			}

			else if (holder[table_index][2] == "float") {
				ops[ops_size][1] = "PUSHF";
				ops[ops_size][2] = holder[table_index][3];
			}
			else {
				cout << "ERROR: Type not found!" << endl;
				cout << holder[table_index][0] << "\t" << holder[table_index][1] << "\t" << holder[table_index][2] << endl;
			}
			++ops_size;

			ops[ops_size][0] = to_string(ops_size);
			ops[ops_size][1] = "PUSHM";
			ops[ops_size][2] = holder[table_index][1];
			++ops_size;

			if (ltok[i][1] == ";") {
				str_complete = 1;
			}
		}
		else {
			cout << "ERROR: INVALID INDEX" << endl;
		}
	}
}

void Tprime(vector<vector<string>>& ltok, int& i) {
	if (ltok[i][1] == "*") {
		tmpstring += ltok[i][1];
		statestring += "*";
		++i;

		ops[ops_size][0] = to_string(ops_size);
		ops[ops_size][1] = "MUL";
		ops[ops_size][2] = "nil";
		++ops_size;

		F(ltok, i);
		Tprime(ltok, i);
	}
	else if (ltok[i][1] == "/") {
		tmpstring += ltok[i][1];
		statestring += "/";
		++i;

		ops[ops_size][0] = to_string(ops_size);
		ops[ops_size][1] = "DIV";
		ops[ops_size][2] = "nil";
		++ops_size;

		F(ltok, i);
		Tprime(ltok, i);
	}
}

void T(vector<vector<string>>& ltok, int& i) {
	F(ltok, i);
	Tprime(ltok, i);
}

void Eprime(vector<vector<string>>& ltok, int& i) {
	if (ltok[i][1] == "+") {
		tmpstring += ltok[i][1];
		statestring += "+";
		++i;

		ops[ops_size][0] = to_string(ops_size);
		ops[ops_size][1] = "ADD";
		ops[ops_size][2] = "nil";
		++ops_size;

		T(ltok, i);
		Eprime(ltok, i);
	}
	else if (ltok[i][1] == "-") {
		tmpstring += ltok[i][1];
		statestring += "-";
		++i;

		ops[ops_size][0] = to_string(ops_size);
		ops[ops_size][1] = "SUB";
		ops[ops_size][2] = "nil";
		++ops_size;

		T(ltok, i);
		Eprime(ltok, i);
	}
}

void E(vector<vector<string>>& ltok, int& i) {
	T(ltok, i);
	Eprime(ltok, i);
}


bool A(vector<vector<string>> &ltok, int &i) {			//Do not pass i in by ref, only by value. For helper functions, pass by ref to ensure proper states
	int tmp_index = -1;
	str_complete = 0;

	while (!str_complete) {
		if (ltok[i][0] == "Identifier" && id_search(ltok[i][1])) {				//If we have a documented identifier a
			tmpstring += ltok[i][1];
			//statestring += "i";
			tmp_index = id_index(ltok[i][1]);
			++i;
			if (ltok[i][1] == "=") {											//a =
				tmpstring += ltok[i][1];
				//statestring += "=";
				++i;

				if (ltok[i][0] == "Number") {									//a = num
					tmpstring += ltok[i][1];

					holder[tmp_index][3] = ltok[i][1];

					++i;

					if (tmp_index != -1) {
						if (ltok[i][1] == ";") {
							++i;												//Go to next string
							str_complete = 1;
						}
						else {
							cout << "ERROR: Separator ';' expected" << endl;
							cout << "RECEIVED: '" << ltok[i][0] << " : " << ltok[i][1] << "'" << endl;
							break;
						}
					}
					else {
						cout << "ERROR: INVALID INDEX while getting number" << endl;
						break;
					}
				}

				else if (ltok[i][0] == "Identifier") {														//a = b
					E(ltok, i);

					ops[ops_size][0] = to_string(ops_size);
					ops[ops_size][1] = "POPM";
					ops[ops_size][2] = holder[tmp_index][1];
					++ops_size;

					if (ltok[i][1] == ";") {
						++i;																				//Go to next string
						str_complete = 1;
					}
					else {
						cout << "ERROR: Separator ';' expected" << endl;
						cout << "RECEIVED: '" << ltok[i][0] << " : " << ltok[i][1] << "'" << endl;
						break;
					}
				}

				else {
					cout << "ERROR: Identifier or number expected" << endl;
					cout << "RECEIVED: '" << ltok[i][0] << " : " << ltok[i][1] << "'" << endl;
					break;
				}
			}
			else {
				cout << "ERROR: '=' expected" << endl;
				cout << "RECEIVED: '" << ltok[i][0] << " : " << ltok[i][1] << "'" << endl;
				break;
			}
		}
		else {
			cout << "ERROR: Identifier expected" << endl;
			cout << "RECEIVED: '" << ltok[i][0] << " : " << ltok[i][1] << "'" << endl;
			break;
		}
	}

	return str_complete;
}


int main() {
	string stackstr, ent;

	int table_index1 = -1, table_index2 = -1, type = 0;
	unsigned int mem_loc = 2000;

	vector<vector<string>> lex_tokens;		//Table to hold our 

	string doc = open_file();				//File we are opening

	ifstream fin;
	fin.open(doc);

	if (!fin.is_open()) {
		printf("Failed to open file. Please restart\n");
		fin.close();
		return 0;
	}

	lex_tokens = lexer(fin);		//Generate our tokens with lexical analysis

	//Print out the tokens from the lexer
	for (int i = 0; i < lex_tokens.size(); ++i) {
		std::cout << setw(15) << lex_tokens[i][0] << setw(10) << "=\t" << lex_tokens[i][1] << std::endl;
	}

	for (int i = 0; i < lex_tokens.size(); ++i) {
		if (lex_tokens[i][0] == "Keyword" && lex_tokens[i][1] == "int" || lex_tokens[i][1] == "bool" || lex_tokens[i][1] == "float") {
			type = i;
			++i;

			while (lex_tokens[i][1] != ";") {
				if (lex_tokens[i][0] == "Identifier" && lex_tokens[i + 1][0] == "Separator") {
					if (!id_search(lex_tokens[i][0])) {						//If our identifier is not defined
						holder[table_size][0] = lex_tokens[i][1];			//Add current ID name
						holder[table_size][1] = to_string(mem_loc);					//Add mem location
						holder[table_size][2] = lex_tokens[type][1];		//Add ID type (int, float, bool, etc)
						++mem_loc;											//Increment memory
						++table_size;
						++i;
					}
					else {													//Identifier already defined, error
						cout << "ERROR: Identifier already defined!" << endl;
						break;
					}
				}
				else {
					cout << "ERROR: Expected identifier then separator!" << endl;
					cout << "RECEIVED: '" << lex_tokens[i][0] << " : " << lex_tokens[i][1] << "' AND '" << lex_tokens[i + 1][0] << " : " << lex_tokens[i + 1][1] << "'" << endl;
					break;
				}

				if (lex_tokens[i][1] == "," && lex_tokens[i + 1][0] == "Identifier") {		//if out next token is a comma, then an identifier, it is fine
					++i;
				}
				else if (lex_tokens[i][1] == ";") {
					continue;
				}
				else {
					cout << "ERROR: Expected separator then identifier" << endl;
					cout << "RECEIVED: '" << lex_tokens[i][0] << " : " << lex_tokens[i][1] << "' AND '" << lex_tokens[i + 1][0] << " : " << lex_tokens[i + 1][1] << "'" << endl;
				}
			}
		}

		else if (lex_tokens[i][0] == "Identifier") {
			if (A(lex_tokens, i)) {						//If we had a syntactically correct string for assignment
				//Begin grammar matching
				//R1: E -> E + T      [E = 8, T = 9, F = 10]
				//R2: E -> E - T
				//R3: E -> T
				//R4: T -> T * F
				//R5: T -> T / F
				//R6: T -> F
				//R7: T -> (E)
				//R8: F -> i

				if (isdigit(tmpstring.back())) {
					--i;
					tmpstring.clear();
					statestring.clear();

				}

				else {
					stackstr.clear();							//clear the stack
					stackstr += "0";							//Add initial states
					statestring += "$";

					cout << "\n========================" << endl;
					cout << "====STARTING STACK====" << endl;
					cout << "\n========================" << endl << endl;
					cout << "Action" << setw(20) << "Stack" << setw(20) << "Input" << endl;
					cout << "strt" << setw(20) << stackstr << setw(20) << statestring << endl;

					while (ent != "99") {
						ent = syn_table[stackstr.back() - '0'][convert_char(statestring[0])];
						if (ent[0] == 's') {
							stackstr += statestring[0];
							stackstr += ent[1];
							statestring.erase(0, 1);
							cout  << ent << setw(20) << stackstr << setw(20) << statestring << endl;
							ent = syn_table[stackstr.back() - '0'][convert_char(statestring[0])];
						}

						else if (ent[0] == 'r') {
							if (ent[1] == '1') {
								stackstr.erase(stackstr.size() - 6);
								ent = syn_table[stackstr.back() - '0'][8];
								stackstr += "E" + ent;
								cout << "r1" << setw(20) << stackstr << setw(20) << statestring << endl;

							}
							else if (ent[1] == '2') {
								stackstr.erase(stackstr.size() - 6);
								ent = syn_table[stackstr.back() - '0'][8];
								stackstr += "E" + ent;
								cout << "r2" << setw(20) << stackstr << setw(20) << statestring << endl;

							}
							else if (ent[1] == '3') {
								stackstr.erase(stackstr.size() - 2);
								ent = syn_table[stackstr.back() - '0'][8];
								stackstr += "E" + ent;
								cout << "r3" << setw(20) << stackstr << setw(20) << statestring << endl;

							}
							else if (ent[1] == '4') {
								stackstr.erase(stackstr.size() - 6);
								ent = syn_table[stackstr.back() - '0'][convert_char(stackstr.back())];
								stackstr += "T" + ent;
								cout << "r4" << setw(20) << stackstr << setw(20) << statestring << endl;

							}
							else if (ent[1] == '5') {
								stackstr.erase(stackstr.size() - 6);
								ent = syn_table[stackstr.back() - '0'][9];
								stackstr += "T" + ent;
								cout << "r5" << setw(20) << stackstr << setw(20) << statestring << endl;

							}
							else if (ent[1] == '6') {
								stackstr.erase(stackstr.size() - 2);
								ent = syn_table[stackstr.back() - '0'][(9)];
								stackstr += "T" + ent;
								cout << "r6" << setw(20) << stackstr << setw(20) << statestring << endl;

							}
							else if (ent[1] == '7') {
								stackstr.erase(stackstr.size() - 6);
								ent = syn_table[stackstr.back() - '0'][9];
								stackstr += "T" + ent;
								cout << "r7" << setw(20) << stackstr << setw(20) << statestring << endl;

							}
							else if (ent[1] == '8') {
								stackstr.erase(stackstr.size() - 2);
								ent = syn_table[stackstr.back() - '0'][10];
								stackstr += "F" + ent;
								cout << "r8" << setw(20) << stackstr << setw(20) << statestring << endl;

							}
							else {
								cout << "ERROR: reducing" << endl;
								return 0;
							}
						}
						else if (ent == "99") {
							cout << "String complete and valid!" << endl;
							tmpstring.clear();
							statestring.clear();
							stackstr.clear();
						}
						else {
							cout << "ERROR: INVALID STATE" << endl;
							break;
						}
					}
				}

			}

			else {
				cout << "ERROR: INVALID STRING" << endl;
				cout << "RECEIVED '" << tmpstring << "'" << endl;
				break;
			}
		}

		else {
			cout << "ERROR: Expected an identifier or keyword!" << endl;
			cout << "RECEIVED: '" << lex_tokens[i][0] << " : " << lex_tokens[i][1] << "'" << endl;
		}
	}

	cout << "\n\n=======Stored Variables========\n";
	for (int i = 0; i < table_size; ++i) {
		cout << holder[i][0] << setw(15) << holder[i][1] << setw(30) << holder[i][2] << setw(20) << holder[i][3] << endl;
	}

	cout << "\n\n==========Operations===========\n";
	for (int i = 0; i < ops_size; ++i) {
		cout << ops[i][0] << "\t" << ops[i][1] << "\t" << ops[i][2] << endl;
	}

	system("pause");
	fin.close();
	return 0;
}