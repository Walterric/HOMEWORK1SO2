//
// Created by Mattia on 09/05/2025.
// Eros entered on 09/05/2025.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "HW2.h"

int main(int argc, char *args[]) 
{
    if (argc < 2) //avvio senza argomenti
    {
        perror("No args \n Do: <nameProg> -h || --help> for command options\n");
        exit(1);
    }

    if (strcmp(args[1], "--help") == 0 || strcmp(args[1], "-h") == 0) {
        help();
        return 0;
    }
    else if(argc == 2 ) 
    {
        //avvio con un file c
        printf("processing %s\n", args[1]);
        if(endswith(args[1]) == false)
        {
            perror("Error: you can only put ¨file.c type¨ \n -h/--help for commands");
            exit(1);
        }
        FILE *fileIn = fopen(args[1], "r");
        //controllo apertura file   
        if (fileIn == NULL) {
            perror("Error opening file input");
            exit(1);
        }
        // Process the file here
        char *outPut = processaFile(fileIn);
        puts(outPut);
        if (fclose(fileIn) != 0){
            perror("Error closing input file");
            exit(1);
        }
    return 0;
    }
    else if(argc ==3 && (strcmp(args[1], "-i") == 0 || strcmp(args[1], "--in") == 0))
    {
        //avvio con un file c
        if(endswith(args[2]) == false)
        {
            perror("Error: you can only put ¨file.c type¨ \n -h/--help for commands");
            exit(1);
        }
        FILE *fileIn = fopen(args[2], "r");
        //controllo apertura file 
        if (fileIn == NULL) {
            perror("Error opening file input");
            exit(1);
        }
        // processa il file
        char *outPut = processaFile(fileIn);
        puts(outPut);
        if (fclose(fileIn) != 0) {
            perror("Error closing input file");
            exit(1);
        }
        return 0;
    }
    else if(argc == 5 && (strcmp(args[1], "-i") == 0 || strcmp(args[1], "--in") == 0 || strcmp(args[1], "-c") == 0) && (strcmp(args[3], "-o") == 0 || strcmp(args[3], "--out") == 0))
    {
        //avvio con un file c
        printf("processing %s\n", args[1]);
        if(endswith(args[2]) == false)
        {
            perror("Error: you can only put ¨file.c type¨ \n -h/--help for commands");
            exit(1);
        }
        FILE *fileIn = fopen(args[2], "r");
        FILE *fileOut = fopen(args[4], "w");
        //controllo apertura file 
        if (fileIn == NULL || fileOut == NULL) {
            perror("Error opening file input or output");
            exit(1);
        }
        // processa il file
        char *outPut = processaFile(fileIn);
        fprintf(fileOut, "%s", outPut);
        if (fileOut == NULL) {
            perror("Error writing in output file");
            exit(1);
        }
        if (fclose(fileIn) != 0 || fclose(fileOut)!=0) {
            perror("Error closing input/output file");
            exit(1);
        }
        return 0;
    }
    else if(argc == 6 && (strcmp(args[1], "-v") == 0 || strcmp(args[1], "--verbose") == 0) && strcmp(args[1], "-c") == 0 && (strcmp(args[5], "-o") == 0 || strcmp(args[5], "--out") == 0))
    {
        printf("processing %s\n", args[1]);
        if(endswith(args[2]) == false)
        {
            perror("Error: you can only put ¨file.c type¨ \n -h/--help for commands");
            exit(1);
        }
        FILE *fileIn = fopen(args[2], "r");
        FILE *fileOut = fopen(args[4], "w");
        //controllo apertura file 
        if (fileIn == NULL || fileOut == NULL) {
            perror("Error opening file input or output");
            exit(1);
        }
        // processa il file
        char *outPut = processaFile(fileIn);
        fprintf(fileOut, "%s", outPut);
        if (fileOut == NULL) {
            perror("Error writing in output file");
            exit(1);
        }
        if (fclose(fileIn) != 0 || fclose(fileOut)!=0) {
            perror("Error closing input/output file");
            exit(1);
        }
        return 0;
    }
    else 
    {
        perror("Error: Invalid arguments\n");
        help();
        return 0;
    }

}


