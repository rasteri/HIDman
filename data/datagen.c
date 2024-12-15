
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "datagen.h"

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

void PrintHexBytes(uint8_t *bytes) {
    uint8_t length = bytes[0] + 1;

    while (length--) {
        printf("0x%02X, ", *(bytes++));
    }
}


void PrintLookupTable(char *TableName, char *Formatting)
{
    printf("const unsigned char * const %s[] = {\n", TableName);

    // for each entry in the hid lookup table
    for (int i = 0; i < 256; i++)
    {
        int found = 0;
        // search for entries in the DB that correspond to that
        for (int j = 0; j < 256; j++)
        {
            if (keys[j].UsagePage == 7 && keys[j].UsageID == i && keys[j].Set1Make[0] != 0)
            {
                printf(Formatting, keys[j].ShortName);
                found = 1;
                break;
            }
        }
        if (!found)
            printf("NULL,\n");
    }

    printf("};\n\n");
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

    //Make/break codes for SET1 and SET2
    //eg --  __code unsigned char KEY_SET1_LSHIFT_MAKE[] = {1, 0x2A};
    for (int i = 0; i < 256; i++)
    {
        if (keys[i].Set1Make[0] != 0) {
            printf("__code unsigned char KEY_SET1_%s_MAKE[] = { ", keys[i].ShortName);
            PrintHexBytes(keys[i].Set1Make);
            printf("};\n");
        }
        if (keys[i].Set1Break[0] != 0) {
            printf("__code unsigned char KEY_SET1_%s_BREAK[] = { ", keys[i].ShortName);
            PrintHexBytes(keys[i].Set1Break);
            printf("};\n");
        }

        if (keys[i].Set2Make[0] != 0) {
            printf("__code unsigned char KEY_SET2_%s_MAKE[] = { ", keys[i].ShortName);
            PrintHexBytes(keys[i].Set2Make);
            printf("};\n");
        }
        if (keys[i].Set2Break[0] != 0) {
            printf("__code unsigned char KEY_SET2_%s_BREAK[] = { ", keys[i].ShortName);
            PrintHexBytes(keys[i].Set2Break);
            printf("};\n\n");
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

    PrintLookupTable("HIDtoSET1_MAKE", "KEY_SET1_%s_MAKE,\n");
    PrintLookupTable("HIDtoSET1_BREAK", "KEY_SET1_%s_BREAK,\n");
    PrintLookupTable("HIDtoSET2_MAKE", "KEY_SET2_%s_MAKE,\n");
    PrintLookupTable("HIDtoSET2_BREAK", "KEY_SET2_%s_BREAK,\n");

    return 0;
}
