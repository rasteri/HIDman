
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char *
strsep (char **stringp, const char *delim)
{
  char *start = *stringp;
  char *ptr;

  if (start == NULL)
    return NULL;

  /* Optimize the case of no delimiters.  */
  if (delim[0] == '\0')
    {
      *stringp = NULL;
      return start;
    }

  /* Optimize the case of one delimiter.  */
  if (delim[1] == '\0')
    ptr = strchr (start, delim[0]);
  else
    /* The general case.  */
    ptr = strpbrk (start, delim);
  if (ptr == NULL)
    {
      *stringp = NULL;
      return start;
    }

  *ptr = '\0';
  *stringp = ptr + 1;

  return start;
}

typedef struct _KEY {
    char Name[100];
    char FriendlyName[100];
    char ShortName[100];
    char Legend[100];
    unsigned int xpos, ypos, xsize, ysize, UsagePage, UsageID;
    uint8_t Set1Make[100], Set1Break[100], Set2Make[100], Set2Break[100];
} KEY;

void parsehexbytesequence(uint8_t *dest, char *src) {
    
    char *oldsrc = 0;

    // first byte of dest is length of sequence
    char *origdest = dest;
    *origdest = 0;
    dest++;

    while (1) {
        oldsrc = src;

        *(dest++) = (uint8_t)strtoul(src, &src, 16);

        if (oldsrc == src) 
            break;

        (*origdest)++;
    }
    
}

const char* tokline(char* line, KEY *key)
{
    #define KEYNAME 0
    #define FRIENDLYNAME 1
    #define SHORTNAME 2
    #define LEGEND 3
    #define XPOS 4
    #define YPOS 5
    #define XSIZE 6
    #define YSIZE 7
    #define USAGEPAGE 8
    #define USAGEID 9
    #define SET1MAKE 10
    #define SET1BREAK 11
    #define SET2MAKE 12
    #define SET2BREAK 13
    #define ENDOFLINE 14
    
    uint8_t field;
    
    char* tok;
    
    while ((tok = strsep(&line, ",")) != NULL)
    {
        switch(field) {
            case KEYNAME:
                strncpy(key->Name, tok, 100);
                break;
            case FRIENDLYNAME:
                strncpy(key->FriendlyName, tok, 100);
                break;
            case SHORTNAME:
                strncpy(key->ShortName, tok, 100);
                break;
            case LEGEND:
                parsehexbytesequence(key->Legend, tok);
                break;
            case XPOS:
                key->xpos = atoi(tok);
                break;
            case YPOS:
                key->ypos = atoi(tok);
                break;
            case XSIZE:
                key->xsize = atoi(tok);
                break;
            case YSIZE:
                key->ysize = atoi(tok);
                break;
            case USAGEPAGE:
                key->UsagePage = strtoul(tok, NULL, 16);
                break;
            case USAGEID:
                key->UsageID = strtoul(tok, NULL, 16);
                break;
            case SET1MAKE:
                parsehexbytesequence(key->Set1Make, tok);
                break;
            case SET1BREAK:
                parsehexbytesequence(key->Set1Break, tok);
                break;
            case SET2MAKE:
                parsehexbytesequence(key->Set2Make, tok);
                break;
            case SET2BREAK:
                parsehexbytesequence(key->Set2Break, tok);
                break;
        }
        
        if (field++ == ENDOFLINE)
            break;
    }
    return NULL;
}


KEY keys[256];

void PrintHexBytesWithLength(FILE *file, uint8_t *bytes) {
    uint8_t length = bytes[0] + 1;

    while (length--) {
        fprintf(file, "0x%02X, ", *(bytes++));
    }
}

void PrintHexBytes(FILE *file, uint8_t *bytes) {
    uint8_t length = bytes[0] + 1;

    while (--length) {
        fprintf(file, "0x%02X, ", *(++bytes));
    }
}

#define LU_SET1MAKE 0
#define LU_SET1BREAK 1
#define LU_SET2MAKE 2
#define LU_SET2BREAK 3

#define LU_0C_SET1MAKE 4
#define LU_0C_SET1BREAK 5
#define LU_0C_SET2MAKE 6
#define LU_0C_SET2BREAK 7

