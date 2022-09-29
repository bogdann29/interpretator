#include "Parser.h"
vector<Label*> Parser::goto_labels;

void Parser::check_semicolon(string str, bool f){
    if(!f){
        if(type != LEX_SEMICOLON)
            throw runtime_error("no semicolon after " + str + "\n");
        get_nlex();
    }
}


void A::print_TID(){
    for(int i = 0;i < Scanner::TID.Size(); i++)
    cout << Scanner::TID[i].get_name() << "   " << Scanner::lex_to_str[Scanner::TID[i].get_type()] 
         << "   " << Scanner::TID[i].get_value() << "   " << endl;
}


void Parser::dec(lexeme_type type, int value){
    if(Scanner::TID[value].get_declare())
        throw runtime_error("twice declare\n");
    Scanner::TID[value].set_declare();
    Scanner::TID[value].set_type(type);
}


void Parser::check_id(){
    if (Scanner::TID[value].get_declare())
        lex_stack.push(Scanner::TID[value].get_type());
    else
        throw runtime_error("variable was not declared\n");
}

void Parser::check_label(){
    Identifier& label = Scanner::TID[value];
    if(label.get_assign())
        throw runtime_error("use of one label is not allowed\n");
    label.set_assign();

    if(label.get_declare()){         // declared label -> goto before label
        goto_labels[label.get_value()]->set_label_num(prog.get_top());
        //prog.put_lex(Lexeme(POLIZ_LABEL, prog.get_top()), Scanner::TID[value].get_goto_l());
    }
    else {
        dec(LEX_LABEL, value);
        Label* L = new Label(label.get_name(), prog.get_top());
        goto_labels.push_back(L);
        label.set_value(goto_labels.size() - 1);
    }
    get_nlex();
}


void Parser::check_op(){
    lexeme_type t1, t2, op, r = LEX_BOOL;
    t2 = lex_stack.top();
    lex_stack.pop();
    op = lex_stack.top();
    lex_stack.pop();
    t1 = lex_stack.top();
    lex_stack.pop();

    //cout << t1 <<  ' ' << t2  << ' ' << op << endl;

    if (t1 != t2)
        throw runtime_error("different types\n");

    if(t1 == LEX_STRING && (op == LEX_MINUS || op == LEX_MULT || op == LEX_SLASH || op == LEX_OR || op == LEX_AND || op == LEX_PERCENT))
        throw runtime_error("you can only add and compare strings\n");

    if(t1 == LEX_BOOL && (op == LEX_MINUS || op == LEX_MULT || op == LEX_SLASH || op == LEX_PERCENT || op == LEX_PLUS))
        throw runtime_error("wrong operator between bool operands\n");

    if(t1 == LEX_STRING && op == LEX_PLUS)
        r = LEX_STRING;

    if ((op == LEX_PLUS || op == LEX_MINUS || op == LEX_MULT || op == LEX_SLASH || op == LEX_PERCENT) && t1 == LEX_INT)
        r = LEX_INT;
    
    lex_stack.push(r);
    prog.put_lex(op);
}


void Parser::check_read_id(){
    if (!Scanner::TID[value].get_declare())
        throw runtime_error("variable not declared\n");
}


void Parser::check_not(){
    if(lex_stack.top() != LEX_BOOL)
        throw runtime_error("Wrong type with op \"not\"\n");    
}


void Parser::eq_type(){
    auto t = lex_stack.top();
    lex_stack.pop();
    if(lex_stack.top() != t) 
        throw runtime_error("incompatible types in assignment\n");
    lex_stack.pop();
}


void Parser::eq_bool(){
    if(lex_stack.top() != LEX_BOOL)
        throw "expression is not boolean";
    lex_stack.pop();
}


void Parser::Analyze(){
    get_nlex();
    P();
    cout << "Nice program bro!!\n" << endl;
    prog.print();
}


void Parser::P(){
    if(type != LEX_PROGRAM)
        throw runtime_error("\"program\" error\n");
    get_nlex();

    if(type != LEX_LBRACE)
        throw runtime_error("No opening brace\n");
    get_nlex();

    find_structures();
    do{ 
        S();
    }while(type != LEX_RBRACE);

    A::print_TID();
    get_nlex();
    if(type != LEX_FIN)
        throw runtime_error("Bad end of file\n");

    for(auto label: goto_labels){
        for(int i = 0; i < label->size(); i++)
            if(label->get_label_num() != -1){
                prog.put_lex(Lexeme(POLIZ_LABEL, label->get_label_num()), label->operator[](i));
                prog.put_lex(Lexeme(POLIZ_GO), label->operator[](i) + 1);
            }
    }
    
}



