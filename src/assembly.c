#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include "ciList.c"

#define MAX_FILENAME_SIZE 50

/* PROTOTIPOS */
void generateAsm(CIList *list, char path[]);
char *getBaseName(char *path);

char *getBaseName(char *path) {
    char *path2 = strdup(path);
    path = basename(path2);
    char *retstr;
    char *lastdot;
    if (path == NULL)
         return NULL;
    if ((retstr = malloc (strlen (path) + 1)) == NULL)
        return NULL;
    strcpy (retstr, path);
    lastdot = strrchr (retstr, '.');
    if (lastdot != NULL)
        *lastdot = '\0';
    free(path2);
    return retstr;
}

// char path[] => argv[n] name of test file
void generateAsm(CIList *list, char path[]){
    char *fileName = getBaseName(path);
    FILE *file;
    fileName = strcat(fileName, ".s");
    file = fopen(fileName,"w+");

    CIList *index = ciList;
    while(index != NULL){
        if(strcmp(index->node->codOp, "BEGIN") == 0){
            char *str = strcat(index->node->firstOp->info->var.id, ":");
            fprintf(file, "%s\n", str);
            fprintf(file, "\n" );
        }
        if(strcmp(index->node->codOp, "END") == 0){
            fprintf(file, "\n\n" );
        }
        if(strcmp(index->node->codOp, "ADD") == 0){

        }
        if(strcmp(index->node->codOp, "SUB") == 0){

        }
        if(strcmp(index->node->codOp, "MULT") == 0){

        }
        if(strcmp(index->node->codOp, "DIV") == 0){

        }
        if(strcmp(index->node->codOp, "MOD") == 0){

        }
        if(strcmp(index->node->codOp, "MAY") == 0){

        }
        if(strcmp(index->node->codOp, "MIN") == 0){

        }
        if(strcmp(index->node->codOp, "AND") == 0){

        }
        if(strcmp(index->node->codOp, "OR") == 0){

        }
        if(strcmp(index->node->codOp, "EQUAL") == 0){

        }
        if(strcmp(index->node->codOp, "NEGB") == 0){

        }
        if(strcmp(index->node->codOp, "NEGI") == 0){

        }
        if(strcmp(index->node->codOp, "RETURN") == 0){

        }
        if(strcmp(index->node->codOp, "RETURNV") == 0){

        }
        if(strcmp(index->node->codOp, "IFF") == 0){

        }
        if(strcmp(index->node->codOp, "JMP") == 0){

        }
        if(strcmp(index->node->codOp, "LABEL") == 0){

        }
        if(strcmp(index->node->codOp, "LOAD") == 0){

        }
        if(strcmp(index->node->codOp, "CALL") == 0){
            
        }
        index = index->next;
    }

    fclose(file);
}
