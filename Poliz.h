#pragma once
#include "Lexeme.h"
#include "Scanner.h"
#include <iomanip>

class Poliz{
public:
    Poliz(int size): size(size) { pol = vector<Lexeme>(size, Lexeme()); top = 0; }
    void put_lex(const Lexeme& lex, int place){ pol[place] = lex; }
    void put_lex(const Lexeme& lex) { pol[top++] = lex; }
    void blank() { top++; }
    int get_top() const { return top; }
    Lexeme& operator[] (int index) {
        if(index < 0 || index > size)
            throw runtime_error("Out range of array\n");
        else if(index > top)
            throw runtime_error("undefined element\n");
        return pol[index]; 
    }
    void print() const { 
        for(int i = 0;i < top; i++){
            cout << "#" << i << setw(4) << setfill(' ') << pol[i] << " - ";
            if(pol[i].get_type() == POLIZ_ADDRESS || pol[i].get_type() == POLIZ_LABEL)
                cout << "POLIZ " << pol[i].get_value() << endl;
            else
                cout << Scanner::lex_to_str[pol[i].get_type()] << endl;
        }
    }
    
private: 
    int size;
    int top;
    vector<Lexeme> pol;
};