void PrintLookupTable(FILE *file, char which)
{

    char TableName[100];
    char Formatting[100];

    switch (which) {
        case LU_SET1MAKE: strcpy(TableName, "HIDtoSET1_Make"); strcpy(Formatting, "KEY_SET1_%s_MAKE, //%d\n"); break;
        case LU_SET1BREAK: strcpy(TableName, "HIDtoSET1_Break"); strcpy(Formatting, "KEY_SET1_%s_BREAK, //%d\n"); break;
        case LU_SET2MAKE: strcpy(TableName, "HIDtoSET2_Make"); strcpy(Formatting, "KEY_SET2_%s_MAKE, //%d\n"); break;
        case LU_SET2BREAK: strcpy(TableName, "HIDtoSET2_Break"); strcpy(Formatting, "KEY_SET2_%s_BREAK, //%d\n"); break;

        case LU_0C_SET1MAKE: strcpy(TableName, "HID0CtoSET1_Make"); strcpy(Formatting, "KEY_SET1_%s_MAKE}, //%d\n"); break;
        case LU_0C_SET1BREAK: strcpy(TableName, "HID0CtoSET1_Break"); strcpy(Formatting, "KEY_SET1_%s_BREAK}, //%d\n"); break;
        case LU_0C_SET2MAKE: strcpy(TableName, "HID0CtoSET2_Make"); strcpy(Formatting, "KEY_SET2_%s_MAKE}, //%d\n"); break;
        case LU_0C_SET2BREAK: strcpy(TableName, "HID0CtoSET2_Break"); strcpy(Formatting, "KEY_SET2_%s_BREAK}, //%d\n"); break;
    }

    int UsagePage = 0;
    int NumEntries = 0;

    if (which < 4) {
        UsagePage = 0x07;
        NumEntries = 256;
        fprintf(file, "const unsigned char * const %s[] = {\n", TableName);
    }
    else
    {
        UsagePage = 0x0C;
        NumEntries = 1024;
        fprintf(file, "const EXTCHARLOOKUP %s[] = {\n", TableName);
    }

    // for each entry in the hid lookup table
    for (int i = 0; i < NumEntries; i++)
    {
        int found = 0;
        // search for entries in the DB that correspond to that
        for (int j = 0; j < 256; j++)
        {
            uint8_t *array;

            

            switch (which) {
                case LU_SET1MAKE: case LU_0C_SET1MAKE: array = keys[j].Set1Make; break;
                case LU_SET1BREAK: case LU_0C_SET1BREAK: array = keys[j].Set1Break; break;
                case LU_SET2MAKE: case LU_0C_SET2MAKE: array = keys[j].Set2Make; break;
                case LU_SET2BREAK: case LU_0C_SET2BREAK: array = keys[j].Set2Break; break;
            }



            if (keys[j].UsagePage == UsagePage && keys[j].UsageID == i && array[0] != 0)
            {
                fprintf(file, "\t");

                //0x0C table is sparse, print index field
                if (UsagePage == 0x0C) 
                    fprintf(file, "{%d, ", i);

                fprintf(file, Formatting, keys[j].ShortName, i);

                found = 1;
                break;
            }
        }
        if (!found && UsagePage == 0x07) // print a dummy entry if regular keys
            fprintf(file, "\tNULL, //%d\n", i);
    }

    // Todo, print a sparser table for 0x0C usage page
    // will use the EXTCHARLOOKUP struct in defs.h

    fprintf(file, "};\n\n");
}

