#include "Ident_Table.h"

int Ident_Table::put_ident(const string& name) {
	auto it = find_if(begin(ident_table), end(ident_table), [&name](Identifier id) {return id.get_name() == name; });
	if (it == end(ident_table)) {
		Identifier id;
		id.set_name(name);
		ident_table.push_back(id);
		return size++;
	}
	return it - begin(ident_table);
}