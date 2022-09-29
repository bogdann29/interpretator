#include <iostream>
#include <iomanip>
#include "Interpretator.h"
using namespace std;


int main() {
    try {
        cout << "Enter file name:\n";
        string file_name;
        cin >> file_name;
        Interpretator I(file_name);
        I.Interpretation();
    }
    catch (exception& ex) {
        cout << ex.what() << endl;
    }
	return 0;
}