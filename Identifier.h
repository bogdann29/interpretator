#pragma once
#include "Lexeme.h"

class Identifier {
public:
	string get_name() const { return name; }
	bool get_declare() const { return declare; }
	bool get_assign() const { return assign; }
	lexeme_type get_type() const { return type; }
	int get_value() const { return value; }
	string get_struct_name() const { return name_of_struct; }

	void set_name(const string& name) { this->name = name; }
	void set_declare() { this->declare = true; }
	void set_assign() { this->assign = true; }
	void set_type(lexeme_type type) { this->type = type; }
	void set_value(int value) { this->value = value; }
	void set_struct_name(string name_of_struct) { this->name_of_struct = name_of_struct; }

private:
	string name;
	string name_of_struct;
	bool declare = false;
	bool assign = false;	
	lexeme_type type;
	int value;
};