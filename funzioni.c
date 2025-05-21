#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "HW2.h"

int globalCommCount = 0; //conta le rige di commento eliminate
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
    char *buffalo = (char *)malloc(sizeof(char)*1);
    char *line;
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
            if (buffer == NULL) {
                perror("Error allocating memory");
                return NULL;
            }
            int bufferSize = strlen(buffer);
            buffalo = (char *)realloc(buffalo, sizeof(char)*(bufferSize+strlen(line)));
            strcat(buffalo, buffer);
            free(buffer);
        }
        else {
            /*check tyhe filesize   DEVI RIFARE AFFINCHE ELIMINA I COMMENTI E CERCA DI FARE ELIMINA COMMENTI SU UNA FUNZIONA APPARTE
            int bufferSize = strlen(buffalo);
            // Write the line to the output file
            char *buffer1 = (char *)realloc(buffer,  sizeof(char)*(bufferSize+strlen(line)));
            if (buffer1 == NULL) {
                perror("Error allocating memory");
                return NULL;
            }
            buffer = buffer1;
            strcat(buffer, line);*/  //RICOMINCIARE MEGLIO 
        }

    }
    fclose(fileIn);
   return buffalo;
}

//incolla riga per riga gli include controlando i commenti
char *incolla(FILE *file)
{
    //CERCO LA GRANDEZZA DEL FILE
    int sizeFile = getFSize(file);
    if (sizeFile == -1) {
        perror("Error getting file size");
        return NULL;
    }
    
    //creo un buffer grande quanto il file
    
    char *buffer = (char *)malloc(sizeof(char)*(sizeFile));
    if (buffer == NULL) {
        perror("Error allocating memory (incolla)");
        return NULL;
    } 

    //da qui possibile separazione in una funzione separata
    char *line;
    bool bcomm = false;   // possibile variabile globale 
    while (fgets(line, sizeof(line), file)) {
        if(bcomm == true){
            globalCommCount++;
            if (strstr(line, "*/") != NULL) {
            line = strstr(line, "*/");
            line += 2; // Salta '*/'
                if (line != NULL) {
                    strcat(buffer, line);
                }
                bcomm = false;
            }
        }
        else if (strstr(line, "/*") != NULL) {
            globalCommCount++;
            char *comment = strstr(line, "/*");
            *comment = '\0'; // tronca la riga 
            comment++;
            if (*line != '\0')strcat(buffer, line); // incolla la riga senza commento 
            if (strstr(comment, "*/") == NULL) {
                bcomm = true;
            }
        else if (strstr(line, "//") != NULL) {
            char *comment = strstr(line, "//");
            *comment = '\0'; // tronca la riga 
            if (*line != '\0')strcat(buffer, line); // incolla la riga senza commento
            globalCommCount++;

        }
        } else {
            strcat(buffer, line);
        }
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

int getFSize(FILE *fileH)
{
    fseek(fileH, 0, SEEK_END);
    size_t sizeFile = ftell(fileH);
    rewind(fileH);
    return sizeFile;
}


//funzione di cristiano reintegrato nella funzione incolla.
/*
int rmcomm() {
    FILE *in = fopen(argv[0], "r");
    FILE *out = fopen(argv[1], "w");
    int output[2];
    int commcount = 0;
    char riga[1024];
    int comm = 0;
    while (fgets(riga, 1024, in)) {
        int len = strlen(riga);
        int i = 0;
        int rigacomm = 0;
        while (i < len) {
            if (comm) {
                //Cerca la fine di un commento
                if (i + 1 < len && riga[i] == '*' && riga[i + 1] == '/') {
                    comm = 0;
                    i += 2; //Salta '/*'
                } else {
                    rigacomm= 1;
                    i++;
                }
            } else {
                //Cerca l'inizio di un commento
                if (i + 1 < len && riga[i] == '/' && riga[i + 1] == '*') {
                    comm = 1;
                    rigacomm = 1;
                    i += 2; //Salta 
                } else if (i + 1 < len && riga[i] == '/' && riga[i + 1] == '/') {
                    rigacomm = 1;
                    break; //Commento su una riga, salta il resto della riga
                } else {
                    if (riga[i] != '\n') {
                        fprintf(out, "%c", riga[i]); //Stampa il carattere solo se non è newline
                    }
                    i++;
                }
            }
        }
        if (!comm && riga[len-1] == '\n'){
            fprintf(out, "\n"); //Stampa un newline se non siamo in un commento a più righe
        }
        if (rigacomm) {
            commcount++; //Conta righe di commenti eliminate
        }
    }
    fclose(in);
    fclose(out);
    return commcount;
}*/