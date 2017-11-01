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
            strcpy(str,"_");
            strcat(str,index->node->firstOp->info->func.id);
            fprintf(file, "%s%s\n", "    .globl ",str);
            strcat(str,":");
            fprintf(file, "%s\n",str);
            fprintf(file, "\n" );
            int aux = abs(index->node->firstOp->info->func.offset);
            fprintf(file, "%s%i%s\n", "    enter $",aux,", $0");
            fprintf(file, "\n" );
        }
        if(strcmp(index->node->codOp, "END") == 0){
            fprintf(file, "%s\n", "    leave");
            fprintf(file, "%s\n", "    ret");
            fprintf(file, "\n" );
        }
        if(strcmp(index->node->codOp, "ADD") == 0){
        	Node *first  = index->node->firstOp;
        	Node *second = index->node->secondOp;
        	/*if(first->tag == 1 && first->tag == 1){   Optimizado!
        		int result =  first->info->cons.value + second->info->cons.value;
        		int offSet =  index->node->temp->info->var.offset;
        		fprintf(file,"%s%i%s%i%s\n", "    movq  $",result,", ",offSet,"(%rbp)"); 	
        	}*/
        	if(first->tag == 1 && first->tag == 1){ //Ambos operandos son constantes
        		int op1 = first->info->cons.value;
        		int op2 = second->info->cons.value;
        		int offSet =  index->node->temp->info->var.offset;
        		fprintf(file,"%s%i%s\n", "    mov  $",op1,", \%eax"); 
        		fprintf(file,"%s%i%s\n", "    add  $",op2,", \%eax"); 
        		fprintf(file,"%s%i%s\n", "    mov  \%eax ,",offSet,"(%rbp)"); 
        	}	

        

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
            int offSet;
            switch (index->node->temp->tag) {
                case 0:                         	
                	offSet= index->node->temp->info->var.offset;
                	fprintf(file,"%s%i%s\n", "    mov ",offSet,"(%rbp), \%eax");  	
                	break; 

                case 1:
                    aux = index->node->temp->info->cons.value;
                    fprintf(file,"%s%i%s\n", "    movq $",aux,", %rax");
                    break;
                case 4:
                	offSet= index->node->temp->info->var.offset;
                	fprintf(file,"%s%i%s\n", "    mov ",offSet,"(%rbp), \%eax");  	
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
        	char str[256];
        	strcpy(str,"");
        	strcat(str, index->node->temp->info->var.id);
        	strcat(str,":");
        	fprintf(file, "%s\n",str);
        }
        if(strcmp(index->node->codOp, "LOAD") == 0){

        }
        if(strcmp(index->node->codOp, "CALL") == 0){
        	char str[256];
        	strcpy(str,"_"); // solo para MAC
        	strcat(str,index->node->firstOp->info->func.id);
			fprintf(file,"%s%s\n", "    call ",str);
        }
        index = index->next;
    }

    fclose(file);
}