void Parser::find_structures(){
    while(type == LEX_STRUCT){

        get_nlex();
        dec(LEX_STRUCTYPE, value);
        Structure s(Scanner::TID[value].get_name());
        int val = value;

        get_nlex();
        if(type != LEX_LBRACE)
            throw runtime_error("No opening brace\n");
        get_nlex();

        while(type == LEX_INT || type == LEX_BOOL || type == LEX_STRING){
            lexeme_type tmp = type;
            
            do{
                get_nlex();
                if(type != LEX_ID)
                    throw runtime_error("expect name of variable\n");

                Struct_Field sf = {Scanner::TID[value].get_name(), tmp};
                
                s.put_field(sf);
                Scanner::TID.del();
                get_nlex();

            }while(type == LEX_COMMA);

            check_semicolon("declaration in struct", in_for);
        }

        structures.push_back(s);
        Scanner::TID[val].set_value(structures.size() - 1);

        if(type != LEX_RBRACE)
            throw runtime_error("No closing brace\n");
        
        get_nlex();
        check_semicolon("struct");
    }

    cout << "AFTER STRUCT: " << endl;
    //A::print_TID();
    cout << endl;
}


void Parser::D(){
    while(!lex_stack.empty())
        lex_stack.pop();
    lexeme_type v_type = type;
    
    if(type == LEX_INT || type == LEX_BOOL || type == LEX_STRING){
        do{
            get_nlex();
            if(type != LEX_ID)
                throw runtime_error("expect name of variable\n");
            
            dec(v_type, value);
            lex_stack.push(Scanner::TID[value].get_type());
            int tmp_val = value;
            get_nlex();
            D1(tmp_val);
        }while(type == LEX_COMMA);
    }
    else{
        if(Scanner::TID[value].get_type() != LEX_STRUCTYPE)
            throw runtime_error("Incorrect type of variable\n");
        bool flag = 0;
        int val = value;
        Lexeme first_op = cur_lex;
        get_nlex();
        if(type == LEX_ASSIGN){
            int str_num = Scanner::TID[val].get_value();
            int str_size = structures[str_num].size();
            int num_arr[str_size];
            string var_name = Scanner::TID[val].get_name();

            for(int i = 0; i < str_size; i++){
                int k = Scanner::TID.put_ident(var_name + "." + structures[str_num][i].name);
                num_arr[i] = k;
            }

            struct_assign(first_op, num_arr);
        }
        else{
            do{
                if(flag)
                    get_nlex();
                flag = 1;
                if(type != LEX_ID)
                    throw runtime_error("expect name of variable\n");
                
                string old_name = Scanner::TID[value].get_name();
                dec(LEX_STRUCTYPE, value);
                Scanner::TID[value].set_struct_name(Scanner::TID[val].get_name());
                Scanner::TID[value].set_value(val);
                Lexeme tmp = cur_lex;
                int num_arr[structures[val].size()]; //numbers in TID (new variable)

                for(int i = 0; i < structures[val].size(); i++){
                    int k = Scanner::TID.put_ident(old_name + "." + structures[val][i].name);
                    dec(structures[val][i].type, k);
                    num_arr[i] = k;
                }
                get_nlex();

                struct_assign(tmp, num_arr);
                
            }while(type == LEX_COMMA);
        }
            
    }
}


void Parser::struct_assign(Lexeme first, int* num_arr){

    if(type == LEX_ASSIGN){
        int str_num = Scanner::TID[first.get_value()].get_value();
        get_nlex();
        check_id();
        string var_name = Scanner::TID[value].get_name();
        if(first.get_type() == type && Scanner::TID[first.get_value()].get_struct_name() == Scanner::TID[value].get_struct_name()){
            for(int i = 0; i < structures[str_num].size(); i++){
                int k = Scanner::TID.put_ident(var_name + "." + structures[str_num][i].name);
                prog.put_lex(Lexeme(POLIZ_ADDRESS, num_arr[i]));
                prog.put_lex(Lexeme(LEX_ID, k));
                prog.put_lex(Lexeme(LEX_ASSIGN));
            }
        }
        else
            throw runtime_error("different type in typestruct assignment\n");
        get_nlex();
    }
}


void Parser::D1(int val){
    if(type != LEX_ASSIGN)
        return;
    prog.put_lex(Lexeme(POLIZ_ADDRESS, val));
    get_nlex();
    E();

    eq_type();
    prog.put_lex(Lexeme(LEX_ASSIGN));
}


void Parser::B(bool in_loop){
    if(type != LEX_LBRACE)
        throw runtime_error("No opening brace\n");
    get_nlex();
    do{
        S(in_loop);
        //cout << type << endl;
    }while(type != LEX_RBRACE && type != LEX_FIN);
    if(type != LEX_RBRACE)
        throw runtime_error("No closing brace\n");
    
    get_nlex();
}


