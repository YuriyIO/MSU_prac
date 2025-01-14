#include <iostream>
#include <fstream>
#include <cstring>

enum type_of_lex
{
    LEX_NULL,

    LEX_PROGRAM,
    LEX_INT,
    LEX_BOOL, // do i need it?
    LEX_STRING,
    LEX_STRING_VAL,
    LEX_READ,
    LEX_WRITE,
    LEX_IF,
    LEX_ELSE,
    LEX_DO,
    LEX_WHILE,
    LEX_TRUE,
    LEX_FALSE,
    LEX_AND,
    LEX_NOT,
    LEX_OR,

    LEX_PLUS,
    LEX_MINUS,
    LEX_SLASH,
    LEX_STAR,
    LEX_GRT,
    LEX_LSS,
    LEX_GOE,
    LEX_LOE,
    LEX_EQ,
    LEX_NEQ,
    LEX_ASSIGN,
    LEX_COMMA,
    LEX_SEMICOLON,
    LEX_LPAREN,
    LEX_RPAREN,
    LEX_STBLOCK,
    LEX_FNBLOCK,
    LEX_QOTE,

    LEX_NUM,
    LEX_ID // variable
};

class Lex
{
    type_of_lex type;
    int IntValue = 0;
    std::string StrValue;

public:
    Lex(type_of_lex type = LEX_NULL, int value = 0) : type(type), IntValue(value) {}
    Lex(type_of_lex type, std::string str = "a") : type(type), StrValue(str) {}

    type_of_lex get_type() const { return type; }
    int get_IntValue() const { return IntValue; }
    std::string get_StrValue() const { return StrValue; }
    friend std::ostream &operator<<(std::ostream &s, Lex l)
    {
        s << '(' << l.get_type() << ", " << l.get_IntValue() << ");";
        return s;
    }
};

class Variable
{
    char *name = nullptr;
    type_of_lex type;
    int IntValue;
    std::string StrValue;
    bool declaration = false;
    bool assign = false;

public:
    Variable() {}
    ~Variable()
    {
        if (name != nullptr)
            delete[] name;
    }

    char *get_name() const { return name; }
    void set_name(const char *n)
    {
        name = new char[strlen(n) + 1];
        strcpy(name, n);
    }

    type_of_lex get_type() const { return type; }
    void set_type(type_of_lex t) { type = t; }

    int get_IntValue() const { return IntValue; }
    void set_IntValue(int t) { IntValue = t; }

    std::string get_StrValue() const { return StrValue; }
    void set_StrValue(std::string str) { StrValue = str; }

    bool get_declaration() const { return declaration; }
    void set_declaration() { declaration = true; }

    bool get_assign() const { return assign; }
    void set_assign() { assign = true; }
};

class VTable
{
    Variable *table = nullptr;
    int top;
    int size;

public:
    VTable() {}
    void set_table(int const size)
    {
        if (table == nullptr)
        {
            table = new Variable[size];
            top = 1;
        }
    }
    VTable(int const size) : size(size)
    {
        table = new Variable[size];
        top = 1;
    }
    ~VTable()
    {
        if (table != nullptr)
            delete[] table;
    }

    Variable &operator[](const int k) { return table[k]; }
    int get_size() const { return size; }
    int put(const char *buf)
    {
        for (int i = 1; i < top; i++)
            if (!strcmp(buf, table[i].get_name()))
                return i;
        table[top].set_name(buf);
        top++;
        return top - 1;
    }
};

VTable TID(128);

class Lexer
{
    FILE *fd;
    int c;
    char buf[128];
    int buf_top;
    bool not_eof = true;

    enum state
    {
        H,
        IDENT,
        NUMB,
        COM,
        QOUT,
        COMPR,
        NOT,
        DELIM,
        FIN_OF_FILE,
        SLASH
    };
    state G;

    static const char *TW[];
    static type_of_lex wtypes[];
    static const char *TS[];
    static type_of_lex stypes[];

    void clear()
    {
        buf_top = 0;
        for (int i = 0; i < 128; i++)
            buf[i] = 0;
    }

    void add() { buf[buf_top++] = c; };

    void get_symb() { c = fgetc(fd); }

    int define_type(const char *buf, const char **table)
    {
        int i = 0;
        while (table[i])
        {
            if (!strcmp(buf, table[i]))
                return i;
            ++i;
        }
        return 0;
    }

public:
    Lexer(const char *path)
    {
        if ((fd = fopen(path, "r")) == NULL)
        {
            std::cout << "Error: file didn't open" << std::endl;
            exit(1);
        }
        clear();
        get_symb();
    }
    ~Lexer() { fclose(fd); }

