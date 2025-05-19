#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "HW2.h"

void help()
{
    printf("Otions: \n1) -i/--in \n2) -o/--out \n3) -v/--verbose\n\n");
    printf("1) ¨-i||--in¨\nmyPreCompiler -i file.c\nmyPreCompiler file.c\nthe two options produce the same result\nThe program will process the input file and print results in stdout\n\n");
    printf("2) ¨-o||--out¨\nmyPreCompiler -i filein.c -o fileOut\nInstead of printing in stdout it will put the results in the indicated output file\n\n");
    printf("3) ¨-v||--verbose¨\nmyPreCompiler -v -c filein.c -o fileOut\nIf in args results will be print o stdout before puting it in Output file\n");
}

bool endswith(char *file)  //da mettere su header 
{
    // Check if the file ends with ".c"
    // Assuming file is a null-terminated string
    int fLen = strlen(file);
    char *ext = file + fLen - 2;
    if (strcmp(ext, ".c") == 0) {
        return true;
    }
    return false;
}


char *risolviInclude(FILE *fileIn)
{
    char *buffer;
    char line[256];
    char *start;
    while (fgets(line, sizeof(line), fileIn)) {
        if (strstr(line, "#include") != NULL) {
            // Found an include line, process it
            start = getInclude(line);
            // Write the include line to the output file
            FILE *fileH = fopen(start, "r");
            if (fileH == NULL) {
                perror("Error opening file header");
                return NULL;
            }
            buffer = incolla(fileH);
        }   
    }
    fclose(fileIn);
   return buffer;
}

//incolla riga per riga gli include
char *incolla(FILE *fileH)
{
    printf("incolla\n");
    int i = 1;
    fseek(fileH, 0, SEEK_END);
    size_t sizeFile = ftell(fileH);
    rewind(fileH);
    char line[256];
    char *buffer = (char *)malloc(sizeof(char)*(sizeFile+i));
    if (buffer == NULL) {
        perror("Error allocating memory");
        return NULL;
    } 
    while (fgets(line, sizeof(line), fileH)) {
        i++;
        char *buffer1 = realloc(buffer, sizeof(char)*(sizeFile+i));
        if (buffer1 == NULL) {
            perror("Error reallocating memory");
            return NULL;
        }
        buffer = buffer1;
        strcat(buffer, line);
    }
    return buffer;
}

char *getInclude(char *line)
{
    char *start = strchr(line, '<');
    if (start == NULL) {
        start = strchr(line, '"');
    }
    if (start != NULL) {
        start++;
        char *end = strchr(start, '>');
        if (end == NULL) {
            end = strchr(start, '"');
        }
        if (end != NULL) {
            *end = '\0'; // Null-terminate the string
            return start;
        }
    }
    return NULL;
}