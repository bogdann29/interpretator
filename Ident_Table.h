#pragma once
#include "Identifier.h"
#include <string>
#include <algorithm>

class Ident_Table {
public:
	Identifier& operator[](int i) { return ident_table[i]; }

	int put_ident(const string& name);

	void del(){ident_table.pop_back(); size--;}

	int Size() const { return size; }
private:
	vector<Identifier> ident_table;
	int size = 0;
};