    Lex get_lex();
    bool check_eof() const { return not_eof; }
};

Lex Lexer::get_lex()
{
    int digit;
    int num;
    std::string str;

    G = H;
    while (true)
    {
        switch (G)
        {
        case H:
            if (c == '\n' || c == '\r' || c == ' ' || c == '\t')
                get_symb();
            else if (isalpha(c))
            {
                clear();
                add();
                get_symb();
                G = IDENT;
            }
            else if (isdigit(c))
            {
                digit = c - '0';
                get_symb();
                G = NUMB;
            }
            else if (c == '/')
            {
                clear();
                add();
                get_symb();
                if (c == '*')
                {
                    get_symb();
                    G = COM;
                }
                else
                    G = SLASH;
            }
            else if (c == '"')
            {
                get_symb();
                G = QOUT;
            }
            else if (c == '<' || c == '>' || c == '=')
            {
                clear();
                add();
                get_symb();
                G = COMPR;
            }
            else if (c == '!')
            {
                clear();
                add();
                get_symb();
                G = NOT;
            }
            else if (c != EOF)
            {
                clear();
                add();
                G = DELIM;
            }
            else
                G = FIN_OF_FILE;
            break;

        case IDENT:
            if (isalpha(c) || isdigit(c))
            {
                add();
                get_symb();
            }
            else if ((num = define_type(buf, TW)))
                return Lex(wtypes[num], num);
            else // variable
            {
                num = TID.put(buf); // new or old
                return Lex(LEX_ID, num);
            }
            break;

        case NUMB:
            if (isdigit(c))
            {
                digit = digit * 10 + c - '0';
                get_symb();
            }
            else
                return Lex(LEX_NUM, digit);
            break;

        case COM:
            if (c == '*')
            {
                get_symb();
                if (c == '/')
                {
                    get_symb();
                    G = H;
                }
            }
            else if (c != EOF)
                get_symb();
            else if (c == EOF)
                G = FIN_OF_FILE;
            break;

        case SLASH:
            num = define_type(buf, TS);
            return Lex(stypes[num], num);

        case QOUT:
            if (c == '"')
            {
                get_symb();
                return Lex(LEX_STRING_VAL, str);
            }
            else if (c != EOF)
            {
                str.push_back(c);
                get_symb();
            }
            else if (c == EOF)
                G = FIN_OF_FILE;
            break;

        case COMPR:
            if (c == '=')
            {
                add();
                get_symb();
            }
            num = define_type(buf, TS);
            return Lex(stypes[num], num);

        case NOT:
            if (c == '=')
            {
                add();
                get_symb();
            }
            num = define_type(buf, TS);
            return Lex(stypes[num], num);

        case DELIM:
            if ((num = define_type(buf, TS)))
            {
                get_symb();
                return Lex(stypes[num], num);
            }
            else
                throw c;

        case FIN_OF_FILE:
            not_eof = false;
            return Lex();
        }
    }
}

const char *Lexer::TW[] =
    {
        "", // пусто1 т.к. при 0 считаем за ошибку
        "program",

        "int", "string",

        "read", "write",

        "if", "else",

        "do", "while",

        "true", "false",
        "or", "and",

        NULL};
type_of_lex Lexer::wtypes[] =
    {
        LEX_NULL,
        LEX_PROGRAM,
        LEX_INT, LEX_STRING,
        LEX_READ, LEX_WRITE,
        LEX_IF, LEX_ELSE,
        LEX_DO, LEX_WHILE,
        LEX_TRUE, LEX_FALSE,
        LEX_OR, LEX_AND,
        LEX_NULL};

const char *Lexer::TS[] =
    {
        "",

        "+", "-", "/", "*",

        ">", "<", ">=", "<=", "==", "!=",

        "=",

        ",", ";", "(", ")", "{", "}", "\"",
        "!",
        NULL};
type_of_lex Lexer::stypes[]{
    LEX_NULL,
    LEX_PLUS, LEX_MINUS, LEX_SLASH, LEX_STAR,
    LEX_GRT, LEX_LSS, LEX_GOE, LEX_LOE, LEX_EQ, LEX_NEQ,
    LEX_ASSIGN,
    LEX_COMMA, LEX_SEMICOLON, LEX_LPAREN, LEX_RPAREN, LEX_STBLOCK, LEX_FNBLOCK, LEX_QOTE,
    LEX_NOT,
    LEX_NULL};

