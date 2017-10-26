#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.c"
#include "ciList.c"

extern CIList *ciList;

/**
 * Funcion utilizada para generar codigo intermedio a partir de un arbols sintactico.
 */
Node *generateIC(Node *root){
    static int tempCount = 0;
    static int labelCount = 0;

    if((root->tag == 0)||(root->tag == 1)){
        return root;
    }

    if(root->tag == 2){
        if (strcmp(root->info->op.id, "=") == 0){
             Node *dir2 = generateIC(root->mid);

            NodeCI *new = newNodeCI("MOV",generateIC(root->left),dir2, NULL);
            ciList = insertLastCI(ciList,new);
        }
        if (strcmp(root->info->op.id, "+") == 0){
            Node *dir2 = generateIC(root->mid);
            Node *dir1 = generateIC(root->left);
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            tempCount++;
            Node *newTemporal = newVar(tempId,0,0,0);
            NodeCI *new = newNodeCI("ADD",dir1, dir2, newTemporal);
            ciList = insertLastCI(ciList,new);
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "-") == 0){
            Node *dir2 = generateIC(root->mid);
            Node *dir1 = generateIC(root->left);
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            tempCount++;
            Node *newTemporal = newVar(tempId,0,0,0);
            NodeCI *new = newNodeCI("SUB",dir1, dir2, newTemporal);
            ciList = insertLastCI(ciList,new);
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "*") == 0){
            Node *dir2 = generateIC(root->mid);
            Node *dir1 = generateIC(root->left);
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            tempCount++;
            Node *newTemporal = newVar(tempId,0,0,0);
            NodeCI *new = newNodeCI("MULT", dir1, dir2, newTemporal);
            ciList = insertLastCI(ciList,new);
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "/") == 0){
            Node *dir2 = generateIC(root->mid);
            Node *dir1 = generateIC(root->left);
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            tempCount++;
            Node *newTemporal = newVar(tempId,0,0,0);
            NodeCI *new = newNodeCI("DIV",dir1, dir2, newTemporal);
            ciList = insertLastCI(ciList,new);
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "%") == 0){
            Node *dir2 = generateIC(root->mid);
            Node *dir1 = generateIC(root->left);
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            tempCount++;
            Node *newTemporal = newVar(tempId,0,0,0);
            NodeCI *new = newNodeCI("MOD", dir1, dir2, newTemporal);
            ciList = insertLastCI(ciList,new);
            return newTemporal;
        }
        if (strcmp(root->info->op.id, ">") == 0){
            Node *dir2 = generateIC(root->mid);
            Node *dir1 = generateIC(root->left);
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            tempCount++;
            Node *newTemporal = newVar(tempId,1,0,0);
            NodeCI *new = newNodeCI("MAY", dir1, dir2, newTemporal);
            ciList = insertLastCI(ciList,new);
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "<") == 0){
            Node *dir2 = generateIC(root->mid);
            Node *dir1 = generateIC(root->left);
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            tempCount++;
            Node *newTemporal = newVar(tempId,1,0,0);
            NodeCI *new = newNodeCI("MIN", dir1, dir2, newTemporal);
            ciList = insertLastCI(ciList,new);
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "&&") == 0){
            Node *dir2 = generateIC(root->mid);
            Node *dir1 = generateIC(root->left);
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            tempCount++;
            Node *newTemporal = newVar(tempId,1,0,0);
            NodeCI *new = newNodeCI("AND", dir1, dir2, newTemporal);
            ciList = insertLastCI(ciList,new);
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "||") == 0){
            Node *dir2 = generateIC(root->mid);
            Node *dir1 = generateIC(root->left);
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            tempCount++;
            Node *newTemporal = newVar(tempId,1,0,0);
            NodeCI *new = newNodeCI("OR", dir1, dir2, newTemporal);
            ciList = insertLastCI(ciList,new);
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "==") == 0){
            Node *dir2 = generateIC(root->mid);
            Node *dir1 = generateIC(root->left);
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            tempCount++;
            Node *newTemporal = newVar(tempId,root->type,0,0);
            NodeCI *new = newNodeCI("EQUAL", dir1, dir2, newTemporal);
            ciList = insertLastCI(ciList,new);
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "!") == 0){
            Node *dir1 = generateIC(root->left);
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            tempCount++;
            Node *newTemporal = newVar(tempId,1,0,0);
            NodeCI *new = newNodeCI("NEGB", dir1 , NULL, newTemporal);
            ciList = insertLastCI(ciList,new);
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "negativo") == 0){
            Node *dir1 = generateIC(root->left);
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            tempCount++;
            Node *newTemporal = newVar(tempId,0,0,0);
            NodeCI *new = newNodeCI("NEGI", dir1, NULL, newTemporal);
            ciList = insertLastCI(ciList,new);
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "return") == 0){
            if (root->left->tag == 0 || root->left->tag == 1){
                NodeCI *new = newNodeCI("RETURN",NULL,NULL, root->left);
                ciList = insertLastCI(ciList,new);
            }else{
                Node* justParam = generateIC(root->left);
                justParam = generateIC(justParam);
                if(justParam->tag != 0 && justParam->tag != 1 ){
                    justParam = generateIC(justParam);
                }
                NodeCI *load = newNodeCI("RETURN",NULL,NULL,justParam);
                ciList = insertLastCI(ciList,load);
            }
            return NULL;
        }
        if (strcmp(root->info->op.id, "returnVoid") == 0){
            NodeCI *new = newNodeCI("RETURNV",NULL,NULL, NULL);
            ciList = insertLastCI(ciList,new);
            return NULL;
        }
        if (strcmp(root->info->op.id, "if") == 0){
            char labelId[20];
            char aux[20];
            sprintf(aux, "%d", labelCount);
            strcpy(labelId,"L");
            strcat(labelId,aux);
            Node *label0 = newVar(labelId,0,0,0);
            labelCount++;
            Node *cond = generateIC(root->left);
            NodeCI *nIF = newNodeCI("IFF",cond,label0, NULL);
            ciList = insertLastCI(ciList,nIF);
            generateIC(root->mid);
            NodeCI *l0 = newNodeCI("LABEL",NULL,NULL, label0);
            ciList = insertLastCI(ciList,l0);
            return NULL;
        }
        if (strcmp(root->info->op.id, "ifElse") == 0){
            char labelId[20];
            char aux[20];
            sprintf(aux, "%d", labelCount);
            strcpy(labelId,"L");
            strcat(labelId,aux);
            Node *label0 = newVar(labelId,0,0,0);
            labelCount++;
            sprintf(aux, "%d", labelCount);
            strcpy(labelId,"L");
            strcat(labelId,aux);
            Node *label1 = newVar(labelId,0,0,0);
            labelCount++;
            Node *cond = generateIC(root->left);
            NodeCI *nIF = newNodeCI("IFF",cond,label0, NULL);
            ciList = insertLastCI(ciList,nIF);
            generateIC(root->mid);
            NodeCI *jmp = newNodeCI("JMP",NULL,NULL, label1);
            ciList = insertLastCI(ciList,jmp);
            NodeCI *l0 = newNodeCI("LABEL",NULL,NULL, label0);
            ciList = insertLastCI(ciList,l0);
            generateIC(root->right);
            NodeCI *l1 = newNodeCI("LABEL",NULL,NULL, label1);
            ciList = insertLastCI(ciList,l1);
            return NULL;
        }
        if (strcmp(root->info->op.id, "while") == 0){
            char labelId[20];
            char aux[20];
            sprintf(aux, "%d", labelCount);
            strcpy(labelId,"L");
            strcat(labelId,aux);
            Node *label0 = newVar(labelId,0,0,0);
            labelCount++;
            sprintf(aux, "%d", labelCount);
            strcpy(labelId,"L");
            strcat(labelId,aux);
            Node *label1 = newVar(labelId,0,0,0);
            labelCount++;
            NodeCI *l0 = newNodeCI("LABEL",NULL,NULL, label0);
            ciList = insertLastCI(ciList,l0);
            Node *cond = generateIC(root->left);
            NodeCI *nIF = newNodeCI("IFF",cond,label1, NULL);
            ciList = insertLastCI(ciList,nIF);
            generateIC(root->mid);
            NodeCI *jmp = newNodeCI("JMP",NULL,NULL, label0);
            ciList = insertLastCI(ciList,jmp);
            NodeCI *l1 = newNodeCI("LABEL",NULL,NULL, label1);
            ciList = insertLastCI(ciList,l1);
            return NULL;
        }
        if (strcmp(root->info->op.id, "function") == 0){
            List *par = root->mid->info->func.param;
            CIList *list_aux = newCIList(list_aux);
            while(par != NULL){
            	Node* justParam = generateIC(par->node);
            	if(justParam->tag != 0 && justParam->tag != 1 ){
            		justParam = generateIC(justParam);
            	}
                NodeCI *load = newNodeCI("LOAD",justParam,NULL,NULL);
                list_aux = insertLastCI(list_aux,load);
                par = par->next;
            }
            while(list_aux != NULL){
                ciList = insertLastCI(ciList,list_aux->node);
                list_aux = list_aux->next;
            }
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            Node *newTemporal = newVar(tempId,root->left->type,0,0);
            NodeCI *callF = newNodeCI("CALL",root->left,NULL,newTemporal);
            ciList = insertLastCI(ciList,callF);
            tempCount++;
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "functionVoid") == 0){
            NodeCI *callF = newNodeCI("CALL",root->left,NULL,NULL);
            ciList = insertLastCI(ciList,callF);
            return NULL;
        }
        if (strcmp(root->info->op.id, ";") == 0){
            generateIC(root->left);
            generateIC(root->mid);
            return NULL;
        }
    }
    return NULL;
}

void insertInitIC(Node *root){
    NodeCI *beginF = newNodeCI("BEGIN",root,NULL,NULL);
    ciList = insertLastCI(ciList,beginF);
}

void insertEndIC(Node *root){
    NodeCI *endF = newNodeCI("END",root,NULL,NULL);
    ciList = insertLastCI(ciList,endF);
}
