#include<stdio.h>
#include<stdlib.h>


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
    if(c == ' ' || c == '&' || c == '|' || c == ';' || c == '>' || c == '<' || c == '(' || c == ')')
        return 1;
    else return 0;
}

int double_symbl(char c)
{
    if(c == '&' || c == '|' || c == '>' || c == '<')
        return 1;
    else return 0;
}

char* create_symb(int* j, int str_length, char* str, int* kol_word)
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
    
    (*kol_word)++;
    return m;
}

char* create_word(int* j, int str_length, char* str, int* kol_word)
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

    m[i] = 0;
    (*kol_word)++;

    return m;
}

char** add_word_in_array(char** current_word, int kol_word, char** array_ex, int* array_size)
{
    char **array = array_ex;

    if(kol_word>=*array_size)
        {
            *array_size=*array_size*2+1;
            array=(char**)realloc(array,*array_size * sizeof(char**));
        }
    array[kol_word-1]=*current_word;
    *current_word = NULL;
    return array;
}

void print_array(char** array, int kol_word)
{
    int i;
    for(i=0; i < kol_word; i++)
    {
        printf("%s\n", array[i]);
    }
}

void clean(char*** array, int kol_word)
{
    int j;
    char** tmp = *array;
    for(j = 0; j < kol_word; j++)
    {
        free(tmp[j]);
    }
    free(tmp);
    tmp = NULL;
}

int main(int argc,char* argv[])
{
    FILE* file_in;
    if(argc>1)
    {
        if((file_in=fopen(argv[1],"r"))==NULL)
        {
            printf("File wasn't found\n");
            return 1;
        }
    }
    else file_in=stdin;

    int check_EOF;
    int str_length = 0;
    char* current_str = NULL;

    char** array = NULL;
    int array_size = 0;

    int kol_word = 0;
    char* current_word = NULL;
    int j = 0;

    do 
    {
        current_str = read_str(&check_EOF, &str_length, file_in);
        if(str_length)
        {
            j = 0;
            while(j < str_length)
            {
                if(current_str[j] == ' ')
                {
                    j++;
                }
                else if(separator(current_str[j]))
                {
                    current_word = create_symb(&j, str_length, current_str, &kol_word);
                    array = add_word_in_array(&current_word, kol_word, array, &array_size);
                }
                else
                {
                    current_word = create_word(&j, str_length, current_str, &kol_word);
                    if(current_word != NULL)
                        array = add_word_in_array(&current_word, kol_word, array, &array_size);
                }
                
            }
            
        }
        free(current_str);
        current_str = NULL;
    } while (check_EOF != EOF);

    print_array(array, kol_word);

    clean(&array, kol_word);

    return 0;
}