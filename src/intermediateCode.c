#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.c"
#include "ciList.c"

extern CIList *ciList;
/*
    Antes de llamar a generateIC se debe inicializar tempCount, labelCount y newCIList
 */
Node *generateIC(Node *root){
    static int tempCount = 0;
    static int labelCount = 0;
    if((root->tag == 0)||(root->tag == 1)){  //es una constante o variable
        return root;
    }

    if(root->tag == 2){  //es un operador
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
            Node *newTemporal = newVar(tempId,0,0,0);
            NodeCI *new = newNodeCI("ADD",generateIC(root->left),generateIC(root->mid), newTemporal);
            ciList = insertLastCI(ciList,new);
            tempCount++;
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "-") == 0){
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            Node *newTemporal = newVar(tempId,0,0,0);
            NodeCI *new = newNodeCI("SUB",generateIC(root->left),generateIC(root->mid), newTemporal);
            ciList = insertLastCI(ciList,new);
            tempCount++;
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "*") == 0){
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            Node *newTemporal = newVar(tempId,0,0,0);
            NodeCI *new = newNodeCI("MULT",generateIC(root->left),generateIC(root->mid), newTemporal);
            ciList = insertLastCI(ciList,new);
            tempCount++;
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "/") == 0){
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            Node *newTemporal = newVar(tempId,0,0,0);
            NodeCI *new = newNodeCI("DIV",generateIC(root->left),generateIC(root->mid), newTemporal);
            ciList = insertLastCI(ciList,new);
            tempCount++;
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "%") == 0){
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            Node *newTemporal = newVar(tempId,0,0,0);
            NodeCI *new = newNodeCI("MOD",generateIC(root->left),generateIC(root->mid), newTemporal);
            ciList = insertLastCI(ciList,new);
            tempCount++;
            return newTemporal;
        }
        if (strcmp(root->info->op.id, ">") == 0){
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            Node *newTemporal = newVar(tempId,1,0,0);
            NodeCI *new = newNodeCI("MAY",generateIC(root->left),generateIC(root->mid), newTemporal);
            ciList = insertLastCI(ciList,new);
            tempCount++;
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "<") == 0){
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            Node *newTemporal = newVar(tempId,1,0,0);
            NodeCI *new = newNodeCI("MIN",generateIC(root->left),generateIC(root->mid), newTemporal);
            ciList = insertLastCI(ciList,new);
            tempCount++;
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "&&") == 0){
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            Node *newTemporal = newVar(tempId,1,0,0);
            NodeCI *new = newNodeCI("AND",generateIC(root->left),generateIC(root->mid), newTemporal);
            ciList = insertLastCI(ciList,new);
            tempCount++;
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "||") == 0){
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            Node *newTemporal = newVar(tempId,1,0,0);
            NodeCI *new = newNodeCI("OR",generateIC(root->left),generateIC(root->mid), newTemporal);
            ciList = insertLastCI(ciList,new);
            tempCount++;
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "==") == 0){
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            Node *newTemporal = newVar(tempId,root->type,0,0);
            NodeCI *new = newNodeCI("EQUAL",generateIC(root->left),generateIC(root->mid), newTemporal);
            ciList = insertLastCI(ciList,new);
            tempCount++;
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "!") == 0){
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            Node *newTemporal = newVar(tempId,1,0,0);
            NodeCI *new = newNodeCI("NEGB",generateIC(root->left),NULL, newTemporal);
            ciList = insertLastCI(ciList,new);
            tempCount++;
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "negativo") == 0){
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            Node *newTemporal = newVar(tempId,0,0,0);
            NodeCI *new = newNodeCI("NEGI",generateIC(root->left),NULL, newTemporal);
            ciList = insertLastCI(ciList,new);
            tempCount++;
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "return") == 0){
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            Node *newTemporal = newVar(tempId,root->type,0,0);
            NodeCI *new = newNodeCI("RETURN",NULL,NULL, newTemporal);
            ciList = insertLastCI(ciList,new);
            tempCount++;
            return newTemporal;
        }
        if (strcmp(root->info->op.id, "returnVoid") == 0){
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            Node *newTemporal = newVar(tempId,root->type,0,0);
            NodeCI *new = newNodeCI("RETURNV",NULL,NULL, newTemporal);
            ciList = insertLastCI(ciList,new);
            tempCount++;
            return newTemporal;
        }

        if (strcmp(root->info->op.id, "if") == 0){
            char labelId[20];
            char aux[20];
            sprintf(aux, "%d", labelCount);
            strcpy(labelId,"L");
            strcat(labelId,aux);
            Node *label0 = newVar(labelId,0,0,0);
            labelCount++;
            // condicion if
            Node *cond = generateIC(root->left);

            // IF temp L0 (si temp es false, salto hacia L0)
            NodeCI *nIF = newNodeCI("IFF",cond,label0, NULL);
            ciList = insertLastCI(ciList,nIF);

            // cuerpo if
            generateIC(root->mid);

            // LABEL L0
            NodeCI *l0 = newNodeCI("LABEL",NULL,NULL, label0);
            ciList = insertLastCI(ciList,l0);

            return NULL;
            //eval condition -> eval statements -> add new Level
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

            // condicion del ifElse
            Node *cond = generateIC(root->left);

            // IF temp L0 (si temp es false, salto hacia L0)
            NodeCI *nIF = newNodeCI("IFF",cond,label0, NULL);
            ciList = insertLastCI(ciList,nIF);

            // cuerpo del if
            generateIC(root->mid);

            // JMP L1
            NodeCI *jmp = newNodeCI("JMP",NULL,NULL, label1);
            ciList = insertLastCI(ciList,jmp);

            // LABEL L0
            NodeCI *l0 = newNodeCI("LABEL",NULL,NULL, label0);
            ciList = insertLastCI(ciList,l0);

            // cuerpo del Else
            generateIC(root->right);

            // LABEL L1
            NodeCI *l1 = newNodeCI("LABEL",NULL,NULL, label1);
            ciList = insertLastCI(ciList,l1);

            return NULL;
            //eval condition -> eval statements -> add new Level ->
            //eval statements else -> add new Level
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

            // LABEL L0
            NodeCI *l0 = newNodeCI("LABEL",NULL,NULL, label0);
            ciList = insertLastCI(ciList,l0);

            // condicion del while
            Node *cond = generateIC(root->left);

            // IF temp L1 (si temp es false, salto hacia L1)
            NodeCI *nIF = newNodeCI("IFF",cond,label1, NULL);
            ciList = insertLastCI(ciList,nIF);

            // cuerpo del while
            generateIC(root->mid);

            // JMP L0
            NodeCI *jmp = newNodeCI("JMP",NULL,NULL, label0);
            ciList = insertLastCI(ciList,jmp);

            // LABEL L1
            NodeCI *l1 = newNodeCI("LABEL",NULL,NULL, label1);
            ciList = insertLastCI(ciList,l1);

            return NULL;
            // add new Level -> eval condition -> eval statements -> add new Level
        }
        if (strcmp(root->info->op.id, "function") == 0){
            List *par = root->mid->info->func.param;
            while(par != NULL){
                NodeCI *load = newNodeCI("LOAD",par->node,NULL,NULL);
                ciList = insertLastCI(ciList,load);
                par = par->next;
            }
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            Node *newTemporal = newVar(tempId,root->left->type,0,0);
            NodeCI *callF = newNodeCI("CALL",root->left,newTemporal,NULL);
            ciList = insertLastCI(ciList,callF);
            tempCount++;

            return newTemporal;
        }

        if (strcmp(root->info->op.id, "functionVoid") == 0){
            char tempId[20];
            char aux[20];
            sprintf(aux,"%d",tempCount);
            strcpy(tempId,"T");
            strcat(tempId,aux);
            Node *newTemporal = newVar(tempId,root->left->type,0,0);
            NodeCI *callF = newNodeCI("CALL",root->left,newTemporal,NULL);
            ciList = insertLastCI(ciList,callF);
            tempCount++;

            return newTemporal;
        }

        if (strcmp(root->info->op.id, ";") == 0){
            generateIC(root->left);
            generateIC(root->mid);
            return NULL;
        }
    }
    if(root->tag == 3){  // es funcion
        char nameFunc[20];

        strcpy(nameFunc,"BEGIN ");
        strcat(nameFunc, root->info->func.id);
        NodeCI *beginF = newNodeCI(nameFunc,NULL,NULL,NULL);
        ciList = insertLastCI(ciList,beginF);

        generateIC(root->info->func.AST);

        strcpy(nameFunc,"END ");
        strcat(nameFunc, root->info->func.id);
        NodeCI *endF = newNodeCI(nameFunc,NULL,NULL,NULL);
        ciList = insertLastCI(ciList,endF);
        return NULL;
    }
    return NULL;
}
