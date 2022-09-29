#pragma once
#include "Scanner.h"
#include <stack>
#include <exception>
#include "Poliz.h"
#include "Structure.h"
#include "Label.h"

namespace A{
    void print_TID();
}

class Parser{
public:
    Parser(const string& progr): scan(progr), prog(1000){}

    Poliz prog;

    void Analyze();

private:
    static vector<Label*> goto_labels;
    Scanner scan;
    Lexeme cur_lex;
    lexeme_type type;
    int value;
    stack<lexeme_type> lex_stack;
    stack<int> int_stack;
    vector<int> break_ident;
    vector<Structure> structures;
    bool in_for = false;

    void P();
    void D();
    void D1(int);
    void B(bool in_loop = false);
    void S(bool in_loop = false);
    void E();
    void E0();
    void E1();
    void E2();
    void T();
    void F();
    void find_structures();
    void check_semicolon(string, bool f = false);
   
    void struct_assign(Lexeme, int*);
    void dec(lexeme_type type, int value);
    void check_id();
    void check_label();
    void check_op();
    void check_not();
    void eq_type();
    void eq_bool();
    void check_read_id();

    void get_nlex(){
        cur_lex = scan.get_lex();
        type = cur_lex.get_type();
        value = cur_lex.get_value();
        //cout << "CURRENT LEXEME = " << cur_lex << endl;
    }

};