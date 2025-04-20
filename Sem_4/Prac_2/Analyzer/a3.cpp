#include <iostream>
#include <fstream>
#include <cstring>

enum type_of_lex
{
    LEX_NULL,

    LEX_PROGRAM,
    LEX_INT, LEX_STRING,
    LEX_READ, LEX_WRITE,
    LEX_IF, LEX_THEN, LEX_ELSE,
    LEX_DO, LEX_WHILE,
    LEX_TRUE, LEX_FALSE,
    LEX_AND, LEX_NOT, LEX_OR,

    LEX_PLUS, LEX_MINUS, LEX_SLASH, LEX_STAR,
    LEX_GRT, LEX_LSS, LEX_GOE, LEX_LOE, LEX_EQ, LEX_NEQ,
    LEX_ASSIGN,
    LEX_COMMA, LEX_SEMICOLON, LEX_LPAREN, LEX_RPAREN, LEX_STBLOCK, LEX_FNBLOCK, LEX_QOTE, 

    LEX_NUM,
    LEX_ID // variable
};

class Lex
{
    type_of_lex type;
    int value;

public:
    Lex(type_of_lex type = LEX_NULL, int value = 0) : type(type), value(value) {}
    type_of_lex get_type() const { return type; }
    int get_value() const { return value; }
    friend std::ostream &operator<<(std::ostream &s, Lex l)
    {
        s << '(' << l.get_type() << ", " << l.get_value() << ");";
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

    VTable(int const size):size(size)
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

class Lexer
{
    FILE *fd;
    int c;
    char buf[128];
    int buf_top;
    bool not_eof = true;

    static const char *TW[];
    static type_of_lex wtypes[];
    static const char *TS[];
    static type_of_lex stypes[];

    void clear()
    {
        buf_top = 0;
        for(int i = 0; i < 128; i++)
            buf[i] = 0;
    }

    void add() { buf[buf_top++] = c; };

    void get_symb() { not_eof = ((c = fgetc(fd)) != EOF); }

    int define_type(const char* buf, const char** table)
    {
        int i = 0;
        while(table[i])
        {
            if(!strcmp(buf, table[i]))
                return i;
            i++;
        }
        return 0;
    }

public:
    Lexer(const char* path)
    {
        if((fd = fopen(path, "r")) == NULL)
        {
            std::cout << "Error: file didn't open" << std::endl;
            exit(1);
        }
        clear();
        get_symb();
    }
    ~Lexer() { fclose(fd); }

    Lex get_lex(VTable &T);
    bool check_eof() const { return not_eof; }
};

Lex Lexer::get_lex(VTable &T)
{
    int digit;
    int num;
    while(not_eof)
    {
        while (c == '\n' || c == '\r' || c == ' ' || c == '\t')
            get_symb();
        if (isalpha(c)) // IDENT
        {
            clear();
            while (isalpha(c) || isdigit(c))
            {
                add();
                get_symb();
            }
            if ((num = define_type(buf, TW)))
                return Lex(wtypes[num], num);
            else // variable
            {
                num = T.put(buf);//new or old
                return Lex(LEX_ID, num);
            }
        }
        else if (isdigit(c)) // NUMB
        {
            digit = c - '0';
            get_symb();
            while (isdigit(c))
            {
                digit = digit * 10 + c - '0';
                get_symb();
            }
            return Lex(LEX_NUM, digit);
        }
        else if (c == '<' || c == '>')
        {
            clear();
            add();
            get_symb();
            if (c == '=')
            {
                add();
                get_symb();
            }
            num = define_type(buf, TS);
            return Lex(stypes[num], num);
        }
        else if (c == '=')
        {
            clear();
            add();
            get_symb();
            if (c == '=')
            {
                add();
                get_symb();
            }
            num = define_type(buf, TS);
            return Lex(stypes[num], num);
        }
        else if (c == '!')
        {
            clear();
            add();
            get_symb();
            if (c == '=')
            {
                add();
                get_symb();
                num = define_type(buf, TS);
                return Lex(stypes[num], num);
            }
            else
                throw '!';
        }
        else if(not_eof)
        {
            clear();
            add();
            if ((num = define_type(buf, TS)))
            {
                get_symb();
                return Lex(stypes[num], num);
            }
            else
                throw c;
        }
    }
    std::cout << "How did you end up here?\n";
    std::cout << "Maybe the file has ended?\n";
    return Lex();
}

const char* Lexer::TW[] =
{
    "" // пусто1 т.к. при 0 считаем за ошибку
    "program",

    "int", "string",

    "read", "write",

    "if", "then", "else",

    "do", "while",

    "true", "false",

    "and", "not", "or",

    NULL
};
type_of_lex Lexer:: wtypes[] =
{
    LEX_NULL,
    LEX_PROGRAM,
    LEX_INT, LEX_STRING,
    LEX_READ, LEX_WRITE,
    LEX_IF, LEX_THEN, LEX_ELSE,
    LEX_DO, LEX_WHILE,
    LEX_TRUE, LEX_FALSE,
    LEX_AND, LEX_NOT, LEX_OR,
    LEX_NULL
};

const char* Lexer::TS[] =
{
    "",

    "+", "-", "/", "*",

    ">", "<", ">=", "<=", "==", "!="

    "=",

    ",", ";", "(", ")","{", "}","\"",
    NULL
};
type_of_lex Lexer::stypes[]
{
    LEX_NULL,
    LEX_PLUS, LEX_MINUS, LEX_SLASH, LEX_STAR,
    LEX_GRT, LEX_LSS, LEX_GOE, LEX_LOE, LEX_EQ, LEX_NEQ,
    LEX_ASSIGN,
    LEX_COMMA, LEX_SEMICOLON, LEX_LPAREN, LEX_RPAREN, LEX_STBLOCK, LEX_FNBLOCK, LEX_QOTE, 
    LEX_NULL
};



int main(int argc, char** argv)
{
    VTable T(128);

    Lexer R(argv[1]);
    while(R.check_eof())
    {
        R.get_lex(T);
    }


    return 0;
}














