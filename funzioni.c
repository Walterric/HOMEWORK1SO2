#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include "HW2.h"

int globalCommCount = 0; //conta le rige di commento eliminate
bool bcomm = false; //dice se sto leggendo un blocco di commento o no
char *gbuffer;//buffer per la stringa finale 
int sizeGbuffer; //dimensione del buffer finale
int variableCount = 0; //conta le variabili dichiarate
int nFileIclude = 0; //output file
char *errBuffer; //buffer per gli errori

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
    errBuffer = (char *)malloc(sizeof(char));
    if (gbuffer == NULL || errBuffer == NULL) {
        perror("Error allocating memory for global buffers");
        exit(1);
    }
    gbuffer[0] = '\0'; //Initialize the buffer to an empty string
    errBuffer[0] = '\0';
    sizeGbuffer = 0; //Initialize the size of the buffer
    int sizeErrBuffer = 0;
    char *line = (char *)malloc(sizeof(char)*256); //allocate memory for a line
    if (line == NULL) {
        perror("Error reading line(risolviInclude)");
        return NULL;
    }
    int riga = 1;
    while (fgets(line, 256, fileIn)) {
        char *start;
        if (strstr(line, "#include") != NULL) // Found an include line, process it
        {
            start = getInclude(line); //library name
            if (start == NULL) {
                perror("Error getting include or include not found");
                exit(1);
            }
            FILE *fileH = fopen(start, "r"); //apertura file header 
            if (fileH == NULL){
                perror("Error opening file header");
                exit(1);
            }
            risolviInclude(fileH);//process file header
            fclose(fileH);
            nFileIclude++;
        }
        else {
            processLine(line); //control of every line, exclude comments, variable declaration, valid variable names
            char tline[256]; //temporary line to avoid modifying the original line
            strcpy(tline, line);
            char *bordi = " ;\t";
            char *var = strtok(tline, bordi);
            if (strcmp(var,"int") == 0 || strcmp(var,"float") == 0 || strcmp(var,"char") == 0 || strcmp(var,"double") == 0 || strcmp(var,"long") == 0 || strcmp(var,"short") == 0){
                // Check if the token is a variable declaration
                if (strchr(line, '(') != NULL) {
                    // It's a function declaration, ignore it
                    continue;;
                }
                var = strtok(NULL, bordi);
                if (!checkVar(var)){ //check if variable is valid if not save the error in buffer  
                    char s1[] = "Error: variable "; 
                    char s2[] = " declared in line number ";
                    char *intconv = (char *)malloc(sizeof(char) * 3);
                    sprintf(intconv, "%d\n", riga); // Convert line number to string
                    sizeErrBuffer = strlen(errBuffer) + strlen(s1) + strlen(var) + strlen(s2) + strlen(intconv) + 1;
                    char *buffer1 = (char *)realloc(errBuffer, sizeof(char) * (sizeErrBuffer));
                    if (buffer1 == NULL)
                    {
                        perror("Error reallocating memory");
                        exit(1);
                    }
                    errBuffer = buffer1;
                    strcat(errBuffer, s1);
                    strcat(errBuffer, var);
                    strcat(errBuffer, s2);
                    strcat(errBuffer, intconv);
                }
            }
        }
        riga++;
    }
    return gbuffer;
}

//incolla riga per riga gli include controlando i commenti
void risolviInclude (FILE *file)
{
    char line[256];
    int riga = 0;
    while (fgets(line, sizeof(line), file)) {
        processLine(line);
        riga++;
    }               
}

bool checkVar(char *name)
{
    variableCount++;
    if (name == NULL) { //se variabile vuota 
        return false;
    }
    else if (illegalChar(name)) {
        
        return false;
    }
    else if(isalnum(name[0]) == 0) { //se non inizia con lettera
        return false;
    }
}

bool illegalChar(char *name)
{
    char *illegalChars = "%!@#$^&*()-+=~`[]{}|;:'\",<>?/"; // Definisco caratteri illegali
    // Check if the variable name contains illegal characters
    for (int i = 0; i<strlen(illegalChars); i++) {
        if (strchr(name, illegalChars[i]) != NULL) {
            return true; // Found an illegal character
        }
    }
    return false; // No illegal characters found
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
        globalCommCount++;
        char *comment = strstr(line, "/*");
        *comment = '\n';
        comment++;
        *comment = '\0'; // tronca la riga 
        comment++;
        if (*line != '\0')strcat(gbuffer, line); // incolla la riga senza commento 
        if (strstr(comment, "*/") == NULL) {
            bcomm = true;
        }
    }
    else if (strstr(line, "//") != NULL) {
        char *comment = strstr(line, "//");
        *comment = '\n';
        comment++;
        *comment = '\0'; // tronca la riga 
        if (*line != '\0')strcat(gbuffer, line); // incolla la riga senza commento
        globalCommCount ++;

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



