#pragma once
#include "Lexeme.h"
#include "Ident_Table.h"
#include "Identifier.h"
#include <map>
#include <fstream>

class Scanner {
public:
	Scanner(const string& prog){
		stream = fstream(prog);
		if (!stream.is_open())
			throw invalid_argument("file doesn-t exist\n");
		CS = H;
		clear();
		get_char();
	}

	Lexeme get_lex();
	static map<lexeme_type, string> lex_to_str;
	static void print_lex(const Lexeme&);
	static Ident_Table TID;
	static vector<string> Const_strings;
	static vector<Ident_Table> TID_in_struct;
private:
	enum state {H, IDENT, NUM, SL, COM, STR, ALE, DELIM, NOTEQ, MIN};
	state CS;
	static vector<string> TW;
	static vector<lexeme_type> words;
	static vector<string> TD;
	static vector<lexeme_type> delims;

	char c;
	string buf;
	fstream stream;

	void add() { buf.push_back(c); }

	void clear() { buf.clear(); }

	void get_char() { 
		stream.get(c);
		if (stream.eof())
			c = EOF;
	}

	int look(const string& str, const vector<string> v) const;

};