int main() {
    FILE* file = fopen("c:\\code\\hidman\\data\\keyboard_data.csv", "r");

    char line[1024];

    int keycount = 0;

    memset (keys, 0x00, 256 * sizeof(KEY));

    while (fgets(line, 1024, file))
    {
        tokline(line, &keys[keycount]);
        puts(keys[keycount].ShortName);
        keycount++;
    }

    fclose(file);

    file = fopen("scancode.c", "w");
    fprintf(file, "#include<stdio.h>\n#ifndef __SDCC\n#define __code\n#else\n#include\"defs.h\"\n#endif\n");

    //Make/break codes for SET1 and SET2
    //eg --  __code unsigned char KEY_SET1_LSHIFT_MAKE[] = {1, 0x2A};
    for (int i = 0; i < 256; i++)
    {
        if (keys[i].Set1Make[0] != 0) {
            fprintf(file, "__code unsigned char KEY_SET1_%s_MAKE[] = { ", keys[i].ShortName);
            PrintHexBytesWithLength(file, keys[i].Set1Make);
            fprintf(file, "};\n");
        }
        if (keys[i].Set1Break[0] != 0) {
            fprintf(file, "__code unsigned char KEY_SET1_%s_BREAK[] = { ", keys[i].ShortName);
            PrintHexBytesWithLength(file, keys[i].Set1Break);
            fprintf(file, "};\n");
        }

        if (keys[i].Set2Make[0] != 0) {
            fprintf(file, "__code unsigned char KEY_SET2_%s_MAKE[] = { ", keys[i].ShortName);
            PrintHexBytesWithLength(file, keys[i].Set2Make);
            fprintf(file, "};\n");
        }
        if (keys[i].Set2Break[0] != 0) {
            fprintf(file, "__code unsigned char KEY_SET2_%s_BREAK[] = { ", keys[i].ShortName);
            PrintHexBytesWithLength(file, keys[i].Set2Break);
            fprintf(file, "};\n\n");
        }
        
    }

    // lookup tables for hid to SET1 and SET2
    // const unsigned char * const HIDtoSET1_Make[] = {
    //        NULL,
    //        NULL,
    //        NULL,
    //        NULL,
    //        KEY_SET1_A_MAKE,
    //        KEY_SET1_B_MAKE,
    //        KEY_SET1_C_MAKE,
    //        KEY_SET1_D_MAKE,
    //        KEY_SET1_E_MAKE
    // ...

    PrintLookupTable(file, LU_SET1MAKE);
    PrintLookupTable(file, LU_SET1BREAK);
    PrintLookupTable(file, LU_SET2MAKE);
    PrintLookupTable(file, LU_SET2BREAK);

    PrintLookupTable(file, LU_0C_SET1MAKE);
    PrintLookupTable(file, LU_0C_SET1BREAK);
    PrintLookupTable(file, LU_0C_SET2MAKE);
    PrintLookupTable(file, LU_0C_SET2BREAK);

    fclose(file);
    file = fopen("../mashpipe/keydefs.c", "w");
    fprintf(file, "#include \"keydefs.h\"\n#include \"..\\firmware\\scancode.c\"\n");

    // Key names for mashpipe
    //char NAME_Z[] = "Z";

    // legend in cp432
    //char LEGEND_Z[] = "Z";

    for (int j = 0; j < 256; j++) 
    {
        if (keys[j].Set1Make[0] != 0){ 
            fprintf(file, "char NAME_%s[] = \"%s\";\n", keys[j].ShortName, keys[j].FriendlyName);
            fprintf(file, "unsigned char LEGEND_%s[] = {", keys[j].ShortName);
            PrintHexBytes(file, keys[j].Legend);
            fprintf(file, "0x00};\n\n");
        }
    }

    fprintf(file, "KeyDef KeyDefs[] = {\n");

    for (int j = 0; j < 256; j++) 
    {
        int length = 0;

        length += sprintf(line+length, "{");

        char printline = 0;

        if (keys[j].Set1Make[0]){ length += sprintf(line+length, "KEY_SET1_%s_MAKE, ", keys[j].ShortName); printline = 1;}
        else length += sprintf(line+length, "NULL, ");

        if (keys[j].Set1Break[0]){ length += sprintf(line+length, "KEY_SET1_%s_BREAK, ", keys[j].ShortName); printline = 1;}
        else length += sprintf(line+length, "NULL, ");

        if (keys[j].Set2Make[0]){ length += sprintf(line+length, "KEY_SET2_%s_MAKE, ", keys[j].ShortName); printline = 1;}
        else length += sprintf(line+length, "NULL, ");

        if (keys[j].Set2Break[0]){ length += sprintf(line+length, "KEY_SET2_%s_BREAK, ", keys[j].ShortName); printline = 1;}
        else length += sprintf(line+length, "NULL, ");

        if (!printline) continue; //if no codes, don't bother to spit out line

        length += sprintf(line+length, "NAME_%s, ", keys[j].ShortName);
        length += sprintf(line+length, "LEGEND_%s, ", keys[j].ShortName);

        fprintf(file, "%s %u, %u, %u, %u},\n", line, keys[j].xpos, keys[j].ypos, keys[j].xsize, keys[j].ysize);
    }
    fprintf(file, "{NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}\n");
    fprintf(file, "};\n");

    //{ XT_KEY_PAD3_MAKE, XT_KEY_PAD3_BREAK, KEY_PAD3_MAKE, KEY_PAD3_BREAK, NAME_PAD3, LEGEND_PAD3,54,14,2,2}

    return 0;
}
