#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define IBIG_ENDIAN 0xFEFF
#define ILITTLE_ENDIAN 0xFFFE

int main(int argc, char* argv[])
{
    int flag_i = 0;
    int flag_o = 0;
    int flag_le = 0;//обратный
    int flag_be = 0;//прямой
    int place_of_wfile;
    int place_of_rfile;

    FILE* file_in;
    FILE* file_out;

    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-i"))
        {
            flag_i = 1;
            place_of_rfile = i + 1;
        }
        if (!strcmp(argv[i], "-o"))
        {
            flag_o = 1;
            place_of_wfile = i + 1;
        }
        if (!strcmp(argv[i], "-le")) flag_le = 1;
        if (!strcmp(argv[i], "-be")) flag_be = 1;
    }
    
    if(flag_le&&flag_be)
    {
        printf("\nerror 1\n");
        return 1;
    }

    if (flag_i) {
        if ((file_in = fopen(argv[place_of_rfile], "r")) == NULL)
        {
            printf("File %s not found\n", argv[place_of_rfile]);
            return 2;
        }
    }
    else file_in = stdin;

    if (flag_o) {
        if ((file_out = fopen(argv[place_of_wfile], "w")) == NULL)
        {
            printf("File '%s' not found\n", argv[place_of_wfile]);
            return 3;
        }
    }
    else file_out = stdout;

    int bom=IBIG_ENDIAN;
    if(flag_be)
    {
        bom=IBIG_ENDIAN;
    }
    else
    {
        bom=ILITTLE_ENDIAN;
    }
    fwrite(&bom,2,1,file_out);

    unsigned char first_byte;
    unsigned char second_byte;
    unsigned char third_byte;

    first_byte = fgetc(file_in);
    second_byte = fgetc(file_in);
    third_byte = fgetc(file_in);

    if(!(first_byte==0xEF && second_byte==0xBB && third_byte==0xBF))
    {
            fseek(file_in,0,SEEK_SET);
    }

    unsigned short utf16_symb;
    unsigned char swop;
    int correct_reading;

    //0 127 0xxxxxxx
    //128 2047 110xxxxx 10xxxxxx
    //2048 65535 1110xxxx 10xxxxxx 10xxxxxx

    first_byte = fgetc(file_in);
    while(!feof(file_in))
    {
        utf16_symb=0;
        correct_reading = 0;

        if((first_byte&0x80)==0)
        {
            utf16_symb+=first_byte;
            correct_reading = 1;
        }
        else if((first_byte&0xE0)==0xC0)
        {
            utf16_symb+=first_byte&0x1F;
            utf16_symb=utf16_symb<<6;
            second_byte = fgetc(file_in);
            if((second_byte & 0xC0)==0x80)
            {
                utf16_symb+=second_byte&0x3F;
                correct_reading = 1;
            }
            else
            {
                fprintf(stderr, "Second byte out of 2 is wrong. Position: %ld\n", ftell(file_in));
            }
        }
        else if((first_byte&0xF0)==0xE0)
        {
            utf16_symb+=first_byte&0xF;
            utf16_symb=utf16_symb<<6;

            second_byte = fgetc(file_in);

            if((second_byte & 0xC0)==0x80)
            {
                utf16_symb+=second_byte&0x3F;
                utf16_symb=utf16_symb<<6;

                third_byte = fgetc(file_in);

                if((third_byte & 0xC0)==0x80)
                {
                    utf16_symb+=third_byte&0x3F;
                    correct_reading = 1;
                }
                else
                {
                fprintf(stderr, "Second byte out of 3 is wrong. Position: %ld\n", ftell(file_in));
                }
            }   
            else
            {
                fprintf(stderr, "Second byte out of 3 is wrong. Position: %ld\n", ftell(file_in));
            }
        }   
        else
        {
            fprintf(stderr, "First byte out of 1 is wrong. Position: %ld\n", ftell(file_in));
        }
        if(correct_reading)
        {
            if(bom==ILITTLE_ENDIAN)
            {
            swop=utf16_symb;
            utf16_symb=utf16_symb>>8;
            utf16_symb=utf16_symb+(swop<<8);
            }
            fwrite(&utf16_symb,2,1,file_out);
        }
        first_byte = fgetc(file_in);
    }

    fclose(file_in);
    fclose(file_out);
    return 0;
}