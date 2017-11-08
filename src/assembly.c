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
                strcpy(str,"_");
                strcat(str,first->info->func.id);
                fprintf(file, "%s%s\n", "    .globl ",str);
                strcat(str,":");
            }else{
                strcpy(str,first->info->func.id);
                fprintf(file, "%s%s\n", "    .globl ",str);
                strcat(str,":");
            }
            fprintf(file, "%s\n",str);
            int aux = abs(first->info->func.offset);
            while( (aux % 16) != 0){
                aux += 8;
            }
            fprintf(file, "%s%i%s\n", "    enter $",aux,", $0");
        }
        if(strcmp(index->node->codOp, "END") == 0){
            /*fprintf(file, "%s\n", "    leave");
            fprintf(file, "%s\n", "    ret");
            fprintf(file, "\n" );*/
        }

        if(strcmp(index->node->codOp, "ADD") == 0){
        	/*if(first->tag == 1 && first->tag == 1){   Optimizado!
        		int result =  first->info->cons.value + second->info->cons.value;
        		int offSet =  index->node->temp->info->var.offset;
        		fprintf(file,"%s%i%s%i%s\n", "    movq  $",result,", ",offSet,"(%rbp)");
        	}*/
            // Ambos operandos son variables o temporales

        	if((first->tag == 0 || first->tag == 4) && (second->tag == 0 || second->tag == 4)){
        		int op1 = first->info->cons.value;
        		int op2 = second->info->cons.value;
        		int offSet1 = first->info->var.offset;
        		int offSet2 = second->info->var.offset;
        		int offSetTemp = temp->info->var.offset;
				fprintf(file,"%s%i%s\n", "    movq ",offSet1,"(%rbp), %rdx");
        		fprintf(file,"%s%i%s\n", "    movq ",offSet2,"(%rbp), %rax");
        		fprintf(file,"%s\n",     "    add %rdx, %rax");
        		fprintf(file,"%s%i%s\n", "    movq %rax ,",offSetTemp,"(%rbp)");
        	}

            if((first->tag != 0 && first->tag != 4) || (second->tag != 0 && second->tag != 4)){
                if(first->tag == 1 && second->tag == 1){ //Ambos operandos son constantes
                    int op1 = first->info->cons.value;
                    int op2 = second->info->cons.value;
                    int offSet =  temp->info->var.offset;
                    fprintf(file,"%s%i%s\n", "    movq $",op1,", %rax");
                    fprintf(file,"%s%i%s\n", "    add $",op2,", %rax");
                    fprintf(file,"%s%i%s\n", "    movq %rax ,",offSet,"(%rbp)");
                }
                else{
                    if(first->tag != 0 && first->tag != 4){ //primer operando es una constante (puede ser funcion?)
                        int op1 = first->info->cons.value;
                        int offSet2 =  second->info->var.offset;
                        int offSetTemp =  temp->info->var.offset;
                        fprintf(file,"%s%i%s\n", "    movq ",offSet2,"(%rbp), %rax");
                        fprintf(file,"%s%i%s\n", "    add $",op1,", %rax");
                        fprintf(file,"%s%i%s\n", "    movq %rax ,",offSetTemp,"(%rbp)");
                    }
                    else{ //segundo operando es una constante
                        int op2 = second->info->cons.value;
                        int offSet1 =  first->info->var.offset;
                        int offSetTemp =  temp->info->var.offset;
                        fprintf(file,"%s%i%s\n", "    movq ",offSet1,"(%rbp), %rax");
                        fprintf(file,"%s%i%s\n", "    add $",op2,", %rax");
                        fprintf(file,"%s%i%s\n", "    movq %rax ,",offSetTemp,"(%rbp)");
                    }
                }
            }
        }


        if(strcmp(index->node->codOp, "SUB") == 0){
            // Ambos operandos son variables o temporales
            if((first->tag == 0 || first->tag == 4) && (second->tag == 0 || second->tag == 4)){
                int op1 = first->info->cons.value;
                int op2 = second->info->cons.value;

                int offSet1 = first->info->var.offset;
                int offSet2 = second->info->var.offset;
                int offSetTemp = temp->info->var.offset;
                fprintf(file,"%s%i%s\n", "    movq ",offSet1,"(%rbp), %rdx");
                fprintf(file,"%s%i%s\n", "    movq ",offSet2,"(%rbp), %rax");
                fprintf(file,"%s\n",     "    sub %rdx, %rax");
                fprintf(file,"%s%i%s\n", "    movq %rax ,",offSetTemp,"(%rbp)");
                //z = x - y;
                //movl  -20(%rbp), %edx  z = x + y;
                //movl  -24(%rbp), %eax
                //sub  %edx, %eax
                //movl  %eax, -4(%rbp)
            }
            if((first->tag != 0 && first->tag != 4) || (second->tag != 0 && second->tag != 4)){
                if(first->tag == 1 && second->tag == 1){ //Ambos operandos son constantes
                    int op1 = first->info->cons.value;
                    int op2 = second->info->cons.value;
                    int offSet =  temp->info->var.offset;
                    fprintf(file,"%s%i%s\n", "    movq $",op1,", %rax");
                    fprintf(file,"%s%i%s\n", "    sub $",op2,", %rax");
                    fprintf(file,"%s%i%s\n", "    movq %rax ,",offSet,"(%rbp)");
                }
                else{
                    if(first->tag != 0 && first->tag != 4){ //primer operando es una constante (puede ser funcion?)
                        int op1 = first->info->cons.value;
                        int offSet2 =  second->info->var.offset;
                        int offSetTemp =  temp->info->var.offset;
                        fprintf(file,"%s%i%s\n", "    movq ",offSet2,"(%rbp), %rax");
                        fprintf(file,"%s%i%s\n", "    sub $",op1,", %rax");
                        fprintf(file,"%s%i%s\n", "    movq %rax ,",offSetTemp,"(%rbp)");

                        // y = 1 - x;
                        // movl    -4(%rbp), %eax
                        // sub     $1, %eax
                        // movl    %eax, -8(%rbp)
                    }
                    else{ //segundo operando es una constante
                        int offSet1 =  first->info->var.offset;
                        int offSet2 =  second->info->var.offset;
                        int offSetTemp =  temp->info->var.offset;
                        fprintf(file,"%s%i%s\n", "    movq ",offSet2,"(%rbp), %rax");
                        fprintf(file,"%s%i%s\n", "    sub ",offSet1,"(%rbp), %rax");
                        fprintf(file,"%s%i%s\n", "    movq %rax ,",offSetTemp,"(%rbp)");

                        // y = x - 2;
                        // movl    -4(%rbp), %eax
                        // sub     -8(%rbp), %eax
                        // movl    %eax, -8(%rbp)
                    }
                }
            }
        }

        if(strcmp(index->node->codOp, "MULT") == 0){
            // Ambos operandos son variables o temporales
            if((first->tag == 0 || first->tag == 4) && (second->tag == 0 || second->tag == 4)){
                int op1 = first->info->cons.value;
                int op2 = second->info->cons.value;

                int offSet1 = first->info->var.offset;
                int offSet2 = second->info->var.offset;
                int offSetTemp = temp->info->var.offset;
                fprintf(file,"%s%i%s\n", "    movq ",offSet1,"(%rbp), %rdx");
                fprintf(file,"%s%i%s\n", "    movq ",offSet2,"(%rbp), %rax");
                fprintf(file,"%s\n",     "    imul %rdx, %rax");
                fprintf(file,"%s%i%s\n", "    movq %rax ,",offSetTemp,"(%rbp)");
            }
            if((first->tag != 0 && first->tag != 4) || (second->tag != 0 && second->tag != 4)){
                if(first->tag == 1 && second->tag == 1){ //Ambos operandos son constantes
                    int op1 = first->info->cons.value;
                    int op2 = second->info->cons.value;
                    int offSet =  temp->info->var.offset;
                    fprintf(file,"%s%i%s\n", "    movq $",op1,", %rax");
                    fprintf(file,"%s%i%s\n", "    imul $",op2,", %rax");
                    fprintf(file,"%s%i%s\n", "    movq %rax ,",offSet,"(%rbp)");
                }
                else{
                    if(first->tag != 0 && first->tag != 4){ //primer operando es una constante (puede ser funcion?)
                        int op1 = first->info->cons.value;
                        int offSet2 =  second->info->var.offset;
                        int offSetTemp =  temp->info->var.offset;
                        fprintf(file,"%s%i%s\n", "    movq ",offSet2,"(%rbp), %rax");
                        fprintf(file,"%s%i%s\n", "    imul $",op1,", %rax");
                        fprintf(file,"%s%i%s\n", "    movq %rax ,",offSetTemp,"(%rbp)");
                    }
                    else{ //segundo operando es una constante
                        int op2 = second->info->cons.value;
                        int offSet1 =  first->info->var.offset;
                        int offSetTemp =  temp->info->var.offset;
                        fprintf(file,"%s%i%s\n", "    movq ",offSet1,"(%rbp), %rax");
                        fprintf(file,"%s%i%s\n", "    imul $",op2,", %rax");
                        fprintf(file,"%s%i%s\n", "    movq %rax ,",offSetTemp,"(%rbp)");
                    }
                }
            }
        }


        if(strcmp(index->node->codOp, "DIV") == 0){
            // Ambos operandos son variables o temporales
            if((first->tag == 0 || first->tag == 4) && (second->tag == 0 || second->tag == 4)){
                int offSet1 = first->info->var.offset;
                int offSet2 = second->info->var.offset;
                int offSetTemp = temp->info->var.offset;
                fprintf(file,"%s%i%s\n", "    movq ",offSet1,"(%rbp), %rax");
                fprintf(file,"%s\n",     "    movq $0, %rdx");
                fprintf(file,"%s%i%s\n", "    idiv ",offSet2,"(%rbp)");
                fprintf(file,"%s%i%s\n", "    movq %rax ,",offSetTemp,"(%rbp)");
                /*
                División sin signo %rdx:%rax por S
                Cociente almacenado en% rax
                Resto almacenado en% rdx
                */
            }
            if((first->tag != 0 && first->tag != 4) || (second->tag != 0 && second->tag != 4)){
                if(first->tag == 1 && second->tag == 1){ //Ambos operandos son constantes
                    int op1 = first->info->cons.value;
                    int op2 = second->info->cons.value;
                    int offSetTemp = temp->info->var.offset;
                    fprintf(file,"%s%i%s\n", "    movq $",op1,", %rax");
                    fprintf(file,"%s\n",     "    movq $0, %rdx");
                    fprintf(file,"%s%i%s\n", "    movq $",op2,", %r10");
                    fprintf(file,"%s\n",     "    idiv %r10");
                    fprintf(file,"%s%i%s\n", "    movq %rax ,",offSetTemp,"(%rbp)");
                }
                else{
                    if(first->tag != 0 && first->tag != 4){ //primer operando es una constante (puede ser funcion?)
                        int op1 = first->info->cons.value;
                        int offSet2 = second->info->var.offset;
                        int offSetTemp = temp->info->var.offset;
                        fprintf(file,"%s%i%s\n", "    movq $",op1,", %rax");
                        fprintf(file,"%s\n",     "    movq $0, %rdx");
                        fprintf(file,"%s%i%s\n", "    idiv ",offSet2,"(%rbp)");
                        fprintf(file,"%s%i%s\n", "    movq %rax ,",offSetTemp,"(%rbp)");

                    }
                    else{ //segundo operando es una constante
                        int op1 = first->info->cons.value;
                        int offSet1 = first->info->var.offset;
                        int offSetTemp = temp->info->var.offset;
                        fprintf(file,"%s%i%s\n", "    movq ",offSet1,"(%rbp), %rax");
                        fprintf(file,"%s\n",     "    movq $0, %rdx");
                        fprintf(file,"%s%i%s\n", "    movq $",op1,", %r10");
                        fprintf(file,"%s\n",     "    idiv %r10");
                        fprintf(file,"%s%i%s\n", "    movq %rax ,",offSetTemp,"(%rbp)");
                    }
                }
            }
        }


        if(strcmp(index->node->codOp, "MOD") == 0){
            // Ambos operandos son variables o temporales
            if((first->tag == 0 || first->tag == 4) && (second->tag == 0 || second->tag == 4)){
                int offSet1 = first->info->var.offset;
                int offSet2 = second->info->var.offset;
                int offSetTemp = temp->info->var.offset;
                fprintf(file,"%s%i%s\n", "    movq ",offSet1,"(%rbp), %rax");
                fprintf(file,"%s\n",     "    movq $0, %rdx");
                fprintf(file,"%s%i%s\n", "    idiv ",offSet2,"(%rbp)");
                fprintf(file,"%s%i%s\n", "    movq %rdx ,",offSetTemp,"(%rbp)");
            }
            if((first->tag != 0 && first->tag != 4) || (second->tag != 0 && second->tag != 4)){
                if(first->tag == 1 && second->tag == 1){ //Ambos operandos son constantes
                    int op1 = first->info->cons.value;
                    int op2 = second->info->cons.value;
                    int offSetTemp = temp->info->var.offset;
                    fprintf(file,"%s%i%s\n", "    movq $",op1,", %rax");
                    fprintf(file,"%s\n",     "    movq $0, %rdx");
                    fprintf(file,"%s%i%s\n", "    movq $",op2,", %r10");
                    fprintf(file,"%s\n",     "    idiv %r10");
                    fprintf(file,"%s%i%s\n", "    movq %rdx ,",offSetTemp,"(%rbp)");
                }
                else{
                    if(first->tag != 0 && first->tag != 4){ //primer operando es una constante (puede ser funcion?)
                        int op1 = first->info->cons.value;
                        int offSet2 = second->info->var.offset;
                        int offSetTemp = temp->info->var.offset;
                        fprintf(file,"%s%i%s\n", "    movq $",op1,", %rax");
                        fprintf(file,"%s\n",     "    movq $0, %rdx");
                        fprintf(file,"%s%i%s\n", "    idiv ",offSet2,"(%rbp)");
                        fprintf(file,"%s%i%s\n", "    movq %rdx ,",offSetTemp,"(%rbp)");

                    }
                    else{ //segundo operando es una constante
                        int op1 = first->info->cons.value;
                        int offSet1 = first->info->var.offset;
                        int offSetTemp = temp->info->var.offset;
                        fprintf(file,"%s%i%s\n", "    movq ",offSet1,"(%rbp), %rax");
                        fprintf(file,"%s\n",     "    movq $0, %rdx");
                        fprintf(file,"%s%i%s\n", "    movq $",op1,", %r10");
                        fprintf(file,"%s\n",     "    idiv %r10");
                        fprintf(file,"%s%i%s\n", "    movq %rdx ,",offSetTemp,"(%rbp)");
                    }
                }
            }
        }
        if(strcmp(index->node->codOp, "MAY") == 0){
            int offSetTemp = temp->info->var.offset;
            // Ambos operandos son variables o temporales
            if((first->tag == 0 || first->tag == 4) && (second->tag == 0 || second->tag == 4)){
                int offSet1 = first->info->var.offset;
                int offSet2 = second->info->var.offset;
                fprintf(file,"%s%i%s\n", "    movq ",offSet1,"(%rbp), %rax");
                fprintf(file,"%s%i%s\n", "    cmp ",offSet2,"(%rbp), %rax");
            }
            if((first->tag != 0 && first->tag != 4) || (second->tag != 0 && second->tag != 4)){
                if(first->tag == 1 && second->tag == 1){ //Ambos operandos son constantes
                    int op1 = first->info->cons.value;
                    int op2 = second->info->cons.value;
                    fprintf(file,"%s%i%s\n", "    movq $",op1,", %rax");
                	fprintf(file,"%s%i%s\n", "    cmp $",op2,", %rax");

                }
                else{
                    if(first->tag != 0 && first->tag != 4){ //primer operando es una constante (puede ser funcion?)
                        int op1 = first->info->cons.value;
                        int offSet2 =  second->info->var.offset;
               		    fprintf(file,"%s%i%s\n", "    movq ",offSet2,"(%rbp), %rax");
               		    fprintf(file,"%s%i%s\n", "    cmp $",op1,", %rax");

                    }
                    else{ //segundo operando es una constante
                        int op2 = second->info->cons.value;
                        int offSet1 =  first->info->var.offset;
               		    fprintf(file,"%s%i%s\n", "    movq ",offSet1,"(%rbp), %rax");
               		    fprintf(file,"%s%i%s\n", "    cmp $",op2,", %rax");
                    }
                }
            }

     		fprintf(file,"%s\n",	"    setg	%al");
     		fprintf(file,"%s\n",	"    movzbl  %al, %rax");
     		fprintf(file,"%s%i%s\n","    movq  %rax,",offSetTemp,"(%rbp)");
        }

        if(strcmp(index->node->codOp, "MIN") == 0){
            int offSetTemp = temp->info->var.offset;
            // Ambos operandos son variables o temporales
            if((first->tag == 0 || first->tag == 4) && (second->tag == 0 || second->tag == 4)){
                int offSet1 = first->info->var.offset;
                int offSet2 = second->info->var.offset;
                fprintf(file,"%s%i%s\n", "    movq ",offSet1,"(%rbp), %rax");
                fprintf(file,"%s%i%s\n", "    cmp ",offSet2,"(%rbp), %rax");
            }
            if((first->tag != 0 && first->tag != 4) || (second->tag != 0 && second->tag != 4)){
                if(first->tag == 1 && second->tag == 1){ //Ambos operandos son constantes
                    int op1 = first->info->cons.value;
                    int op2 = second->info->cons.value;
                    fprintf(file,"%s%i%s\n", "    movq $",op1,", %rax");
                	fprintf(file,"%s%i%s\n", "    cmp $",op2,", %rax");

                }
                else{
                    if(first->tag != 0 && first->tag != 4){ //primer operando es una constante (puede ser funcion?)
                        int op1 = first->info->cons.value;
                        int offSet2 =  second->info->var.offset;
               		    fprintf(file,"%s%i%s\n", "    movq ",offSet2,"(%rbp), %rax");
               		    fprintf(file,"%s%i%s\n", "    cmp $",op1,", %rax");

                    }
                    else{ //segundo operando es una constante
                        int op2 = second->info->cons.value;
                        int offSet1 =  first->info->var.offset;
               		    fprintf(file,"%s%i%s\n", "    movq ",offSet1,"(%rbp), %rax");
               		    fprintf(file,"%s%i%s\n", "    cmp $",op2,", %rax");
                    }
                }
            }
     		fprintf(file,"%s\n",	"    setl	%al");
     		fprintf(file,"%s\n",	"    movzbl  %al, %rax");
     		fprintf(file,"%s%i%s\n","    movq  %rax,",offSetTemp,"(%rbp)");
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
                fprintf(file,"%s%i\n", "    cmp $0, $",op1);
                fprintf(file,"%s%s\n", "    je ",label1);
            }else{
                int offSet1 = first->info->var.offset;
                fprintf(file,"%s%i%s\n", "    cmp $0, ",offSet1,"(%rbp)");
                fprintf(file,"%s%s\n",   "    je ",label1);
            }

            if(second->tag == 1){
                int op2 = second->info->cons.value;
                fprintf(file,"%s%i\n", "    cmp $0, $",op2);
                fprintf(file,"%s%s\n",   "    je ",label1);
            }else{
                int offSet2 = second->info->var.offset;
                fprintf(file,"%s%i%s\n", "    cmp $0, ",offSet2,"(%rbp)");
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

            if(first->tag == 1){ //primer operando es una constante
                int op1 = first->info->cons.value;
                fprintf(file,"%s%i\n", "    cmp $1, $",op1);
                fprintf(file,"%s%s\n", "    je ",label1);
            }else{
                int offSet1 = first->info->var.offset;
                fprintf(file,"%s%i%s\n", "    cmp $1, ",offSet1,"(%rbp)");
                fprintf(file,"%s%s\n",   "    je ",label1);
            }

            if(second->tag == 1){ // segundo opetando es una constante
                int op2 = second->info->cons.value;
                fprintf(file,"%s%i\n", "    cmp $1, $",op2);
                fprintf(file,"%s%s\n",   "    je ",label1);
            }else{
                int offSet2 = second->info->var.offset;
                fprintf(file,"%s%i%s\n", "    cmp $1, ",offSet2,"(%rbp)");
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
            // Ambos operandos son variables o temporales
            if((first->tag == 0 || first->tag == 4) && (second->tag == 0 || second->tag == 4)){
                int offSet1 = first->info->var.offset;
                int offSet2 = second->info->var.offset;
                fprintf(file,"%s%i%s\n", "    movq ",offSet1,"(%rbp), %rax");
                fprintf(file,"%s%i%s\n", "    cmp ",offSet2,"(%rbp), %rax");
            }
            if((first->tag != 0 && first->tag != 4) || (second->tag != 0 && second->tag != 4)){
                if(first->tag == 1 && second->tag == 1){ //Ambos operandos son constantes
                    int op1 = first->info->cons.value;
                    int op2 = second->info->cons.value;
                    fprintf(file,"%s%i%s\n", "    movq $",op1,", %rax");
                	fprintf(file,"%s%i%s\n", "    cmp $",op2,", %rax");

                }
                else{
                    if(first->tag != 0 && first->tag != 4){ //primer operando es una constante (puede ser funcion?)
                        int op1 = first->info->cons.value;
                        int offSet2 =  second->info->var.offset;
               		    fprintf(file,"%s%i%s\n", "    movq ",offSet2,"(%rbp), %rax");
               		    fprintf(file,"%s%i%s\n", "    cmp $",op1,", %rax");

                    }
                    else{ //segundo operando es una constante
                        int op2 = second->info->cons.value;
                        int offSet1 =  first->info->var.offset;
               		    fprintf(file,"%s%i%s\n", "    movq ",offSet1,"(%rbp), %rax");
               		    fprintf(file,"%s%i%s\n", "    cmp $",op2,", %rax");
                    }
                }
            }
     		fprintf(file,"%s\n",	"    sete	%al");
     		fprintf(file,"%s\n",	"    movzbl  %al, %rax");
     		fprintf(file,"%s%i%s\n","    movq  %eax,",offSetTemp,"(%rbp)");
        }


        if(strcmp(index->node->codOp, "NEGB") == 0){
        	int offSetTemp = temp->info->var.offset;
        	if(first->tag == 0 || first->tag == 4){ //primer operando es una variable
        		int offSet1 =  first->info->var.offset;
        		fprintf(file,"%s%i%s\n", "    cmp  $0,",offSet1,"(%rbp)");
        	}else{
        		int op1 = first->info->cons.value;
        		fprintf(file,"%s%i%s\n", "    movq $",op1,", %rax");
                fprintf(file,"%s\n",     "    cmp $0, %rax");
        	}
     		fprintf(file,"%s\n",	"    sete	%al");
     		fprintf(file,"%s\n",	"    movzbl  %al, %rax");
     		fprintf(file,"%s%i%s\n","    movq  %rax,",offSetTemp,"(%rbp)");
        }

        if(strcmp(index->node->codOp, "NEGI") == 0){
        	int offSetTemp = temp->info->var.offset;
        	if(first->tag == 0 || first->tag == 4){
        		int offSet1 = first->info->var.offset;
        		fprintf(file,"%s%i%s\n", "    movq ",offSet1,"(%rbp), %rax");
    			fprintf(file,"%s\n",     "    imul $-1, %rax");
    			fprintf(file,"%s%i%s\n", "    movq %rax ,",offSetTemp,"(%rbp)");
			}else{
				int op1 = first->info->cons.value;
				fprintf(file,"%s%i%s\n", "    movq $",op1,", %rax");
    			fprintf(file,"%s\n",     "    imul $-1, %rax");
    			fprintf(file,"%s%i%s\n", "    movq %rax ,",offSetTemp,"(%rbp)");
			}
        }

        if(strcmp(index->node->codOp, "RETURN") == 0){
            int aux;
            int offSet;
            switch (index->node->temp->tag) {
                case 0:
                	offSet= temp->info->var.offset;
                	fprintf(file,"%s%i%s\n", "    movq ",offSet,"(%rbp), %rax");
                    fprintf(file, "%s\n", "    leaq	L_.str(%rip), %rdi");
                    fprintf(file, "%s\n", "    movq	%rax, %rsi");
                    fprintf(file, "%s\n", "    movb	$0, %al");
                    fprintf(file, "%s\n", "    callq	_printf");
                	fprintf(file, "%s\n", "    leave");
            		fprintf(file, "%s\n", "    ret");
            		fprintf(file, "\n" );
                	break;
                case 1:
                    aux = temp->info->cons.value;
                    fprintf(file,"%s%i%s\n", "    movq $",aux,", %rax");
                    fprintf(file, "%s\n", "    leaq	L_.str(%rip), %rdi");
                    fprintf(file, "%s\n", "    movq	%rax, %rsi");
                    fprintf(file, "%s\n", "    movb	$0, %al");
                    fprintf(file, "%s\n", "    callq	_printf");
                    fprintf(file, "%s\n", "    leave");
            		fprintf(file, "%s\n", "    ret");
            		fprintf(file, "\n" );
                    break;
                case 4:
                	offSet= temp->info->var.offset;
                	fprintf(file,"%s%i%s\n", "    movq ",offSet,"(%rbp), %rax");
                    fprintf(file, "%s\n", "    leaq	L_.str(%rip), %rdi");
                    fprintf(file, "%s\n", "    movq	%rax, %rsi");
                    fprintf(file, "%s\n", "    movb	$0, %al");
                    fprintf(file, "%s\n", "    callq	_printf");
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
	    	int cond = first->info->cons.value;
	    	fprintf(file,"%s%i%s\n", "    mov $",cond,", %rax");
	    	fprintf(file,"%s\n",     "    cmpq $0, %rax");   
	    	fprintf(file,"%s%s\n",   "    je ", second->info->var.id);
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

        }


        if(strcmp(index->node->codOp, "CALL") == 0){
			fprintf(file,"%s%s\n", "    call ",first->info->func.id);
        }

        if(strcmp(index->node->codOp, "MOV") == 0){
        	int offSet1 =  first->info->var.offset;
        	if(second->tag == 0 || second->tag == 4){ //primer operando es una variable
        		int offSet2 =  second->info->var.offset;
				fprintf(file,"%s%i%s\n", "    movq ",offSet2,"(%rbp), %rax");
				fprintf(file,"%s%i%s\n", "    movq %rax, ",offSet1,"(%rbp)");
        	}else{
        		int op2 = second->info->cons.value;
        		fprintf(file,"%s%i%s%i%s\n", "    movq $",op2,", ",offSet1,"(%rbp)");
        	}
        }

        index = index->next;
    }

    //Para printf
    fprintf(file, "%s\n", "    .section	__TEXT,__cstring,cstring_literals");
    fprintf(file, "%s\n", "L_.str:");
    fprintf(file, "%s\n", "    .asciz	\"%i\\n\"");
    fclose(file);
}
