//
// Created by Mattia on 09/05/2025.
// Eros entered on 09/05/2025.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void help()
{
    printf("Otions: \n1) -i/--in \n2) -o/--out \n3) -v/--verbose\n\n");
    printf("1) ¨-i||--in¨\nyPreCompiler -i file.c\nmyPreCompiler file.c\nthe two options produce the same result\nThe program will process the input file and print results in stdout\n\n");
    printf("2) ¨-o||--out¨\nmyPreCompiler -i filein.c -o fileOut\nInstead of printing in stdout it will put the results in the indicated output file\n\n");
    printf("3) ¨-v||--verbose¨\nmyPreCompiler -i filein.c -o fileOut\nIf in args results will be print o stdout before puting it in Output file\n");
}

bool endswith(char *file);

int main(int argc, char *args[]) 
{
    if (argc < 2) 
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
        // Check if the next argument is provided
        if(endswith(args[1]) == false)
        {
            printf("Error: you can only put ¨file.c type¨ \n");
            return 1;
        }
        FILE *fileIn = fopen(args[2], "r");
        if (fileIn == NULL) {
            perror("Error opening file");
            return 1;
        }
        // Process the file here
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