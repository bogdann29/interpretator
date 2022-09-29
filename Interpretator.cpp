#include "Interpretator.h"

void Interpretator::Interpretation(){
    pars.Analyze();
    exec.Execute(pars.prog);
}