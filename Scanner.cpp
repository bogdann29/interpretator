#include "Scanner.h"

vector<string> Scanner::TW = {
	"",
	"and",
	"bool",
	"break",
	"else",
	"if",
	"false",
	"for",
	"goto",
	"int",
	"not",
	"or",
	"program",
	"read",
	"string",
	"struct",
	"true",
	"while",
	"write",
};


vector<string> Scanner::TD = {
	"",
	";",
	",",
	".",
	":",
	"=",
	"\"",
	"{",
	"}",
	"(",
	")",
	"==",
	"<",
	">",
	"<=",
	">=",
	"!=",
	"+",
	"-",
	"*",
	"/",
	"%",
};


vector<lexeme_type> Scanner::words = {
	LEX_NULL,
	LEX_AND,
	LEX_BOOL,
	LEX_BREAK,
	LEX_ELSE,
	LEX_IF,
	LEX_FALSE,
	LEX_FOR,
	LEX_GOTO,
	LEX_INT,
	LEX_NOT,
	LEX_OR,
	LEX_PROGRAM,
	LEX_READ,
	LEX_STRING,
	LEX_STRUCT,
	LEX_TRUE,
	LEX_WHILE,
	LEX_WRITE,
	LEX_NULL
};


vector<lexeme_type> Scanner::delims = {
	LEX_NULL,
	LEX_SEMICOLON,
	LEX_COMMA,
	LEX_POINT,
	LEX_COLON,
	LEX_ASSIGN,
	LEX_QUOTE,
	LEX_LBRACE,
	LEX_RBRACE,
	LEX_LPAREN,
	LEX_RPAREN,
	LEX_EQUAL,
	LEX_LESS,
	LEX_GREATER,
	LEX_LSEQ,
	LEX_GREQ,
	LEX_NOTEQ,
	LEX_PLUS,
	LEX_MINUS,
	LEX_MULT,
	LEX_SLASH,
	LEX_PERCENT,
	LEX_NULL
};


int Scanner::look(const string& str, const vector<string> v) const {
	auto it = find(begin(v), end(v), str);
	if (it == end(v))
		return 0;
	return it - begin(v);
}


Ident_Table Scanner::TID;
vector<string> Scanner::Const_strings;
vector<Ident_Table> Scanner::TID_in_struct;



void Scanner::print_lex(const Lexeme& l) {
	cout << l << "     " << lex_to_str[l.get_type()];
	if (l.get_type() == LEX_NUM)
		cout << " = " << l.get_value();
	if (l.get_type() == LEX_CONSTSTR)
		cout << " = " << Const_strings[l.get_value()];
	if (l.get_type() == LEX_ID)
		cout << " = " << TID[l.get_value()].get_name();
	cout << endl;
}


