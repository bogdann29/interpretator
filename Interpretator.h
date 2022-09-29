#pragma once
#include "Parser.h"
#include "Executer.h"

class Interpretator{ 
public:
    Interpretator(string program): pars(program){}
    void Interpretation();

private:
    Parser pars;
    Executer exec;
};