template <typename T, int max_size>
class Stack
{
    T s[max_size];
    int top = 0;

public:
    Stack() {}
    void push(T t)
    {
        if (!is_full())
            s[top++] = t;
        else
            throw "Stack is full";
    }
    T pop()
    {
        if (!is_empty())
        {
            // top--;
            return s[--top];
        }
        else
            throw "Stack is empty";
    }

    void reset() { top = 0; }
    bool is_empty() const { return top == 0; }
    bool is_full() const { return top == max_size - 1; }
};

class Parser
{
    Lex curr_lex;
    type_of_lex c_type;
    int c_IntVal;
    std::string c_StrVal;
    int IntVal;
    std::string StrVal;

    Stack<int, 100> st_int;
    Stack<std::string, 100> st_str;
    Stack<type_of_lex, 100> st_lex;

    Lexer scan;

    void check_id()
    {
        if (TID[c_IntVal].get_declaration())
            st_lex.push(TID[c_IntVal].get_type());
        else
            throw "not declarated";
    }

    void check_semicolon()
    {
        if(c_type != LEX_SEMICOLON)
            throw curr_lex;
        get_lex();//were do you want to check ;?
    }
    void get_lex()
    {
        curr_lex = scan.get_lex();
        c_type = curr_lex.get_type();
        c_IntVal = curr_lex.get_IntValue();
        c_StrVal = curr_lex.get_StrValue();
    }

    void check_not()
    {
        if (st_lex.pop() != LEX_BOOL)
            throw "wrong type is in not";
        else
            st_lex.push(LEX_BOOL);
    }

    void check_op()
    {
        type_of_lex t1, t2, op, r;
        t2 = st_lex.pop();
        op = st_lex.pop();
        t1 = st_lex.pop();
        if(t1 == t2)
        {
            if(op == LEX_EQ || op == LEX_NEQ || op == LEX_GRT || op == LEX_LSS || op == LEX_GOE || op == LEX_LOE)
                r = LEX_BOOL;
            else
            {
                if(t1 == LEX_INT && (op == LEX_MINUS || op == LEX_PLUS || op == LEX_SLASH || op == LEX_STAR))
                    r = LEX_INT;
                else if(t1 == LEX_STRING && op == LEX_PLUS)
                    r = LEX_STRING;
                else 
                    throw t2;
            }
        }
        else 
            throw t2;
        st_lex.push(r);
    }

    void eq_bool()
    {
        if (st_lex.pop() != LEX_BOOL)
            throw "expression is not boolean";
    }

    void eq_type()
    {
        if (st_lex.pop() != st_lex.pop())
            throw "wrong types are in =";
    }
    void check_Lparen()
    {
        if (c_type != LEX_LPAREN)
            throw curr_lex;
        get_lex();
    }
    void check_Rparen()
    {
        if (c_type != LEX_RPAREN)
            throw curr_lex;
        get_lex();
    }

    void check_decl()
    {
        if (!TID[c_IntVal].get_declaration())
            throw curr_lex;
    }

    void P();
    void B();
    void S();
    void E();
    void E1();
    void T();
    void F();

    void STR();
    void I();

public:
    Parser(char *program) : scan(program) {}
    void analyze();
};

void Parser::analyze()
{
    get_lex();
    P();
    std::cout << "Success !!\n";
}

void Parser::P()
{
    if (c_type != LEX_PROGRAM)
        throw curr_lex;

    get_lex();

    B();
}

void Parser::B()
{
    if (c_type == LEX_STBLOCK)
    {
        get_lex();
        while(c_type != LEX_FNBLOCK)
        {
            S();
        }
        get_lex();
    }
    else
        throw curr_lex;
}

void Parser::S()
{

    if (c_type == LEX_STRING)
    {
        get_lex();
        STR();
    }
    else if (c_type == LEX_INT)
    {
        get_lex();
        I();
    }
    else if (c_type == LEX_IF)
    {
        get_lex();
        check_Lparen();
        E();
        eq_bool();
        while(c_type == LEX_AND || c_type == LEX_OR)
        {
            get_lex();
            E();
            eq_bool();
        }
        check_Rparen();
        
        if(c_type == LEX_STBLOCK)
        {
            B();
        }
        else S();

        if(c_type == LEX_ELSE)
        {
            get_lex();
            if(c_type == LEX_STBLOCK)
            {
                B();
            }
            else S();
        }

    }
    else if (c_type == LEX_WHILE)
    {
        get_lex();
        check_Lparen();
        E();
        eq_bool();
        while(c_type == LEX_AND || c_type == LEX_OR)
        {
            get_lex();
            E();
            eq_bool();
        }
        check_Rparen();

        if(c_type == LEX_STBLOCK)
        {
            B();
        }
        else S();

    }
    else if (c_type == LEX_READ)
    {
        get_lex();
        check_Lparen();
        if (c_type != LEX_ID)
            throw curr_lex;
        check_decl();
        get_lex();
        check_Rparen();
        check_semicolon();
    }
    else if (c_type == LEX_WRITE)
    {
        get_lex();
        check_Lparen();
        if (c_type != LEX_ID)
            throw curr_lex;
        check_decl();
        get_lex();
        check_Rparen();
        check_semicolon();
    }
    else if (c_type == LEX_ID)
    {
        check_id();
        get_lex();
        if (c_type == LEX_ASSIGN)
        {
            get_lex();
            E();
            eq_type();
        }
        else
            throw curr_lex;
        check_semicolon();
    }
    else
        B();
}