void Parser::S(bool in_loop){
    // if(!goto_labels.empty())
    //     for(auto i: goto_labels)
    //         cout << i->get_name() << endl;
    int pol1, pol2, pol3, pol4;
    //cout << cur_lex << " - " << Scanner::lex_to_str[type] << endl;
    //IF
    if(type == LEX_IF){
        get_nlex();
        if(type != LEX_LPAREN)
            throw runtime_error("no opening paren after \"if\"\n");
        E();
        eq_bool();

        pol1 = prog.get_top();
        prog.blank();
        prog.put_lex(Lexeme(POLIZ_FGO));
        if(type == LEX_LBRACE)
            B(in_loop);
        else 
            S(in_loop);
        
        pol2 = prog.get_top();
        prog.blank();
        prog.put_lex(Lexeme(POLIZ_GO));
        prog.put_lex(Lexeme(POLIZ_LABEL, prog.get_top()), pol1);

        if(type == LEX_ELSE){
            get_nlex();
            if(type == LEX_LBRACE)
                B(in_loop);
            else 
                S(in_loop);
            prog.put_lex(Lexeme(POLIZ_LABEL, prog.get_top()), pol2);
        }
    }

    //WHILE
    else if(type == LEX_WHILE){
        pol1 = prog.get_top();

        get_nlex();
        E();
        eq_bool();

        pol2 = prog.get_top();
        prog.blank();
        prog.put_lex(Lexeme(POLIZ_FGO));

        vector<int> tmp_br = break_ident;
        break_ident.clear();
    
        if(type == LEX_LBRACE)
            B(true);
        else 
            S(true);

        prog.put_lex(Lexeme(POLIZ_LABEL, pol1));
        prog.put_lex(Lexeme(POLIZ_GO));
        prog.put_lex(Lexeme(POLIZ_LABEL, prog.get_top()), pol2);

        if(!break_ident.empty())
            for(auto& i: break_ident)
                prog.put_lex(Lexeme(POLIZ_LABEL, prog.get_top()), i);
        break_ident = tmp_br;
    }
    
    //// FOR
    else if(type == LEX_FOR){
        in_for = true;

        get_nlex();
        if(type != LEX_LPAREN)
            throw runtime_error("No opening bracket after \"for\"\n");
        
        get_nlex();
        if(type != LEX_SEMICOLON)
            S();

        pol1 = prog.get_top();

        if(type != LEX_SEMICOLON)
            throw runtime_error("No semicolon in \"for\"\n");

        get_nlex();  
        E();
        eq_bool();
        
        pol2 = prog.get_top();
        prog.blank();
        prog.put_lex(Lexeme(POLIZ_FGO));

        pol3 = prog.get_top();
        prog.blank();
        prog.put_lex(Lexeme(POLIZ_GO));
        pol4 = prog.get_top();

        if(type != LEX_SEMICOLON)
            throw runtime_error("No semicolon in \"for\"\n");

        get_nlex();
        if(type != LEX_RPAREN)
            S();

        if(type != LEX_RPAREN)
            throw runtime_error("No closing bracket after \"for\"\n");
        

        prog.put_lex(Lexeme(POLIZ_LABEL, pol1));
        prog.put_lex(Lexeme(POLIZ_GO));

        prog.put_lex(Lexeme(POLIZ_LABEL, prog.get_top()), pol3);

        in_for = false;
        vector<int> tmp_br = break_ident;
        break_ident.clear();

        get_nlex();
        if(type == LEX_LBRACE)
            B(true);
        else 
            S(true);

        prog.put_lex(Lexeme(POLIZ_LABEL, pol4));
        prog.put_lex(Lexeme(POLIZ_GO));
        prog.put_lex(Lexeme(POLIZ_LABEL, prog.get_top()), pol2);
        
        if(!break_ident.empty())
            for(auto& i: break_ident)
                prog.put_lex(Lexeme(POLIZ_LABEL, prog.get_top()), i);
        break_ident = tmp_br;
    }

    //READ
    else if(type == LEX_READ){
        get_nlex();
        if(type != LEX_LPAREN)
            throw runtime_error("No opening bracket after \"read\"\n");
        
        get_nlex();
        if(type != LEX_ID)
            throw runtime_error("No variable in \"read\"\n");

        check_read_id();
        prog.put_lex(Lexeme(POLIZ_ADDRESS, value));
        get_nlex();

        if(type != LEX_RPAREN)
            throw runtime_error("You can read only one variable\n");

        get_nlex();
        prog.put_lex(Lexeme(LEX_READ));

        check_semicolon("read", in_for);
    }

    //WRITE
    else if(type == LEX_WRITE){
        get_nlex();
        if(type != LEX_LPAREN)
            throw runtime_error("No opening bracket after \"write\"\n");
        
        get_nlex();
        E();

        if(type != LEX_RPAREN)
            throw runtime_error("You can read only one variable\n");

        get_nlex();
        prog.put_lex(Lexeme(LEX_WRITE));

        check_semicolon("write");
    }

    //ASSIGN
    else if(type == LEX_ID){
        if(Scanner::TID[value].get_type() == LEX_STRUCTYPE){
            D();
            check_semicolon("declaration variable of TYPESTRUCT", in_for);
        }
        else{
            Lexeme tmp = cur_lex;
            get_nlex();
            if(type == LEX_COLON){
                cur_lex = tmp;
                value = tmp.get_value();
                type = tmp.get_type();
                check_label();
                //get_nlex();
            }
            else if(type == LEX_ASSIGN){
                value = tmp.get_value();
                check_id();
                D1(value);
                check_semicolon("assignment", in_for);
            }
            else
                throw runtime_error("no assignment after variable\n");
        }
    }
    
    //BREAK
    else if(type == LEX_BREAK){
        if(in_loop == false)
            throw runtime_error("\"break\" should only be in the loop");

        break_ident.push_back(prog.get_top());
        prog.blank();
        prog.put_lex(Lexeme(POLIZ_GO));
        get_nlex();
        check_semicolon("break");
    }
    
    //GOTO
    else if(type == LEX_GOTO){
        get_nlex();
        Identifier& label = Scanner::TID[value];
        if(!label.get_declare()){
            dec(LEX_LABEL, value);

            Label* L = new Label(label.get_name());
            L->put_goto(prog.get_top());
            prog.blank();
            prog.blank();
    
            goto_labels.push_back(L);
            label.set_value(static_cast<int>(goto_labels.size()) - 1);
            //prog.put_lex(Lexeme(POLIZ_GO));
        }else{
            //cout << goto_labels[label.get_value()]->get_name() << " " << goto_labels[label.get_value()]->get_label_num() << endl;
            goto_labels[label.get_value()]->put_goto(prog.get_top());
            prog.blank();
            prog.blank();
            //prog.put_lex(Lexeme(POLIZ_GO));
            //A::print_TID();
        }
        get_nlex();
        check_semicolon("goto", in_for);
    }
    
    else{
        D();
        check_semicolon("declaration variable", in_for);
    }
}


