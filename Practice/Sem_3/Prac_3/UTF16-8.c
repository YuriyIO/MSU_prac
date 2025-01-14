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

    int bom;
    unsigned short utf16ch;

    fread(&utf16ch, sizeof(utf16ch), 1, file_in);
    if(utf16ch==IBIG_ENDIAN)
    {
        bom=ILITTLE_ENDIAN;
        //printf("LITTLE_ENDIAN\n");
    }
    else if(utf16ch==ILITTLE_ENDIAN)
    {
        bom=IBIG_ENDIAN;
        //printf("BIG_ENDIAN\n");
    }
    else
    {
        if(flag_be) bom=IBIG_ENDIAN;
        else bom=ILITTLE_ENDIAN;
        fseek(file_in, 0, SEEK_SET);
    } 


    //0 127 => 0xxxxxxx
    //128 2047 => 110xxxxx 10xxxxxx
    //2048 65535 => 1110xxxx 10xxxxxx 10xxxxxx
    int kol_byte;
    unsigned char first_byte = 0xEF;
    unsigned char second_byte = 0xBB;
    unsigned char third_byte = 0xBF;

    fwrite(&first_byte, 1, 1, file_out);
    fwrite(&second_byte, 1, 1, file_out);
    fwrite(&third_byte, 1, 1, file_out);
    
    char swop;
    kol_byte = fread(&utf16ch, sizeof(char), 2, file_in);
    while (kol_byte == 2) 
    {
         if (bom == IBIG_ENDIAN) 
         {
            swop = (char)(utf16ch >> 8);
            utf16ch = utf16ch << 8;
            utf16ch += swop;
        }

        if(utf16ch<=127)
        {
            first_byte=utf16ch;
            fwrite(&first_byte, 1, 1, file_out);
        }
        else if((128<=utf16ch) && (utf16ch<=2047))
        {
            second_byte = (utf16ch & 0x3F) | 0x80;
            first_byte = ((utf16ch>>6) & 0x1F) | 0xC0;
            fwrite(&first_byte, 1, 1, file_out);
            fwrite(&second_byte, 1, 1, file_out);
        }
        else if((2048<=utf16ch) && (utf16ch<=65535))
        {
            third_byte = (utf16ch & 0x3F) | 0x80;
            second_byte = ((utf16ch>>6) & 0x3F) | 0x80;
            first_byte = ((utf16ch>>12) & 0xF) | 0xE0;
            fwrite(&first_byte, 1, 1, file_out);
            fwrite(&second_byte, 1, 1, file_out);
            fwrite(&third_byte, 1, 1, file_out);
        }   


        kol_byte=fread(&utf16ch, sizeof(char), 2, file_in);
    }

    if (kol_byte == 1)
    {
        fprintf(stderr, "Single byte was read. Position: %ld\n", ftell(file_in));
    } 


    fclose(file_in);
    fclose(file_out);
    return 0;
}