#include "Executer.h"
#include <stack>
#include <string>
#include "Parser.h"

template<typename T>
void print(stack<T> s){
    while(!s.empty()){
        cout << s.top() << " ";
        s.pop();
    }
    cout << "empty";
    cout << endl;
}


void Executer::Execute(Poliz& prog){
    stack<int> args;
    stack<string> string_args;
    int i = 0;
    int size = prog.get_top();
    int tmp1, tmp2;
    while(i < size){
        lex = prog[i];
        switch(lex.get_type()){
            case LEX_TRUE:
            case LEX_FALSE:
            case LEX_NUM:
            case POLIZ_ADDRESS:
            case POLIZ_LABEL:
                args.push(lex.get_value());
                break;
            case LEX_CONSTSTR:
                string_args.push(Scanner::Const_strings[lex.get_value()]);
                break;
            case LEX_NULL:
                break;
            case LEX_ID:{
                int val = lex.get_value();
                if(Scanner::TID[val].get_assign()){
                    //cout << "aioghioas\n";
                    if(Scanner::TID[val].get_type() == LEX_STRING){
                        string_args.push(Scanner::Const_strings[Scanner::TID[val].get_value()]);
                    }
                    else
                        args.push(Scanner::TID[val].get_value());
                    break;
                }
                else
                    throw runtime_error("POLIZ: indefinite identifier\n");
            }
            case LEX_NOT:
                tmp1 = args.top();
                args.pop();
                args.push(!tmp1);
                break;

            case LEX_OR:
                tmp1 = args.top();
                args.pop();
                tmp2 = args.top();
                args.pop();
                args.push(tmp1 || tmp2);
                break;

            case LEX_AND:
                tmp1 = args.top();
                args.pop();
                tmp2 = args.top();
                args.pop();
                args.push(tmp1 && tmp2);
                break;

            case POLIZ_GO:
                if(args.empty())
                    break;
                i = args.top() - 1;
                args.pop();
                break;
            
            case POLIZ_FGO:
                tmp1 = args.top();
                args.pop();
                if (!args.top())
                    i = tmp1 - 1;
                args.pop();
                break;

            case LEX_WRITE:
                if(!string_args.empty()){
                    cout << string_args.top() << endl;
                    string_args.pop();
                }
                else{
                    cout << args.top() << endl;
                    args.pop();
                }
                break;
            
            case LEX_READ:{
                int k;
                tmp1 = args.top();
                args.pop();
                Identifier& var_id = Scanner::TID[tmp1];
                if(var_id.get_type() == LEX_INT){
                    while(true){
                        cout << "Enter int value for variable ";
                        cout << var_id.get_name() << endl;
                        string s;
                        cin >> s;
                        try{
                            k = stoi(s);
                            break;
                        }
                        catch(...){
                            cout << "stoi" << endl;
                        }
                    }

                }
                else if(var_id.get_type() == LEX_STRING){
                    string var_value;
                    cout << "Enter string value for variable ";
                    cout << var_id.get_name() << endl;
                    cin >> var_value;
                    Scanner::Const_strings.push_back(var_value);
                    k = Scanner::Const_strings.size() - 1;
                }
                else{
                    string buf;
                    do{
                        cout << "Enter boolean value (true or false) for variable ";
                        cout << var_id.get_name() << endl;
                        cin >> buf;
                    }while(buf != "true" && buf != "false");

                    if(buf == "true")
                        k = 1;
                    else 
                        k = 0;
                }
                Scanner::TID[tmp1].set_value(k);
                Scanner::TID[tmp1].set_assign();
                break;
            }
            case LEX_PLUS:
                if(string_args.empty()){
                    tmp1 = args.top();
                    args.pop();
                    tmp2 = args.top();
                    args.pop();
                    args.push(tmp1 + tmp2);
                }
                else{
                    string tmp3 = string_args.top();
                    string_args.pop();
                    string tmp4 = string_args.top();
                    string_args.pop();
                    string_args.push(tmp4 + tmp3);
                }
                break;


            case LEX_MULT:
                tmp1 = args.top();
                args.pop();
                tmp2 = args.top();
                args.pop();
                args.push(tmp1 * tmp2);
                break;

            case LEX_MINUS:
                tmp1 = args.top();
                args.pop();
                tmp2 = args.top();
                args.pop();
                args.push(tmp2 - tmp1);
                break;
            
            case LEX_SLASH:
                tmp1 = args.top();
                args.pop();
                if(tmp1 != 0){
                    tmp2 = args.top();
                    args.pop();
                    args.push(tmp2 / tmp1);
                    break;
                }
                else 
                    throw runtime_error("POLIZ:divide by zero\n");

            case LEX_PERCENT:
                tmp1 = args.top();
                args.pop();
                if(tmp1 != 0){
                    tmp2 = args.top();
                    args.pop();
                    args.push(tmp2 % tmp1);
                    break;
                }
                else 
                    throw runtime_error("POLIZ:divide by zero\n");     

            case LEX_EQUAL:
                if(string_args.empty()){
                    tmp1 = args.top();
                    args.pop();
                    tmp2 = args.top();
                    args.pop();
                    args.push(tmp1 == tmp2);
                }
                else{
                    string tmp3 = string_args.top();
                    string_args.pop();
                    string tmp4 = string_args.top();
                    string_args.pop();
                    args.push(tmp3 == tmp4);
                }
                break;

            case LEX_LESS:
                if(string_args.empty()){
                    tmp1 = args.top();
                    args.pop();
                    tmp2 = args.top();
                    args.pop();
                    args.push(tmp1 > tmp2);
                }
                else{
                    string tmp3 = string_args.top();
                    string_args.pop();
                    string tmp4 = string_args.top();
                    string_args.pop();
                    args.push(tmp3 > tmp4);
                }
                break;

            case LEX_GREATER:
                if(string_args.empty()){
                    tmp1 = args.top();
                    args.pop();
                    tmp2 = args.top();
                    args.pop();
                    args.push(tmp1 < tmp2);
                }
                else{
                    string tmp3 = string_args.top();
                    string_args.pop();
                    string tmp4 = string_args.top();
                    string_args.pop();
                    args.push(tmp3 < tmp4);
                }
                break;

            case LEX_LSEQ:
                if(string_args.empty()){
                    tmp1 = args.top();
                    args.pop();
                    tmp2 = args.top();
                    args.pop();
                    args.push(tmp1 >= tmp2);
                }
                else{
                    string tmp3 = string_args.top();
                    string_args.pop();
                    string tmp4 = string_args.top();
                    string_args.pop();
                    args.push(tmp3 >= tmp4);
                }
                break;

            case LEX_GREQ:
                if(string_args.empty()){
                    tmp1 = args.top();
                    args.pop();
                    tmp2 = args.top();
                    args.pop();
                    args.push(tmp1 <= tmp2);
                }
                else{
                    string tmp3 = string_args.top();
                    string_args.pop();
                    string tmp4 = string_args.top();
                    string_args.pop();
                    args.push(tmp3 <= tmp4);
                }
                break;

            case LEX_NOTEQ:
                if(string_args.empty()){
                    tmp1 = args.top();
                    args.pop();
                    tmp2 = args.top();
                    args.pop();
                    args.push(tmp1 != tmp2);
                }
                else{
                    string tmp3 = string_args.top();
                    string_args.pop();
                    string tmp4 = string_args.top();
                    string_args.pop();
                    args.push(tmp3 != tmp4);
                }
                break;

            case LEX_ASSIGN:
                if(string_args.empty()){
                    tmp1 = args.top();
                    args.pop();
                    tmp2 = args.top();
                    args.pop();
                    Scanner::TID[tmp2].set_value(tmp1);
                    Scanner::TID[tmp2].set_assign();
                }
                else{
                    string tmp3 = string_args.top();
                    string_args.pop();
                    tmp1 = args.top();
                    args.pop();
                    Scanner::Const_strings.push_back(tmp3);
                    Scanner::TID[tmp1].set_value(Scanner::Const_strings.size() - 1);
                    Scanner::TID[tmp1].set_assign();
                }
                break;

            default:
                throw runtime_error("POLIZ: unexpected elem\n");
        }
        i++;

        // print(args);
        // print(string_args);
        // cout << endl;
    }
}