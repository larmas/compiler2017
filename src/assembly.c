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

void generateAsm(CIList *list, char path[]){
    char *fileName = getBaseName(path);
    FILE *file;
    fileName = strcat(fileName, ".s");
    file = fopen(fileName,"w+");

    CIList *index = ciList;
    while(index != NULL){
        if(strcmp(index->node->codOp, "BEGIN") == 0){
            char str[256];
            if(strcmp(index->node->firstOp->info->func.id, "main") == 0){
                strcpy(str,"_");
                strcat(str,index->node->firstOp->info->func.id);
                fprintf(file, "%s%s\n", "    .globl ",str);
                strcat(str,":");
            }else{
                strcpy(str,index->node->firstOp->info->func.id);
                fprintf(file, "%s%s\n", "    .globl ",str);
                strcat(str,":");
            }
            fprintf(file, "%s\n",str);
            int aux = abs(index->node->firstOp->info->func.offset);
            fprintf(file, "%s%i%s\n", "    enter $",aux,", $0");
        }
        if(strcmp(index->node->codOp, "END") == 0){
            fprintf(file, "%s\n", "    leave");
            fprintf(file, "%s\n", "    ret");
            fprintf(file, "\n" );
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
            int aux;
            switch (index->node->temp->tag) {
                case 0:            
                	/*int offSet = index->node->temp->info->var.offset;
                	char aux1[20];
            		sprintf(aux,"%d",offSet);
            		aux =   strcat(aux,"(%rbp)");
                	fprintf(file,"%s%i%s\n", "    mov ",aux,", \%eax");
                	*/
                	break;               
                case 1:
                    aux = index->node->temp->info->cons.value;
                    fprintf(file,"%s%i%s\n", "    mov $",aux,", %eax");
                    break;
                case 4:
                    break;
                default:
                    break;
            }
        }
        if(strcmp(index->node->codOp, "RETURNV") == 0){
        	//no hago nada
        }
        if(strcmp(index->node->codOp, "IFF") == 0){

        }
        if(strcmp(index->node->codOp, "JMP") == 0){
        	fprintf(file,"%s%s\n", "    jmp ",index->node->temp->info->var.id);

        }
        if(strcmp(index->node->codOp, "LABEL") == 0){
        	char *str = strcat(index->node->temp->info->var.id, ":");
        	fprintf(file, "%s\n",str);
        }
        if(strcmp(index->node->codOp, "LOAD") == 0){

        }
        if(strcmp(index->node->codOp, "CALL") == 0){
        	char str[256];
        	strcpy(str,"_");
        	strcat(str,index->node->firstOp->info->func.id);
			fprintf(file,"%s%s\n", "    call ",str);
        }
        index = index->next;
    }

    fclose(file);
}
