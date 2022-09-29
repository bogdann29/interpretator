#pragma once
#include "Poliz.h"

class Executer{
public:
    void Execute(Poliz& prog);
private:
    Lexeme lex;
};