void Parser::STR()
{
    if (c_type != LEX_ID)
        throw curr_lex;

    int var_num;
    while (c_type != LEX_SEMICOLON) // what if string; ?
    {
        if (c_type != LEX_ID)
            throw curr_lex;

        var_num = c_IntVal;
        if (TID[c_IntVal].get_declaration() == true)
            throw  "double declaration";

        TID[var_num].set_declaration();
        TID[var_num].set_type(LEX_STRING);

        get_lex(); // comma || assign
        if (c_type == LEX_ASSIGN)
        {
            get_lex();
            if (c_type == LEX_STRING_VAL)
            {
                TID[var_num].set_assign();
                TID[var_num].set_StrValue(c_StrVal);
            }
            else
                throw curr_lex;
            get_lex(); // comma || semicolon
        }
        if (c_type == LEX_COMMA)
            get_lex();
    }
    check_semicolon();
}

void Parser::I()
{
    if (c_type != LEX_ID)
        throw curr_lex;

    int var_num;
    while (c_type != LEX_SEMICOLON)
    {
        if (c_type != LEX_ID)
            throw curr_lex;

        var_num = c_IntVal;
        if (TID[c_IntVal].get_declaration() == true)
            throw  "double declaration";

        TID[var_num].set_declaration();
        TID[var_num].set_type(LEX_INT);

        get_lex(); // comma || assign
        if (c_type == LEX_ASSIGN)
        {
            get_lex();
            if (c_type == LEX_NUM)
            {
                TID[var_num].set_assign();
                TID[var_num].set_IntValue(c_IntVal);
            }
            else
                throw curr_lex;
            get_lex(); // comma || semicolon
        }
        if (c_type == LEX_COMMA)
            get_lex();
    }
    check_semicolon();
}

void Parser::E()
{
    E1();
    if (c_type == LEX_EQ || c_type == LEX_LSS || c_type == LEX_GRT || c_type == LEX_LOE || c_type == LEX_GOE || c_type == LEX_NEQ)
    {
        st_lex.push(c_type);
        get_lex();
        E1();
        check_op();
    }
}

void Parser::E1()
{
    T();
    while (c_type == LEX_PLUS || c_type == LEX_MINUS)
    {
        st_lex.push(c_type);
        get_lex();
        T();
        check_op();
    }
}

void Parser::T()
{
    F();
    while (c_type == LEX_STAR || c_type == LEX_SLASH)
    {
        st_lex.push(c_type);
        get_lex();
        F();
        check_op();
    }
}

void Parser::F()
{
    if (c_type == LEX_ID)
    {
        check_id();
        get_lex();
    }
    else if (c_type == LEX_NUM)
    {
        st_lex.push(LEX_INT);
        get_lex();
    }
    else if (c_type == LEX_STRING_VAL)
    {
        st_lex.push(LEX_STRING);
        get_lex();
    }
    else if (c_type == LEX_TRUE)
    {
        st_lex.push(LEX_BOOL);
        get_lex();
    }
    else if (c_type == LEX_FALSE)
    {
        st_lex.push(LEX_BOOL);
        get_lex();
    }
    else if (c_type == LEX_NOT)
    {
        get_lex();
        F();
        check_not();
    }
    else if (c_type == LEX_LPAREN)
    {
        get_lex();
        E();
        if (c_type == LEX_RPAREN)
            get_lex();
        else
            throw curr_lex;
    }
    else
        throw curr_lex;
}

int main(int argc, char **argv)
{
    try
    {
        Parser p(argv[1]);
        p.analyze();
    }
    catch(type_of_lex t)
    {
        std::cout <<"Error: lex type == " << t << std::endl;
    }
    catch(Lex l)
    {
        std::cout <<"Error: lex type == " << l.get_type() << std::endl;
    }
    catch(const char* str)
    {
        std::cout << "Error: " << str << std::endl;
    }

    return 0;
}
