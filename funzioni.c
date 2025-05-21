#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "HW2.h"

int globalCommCount = 0; //conta le rige di commento eliminate
bool bcomm = false; //dice se sto leggendo un blocco di commento o no
char *gbuffer;//buffer per la stringa finale 
int sizeGbuffer; //dimensione del buffer finale
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


char *processaFile(FILE *fileIn)
{   
    gbuffer = (char *)malloc(sizeof(char));
    if (gbuffer == NULL) {
        perror("Error allocating memory");
        return NULL;
    }
    gbuffer[0] = '\0'; // Initialize the buffer to an empty string
    sizeGbuffer = 0; // Initialize the size of the buffer
    char *line = (char *)malloc(sizeof(char)*256); //allocate memory for a line
    if (line == NULL) {
        perror("Error reading line(risolviInclude)");
        return NULL;
    }
    printf("start processing\n");
    while (fgets(line, 256, fileIn)) {
        char *start;
        if (strstr(line, "#include") != NULL) // Found an include line, process it
        {
            start = getInclude(line); //library name
            if (start == NULL) {
                perror("Error getting include");
                return NULL;
            }
            // Write the include line to the output file
            FILE *fileH = fopen(start, "r");
            if (fileH == NULL) {
                perror("Error opening file header");
                return NULL;
            }
            risolviInclude(fileH); //processa il file header
            fclose(fileH);
        }
        else {
            processLine(line); //processa la riga del fil c
        }

    }
    fclose(fileIn);
    return gbuffer;
}

//incolla riga per riga gli include controlando i commenti
char *risolviInclude (FILE *file)
{
    char line[256];
     
    while (fgets(line, sizeof(line), file)) {
        processLine(line);
    }    
    return gbuffer;
}

void processLine (char *line)
{
    sizeGbuffer = strlen(gbuffer) + strlen(line) + 1;
    char *buffer = (char *)realloc(gbuffer, sizeof(char) * (sizeGbuffer));
    if (buffer == NULL)
    {
        perror("Error reallocating memory");
        return;
    }
    gbuffer = buffer;
    if(bcomm == true){
        printf("commento su una riga\n");
        globalCommCount++;
        if (strstr(line, "*/") != NULL) {
        char *line1 = strstr(line, "*/");
        line1 += 2; // Salta '*/'
            if (line1 != NULL) {
                strcat(gbuffer, line1);
            }
        bcomm = false;
        }
    }
    else if (strstr(line, "/*") != NULL) {
        printf("commento su una riga\n");
        globalCommCount++;
        char *comment = strstr(line, "/*");
        *comment = '\0'; // tronca la riga 
        comment++;
        if (*line != '\0')strcat(gbuffer, line); // incolla la riga senza commento 
        if (strstr(comment, "*/") == NULL) {
            bcomm = true;
        }
    }
    else if (strstr(line, "//") != NULL) {
        printf("commento su una riga\n");
        char *comment = strstr(line, "//");
        *comment = '\0'; // tronca la riga 
        if (*line != '\0')strcat(gbuffer, line); // incolla la riga senza commento
        globalCommCount++;

    }
    else {
        strcat(gbuffer, line);
    }
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