void Parser::E(){
    E0();
    while(type == LEX_OR){
        lex_stack.push(type);
        get_nlex();
        E0();
        check_op();
    }
}

void Parser::E0(){
    E1();
    while(type == LEX_AND){
        lex_stack.push(type);
        get_nlex();
        E1();
        check_op();
    }
}


void Parser::E1(){
    E2();
    if(type == LEX_EQUAL || type == LEX_LESS || type == LEX_GREATER || 
        type == LEX_LSEQ || type == LEX_GREQ || type == LEX_NOTEQ) {
        
        lex_stack.push(type);
        get_nlex();
        E2();
        check_op();
    }
}


void Parser::E2(){
    T();
    while (type == LEX_PLUS || type == LEX_MINUS){
        lex_stack.push(type);
        get_nlex();
        T();
        check_op();
    }
}


void Parser::T(){
    F();
    while(type == LEX_SLASH || type == LEX_MULT || type == LEX_PERCENT){
        lex_stack.push(type);
        get_nlex();
        F();
        check_op();
    }
}


void Parser::F(){
    if (type == LEX_ID){
        check_id();
        prog.put_lex(Lexeme(LEX_ID, value));
        get_nlex();
        //cout << lex_stack.top() << endl;
    }
    else if(type == LEX_NUM){
        lex_stack.push(LEX_INT);
        prog.put_lex(cur_lex);
        get_nlex();
    }
    else if(type == LEX_CONSTSTR){
        lex_stack.push(LEX_STRING);
        prog.put_lex(cur_lex);
        get_nlex();
    }
    else if(type == LEX_TRUE){
        lex_stack.push(LEX_BOOL);
        prog.put_lex(Lexeme(LEX_TRUE, 1) );
        get_nlex();
    }
    else if(type == LEX_FALSE){
        lex_stack.push(LEX_BOOL);
        prog.put_lex(Lexeme(LEX_FALSE, 0) );
        get_nlex();
    }
    else if(type == LEX_NOT){
        get_nlex();
        F();
        prog.put_lex(Lexeme(LEX_NOT));
        check_not();
    }
    else if (type == LEX_LPAREN){
        get_nlex();
        E();
        if(type != LEX_RPAREN)
            throw runtime_error("No closing bracket\n");
        get_nlex();
    }
    else throw runtime_error("Unknown expression\n");
}