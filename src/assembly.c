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
            fprintf(file, "%s\n", "    leave");
            fprintf(file, "%s\n", "    ret");
            fprintf(file, "\n" );
        }

        if(strcmp(index->node->codOp, "ADD") == 0){
            Node *first  = index->node->firstOp;
        	Node *second = index->node->secondOp;
            Node *temp = index->node->temp;
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
				fprintf(file,"%s%i%s\n", "    mov ",offSet1,"(%ebp), %edx");
        		fprintf(file,"%s%i%s\n", "    mov ",offSet2,"(%ebp), %eax");
        		fprintf(file,"%s\n",     "    add %edx, %eax");
        		fprintf(file,"%s%i%s\n", "    mov %eax ,",offSetTemp,"(%ebp)");
        	}       

            if((first->tag != 0 && first->tag != 4) || (second->tag != 0 && second->tag != 4)){
                if(first->tag == 1 && second->tag == 1){ //Ambos operandos son constantes
                    int op1 = first->info->cons.value;
                    int op2 = second->info->cons.value;
                    int offSet =  temp->info->var.offset;
                    fprintf(file,"%s%i%s\n", "    mov $",op1,", %eax");
                    fprintf(file,"%s%i%s\n", "    add $",op2,", %eax");
                    fprintf(file,"%s%i%s\n", "    mov %eax ,",offSet,"(%ebp)");
                }
                else{
                    if(first->tag != 0 && first->tag != 4){ //primer operando es una constante (puede ser funcion?)
                        int op1 = first->info->cons.value;
                        int offSet2 =  second->info->var.offset;
                        int offSetTemp =  temp->info->var.offset;
                        fprintf(file,"%s%i%s\n", "    mov ",offSet2,"(%ebp), %eax");
                        fprintf(file,"%s%i%s\n", "    add $",op1,", %eax");
                        fprintf(file,"%s%i%s\n", "    mov %eax ,",offSetTemp,"(%ebp)");
                    }
                    else{ //segundo operando es una constante
                        int op2 = second->info->cons.value;
                        int offSet1 =  first->info->var.offset;
                        int offSetTemp =  temp->info->var.offset;
                        fprintf(file,"%s%i%s\n", "    mov ",offSet1,"(%ebp), %eax");
                        fprintf(file,"%s%i%s\n", "    add $",op2,", %eax");
                        fprintf(file,"%s%i%s\n", "    mov %eax ,",offSetTemp,"(%ebp)");
                    }
                }
            }
        }


        if(strcmp(index->node->codOp, "SUB") == 0){
            Node *first  = index->node->firstOp;
            Node *second = index->node->secondOp;
            Node *temp = index->node->temp;
            // Ambos operandos son variables o temporales
            if((first->tag == 0 || first->tag == 4) && (second->tag == 0 || second->tag == 4)){ 
                int op1 = first->info->cons.value;
                int op2 = second->info->cons.value;

                int offSet1 = first->info->var.offset;
                int offSet2 = second->info->var.offset;
                int offSetTemp = temp->info->var.offset;
                fprintf(file,"%s%i%s\n", "    mov ",offSet1,"(%ebp), %edx");
                fprintf(file,"%s%i%s\n", "    mov ",offSet2,"(%ebp), %eax");
                fprintf(file,"%s\n",     "    sub %edx, %eax");
                fprintf(file,"%s%i%s\n", "    mov %eax ,",offSetTemp,"(%ebp)");
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
                    fprintf(file,"%s%i%s\n", "    mov $",op1,", %eax");
                    fprintf(file,"%s%i%s\n", "    sub $",op2,", %eax");
                    fprintf(file,"%s%i%s\n", "    mov %eax ,",offSet,"(%ebp)");
                }
                else{
                    if(first->tag != 0 && first->tag != 4){ //primer operando es una constante (puede ser funcion?)
                        int op1 = first->info->cons.value;
                        int offSet2 =  second->info->var.offset;
                        int offSetTemp =  temp->info->var.offset;
                        fprintf(file,"%s%i%s\n", "    mov ",offSet2,"(%ebp), %eax");
                        fprintf(file,"%s%i%s\n", "    sub $",op1,", %eax");
                        fprintf(file,"%s%i%s\n", "    mov %eax ,",offSetTemp,"(%ebp)");

                        // y = 1 - x;
                        // movl    -4(%rbp), %eax
                        // sub     $1, %eax
                        // movl    %eax, -8(%rbp)
                    }
                    else{ //segundo operando es una constante                     
                        int offSet1 =  first->info->var.offset;
                        int offSet2 =  second->info->var.offset;
                        int offSetTemp =  temp->info->var.offset;
                        fprintf(file,"%s%i%s\n", "    mov ",offSet2,"(%ebp), %eax");
                        fprintf(file,"%s%i%s\n", "    sub ",offSet1,"(%ebp), %eax");
                        fprintf(file,"%s%i%s\n", "    mov %eax ,",offSetTemp,"(%ebp)");

                        // y = x - 2;
                        // movl    -4(%rbp), %eax
                        // sub     -8(%rbp), %eax
                        // movl    %eax, -8(%rbp)
                    }
                }
            }
        }

        if(strcmp(index->node->codOp, "MULT") == 0){
            Node *first  = index->node->firstOp;
            Node *second = index->node->secondOp;
            Node *temp = index->node->temp;
            // Ambos operandos son variables o temporales
            if((first->tag == 0 || first->tag == 4) && (second->tag == 0 || second->tag == 4)){ 
                int op1 = first->info->cons.value;
                int op2 = second->info->cons.value;

                int offSet1 = first->info->var.offset;
                int offSet2 = second->info->var.offset;
                int offSetTemp = temp->info->var.offset;
                fprintf(file,"%s%i%s\n", "    mov ",offSet1,"(%ebp), %edx");
                fprintf(file,"%s%i%s\n", "    mov ",offSet2,"(%ebp), %eax");
                fprintf(file,"%s\n",     "    imul %edx, %eax");
                fprintf(file,"%s%i%s\n", "    mov %eax ,",offSetTemp,"(%ebp)");
            }
            if((first->tag != 0 && first->tag != 4) || (second->tag != 0 && second->tag != 4)){
                if(first->tag == 1 && second->tag == 1){ //Ambos operandos son constantes
                    int op1 = first->info->cons.value;
                    int op2 = second->info->cons.value;
                    int offSet =  temp->info->var.offset;
                    fprintf(file,"%s%i%s\n", "    mov $",op1,", %eax");
                    fprintf(file,"%s%i%s\n", "    imul $",op2,", %eax");
                    fprintf(file,"%s%i%s\n", "    mov %eax ,",offSet,"(%ebp)");
                }
                else{
                    if(first->tag != 0 && first->tag != 4){ //primer operando es una constante (puede ser funcion?)
                        int op1 = first->info->cons.value;
                        int offSet2 =  second->info->var.offset;
                        int offSetTemp =  temp->info->var.offset;
                        fprintf(file,"%s%i%s\n", "    mov ",offSet2,"(%ebp), %eax");
                        fprintf(file,"%s%i%s\n", "    imul $",op1,", %eax");
                        fprintf(file,"%s%i%s\n", "    mov %eax ,",offSetTemp,"(%ebp)");
                    }
                    else{ //segundo operando es una constante
                        int op2 = second->info->cons.value;
                        int offSet1 =  first->info->var.offset;
                        int offSetTemp =  temp->info->var.offset;
                        fprintf(file,"%s%i%s\n", "    mov ",offSet1,"(%ebp), %eax");
                        fprintf(file,"%s%i%s\n", "    imul $",op2,", %eax");
                        fprintf(file,"%s%i%s\n", "    mov %eax ,",offSetTemp,"(%ebp)");
                    }
                }
            }
        }


        if(strcmp(index->node->codOp, "DIV") == 0){
            Node *first  = index->node->firstOp;
            Node *second = index->node->secondOp;
            Node *temp = index->node->temp;
            // Ambos operandos son variables o temporales
            if((first->tag == 0 || first->tag == 4) && (second->tag == 0 || second->tag == 4)){ 
                int offSet1 = first->info->var.offset;
                int offSet2 = second->info->var.offset;
                int offSetTemp = temp->info->var.offset;
                fprintf(file,"%s%i%s\n", "    mov ",offSet1,"(%ebp), %rax");
                fprintf(file,"%s\n",     "    mov $0, %rdx");
                fprintf(file,"%s%i%s\n", "    idiv ",offSet2,"(%ebp)");
                fprintf(file,"%s%i%s\n", "    mov %rax ,",offSetTemp,"(%ebp)");
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
                    fprintf(file,"%s%i%s\n", "    mov $",op1,", %rax");
                    fprintf(file,"%s\n",     "    mov $0, %rdx");
                    fprintf(file,"%s%i%s\n", "    mov $",op2,", %r10");
                    fprintf(file,"%s\n",     "    idiv %r10");
                    fprintf(file,"%s%i%s\n", "    mov %eax ,",offSetTemp,"(%ebp)");
                }
                else{
                    if(first->tag != 0 && first->tag != 4){ //primer operando es una constante (puede ser funcion?)
                        int op1 = first->info->cons.value;
                        int offSet2 = second->info->var.offset;
                        int offSetTemp = temp->info->var.offset;
                        fprintf(file,"%s%i%s\n", "    mov $",op1,", %rax");
                        fprintf(file,"%s\n",     "    mov $0, %rdx");
                        fprintf(file,"%s%i%s\n", "    idiv ",offSet2,"(%ebp)");
                        fprintf(file,"%s%i%s\n", "    mov %eax ,",offSetTemp,"(%ebp)");   

                    }
                    else{ //segundo operando es una constante
                        int op1 = first->info->cons.value;
                        int offSet1 = first->info->var.offset;
                        int offSetTemp = temp->info->var.offset;
                        fprintf(file,"%s%i%s\n", "    mov ",offSet1,"(%ebp), %rax");
                        fprintf(file,"%s\n",     "    mov $0, %rdx");                          
                        fprintf(file,"%s%i%s\n", "    mov $",op1,", %r10");
                        fprintf(file,"%s\n",     "    idiv %r10");
                        fprintf(file,"%s%i%s\n", "    mov %eax ,",offSetTemp,"(%ebp)"); 
                    }
                }
            }
        }

        
        if(strcmp(index->node->codOp, "MOD") == 0){
            Node *first  = index->node->firstOp;
            Node *second = index->node->secondOp;
            Node *temp = index->node->temp;
            // Ambos operandos son variables o temporales
            if((first->tag == 0 || first->tag == 4) && (second->tag == 0 || second->tag == 4)){ 
                int offSet1 = first->info->var.offset;
                int offSet2 = second->info->var.offset;
                int offSetTemp = temp->info->var.offset;
                fprintf(file,"%s%i%s\n", "    mov ",offSet1,"(%ebp), %rax");
                fprintf(file,"%s\n",     "    mov $0, %rdx");
                fprintf(file,"%s%i%s\n", "    idiv ",offSet2,"(%ebp)");
                fprintf(file,"%s%i%s\n", "    mov %edx ,",offSetTemp,"(%ebp)");
            }       
            if((first->tag != 0 && first->tag != 4) || (second->tag != 0 && second->tag != 4)){
                if(first->tag == 1 && second->tag == 1){ //Ambos operandos son constantes
                    int op1 = first->info->cons.value;
                    int op2 = second->info->cons.value;
                    int offSetTemp = temp->info->var.offset;
                    fprintf(file,"%s%i%s\n", "    mov $",op1,", %rax");
                    fprintf(file,"%s\n",     "    mov $0, %rdx");
                    fprintf(file,"%s%i%s\n", "    mov $",op2,", %r10");
                    fprintf(file,"%s\n",     "    idiv %r10");
                    fprintf(file,"%s%i%s\n", "    mov %edx ,",offSetTemp,"(%ebp)");
                }
                else{
                    if(first->tag != 0 && first->tag != 4){ //primer operando es una constante (puede ser funcion?)
                        int op1 = first->info->cons.value;
                        int offSet2 = second->info->var.offset;
                        int offSetTemp = temp->info->var.offset;
                        fprintf(file,"%s%i%s\n", "    mov $",op1,", %rax");
                        fprintf(file,"%s\n",     "    mov $0, %rdx");
                        fprintf(file,"%s%i%s\n", "    idiv ",offSet2,"(%ebp)");
                        fprintf(file,"%s%i%s\n", "    mov %edx ,",offSetTemp,"(%ebp)");   

                    }
                    else{ //segundo operando es una constante
                        int op1 = first->info->cons.value;
                        int offSet1 = first->info->var.offset;
                        int offSetTemp = temp->info->var.offset;
                        fprintf(file,"%s%i%s\n", "    mov ",offSet1,"(%ebp), %rax");
                        fprintf(file,"%s\n",     "    mov $0, %rdx");                          
                        fprintf(file,"%s%i%s\n", "    mov $",op1,", %r10");
                        fprintf(file,"%s\n",     "    idiv %r10");
                        fprintf(file,"%s%i%s\n", "    mov %edx ,",offSetTemp,"(%ebp)"); 
                    }
                }
            }
        }
        if(strcmp(index->node->codOp, "MAY") == 0){

        }
        if(strcmp(index->node->codOp, "MIN") == 0){

        }


        if(strcmp(index->node->codOp, "AND") == 0){

        /* (a && b)
            cmpl    $0, -4(%rbp)
            je  .L2
            cmpl    $0, -8(%rbp)
            je  .L2
            movl    $1, %eax
            jmp .L4
        .L2:
            movl    $0, %eax
        .L4:
        */
                          
            Node *first  = index->node->firstOp;
            Node *second = index->node->secondOp;
            Node *temp = index->node->temp;
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
                fprintf(file,"%s%i%s\n", "    cmp $0, ",offSet1,"(%ebp)"); 
                fprintf(file,"%s%s\n",   "    je ",label1);
            }

            if(second->tag == 1){
                int op2 = second->info->cons.value;
                fprintf(file,"%s%i\n", "    cmp $0, $",op2);
                fprintf(file,"%s%s\n",   "    je ",label1);
            }else{
                int offSet2 = second->info->var.offset;
                fprintf(file,"%s%i%s\n", "    cmp $0, ",offSet2,"(%ebp)"); 
                fprintf(file,"%s%s\n",   "    je ",label1);
            }
            strcat(label1,":");
            fprintf(file,"%s%i%s\n", "    mov $1,",offSetTemp,"(%ebp)");
            fprintf(file,"%s%s\n",   "    jmp ",label2);
            fprintf(file,"%s\n",label1);    
            fprintf(file,"%s%i%s\n", "    mov $0,",offSetTemp,"(%ebp)");
            strcat(label2,":");
            fprintf(file,"%s\n",label2);
        }


        if(strcmp(index->node->codOp, "OR") == 0){
            Node *first  = index->node->firstOp;
            Node *second = index->node->secondOp;
            Node *temp = index->node->temp;
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
                fprintf(file,"%s%i%s\n", "    cmp $1, ",offSet1,"(%ebp)"); 
                fprintf(file,"%s%s\n",   "    je ",label1);
            }

            if(second->tag == 1){ // segundo opetando es una constante
                int op2 = second->info->cons.value;
                fprintf(file,"%s%i\n", "    cmp $1, $",op2);
                fprintf(file,"%s%s\n",   "    je ",label1);
            }else{
                int offSet2 = second->info->var.offset;
                fprintf(file,"%s%i%s\n", "    cmp $1, ",offSet2,"(%ebp)"); 
                fprintf(file,"%s%s\n",   "    je ",label1);
            }
            strcat(label1,":");
            fprintf(file,"%s%i%s\n", "    mov $0,",offSetTemp,"(%ebp)");
            fprintf(file,"%s%s\n",   "    jmp ",label2);
            fprintf(file,"%s\n",label1);    
            fprintf(file,"%s%i%s\n", "    mov $1,",offSetTemp,"(%ebp)");
            strcat(label2,":");
            fprintf(file,"%s\n",label2);
        }


        if(strcmp(index->node->codOp, "EQUAL") == 0){
        	Node *first  = index->node->firstOp;
            Node *second = index->node->secondOp;
            Node *temp = index->node->temp;
            int offSetTemp = temp->info->var.offset;
            // Ambos operandos son variables o temporales
            if((first->tag == 0 || first->tag == 4) && (second->tag == 0 || second->tag == 4)){ 
                int offSet1 = first->info->var.offset;
                int offSet2 = second->info->var.offset;
                fprintf(file,"%s%i%s\n", "    mov ",offSet1,"(%ebp), %eax"); 
                fprintf(file,"%s%i%s\n", "    cmp ",offSet2,"(%ebp), %eax"); 
            }
            if((first->tag != 0 && first->tag != 4) || (second->tag != 0 && second->tag != 4)){
                if(first->tag == 1 && second->tag == 1){ //Ambos operandos son constantes
                    int op1 = first->info->cons.value;
                    int op2 = second->info->cons.value;
                    fprintf(file,"%s%i%s\n", "    mov $",op1,", %eax"); 
                	fprintf(file,"%s%i%s\n", "    cmp $",op2,", %eax"); 

                }
                else{
                    if(first->tag != 0 && first->tag != 4){ //primer operando es una constante (puede ser funcion?)
                        int op1 = first->info->cons.value;
                        int offSet2 =  second->info->var.offset;
               		    fprintf(file,"%s%i%s\n", "    mov ",offSet2,"(%ebp), %eax");  
               		    fprintf(file,"%s%i%s\n", "    cmp $",op1,", %eax");                      

                    }
                    else{ //segundo operando es una constante
                        int op2 = second->info->cons.value;
                        int offSet1 =  first->info->var.offset;
               		    fprintf(file,"%s%i%s\n", "    mov ",offSet1,"(%ebp), %eax");  
               		    fprintf(file,"%s%i%s\n", "    cmp $",op2,", %eax");   
                    }
                }
            }
     		fprintf(file,"%s\n",	"    sete	%al");
     		fprintf(file,"%s\n",	"    movzbl  %al, %eax");
     		fprintf(file,"%s%i%s\n","    mov  %eax,",offSetTemp,"(%ebp)");
        }


        if(strcmp(index->node->codOp, "NEGB") == 0){
        	int offSetTemp = temp->info->var.offset;
        	if(first->tag == 0 || first->tag == 4){ //primer operando es una variable
        		int offSet1 =  first->info->var.offset;
        		fprintf(file,"%s%i%s\n", "    cmp  $0,",offSet1,"(%ebp)");    
        	}else{
        		int op1 = first->info->cons.value;
        		fprintf(file,"%s%i%s\n", "    mov $",op1,", %eax"); 
                fprintf(file,"%s\n",     "    cmp $0, %eax");
        	}
     		fprintf(file,"%s\n",	"    sete	%al");
     		fprintf(file,"%s\n",	"    movzbl  %al, %eax");
     		fprintf(file,"%s%i%s\n","    mov  %eax,",offSetTemp,"(%ebp)");                      
        }

        if(strcmp(index->node->codOp, "NEGI") == 0){

        }

        if(strcmp(index->node->codOp, "RETURN") == 0){
            int aux;
            int offSet;
            switch (index->node->temp->tag) {
                case 0:
                	offSet= temp->info->var.offset;
                	fprintf(file,"%s%i%s\n", "    mov ",offSet,"(%ebp), %eax");
                	break;
                case 1:
                    aux = temp->info->cons.value;
                    fprintf(file,"%s%i%s\n", "    mov $",aux,", %eax");
                    break;
                case 4:
                	offSet= temp->info->var.offset;
                	fprintf(file,"%s%i%s\n", "    mov ",offSet,"(%ebp), %eax");
                    break;
                default:
                    break;
            }
            fprintf(file, "%s\n", "    leaq	L_.str(%rip), %rdi");
            fprintf(file, "%s\n", "    movl	%eax, %esi");
            fprintf(file, "%s\n", "    movl  $0, -4(%rbp)");
            fprintf(file, "%s\n", "    movb	$0, %al");
            fprintf(file, "%s\n", "    callq	_printf");
        }


        if(strcmp(index->node->codOp, "RETURNV") == 0){
            // NO DEBE HACER NADA
        }
        if(strcmp(index->node->codOp, "IFF") == 0){

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
				fprintf(file,"%s%i%s\n", "    mov ",offSet2,"(%ebp), %eax");
				fprintf(file,"%s%i%s\n", "    mov %eax ",offSet1,"(%ebp)");
        	}else{
        		int op2 = second->info->cons.value;
        		fprintf(file,"%s%i%s%i%s\n", "    mov $",op2," ",offSet1,"(%ebp)");
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
