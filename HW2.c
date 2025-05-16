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
    else if(argc == 2) 
    {
        //avvio con un file c
        printf("%s", args[1]);
        if(endswith(args[1]) == false)
        {
            printf("Error: you can only put ¨file.c type¨ \n");
            return 1;
        }
        FILE *fileIn = fopen(args[1], "r");
        if (fileIn == NULL) {
            perror("Error opening file1");
            return 1;
        }
        // Process the file here
        char *outPut = risolviInclude(fileIn);//risolvi include
        puts(outPut);
    return 0;
    }
    else if(argc ==3 && (strcmp(args[1], "-i") == 0 || strcmp(args[1], "--in") == 0))
    {
        // Check if the next argument is provided
        if(endswith(args[2]) == false)
        {
            printf("Error: you can only put ¨file.c type¨\n");
            return 1;
        }
        FILE *fileIn = fopen(args[2], "r");
        if (fileIn == NULL) {
            perror("Error opening file");
            exit(1);
        }
        // Process the file here
        return 0;
    }
    else if(argc == 5 && (strcmp(args[1], "-i") == 0 || strcmp(args[1], "--in") == 0) && (strcmp(args[3], "-o") == 0 || strcmp(args[3], "--out") == 0))
    {
        // Check if the next argument is provided
        if(endswith(args[2]) == false)
        {
            printf("Error: you can only put ¨file.c type¨ \n");
            return 1;
        }
        FILE *fileIn = fopen(args[2], "r");
        FILE *fileOut = fopen(args[4], "w");
        if (fileIn == NULL || fileOut == NULL) {
            perror("Error opening file");
            exit(1);
        }
        // Process the file here
        return 0;
    }
    else if(argc == 6 && (strcmp(args[1], "-v") == 0 || strcmp(args[1], "--verbose") == 0) && (strcmp(args[3], "-i") == 0 || strcmp(args[3], "--in") == 0) && (strcmp(args[5], "-o") == 0 || strcmp(args[5], "--out") == 0))
    {
        // Check if the next argument is provided
        if(endswith(args[3]) == false)
        {
            printf("Error: you can only put ¨file.c type¨ \n");
            return 1;
        }
        FILE *fileIn = fopen(args[3], "r");
        FILE *fileOut = fopen(args[5], "w");
        if (fileIn == NULL || fileOut == NULL) {
            perror("Error opening file");
            exit(1);
        }
        // Process the file here
        return 0;
    }
    else 
    {
        perror("Error: Invalid arguments\n");
        help();
        return 0;
    }

}


