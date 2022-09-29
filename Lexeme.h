#pragma once
#include <iostream>
#include <vector>


using namespace std;

enum lexeme_type {
	LEX_NULL,
	LEX_PROGRAM,
	LEX_LBRACE,
	LEX_RBRACE,
	LEX_STRUCT,
	LEX_STRUCTYPE,
	LEX_INT,
	LEX_BOOL,
	LEX_STRING,
	LEX_CONSTSTR,
	LEX_ID,
	LEX_NUM,
	LEX_TRUE,
	LEX_FALSE,
	LEX_FIN,
	LEX_IF,
	LEX_ELSE,
	LEX_READ,
	LEX_WRITE,
	LEX_WHILE,
	LEX_FOR,
	LEX_BREAK,
	LEX_GOTO,
	LEX_AND,
	LEX_OR,
	LEX_NOT,
	LEX_LPAREN,
	LEX_RPAREN,
	LEX_COMMA,
	LEX_POINT,
	LEX_SEMICOLON,
	LEX_ASSIGN,
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
	LEX_QUOTE,
	LEX_COLON,
	LEX_PERCENT,
	LEX_LABEL,
	POLIZ_LABEL,
	POLIZ_ADDRESS,
	POLIZ_GO,
	POLIZ_FGO
};

class Lexeme {
public:
	Lexeme(lexeme_type type = LEX_NULL, int value = 0): l_type(type), l_value(value) {}
	lexeme_type get_type() const { return l_type; }
	int get_value() const { return l_value; }
	friend ostream& operator <<(ostream& stream, const Lexeme& l) {
		stream << '(' << l.l_type << ", " << l.l_value << ");";
		return stream; 
	}
private:
	lexeme_type l_type;
	int l_value;
};
