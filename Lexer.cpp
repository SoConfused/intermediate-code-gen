#include "Header.h"

bool is_keyword(string tok) {
	for (int i = 0; i < 21; ++i) {		
		if (tok == KEYW[i]) {
			return true;
		}
	}
	return false;
}

bool is_number(string tok) {
	if (isdigit(tok[0])) {
		return true;
	}
	return false;
}

bool is_operator(char tok) {
	for (int i = 0; i < 8; ++i) {
		if (tok == OPS[i]) {
			return true;
		}
	}
	return false;
}

bool is_separator(char tok) {
	if (isspace(tok)) {
		return true;
	}
	for (int i = 0; i < 10; ++i) {
		if (tok == SEPARATORS[i]) {
			return true;
		}
	}
	return false;
}

int update_col(char t) {
	int col = 100;

	if (t == '!') { col = 20; }
	else if (isalpha(t)) { col = 0; }
	else if (isdigit(t)) { col = 1; }
	else if (is_operator(t)) { col = 14; }
	else if (is_separator(t)) { col = 3; }
	else if (t == '$') { col = 22; }
	else if (t == '_') { col = 23;  }

	return col;
}

string open_file() {
	string file;
	cout << "Enter a file to lexically analyze: ";
	cin >> file;

	return file;
}

vector<vector<string>> lexer(ifstream &fin) {
	int state = 1, row = 0, col = 21, prev_state = 1, table_size = 0;

	vector<vector<string>> lex_table;
	vector<string> tmp;

	string tok, line; 

	while (getline(fin, line)) {
		//tok.clear();
		//state = 1;
		for (int cp = 0; cp < line.length(); ++cp) {
			col = update_col(line[cp]);
			if(col == 100) { 
				cout << "INVALID CHARACTER FOUND: " << line[cp] << endl;
				cout << "EXITING!" << endl;
				throw 1;
			}

			prev_state = state;
			state = state_table[state][col];

			if (state == 999) {
				cout << "INVALID CHARACTER FOUND: " << line[cp] << endl;
				cout << "EXITING!" << endl;
				throw 1;
			}

			else if (state != 6 && state != 0) {
				tok += line[cp];
			}

			if (state == 4) {
				state = 0;
				prev_state = 4;
			}
			else if (state == 5) {
				state = 0;
				prev_state = 5;
			}


			if (state == 0 && !tok.empty()) {
				//Is Keyword
				if (is_keyword(tok)) {
					lex_table.push_back(tmp);
					lex_table[row].push_back("Keyword");
					lex_table[row++].push_back(tok);
				}

				//Is Number
				else if (prev_state == 3) {
					lex_table.push_back(tmp);
					lex_table[row].push_back("Number");
					lex_table[row++].push_back(tok);
				}

				//Is Separator
				else if (prev_state == 4 && !isspace(tok[0])) {
					lex_table.push_back(tmp);
					lex_table[row].push_back("Separator");
					lex_table[row++].push_back(tok);
				}

				//Is Operator
				else if (prev_state == 5) {
					lex_table.push_back(tmp);
					lex_table[row].push_back("Operator");
					lex_table[row++].push_back(tok);
				}

				//Otherwise, identifier
				else if (!isspace(tok[0])) {
					lex_table.push_back(tmp);
					lex_table[row].push_back("Identifier");
					lex_table[row++].push_back(tok);
				}

				tok.clear();
				state = 1;

				if (!isspace(line[cp]) && prev_state != 4 && prev_state != 5) {
					--cp;
				}
			}
		}
	}

	return lex_table;
}