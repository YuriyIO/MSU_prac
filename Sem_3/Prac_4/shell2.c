#include<stdio.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>

char* read_str(int* check_EOF,int* str_length, FILE* file_in)
{
    int c;
    int str_size = 0;
    int i = 0;
    char* str = NULL;
    int kol_quot = 0;

    while((c = getc(file_in)) != '\n' && c != EOF && c != '\r')
    {
        if(i >= str_size)
        {
            str_size = str_size*2 +1;
            str=(char*)realloc(str, str_size + 1);
        }
        str[i] = c;
        if(c == '"')
            kol_quot++;
        i++;
    }

    if(kol_quot % 2 != 0)
    {
        str[i] = 0;
        printf("Wrong string: %s\n",str);
        free(str);
        str = NULL;
        i = 0;
    }
    
    if(str != NULL)
    {
        str[i] = 0;
    }
    *str_length = i;
    *check_EOF = c;
    return str;
}

int separator(char c)
{
    return (c == ' ' || c == '&' || c == '|' || c == ';' || c == '>' || c == '<' || c == '(' || c == ')');
}

int double_symbl(char c)
{
    return (c == '&' || c == '|' || c == '>');
}

char* create_symb(int* j, int str_length, char* str, int* word_count)
{
    char* m = NULL;
    int size;
    if(((*j) < (str_length - 1)) && double_symbl(str[*j]) && (str[*j] == str[(*j) + 1]))
    {
        size=3;
        m=(char*)realloc(m, size);
        m[0] = str[*j];
        m[1] = str[(*j)+1];
        m[2] = 0;
        (*j)+=2;
    }
    else
    {
        size=2;
        m=(char*)realloc(m, size);
        m[0] = str[*j];
        m[1] = 0;
        (*j)++;
    }
    
    (*word_count)++;
    return m;
}

char* create_word(int* j, int str_length, char* str, int* word_count)
{
    char* m = NULL;
    int size = 0;
    int i = 0;

    while(!separator(str[*j]) && (*j) < str_length)
    {
        if(str[*j] != '"')
        {
            while(!separator(str[*j]) && str[*j] != '"' && (*j) < str_length)
            {
                if(i >= size)
                {
                    size = size*2 +1;
                    m = (char*)realloc(m, (size + 1) * sizeof(char));
                }
            m[i] = str[*j];
            (*j)++;
            i++;
            }
        }
        else
        {
            (*j)++;
            while(str[*j] != '"' && (*j) < str_length)
            {
                if(i >= size)
                {
                    size = size*2 +1;
                    m = (char*)realloc(m, (size + 1) * sizeof(char));
                }
            m[i] = str[*j];
            (*j)++;
            i++;
            }
            (*j)++;
        }
    }
    if(i == 0)
        return NULL; 
    m[i] = 0;
    (*word_count)++;
    return m;
}

void add_word_in_str(char*** str, int* str_length, char** current_word, int word_count)
{
    if(word_count>=*str_length)
        {
            *str_length = *str_length * 2 + 2;
            (*str) = (char**)realloc((*str), *str_length * sizeof(char*));
        }
    (*str) [word_count-1] = *current_word;
    *current_word = NULL;
}

char** create_str( char** buf, int buf_length, int* word_count)
{
    char** str = NULL;
    int str_length = 0;
    int j = 0;
    char* word = NULL;

    if(buf_length)
    {
        while(j < buf_length)
        {
            if((*buf) [j] == ' ')
            {
                j++;
            }
            else
            {
                if(separator((*buf) [j]))
                {
                    word = create_symb(&j, buf_length, *buf, word_count);
                }
                else
                {
                    word = create_word(&j, buf_length, *buf, word_count);
                }
                if(word != NULL)
                    add_word_in_str(&str, &str_length, &word, *word_count);
            }

        }
        if(*word_count)
            str[*word_count] = NULL;
    }
    return str;
}

void print_str(char** str, int word_count)
{
    int i;
    for(i=0; i < word_count; i++)
    {
        printf("%s\n", str[i]);
    }
}

void cmd_impl(char** str)
{
    int pid;
    if(!strcmp(str[0], "cd"))
    {
        if(str[1] != NULL)
        {
            if(chdir(str[1]) != 0)
                perror(str[1]);
        }
        else chdir(getenv("HOME"));
    }
    else if(!strcmp(str[0], "exit"))
    {
        exit(0);
    }
    else
    {
        pid = fork();
        if(pid == -1)
        {
            perror("1");
            exit(-1);
        }
        else if(!pid)
        {
            execvp(str[0], str);
            perror("2");
            exit(-2);
        }
        wait(NULL);
    }

}

void clean_str(char*** str, int word_count)
{
    char** tmp = *str;
    for(int i = 0; i <= word_count; i++)
    {
        free(tmp[i]);
    }
    free(tmp);
    tmp = NULL;
}

FILE* select_file(int argc, char** argv)
{
    FILE* file_in;
    if(argc > 1)
    {
        if((file_in=fopen(argv[1],"r"))==NULL)
        {
            printf("File wasn't found, file_in == stdin\n");
            file_in = stdin;
        }
    }
    else file_in = stdin;
    return file_in;
}

int main(int argc,char* argv[])
{
    FILE* file_in = select_file(argc, argv);

    char** str = NULL;
    int word_count;

    int check_EOF;
    char* buf = NULL;
    int buf_length = 0;

    do 
    {
        word_count = 0;
        buf = read_str(&check_EOF, &buf_length, file_in);
        str = create_str(&buf, buf_length, &word_count);
        if(str != NULL)
        {
            cmd_impl(str);
            clean_str(&str, word_count);
        }
        free(buf);
        buf = NULL;
    } while (check_EOF != EOF);

    fclose(file_in);
    return 0;
}