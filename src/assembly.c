#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include "ciList.c"

#define MAX_FILENAME_SIZE 50

/* PROTOTIPOS */
void generateAsm(CIList *list, char path[], char machine[]);
char *getBaseName(char *path);

/* Obtiene el nombre base de un archivo sin la extension */
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

/* Genera el codigo assembler equivalente a una lista de codigo intermedio */
void generateAsm(CIList *list, char path[], char machine[]){
    static int labelCount = 0;
    char *fileName = getBaseName(path);
    FILE *file;
    fileName = strcat(fileName, ".s");
    file = fopen(fileName,"w+");
    CIList *index = ciList;
    while(index != NULL){

        Node *first  = index->node->firstOp;
        Node *second = index->node->secondOp;
        Node *temp = index->node->temp;

        if(strcmp(index->node->codOp, "BEGIN") == 0){
            char str[256];
            if(strcmp(first->info->func.id, "main") == 0){
                if (strcmp(machine, "Mac") == 0){
                    strcpy(str, "_");
                    strcat(str, first->info->func.id);
                    fprintf(file, "%s%s\n", "    .globl ",str);
                } else {
                    strcpy(str, first->info->func.id);
                    fprintf(file, "%s%s\n", "    .globl ",str);
                }
                strcat(str,":");
            } else {
                strcpy(str, first->info->func.id);
                fprintf(file, "%s%s\n", "    .globl ",str);
                strcat(str, ":");
            }
            fprintf(file, "%s\n",str);
            int aux = abs(first->info->func.offset);
            while( (aux % 16) != 0){
                aux += 8;
            }
            fprintf(file, "%s%i%s\n", "    enter $", aux, ", $0");
            if(strcmp(first->info->func.id, "main") != 0){
				int longParam = longList(first->info->func.param);
				List *param = first->info->func.param;
				for (int i = 0; i < longParam; ++i){
					int offSet = param->node->info->var.offset;
		            switch (i) {
		                case 0:
		                    fprintf(file,"%s%i%s\n","    movq %rdi, ",offSet,"(%rbp)");
		                    break;
		                case 1:
		                    fprintf(file,"%s%i%s\n","    movq %rsi, ",offSet,"(%rbp)");
		                    break;
		                case 2:
		                    fprintf(file,"%s%i%s\n","    movq %rdx, ",offSet,"(%rbp)");
		                    break;
		                case 3:
		                    fprintf(file,"%s%i%s\n","    movq %rcx, ",offSet,"(%rbp)");
		                    break;
		                case 4:
		                    fprintf(file,"%s%i%s\n","    movq %r8, ",offSet,"(%rbp)");
		                    break;
		                case 5:
		                    fprintf(file,"%s%i%s\n","    movq %r9, ",offSet,"(%rbp)");
		                    break;
		                default:
		                    break;
		            }
		            param = param->next;
	            }
	        }
        }


        if(strcmp(index->node->codOp, "ADD") == 0){
        	if((first->tag == 0 || first->tag == 4) && (second->tag == 0 || second->tag == 4)){
        		int offSet1 = first->info->var.offset;
        		int offSet2 = second->info->var.offset;
        		int offSetTemp = temp->info->var.offset;
				fprintf(file,"%s%i%s\n", "    movq ",offSet1,"(%rbp), %rdx");
        		fprintf(file,"%s%i%s\n", "    movq ",offSet2,"(%rbp), %rax");
        		fprintf(file,"%s\n",     "    addq %rdx, %rax");
        		fprintf(file,"%s%i%s\n", "    movq %rax ,",offSetTemp,"(%rbp)");
        	}
            if((first->tag != 0 && first->tag != 4) || (second->tag != 0 && second->tag != 4)){
                if(first->tag == 1 && second->tag == 1){
                    int op1 = first->info->cons.value;
                    int op2 = second->info->cons.value;
                    int offSet =  temp->info->var.offset;
                    fprintf(file,"%s%i%s\n", "    movq $",op1,", %rax");
                    fprintf(file,"%s%i%s\n", "    addq $",op2,", %rax");
                    fprintf(file,"%s%i%s\n", "    movq %rax ,",offSet,"(%rbp)");
                } else {
                    if(first->tag != 0 && first->tag != 4){
                        int op1 = first->info->cons.value;
                        int offSet2 =  second->info->var.offset;
                        int offSetTemp =  temp->info->var.offset;
                        fprintf(file,"%s%i%s\n", "    movq ",offSet2,"(%rbp), %rax");
                        fprintf(file,"%s%i%s\n", "    addq $",op1,", %rax");
                        fprintf(file,"%s%i%s\n", "    movq %rax ,",offSetTemp,"(%rbp)");
                    } else {
                        int op2 = second->info->cons.value;
                        int offSet1 =  first->info->var.offset;
                        int offSetTemp =  temp->info->var.offset;
                        fprintf(file,"%s%i%s\n", "    movq ",offSet1,"(%rbp), %rax");
                        fprintf(file,"%s%i%s\n", "    addq $",op2,", %rax");
                        fprintf(file,"%s%i%s\n", "    movq %rax ,",offSetTemp,"(%rbp)");
                    }
                }
            }
        }


        if(strcmp(index->node->codOp, "SUB") == 0){
            if((first->tag == 0 || first->tag == 4) && (second->tag == 0 || second->tag == 4)){
                int offSet1 = first->info->var.offset;
                int offSet2 = second->info->var.offset;
                int offSetTemp = temp->info->var.offset;
                fprintf(file,"%s%i%s\n", "    movq ",offSet1,"(%rbp), %rax");
                fprintf(file,"%s%i%s\n", "    subq ",offSet2,"(%rbp), %rax");
                fprintf(file,"%s%i%s\n", "    movq %rax ,",offSetTemp,"(%rbp)");
            }
            if((first->tag != 0 && first->tag != 4) || (second->tag != 0 && second->tag != 4)){
                if(first->tag == 1 && second->tag == 1){
                    int op1 = first->info->cons.value;
                    int op2 = second->info->cons.value;
                    int offSet =  temp->info->var.offset;
                    fprintf(file,"%s%i%s\n", "    movq $",op1,", %rax");
                    fprintf(file,"%s%i%s\n", "    subq $",op2,", %rax");
                    fprintf(file,"%s%i%s\n", "    movq %rax ,",offSet,"(%rbp)");
                } else {
                    if(first->tag != 0 && first->tag != 4){
                        int op1 = first->info->cons.value;
                        int offSet2 =  second->info->var.offset;
                        int offSetTemp =  temp->info->var.offset;
                        fprintf(file,"%s%i%s\n", "    movq $",op1,", %rax");
                        fprintf(file,"%s%i%s\n", "    subq ",offSet2,"(%rbp), %rax");
                        fprintf(file,"%s%i%s\n", "    movq %rax ,",offSetTemp,"(%rbp)");
                    } else {
                        int offSet1 =  first->info->var.offset;
                        int op2 =  second->info->cons.value;
                        int offSetTemp =  temp->info->var.offset;
                        fprintf(file,"%s%i%s\n", "    movq ",offSet1,"(%rbp), %rax");
                        fprintf(file,"%s%i%s\n", "    subq $",op2,", %rax");
                        fprintf(file,"%s%i%s\n", "    movq %rax ,",offSetTemp,"(%rbp)");
                    }
                }
            }
        }


        if(strcmp(index->node->codOp, "MULT") == 0){
            if((first->tag == 0 || first->tag == 4) && (second->tag == 0 || second->tag == 4)){
                int op1 = first->info->cons.value;
                int op2 = second->info->cons.value;
                int offSet1 = first->info->var.offset;
                int offSet2 = second->info->var.offset;
                int offSetTemp = temp->info->var.offset;
                fprintf(file,"%s%i%s\n", "    movq ",offSet1,"(%rbp), %rdx");
                fprintf(file,"%s%i%s\n", "    movq ",offSet2,"(%rbp), %rax");
                fprintf(file,"%s\n",     "    imulq %rdx, %rax");
                fprintf(file,"%s%i%s\n", "    movq %rax ,",offSetTemp,"(%rbp)");
            }
            if((first->tag != 0 && first->tag != 4) || (second->tag != 0 && second->tag != 4)){
                if(first->tag == 1 && second->tag == 1){
                    int op1 = first->info->cons.value;
                    int op2 = second->info->cons.value;
                    int offSet =  temp->info->var.offset;
                    fprintf(file,"%s%i%s\n", "    movq $",op1,", %rax");
                    fprintf(file,"%s%i%s\n", "    imulq $",op2,", %rax");
                    fprintf(file,"%s%i%s\n", "    movq %rax ,",offSet,"(%rbp)");
                } else {
                    if(first->tag != 0 && first->tag != 4){
                        int op1 = first->info->cons.value;
                        int offSet2 =  second->info->var.offset;
                        int offSetTemp =  temp->info->var.offset;
                        fprintf(file,"%s%i%s\n", "    movq ",offSet2,"(%rbp), %rax");
                        fprintf(file,"%s%i%s\n", "    imulq $",op1,", %rax");
                        fprintf(file,"%s%i%s\n", "    movq %rax ,",offSetTemp,"(%rbp)");
                    } else {
                        int op2 = second->info->cons.value;
                        int offSet1 =  first->info->var.offset;
                        int offSetTemp =  temp->info->var.offset;
                        fprintf(file,"%s%i%s\n", "    movq ",offSet1,"(%rbp), %rax");
                        fprintf(file,"%s%i%s\n", "    imulq $",op2,", %rax");
                        fprintf(file,"%s%i%s\n", "    movq %rax ,",offSetTemp,"(%rbp)");
                    }
                }
            }
        }


        if(strcmp(index->node->codOp, "DIV") == 0){
            if((first->tag == 0 || first->tag == 4) && (second->tag == 0 || second->tag == 4)){
                int offSet1 = first->info->var.offset;
                int offSet2 = second->info->var.offset;
                int offSetTemp = temp->info->var.offset;
                fprintf(file,"%s%i%s\n", "    movq ",offSet1,"(%rbp), %rax");
                fprintf(file,"%s\n",     "    cqto");
                fprintf(file,"%s%i%s\n", "    movq ",offSet2,"(%rbp), %rbx");
                fprintf(file,"%s\n",     "    idivq %rbx");
                fprintf(file,"%s%i%s\n", "    movq %rax ,",offSetTemp,"(%rbp)");
            }
            if((first->tag != 0 && first->tag != 4) || (second->tag != 0 && second->tag != 4)){
                if(first->tag == 1 && second->tag == 1){
                    int op1 = first->info->cons.value;
                    int op2 = second->info->cons.value;
                    int offSetTemp = temp->info->var.offset;
                    fprintf(file,"%s%i%s\n", "    movq $",op1,", %rax");
                    fprintf(file,"%s\n",     "    cqto");
                    fprintf(file,"%s%i%s\n", "    movq $",op2,", %rbx");
                    fprintf(file,"%s\n",     "    idivq %rbx");
                    fprintf(file,"%s%i%s\n", "    movq %rax ,",offSetTemp,"(%rbp)");
                } else {
                    if(first->tag != 0 && first->tag != 4){
                        int op1 = first->info->cons.value;
                        int offSet2 = second->info->var.offset;
                        int offSetTemp = temp->info->var.offset;
                        fprintf(file,"%s%i%s\n", "    movq $",op1,", %rax");
                        fprintf(file,"%s\n",     "    cqto");
                        fprintf(file,"%s%i%s\n", "    movq ",offSet2,"(%rbp), %rbx");
                        fprintf(file,"%s\n",     "    idivq %rbx");
                        fprintf(file,"%s%i%s\n", "    movq %rax ,",offSetTemp,"(%rbp)");
                    } else {
                        int op2 = second->info->cons.value;
                        int offSet1 = first->info->var.offset;
                        int offSetTemp = temp->info->var.offset;
                        fprintf(file,"%s%i%s\n", "    movq ",offSet1,"(%rbp), %rax");
                        fprintf(file,"%s\n",     "    cqto");
                        fprintf(file,"%s%i%s\n", "    movq $",op2,", %rbx");
                        fprintf(file,"%s\n",     "    idivq %rbx");
                        fprintf(file,"%s%i%s\n", "    movq %rax ,",offSetTemp,"(%rbp)");
                    }
                }
            }
        }


        if(strcmp(index->node->codOp, "MOD") == 0){
            if((first->tag == 0 || first->tag == 4) && (second->tag == 0 || second->tag == 4)){
                int offSet1 = first->info->var.offset;
                int offSet2 = second->info->var.offset;
                int offSetTemp = temp->info->var.offset;
                fprintf(file,"%s%i%s\n", "    movq ",offSet1,"(%rbp), %rax");
                fprintf(file,"%s\n",     "    cqto");
                fprintf(file,"%s%i%s\n", "    movq ",offSet2,"(%rbp), %rbx");
                fprintf(file,"%s\n",     "    idivq %rbx");
                fprintf(file,"%s%i%s\n", "    movq %rdx , ",offSetTemp,"(%rbp)");
            }
            if((first->tag != 0 && first->tag != 4) || (second->tag != 0 && second->tag != 4)){
                if(first->tag == 1 && second->tag == 1){
                    int op1 = first->info->cons.value;
                    int op2 = second->info->cons.value;
                    int offSetTemp = temp->info->var.offset;
                    fprintf(file,"%s%i%s\n", "    movq $",op1,", %rax");
                    fprintf(file,"%s\n",     "    cqto");
                    fprintf(file,"%s%i%s\n", "    movq $",op2,", %rbx");
                    fprintf(file,"%s\n",     "    idivq %rbx");
                    fprintf(file,"%s%i%s\n", "    movq %rdx , ",offSetTemp,"(%rbp)");
                } else {
                    if(first->tag != 0 && first->tag != 4){
                        int op1 = first->info->cons.value;
                        int offSet2 = second->info->var.offset;
                        int offSetTemp = temp->info->var.offset;
                        fprintf(file,"%s%i%s\n", "    movq $",op1,", %rax");
                        fprintf(file,"%s\n",     "    cqto");
                        fprintf(file,"%s%i%s\n", "    movq ",offSet2,"(%rbp), %rbx");
                        fprintf(file,"%s\n",     "    idivq %rbx");
                        fprintf(file,"%s%i%s\n", "    movq %rdx , ",offSetTemp,"(%rbp)");
                    } else {
                        int offSet1 = first->info->var.offset;
                        int op2 = second->info->cons.value;
                        int offSetTemp = temp->info->var.offset;
                        fprintf(file, "%s%i%s\n", "    movq ",offSet1,"(%rbp), %rax");
                        fprintf(file,"%s\n",      "    cqto");
                        fprintf(file,"%s%i%s\n",  "    movq $",op2,", %rbx");
                        fprintf(file,"%s\n",      "    idivq %rbx");
                        fprintf(file,"%s%i%s\n",  "    movq %rdx , ",offSetTemp,"(%rbp)");
                    }
                }
            }
        }


        if(strcmp(index->node->codOp, "MAY") == 0){
            int offSetTemp = temp->info->var.offset;
            if((first->tag == 0 || first->tag == 4) && (second->tag == 0 || second->tag == 4)){
                int offSet1 = first->info->var.offset;
                int offSet2 = second->info->var.offset;
                fprintf(file,"%s%i%s\n", "    movq ",offSet1,"(%rbp), %rax");
                fprintf(file,"%s%i%s\n", "    cmpq ",offSet2,"(%rbp), %rax");
            }
            if((first->tag != 0 && first->tag != 4) || (second->tag != 0 && second->tag != 4)){
                if(first->tag == 1 && second->tag == 1){
                    int op1 = first->info->cons.value;
                    int op2 = second->info->cons.value;
                    fprintf(file,"%s%i%s\n", "    movq $",op1,", %rax");
                	fprintf(file,"%s%i%s\n", "    cmpq $",op2,", %rax");
                } else {
                    if(first->tag != 0 && first->tag != 4){
                        int op1 = first->info->cons.value;
                        int offSet2 =  second->info->var.offset;
               		    fprintf(file,"%s%i%s\n", "    movq $",op1,", %rax");
               		    fprintf(file,"%s%i%s\n", "    cmpq ",offSet2,"(%rbp), %rax");
                    } else {
                        int op2 = second->info->cons.value;
                        int offSet1 =  first->info->var.offset;
               		    fprintf(file,"%s%i%s\n", "    movq ",offSet1,"(%rbp), %rax");
               		    fprintf(file,"%s%i%s\n", "    cmpq $",op2,", %rax");
                    }
                }
            }
     		fprintf(file,"%s\n",	"    setg	%cl");
     		fprintf(file,"%s\n",	"    movzbq  %cl, %rax");
     		fprintf(file,"%s%i%s\n","    movq  %rax, ",offSetTemp,"(%rbp)");
        }


        if(strcmp(index->node->codOp, "MIN") == 0){
            int offSetTemp = temp->info->var.offset;
            if((first->tag == 0 || first->tag == 4) && (second->tag == 0 || second->tag == 4)){
                int offSet1 = first->info->var.offset;
                int offSet2 = second->info->var.offset;
                fprintf(file,"%s%i%s\n", "    movq ",offSet1,"(%rbp), %rax");
                fprintf(file,"%s%i%s\n", "    cmpq ",offSet2,"(%rbp), %rax");
            }
            if((first->tag != 0 && first->tag != 4) || (second->tag != 0 && second->tag != 4)){
                if(first->tag == 1 && second->tag == 1){
                    int op1 = first->info->cons.value;
                    int op2 = second->info->cons.value;
                    fprintf(file,"%s%i%s\n", "    movq $",op1,", %rax");
                	fprintf(file,"%s%i%s\n", "    cmpq $",op2,", %rax");
                } else {
                    if(first->tag != 0 && first->tag != 4){
                        int op1 = first->info->cons.value;
                        int offSet2 =  second->info->var.offset;
                        fprintf(file,"%s%i%s\n", "    movq $",op1,", %rax");
               		    fprintf(file,"%s%i%s\n", "    cmpq ",offSet2,"(%rbp), %rax");
                    } else {
                        int op2 = second->info->cons.value;
                        int offSet1 =  first->info->var.offset;
               		    fprintf(file,"%s%i%s\n", "    movq ",offSet1,"(%rbp), %rax");
               		    fprintf(file,"%s%i%s\n", "    cmpq $",op2,", %rax");
                    }
                }
            }
     		fprintf(file,"%s\n",	"    setl	%al");
     		fprintf(file,"%s\n",	"    movsbq  %al, %rax");
     		fprintf(file,"%s%i%s\n","    movq  %rax, ",offSetTemp,"(%rbp)");
        }


        if(strcmp(index->node->codOp, "AND") == 0){
            int offSetTemp = temp->info->var.offset;
            char label1[20], label2[20];
            char aux[20];
            sprintf(aux, "%d", labelCount);
            strcpy(label1,".LB");
            strcat(label1,aux);
            labelCount++;
            sprintf(aux, "%d", labelCount);
            strcpy(label2,".LB");
            strcat(label2,aux);
            labelCount++;
            if(first->tag == 1){
                int op1 = first->info->cons.value;
                fprintf(file,"%s%i%s\n", "    movq $",op1,", %rax");
                fprintf(file,"%s\n",     "    cmpq $0, %rax");
                fprintf(file,"%s%s\n",   "    je ",label1);
            } else {
                int offSet1 = first->info->var.offset;
                fprintf(file,"%s%i%s\n", "    cmpq $0, ",offSet1,"(%rbp)");
                fprintf(file,"%s%s\n",   "    je ",label1);
            }
            if(second->tag == 1){
                int op2 = second->info->cons.value;
                fprintf(file,"%s%i%s\n", "    movq $",op2,", %rax");
                fprintf(file,"%s\n",     "    cmpq $0, %rax");
                fprintf(file,"%s%s\n",   "    je ",label1);
            } else {
                int offSet2 = second->info->var.offset;
                fprintf(file,"%s%i%s\n", "    cmpq $0, ",offSet2,"(%rbp)");
                fprintf(file,"%s%s\n",   "    je ",label1);
            }
            strcat(label1,":");
            fprintf(file,"%s%i%s\n", "    movq $1,",offSetTemp,"(%rbp)");
            fprintf(file,"%s%s\n",   "    jmp ",label2);
            fprintf(file,"%s\n",label1);
            fprintf(file,"%s%i%s\n", "    movq $0,",offSetTemp,"(%rbp)");
            strcat(label2,":");
            fprintf(file,"%s\n",label2);
        }


        if(strcmp(index->node->codOp, "OR") == 0){
            int offSetTemp = temp->info->var.offset;
            char label1[20], label2[20];
            char aux[20];
            sprintf(aux, "%d", labelCount);
            strcpy(label1,".LB");
            strcat(label1,aux);
            labelCount++;
            sprintf(aux, "%d", labelCount);
            strcpy(label2,".LB");
            strcat(label2,aux);
            labelCount++;
            if(first->tag == 1){
                int op1 = first->info->cons.value;
                fprintf(file,"%s%i%s\n", "    movq $",op1,", %rax");
                fprintf(file,"%s\n",     "    cmpq $1, %rax");
                fprintf(file,"%s%s\n",   "    je ",label1);
            }else{
                int offSet1 = first->info->var.offset;
                fprintf(file,"%s%i%s\n", "    cmpq $1, ",offSet1,"(%rbp)");
                fprintf(file,"%s%s\n",   "    je ",label1);
            }
            if(second->tag == 1){
                int op2 = second->info->cons.value;
                fprintf(file,"%s%i%s\n", "    movq $",op2,", %rax");
                fprintf(file,"%s\n",     "    cmpq $1, %rax");
                fprintf(file,"%s%s\n",   "    je ",label1);
            }else{
                int offSet2 = second->info->var.offset;
                fprintf(file,"%s%i%s\n", "    cmpq $1, ",offSet2,"(%rbp)");
                fprintf(file,"%s%s\n",   "    je ",label1);
            }
            strcat(label1,":");
            fprintf(file,"%s%i%s\n", "    movq $0,",offSetTemp,"(%rbp)");
            fprintf(file,"%s%s\n",   "    jmp ",label2);
            fprintf(file,"%s\n",label1);
            fprintf(file,"%s%i%s\n", "    movq $1,",offSetTemp,"(%rbp)");
            strcat(label2,":");
            fprintf(file,"%s\n",label2);
        }


        if(strcmp(index->node->codOp, "EQUAL") == 0){
            int offSetTemp = temp->info->var.offset;
            if((first->tag == 0 || first->tag == 4) && (second->tag == 0 || second->tag == 4)){
                int offSet1 = first->info->var.offset;
                int offSet2 = second->info->var.offset;
                fprintf(file,"%s%i%s\n", "    movq ",offSet1,"(%rbp), %rax");
                fprintf(file,"%s%i%s\n", "    cmpq ",offSet2,"(%rbp), %rax");
            }
            if((first->tag != 0 && first->tag != 4) || (second->tag != 0 && second->tag != 4)){
                if(first->tag == 1 && second->tag == 1){
                    int op1 = first->info->cons.value;
                    int op2 = second->info->cons.value;
                    fprintf(file,"%s%i%s\n", "    movq $",op1,", %rax");
                	fprintf(file,"%s%i%s\n", "    cmpq $",op2,", %rax");
                } else {
                    if(first->tag != 0 && first->tag != 4){
                        int op1 = first->info->cons.value;
                        int offSet2 =  second->info->var.offset;
               		    fprintf(file,"%s%i%s\n", "    movq ",offSet2,"(%rbp), %rax");
               		    fprintf(file,"%s%i%s\n", "    cmpq $",op1,", %rax");
                    } else {
                        int op2 = second->info->cons.value;
                        int offSet1 =  first->info->var.offset;
               		    fprintf(file,"%s%i%s\n", "    movq ",offSet1,"(%rbp), %rax");
               		    fprintf(file,"%s%i%s\n", "    cmpq $",op2,", %rax");
                    }
                }
            }
     		fprintf(file,"%s\n",	"    sete	%al");
     		fprintf(file,"%s\n",	"    movsbq  %al, %rax");
     		fprintf(file,"%s%i%s\n","    movq  %rax,",offSetTemp,"(%rbp)");
        }


        if(strcmp(index->node->codOp, "NEGB") == 0){
        	int offSetTemp = temp->info->var.offset;
        	if(first->tag == 0 || first->tag == 4){
        		int offSet1 =  first->info->var.offset;
        		fprintf(file,"%s%i%s\n", "    cmpq  $0,",offSet1,"(%rbp)");
        	}else{
        		int op1 = first->info->cons.value;
        		fprintf(file,"%s%i%s\n", "    movq $",op1,", %rax");
                fprintf(file,"%s\n",     "    cmpq $0, %rax");
        	}
     		fprintf(file,"%s\n",	"    sete	%al");
     		fprintf(file,"%s\n",	"    movzbq  %al, %rax");
     		fprintf(file,"%s%i%s\n","    movq  %rax,",offSetTemp,"(%rbp)");
        }


        if(strcmp(index->node->codOp, "NEGI") == 0){
        	int offSetTemp = temp->info->var.offset;
        	if(first->tag == 0 || first->tag == 4){
        		int offSet1 = first->info->var.offset;
        		fprintf(file,"%s%i%s\n", "    movq ",offSet1,"(%rbp), %rax");
    			fprintf(file,"%s\n",     "    imulq $-1, %rax");
    			fprintf(file,"%s%i%s\n", "    movq %rax ,",offSetTemp,"(%rbp)");
			}else{
				int op1 = first->info->cons.value;
				fprintf(file,"%s%i%s\n", "    movq $",op1,", %rax");
    			fprintf(file,"%s\n",     "    imulq $-1, %rax");
    			fprintf(file,"%s%i%s\n", "    movq %rax ,",offSetTemp,"(%rbp)");
			}
        }


        if(strcmp(index->node->codOp, "RETURN") == 0){
            int aux;
            int offSet;
            switch (index->node->temp->tag) {
                case 0:
                	offSet= temp->info->var.offset;
                	fprintf(file,"%s%i%s\n\n", "    movq ",offSet,"(%rbp), %rax");
                    if ( strcmp(machine, "Mac") == 0 ){
                        fprintf(file, "%s\n",   "    leaq L_.str(%rip), %rdi");
                        fprintf(file, "%s\n",   "    movq %rax, %rsi");
                        fprintf(file, "%s\n",   "    movb $0, %al");
                        fprintf(file, "%s\n\n", "    callq _printf");
                    } else {
                    	fprintf(file, "%s\n", "    movq	%rax, %rsi");
                    	fprintf(file, "%s\n", "    movq	$.LC0, %rdi");
                    	fprintf(file, "%s\n", "    movq	$0, %rax");
                    	fprintf(file, "%s\n", "    call	printf");
                    }
                    fprintf(file,"%s%i%s\n\n", "    movq ",offSet,"(%rbp), %rax");
                	fprintf(file, "%s\n", "    leave");
            		fprintf(file, "%s\n", "    ret");
            		fprintf(file, "\n" );
                	break;
                case 1:
                    aux = temp->info->cons.value;
                    fprintf(file,"%s%i%s\n\n", "    movq $",aux,", %rax");
                    if ( strcmp(machine, "Mac") == 0 ){
                        fprintf(file, "%s\n",   "    leaq L_.str(%rip), %rdi");
                        fprintf(file, "%s\n",   "    movq %rax, %rsi");
                        fprintf(file, "%s\n",   "    movb $0, %al");
                        fprintf(file, "%s\n\n", "    callq _printf");
                    } else {
                    	fprintf(file, "%s\n", "    movq	%rax, %rsi");
                    	fprintf(file, "%s\n", "    movq	$.LC0, %rdi");
                    	fprintf(file, "%s\n", "    movq	$0, %rax");
                    	fprintf(file, "%s\n", "    call	printf");
                    }
                    fprintf(file,"%s%i%s\n\n", "    movq $",aux,", %rax");
                    fprintf(file, "%s\n", "    leave");
            		fprintf(file, "%s\n", "    ret");
            		fprintf(file, "\n" );
                    break;
                case 4:
                	offSet= temp->info->var.offset;
                	fprintf(file,"%s%i%s\n\n", "    movq ",offSet,"(%rbp), %rax");
                    if ( strcmp(machine, "Mac") == 0 ){
                        fprintf(file, "%s\n",   "    leaq L_.str(%rip), %rdi");
                        fprintf(file, "%s\n",   "    movq %rax, %rsi");
                        fprintf(file, "%s\n",   "    movb $0, %al");
                        fprintf(file, "%s\n\n", "    callq _printf");
                    } else {
                    	fprintf(file, "%s\n", "    movq	%rax, %rsi");
                    	fprintf(file, "%s\n", "    movq	$.LC0, %rdi");
                    	fprintf(file, "%s\n", "    movq	$0, %rax");
                    	fprintf(file, "%s\n", "    call	printf");
                    }
                    fprintf(file,"%s%i%s\n\n", "    movq ",offSet,"(%rbp), %rax");
                	fprintf(file, "%s\n", "    leave");
            		fprintf(file, "%s\n", "    ret");
            		fprintf(file, "\n" );
                    break;
                default:
                    break;
            }
        }


        if(strcmp(index->node->codOp, "RETURNV") == 0){
        	fprintf(file, "%s\n", "    leave");
    		fprintf(file, "%s\n", "    ret");
    		fprintf(file, "\n" );
        }


        if(strcmp(index->node->codOp, "IFF") == 0){
            if (first->tag == 1){
                int cond = first->info->cons.value;
                fprintf(file,"%s%i%s\n", "    movq $",cond,", %rax");
            } else {
                int offsetRes = first->info->var.offset;
	    	    fprintf(file, "%s%i%s\n", "    movq ",offsetRes,"(%rbp), %rax");
            }
	    	fprintf(file,"%s\n",   "    cmpq $0, %rax");
	    	fprintf(file,"%s%s\n", "    je ", second->info->var.id);
    	}


        if(strcmp(index->node->codOp, "JMP") == 0){
        	fprintf(file,"%s%s\n", "    jmp ",temp->info->var.id);
        }


        if(strcmp(index->node->codOp, "LABEL") == 0){
        	char str[256];
        	strcpy(str, temp->info->var.id);
        	strcat(str,":");
        	fprintf(file, "%s\n",str);
        }


        if(strcmp(index->node->codOp, "LOAD") == 0){
            switch (second->info->cons.value) {
                case 1:
                    if(first->tag == 0 || first->tag == 4){
                        int offSet1 = first->info->var.offset;
                        fprintf(file,"%s%i%s\n","    movq ",offSet1,"(%rbp), %rdi");
                    } else {
                        int op1 = first->info->cons.value;
                        fprintf(file,"%s%i%s\n","    movq $",op1,", %rdi");
                    }
                    break;
                case 2:
                    if(first->tag == 0 || first->tag == 4){
                        int offSet1 = first->info->var.offset;
                        fprintf(file,"%s%i%s\n","    movq ",offSet1,"(%rbp), %rsi");
                    } else {
                        int op1 = first->info->cons.value;
                        fprintf(file,"%s%i%s\n","    movq $",op1,", %rsi");
                    }
                    break;
                case 3:
                    if(first->tag == 0 || first->tag == 4){
                        int offSet1 = first->info->var.offset;
                        fprintf(file,"%s%i%s\n","    movq ",offSet1,"(%rbp), %rdx");
                    } else {
                        int op1 = first->info->cons.value;
                        fprintf(file,"%s%i%s\n","    movq $",op1,", %rdx");
                    }
                    break;
                case 4:
                    if(first->tag == 0 || first->tag == 4){
                        int offSet1 = first->info->var.offset;
                        fprintf(file,"%s%i%s\n","    movq ",offSet1,"(%rbp), %rcx");
                    } else {
                        int op1 = first->info->cons.value;
                        fprintf(file,"%s%i%s\n","    movq $",op1,", %rcx");
                    }
                    break;
                case 5:
                    if(first->tag == 0 || first->tag == 4){
                        int offSet1 = first->info->var.offset;
                        fprintf(file,"%s%i%s\n","    movq ",offSet1,"(%rbp), %r8");
                    } else {
                        int op1 = first->info->cons.value;
                        fprintf(file,"%s%i%s\n","    movq $",op1,", %r8");
                    }
                    break;
                case 6:
                    if(first->tag == 0 || first->tag == 4){
                        int offSet1 = first->info->var.offset;
                        fprintf(file,"%s%i%s\n","    movq ",offSet1,"(%rbp), %r9");
                    } else {
                        int op1 = first->info->cons.value;
                        fprintf(file,"%s%i%s\n","    movq $",op1,", %r9");
                    }
                    break;
                default:
                    if(first->tag == 0 || first->tag == 4){
                        fprintf(file,"%s%i%s\n","    pushq ",first->info->var.offset, "(%rbp)");
                    } else {
                        int op1 = first->info->cons.value;
                        fprintf(file,"%s%i\n","    pushq $", op1);
                    }
                    break;
            }
        }


        if(strcmp(index->node->codOp, "CALL") == 0){
            if ( temp != NULL){
                int offsetTemp = temp->info->var.offset;
        		fprintf(file,"%s%s\n", "    call ",first->info->func.id);
                fprintf(file, "%s%i%s\n", "    movq %rax, ",offsetTemp,"(%rbp) ");
            } else {
                fprintf(file,"%s%s\n", "    call ",first->info->func.id);
            }
        }


        if(strcmp(index->node->codOp, "MOV") == 0){
        	int offSet1 =  first->info->var.offset;
        	if(second->tag == 0 || second->tag == 4){
        		int offSet2 =  second->info->var.offset;
				fprintf(file,"%s%i%s\n", "    movq ",offSet2,"(%rbp), %rax");
				fprintf(file,"%s%i%s\n", "    movq %rax, ",offSet1,"(%rbp)");
        	} else {
        		int op2 = second->info->cons.value;
        		fprintf(file,"%s%i%s%i%s\n", "    movq $",op2,", ",offSet1,"(%rbp)");
        	}
        }
        index = index->next;
    }

    if ( strcmp(machine, "Mac") == 0 ){
        fprintf(file, "%s\n", "    .section	__TEXT,__cstring,cstring_literals");
        fprintf(file, "%s\n", "L_.str:");
        fprintf(file, "%s\n", "    .asciz	\"%i\\n\"");
    } else {
    	fprintf(file, "%s\n", ".LC0:");
    	fprintf(file, "%s\n", "    .string	\"%d\\n\"");
    }
    fclose(file);
}