Lexeme Scanner::get_lex() {
	int digit, sign = 1;
	int j;
	CS = H;
	do {
		switch (CS) {
			
		case H:
			if (c == ' ' || c == '\n' || c == '\r' || c == '\t')
				get_char();
			else if (isalpha(c)) {
				clear();
				add();
				get_char();
				CS = IDENT;
			}
			else if (isdigit(c)) {
				digit = c - '0';
				get_char();
				CS = NUM;
			}
			else if (c == '\"') {
				clear();
				get_char();
				CS = STR;
			}
			else if (c == '/') {
				get_char();
				CS = SL;
			}
			else if (c == '=' || c == '<' || c == '>') {
				clear();
				add();
				get_char();
				CS = ALE;
			}
			else if (c == '!') {
				clear();
				add();
				get_char();
				CS = NOTEQ;
			}
			else if (c == '-') {
				clear();
				add();
				get_char();
				CS = MIN;
			}
			else if (c == EOF) 
				return Lexeme(LEX_FIN);
			else
				CS = DELIM;
			break;

		case MIN:
			if (isdigit(c)) {
				sign = -1;
				digit = c - '0';
				get_char();
				CS = NUM;
			}
			else {
				j = look(buf, TD);
				return Lexeme(delims[j], j);
			}
			break;

		case IDENT:	
			if (isalpha(c) || isdigit(c) || c == '.'){
				add();
				get_char();
			}
			else {
                j = look(buf, TW);
				if (j != 0) 
					return Lexeme(words[j], j);
				else{
					j = TID.put_ident(buf);
					return Lexeme(LEX_ID, j);
				}
			}
			break;

		case NUM:
			if (isdigit(c)){
				digit = digit * 10 + (c - '0');
				get_char();
			}
			else
				return Lexeme(LEX_NUM, sign*digit);
			break;

		case SL:
			if (c == '*') {
				get_char();
				CS = COM;
			}
			else {
				j = look("/", TD);
				return Lexeme(delims[j], j);
			}
			break;

		case COM:
			if (c == '*') {
				get_char();
				if (c == '/') {
					get_char();
					CS = H;
				}
			}
			else if (c != EOF)
				get_char();
			else
				throw invalid_argument("Comment error");
			break;

		case STR:
			if (c == '\"') {
				get_char();
				Const_strings.push_back(buf);
				return Lexeme(LEX_CONSTSTR, Const_strings.size() - 1);
			}
			else if (c == EOF)
				throw invalid_argument("Quote error");
			else {
				add();
				get_char();
			}
			break;

		case ALE:
			if (c == '='){
				add();
				get_char();
				j = look(buf, TD);
				return Lexeme(delims[j], j);
			}
			else{
				j = look(buf, TD);
				return Lexeme(delims[j], j);
			}
			break;

		case NOTEQ:
			if (c == '=')
			{
				add();
				get_char();
				j = look(buf, TD);
				return Lexeme(LEX_NOTEQ, j);
			}
			else
				throw invalid_argument("!");
			break;

		case DELIM:
			clear();
			add();
			j = look(buf, TD);
			if (j != 0) 
			{
				get_char();
				return Lexeme(delims[j], j);
			}
			else
				throw invalid_argument("unknown char" + c);
			break;

		}
	}while (1);
}


map<lexeme_type, string> Scanner::lex_to_str = {
	{LEX_PROGRAM, "program"},
	{LEX_LBRACE, "{"},
	{LEX_RBRACE, "}"},
	{LEX_INT, "int"},
	{LEX_BOOL, "bool"},
	{LEX_STRING, "string"},
	{LEX_CONSTSTR, "const_string"},
	{LEX_ID, "ident"},
	{LEX_NUM, "num"},
	{LEX_TRUE, "true"},
	{LEX_FALSE, "false"},
	{LEX_FIN, "fin"},
	{LEX_IF, "if"},
	{LEX_ELSE, "else"},
	{LEX_READ, "read"},
	{LEX_WRITE, "write"},
	{LEX_LABEL, "label"},
	{LEX_STRUCT, "struct"},
	{LEX_STRUCTYPE, "structype"},
	{LEX_WHILE, "while"},
	{LEX_FOR, "for"},
	{LEX_BREAK, "break"},
	{LEX_GOTO, "goto"},
	{LEX_AND, "and"},
	{LEX_OR, "or"},
	{LEX_NOT, "not"},
	{LEX_LPAREN, "("},
	{LEX_RPAREN, ")"},
	{LEX_COMMA, ","},
	{LEX_POINT, "."},
	{LEX_SEMICOLON, ";"},
	{LEX_ASSIGN, "="},
	{LEX_EQUAL, "=="},
	{LEX_LESS, "<"},
	{LEX_GREATER, ">"},
	{LEX_LSEQ, "<="},
	{LEX_GREQ, ">="},
	{LEX_NOTEQ, "!="},
	{LEX_PLUS, "+"},
	{LEX_MINUS, "-"},
	{LEX_MULT, "*"},
	{LEX_SLASH, "/"},
	{LEX_QUOTE, "\""},
	{LEX_COLON, ":"},
	{LEX_PERCENT, "%"},
	{POLIZ_FGO, "FGO"},
	{POLIZ_GO, "GO"},
	{POLIZ_ADDRESS, "POLIZ_ADR"},
	{POLIZ_LABEL, "POLIZ_L"}
};