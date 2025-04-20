#include<stdio.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include <sys/stat.h>
#include<string.h>
#include<fcntl.h>

int flag_amp;
int flag_larrow;
int flag_rarrow;
int flag_drarrow;
int flag_start;
int flag_fin;
int flag_usr;
int flag_close;

int* amp_pids = NULL;
int amp_len = 0;

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

void set_flags(char** str, int word_count)
{
    int i = 0;
    while(i < word_count)
    {
        if(!strcmp(str[i], ">"))
        {
            if(flag_fin == 0)
                flag_fin = i;
            flag_rarrow = i;
        }
        else if(!strcmp(str[i], ">>"))
        {
            if(flag_fin == 0)
                flag_fin = i;
            flag_drarrow = i;
        }
        else if(!strcmp(str[i], "<"))
        {
            if(flag_fin == 0)
                flag_fin = i;
            flag_larrow = i;
        }
        else if(!strcmp(str[i], "|"))
        {
            if(flag_fin == 0)
                flag_fin = i;
        }
        else if(!strcmp(str[i], "&"))
        {
            if(flag_amp == 0)
                flag_amp = i;
            if(flag_fin == 0)
                flag_fin = i;
        }
        i++; 
    }
    if(flag_fin == 0)
        flag_fin = word_count;
}
       
void rezero_flags()
{
    flag_rarrow = flag_drarrow = flag_larrow  = flag_start = flag_fin = flag_amp = flag_usr = flag_close = 0;
}

int check_pid(int pid)
{  
    int pid_ind = -5;
    int i;
    for (i = 0; i < amp_len; i++)
    {
        if (amp_pids[i] == pid)
        {
            pid_ind = i;
            break;
        }
    }
    if (pid_ind >= 0)
    {
        amp_len--;
        for (i = pid_ind; i < amp_len; i++)
        {
            amp_pids[i] = amp_pids[i+1];
        }
        return 1;
    }
    return 0;
}

void check_amp_pids()
{ 
    int pid;
    int search;
    int status;
    do
    {
        pid = waitpid(-1, &status, WNOHANG);   
        if (pid > 0)
        {
            search = check_pid(pid);
            if (search)
            {
                if (WIFEXITED(status))
                    printf("[%d] done: status = %d\n", pid, WEXITSTATUS (status));
                else if (WIFSIGNALED (status))
                    printf("[%d] killed by signal %d\n", pid, WTERMSIG (status)); 
                else if (WIFSTOPPED(status))
                    printf("[%d] stopped by signal %d\n", pid, WSTOPSIG(status));
                else if (WIFCONTINUED(status))
                    printf("[%d] continued\n", pid);
            }
        }
    }
    while (pid > 0);
}

void print_flags()
{
    printf("flag_rigth_arrow == %d\n", flag_rarrow);
    printf("flag_double_rigth_arrow == %d\n", flag_drarrow);
    printf("flag_left_arrow == %d\n", flag_larrow);
    printf("flag_cmd_start == %d\n", flag_start);
    printf("flag_cmd_fin == %d\n", flag_fin);
    printf("flag_amp == %d\n", flag_amp);
}

char** set_cmd(char** str, int start, int fin)
{
    char** tmp = (char**)malloc((fin - start + 1) * sizeof(char*));
    for(int i = start, j = 0; i < fin; i++, j++)
    {
        tmp[j] = str[i];
    }
    tmp[fin - start] = NULL;
    return tmp;
}

int reset_flag_fin(char** str, int word_count)
{
    if(flag_fin == word_count)
    {
        return word_count;
    }
    if(flag_rarrow || flag_drarrow)
    {
        if(flag_fin == flag_rarrow || flag_fin == flag_drarrow)
        {
            return word_count;
        }
    }
    int i;
    if(flag_fin == 0)
    {
        i = 0;
    }
    else i = flag_fin + 1;
    while(i < word_count)
    {    
        if(!strcmp(str[i], "|") || !strcmp(str[i], "<") || !strcmp(str[i], ">") || !strcmp(str[i], ">>"))
        {
            return i;
        }
        i++; 
    }
    return word_count;
}

void set_flag_amp(int* word_count)
{
    if(flag_amp)
    {
        if(flag_amp != (*word_count) - 1)
        {
            printf("error: ampersand was expected at the end\n");
            flag_amp = -1;
        }
        else
        {
            (*word_count)--;
            if((*word_count) == 0)
            {
                 printf("some words expected\n");
                 flag_amp = -1;
            }
        }
    }
}

void reset_flag(char** str, int word_count)
{
    flag_start = flag_fin + 1;
    flag_usr = flag_fin;
    flag_close = flag_fin == word_count;
    if(flag_fin != word_count)
    {
        flag_close += !strcmp(str[flag_fin], ">") + !strcmp(str[flag_fin], ">>");
        if(!strcmp(str[flag_fin], "<"))
        {
            flag_fin = reset_flag_fin(str, word_count);
            if(flag_fin != word_count)
                flag_close += !strcmp(str[flag_fin], ">") + !strcmp(str[flag_fin], ">>");
            else flag_close = 1;
        }
        else
        {
            flag_fin = reset_flag_fin(str, word_count);
        } 
    }

}

void addpid(int pid)
{   
    amp_pids =(int*)realloc(amp_pids, (amp_len + 1) * sizeof(int));
    amp_pids[amp_len] = pid;
    amp_len++;
}

int cmd_process(char** str, int word_count)
{
    int fd[2];
    int file;
    int pid;
    int recome = 0;
    char** cmd = NULL;
    
    pipe(fd);
    do
    {
        cmd = set_cmd(str, flag_start, flag_fin);
        reset_flag(str, word_count);
        pid = fork();
        if(!pid)
        {
            if(flag_usr == flag_larrow)
            {
                flag_usr = flag_fin;
                fd[0] = open(str[flag_larrow + 1], O_RDONLY);
                dup2(fd[0], 0);
            }
            if(recome)
            {
                dup2(fd[0], 0);
            }

            if(flag_usr == flag_rarrow)
            {
                file = open(str[flag_rarrow + 1], O_CREAT | O_WRONLY | O_TRUNC, 0666);
                dup2(file, 1);
                close(file);
            }
            else if(flag_usr == flag_drarrow)
            {
                file = open(str[flag_drarrow + 1], O_APPEND | O_WRONLY);
                dup2(file, 1);
                close(file);
            }
            else if(flag_usr != word_count)
            {
			    dup2(fd[1],1);
            }

            close(fd[0]);
            close(fd[1]);
            execvp(cmd[0], cmd);
            perror("2.2");
            exit(2);
        }
        recome ++;
        if(flag_close)
        {
            close(fd[0]);
            close(fd[1]);
        }
        if(flag_amp)
        {
            addpid(pid);
            printf("child pid: [%d]\n", amp_pids[amp_len - 1]);
        }
        else wait(NULL);

        free(cmd);
    } while(flag_close == 0);
    
    return 0;
}



void cmd_impl(char** str, int word_count)
{
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
        rezero_flags();
        set_flags(str, word_count);
        set_flag_amp(&word_count);
        if(flag_amp != -1)
        {
            cmd_process(str, word_count);
        }
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

    signal(SIGCHLD, check_amp_pids);

    do 
    {
        word_count = 0;
        buf = read_str(&check_EOF, &buf_length, file_in);
        str = create_str(&buf, buf_length, &word_count);
        if(str != NULL)
        {
            cmd_impl(str, word_count);
            clean_str(&str, word_count);
        }
        free(buf);
        buf = NULL;
    } while (check_EOF != EOF);

    free(amp_pids);
    fclose(file_in);
    return 0;
}