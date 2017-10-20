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
            NodeCI *new = newNodeCI("MOV",generateIC(root->left),generateIC(root->mid), NULL);
            ciList = insertLastCI(ciList,new);
        }
        if (strcmp(root->info->op.id, "+") == 0){
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            tempCount++;
            Node *newTemporal = newVar(tempId,0,0,0);
            NodeCI *new = newNodeCI("ADD",generateIC(root->left),generateIC(root->mid), newTemporal);
            ciList = insertLastCI(ciList,new);
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "-") == 0){
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            tempCount++;
            Node *newTemporal = newVar(tempId,0,0,0);
            NodeCI *new = newNodeCI("SUB",generateIC(root->left),generateIC(root->mid), newTemporal);
            ciList = insertLastCI(ciList,new);
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "*") == 0){
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            tempCount++;
            Node *newTemporal = newVar(tempId,0,0,0);
            NodeCI *new = newNodeCI("MULT",generateIC(root->left),generateIC(root->mid), newTemporal);
            ciList = insertLastCI(ciList,new);
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "/") == 0){
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            tempCount++;
            Node *newTemporal = newVar(tempId,0,0,0);
            NodeCI *new = newNodeCI("DIV",generateIC(root->left),generateIC(root->mid), newTemporal);
            ciList = insertLastCI(ciList,new);
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "%") == 0){
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            tempCount++;
            Node *newTemporal = newVar(tempId,0,0,0);
            NodeCI *new = newNodeCI("MOD",generateIC(root->left),generateIC(root->mid), newTemporal);
            ciList = insertLastCI(ciList,new);
            return newTemporal;
        }
        if (strcmp(root->info->op.id, ">") == 0){
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            tempCount++;
            Node *newTemporal = newVar(tempId,1,0,0);
            NodeCI *new = newNodeCI("MAY",generateIC(root->left),generateIC(root->mid), newTemporal);
            ciList = insertLastCI(ciList,new);
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "<") == 0){
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            tempCount++;
            Node *newTemporal = newVar(tempId,1,0,0);
            NodeCI *new = newNodeCI("MIN",generateIC(root->left),generateIC(root->mid), newTemporal);
            ciList = insertLastCI(ciList,new);
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "&&") == 0){
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            tempCount++;
            Node *newTemporal = newVar(tempId,1,0,0);
            NodeCI *new = newNodeCI("AND",generateIC(root->left),generateIC(root->mid), newTemporal);
            ciList = insertLastCI(ciList,new);
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "||") == 0){
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            tempCount++;
            Node *newTemporal = newVar(tempId,1,0,0);
            NodeCI *new = newNodeCI("OR",generateIC(root->left),generateIC(root->mid), newTemporal);
            ciList = insertLastCI(ciList,new);
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "==") == 0){
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            tempCount++;
            Node *newTemporal = newVar(tempId,root->type,0,0);
            NodeCI *new = newNodeCI("EQUAL",generateIC(root->left),generateIC(root->mid), newTemporal);
            ciList = insertLastCI(ciList,new);
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "!") == 0){
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            tempCount++;
            Node *newTemporal = newVar(tempId,1,0,0);
            NodeCI *new = newNodeCI("NEGB",generateIC(root->left),NULL, newTemporal);
            ciList = insertLastCI(ciList,new);
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "negativo") == 0){
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            tempCount++;
            Node *newTemporal = newVar(tempId,0,0,0);
            NodeCI *new = newNodeCI("NEGI",generateIC(root->left),NULL, newTemporal);
            ciList = insertLastCI(ciList,new);
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "return") == 0){
            if (root->left->tag == 0){
                NodeCI *new = newNodeCI("RETURN",NULL,NULL, root->left);
                ciList = insertLastCI(ciList,new);
            }else{
                char tempId[20];
                char aux[20];
                sprintf(aux,"%d",tempCount);
                strcpy(tempId,"T");
                strcat(tempId,aux);
                tempCount++;
                Node *newTemporal = newVar(tempId,root->type,0,0);
                NodeCI *new = newNodeCI("RETURN",NULL,NULL, newTemporal);
                ciList = insertLastCI(ciList,new);
                tempCount++;
                return newTemporal;
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
            while(par != NULL){
            	Node* justParam = generateIC(par->node);
            	if(justParam->tag != 0 && justParam->tag != 1 ){
            		justParam = generateIC(justParam);
            	}		
                NodeCI *load = newNodeCI("LOAD",justParam,NULL,NULL);
                ciList = insertLastCI(ciList,load);
                par = par->next;
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
    NodeCI *beginF = newNodeCI("BEGIN ",root,NULL,NULL);
    ciList = insertLastCI(ciList,beginF);
}

void insertEndIC(Node *root){
    NodeCI *endF = newNodeCI("END ",root,NULL,NULL);
    ciList = insertLastCI(ciList,endF);
}
