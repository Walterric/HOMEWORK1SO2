#include <stdio.h>
#include <string.h>
char *risolviInclude(FILE *fileIn);
bool endswith(char *file);
void help();   
char *incolla(FILE *fileH);
char *getInclude(char *line);
int getFSize(FILE *fileH);
extern int globalCommCount;