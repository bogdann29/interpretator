#pragma once
#include "Lexeme.h"

struct Struct_Field{
    string name;
    lexeme_type type;
};

class Structure{
public:
    string get_name() const {return name;}
    Struct_Field& operator[](int index){return struct_idents[index];}
    void put_field(const Struct_Field& sf) {
        for(auto i: struct_idents){
            if(sf.name == i.name && sf.type == i.type)
                throw runtime_error("twice declare in structure\n");
        }
        struct_idents.push_back(sf);
    }
    int size() const { return struct_idents.size(); }
    Structure(string name): name(name){}
private:
    string name;
    vector<Struct_Field> struct_idents;
};