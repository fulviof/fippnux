
#include<stdio.h>
//#include<conio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<ctype.h>
//#include<windows.h>
#include<cstring>
#define RAIZ 17
int contDir = 0;
int i_node= 0;

struct dir
{
    int vetl_node[5];
    char vetArqDir[5][10];
    int TL;
};
typedef struct dir Dir;

struct pont_bloco
{
    int bloco;
    struct pont_bloco *prox;
    union
    {
        Dir dir;
        char parteArquivo[74];
    };
};
typedef struct pont_bloco pontBloco;

struct pontl_node
{
    int i_node;
    char tipo;
    char atr[9];
    int tam;
    char data[11];
    char hora[6];
    struct pontl_node *prox;
    int vetBloco[5];
    pontBloco *vetPontoBloco[5];
    int TL;
};
typedef struct pontl_node iNode;


struct pont_sist_arq
{
    iNode *nodes;
    pontBloco *diretorio_raiz;
    pontBloco *arquivos_diretorios;
};
typedef struct pont_sist_arq pontSistArq;


struct list_gen_arq_dir
{
    int i_node;
    iNode *pi_node;
    struct list_gen_arq_dir *head;
    struct list_gen_arq_dir *tail;
};
typedef struct list_gen_arq_dir listaGenArqDir;

struct descrit
{
    pontSistArq *ArqDir;
    listaGenArqDir *ListGen;
};
typedef struct descrit Descritor;


void exibeCalendario(char op[])
{
    int d, m, a, res, x;
    char mes[2];
    char ano[4];
    
    for(int i = 0; i<2; i++)
    {
        mes[i] = op[i+4];
    }
    
    for(int i=0; i<4; i++)
    {
        ano[i] = op[i+6];
    }
    
    
    d = 1;
    m = atoi(mes);
    a = atoi(ano);
    
    if (m > 2)
    {
        m = m - 2;
        x = 8;
    }
    else
    {
        a = a - 1;
        x = 13;
    }
    res = (x + d + 2 * m + ((7 * m) / 12) + a % 7 + a / 4 % 7 - (3 * (1 + a / 100)) / 4 % 7) % 7;
    
    if(res == 0)
        printf("SAB   DOM   SEG   TER   QUA   QUI   SEX\n");
    
    if(res == 1)
        printf("DOM   SEG   TER   QUA   QUI   SEX   SAB\n");
    
    if(res == 2)
        printf("SEG   TER   QUA   QUI   SEX   SAB   DOM\n");
    
    if(res == 3)
        printf("TER   QUA   QUI   SEX   SAB   DOM   SEG\n");
    
    if(res == 4)
        printf("QUA   QUI   SEX   SAB   DOM   SEG   TER\n");
    
    if(res == 5)
        printf("QUI   SEX   SAB   DOM   SEG   TER   QUA\n");
    
    if(res == 6)
        printf("SEX   SAB   DOM   SEG   TER   QUA   QUI\n");
    
    int lin = 0;
    while(lin<5)
    {
        for(int i = 0; i<7; i++, d++)
            if(d <= 31)
            {
                if(d<10)
                    printf("0%d    ", d);
                else
                    printf("%d    ", d);
            }
        printf("\n");
        lin++;
    }
    
    printf("\n\n");
}

Descritor inicializaShell()
{
    Descritor desc;
    
    desc.ArqDir = (pontSistArq*) malloc(sizeof(pontSistArq));
    
    desc.ListGen = (listaGenArqDir*) malloc(sizeof(listaGenArqDir));
    
    desc.ListGen->i_node = 1;
    desc.ListGen->head = NULL;
    desc.ListGen->pi_node = NULL;
    desc.ListGen->tail = NULL;
    
    desc.ArqDir->nodes = NULL;
    desc.ArqDir->arquivos_diretorios = NULL;
    desc.ArqDir->diretorio_raiz = (pontBloco*) malloc(sizeof(pontBloco));
    
    desc.ArqDir->diretorio_raiz->bloco =1;
    desc.ArqDir->diretorio_raiz->dir.TL = 2;
    desc.ArqDir->diretorio_raiz->dir.vetl_node[0] = 1;
    desc.ArqDir->diretorio_raiz->dir.vetl_node[1] = 1;
    desc.ArqDir->diretorio_raiz->prox = NULL;
    
    strcpy(desc.ArqDir->diretorio_raiz->dir.vetArqDir[0], ".");
    
    strcpy(desc.ArqDir->diretorio_raiz->dir.vetArqDir[1], "..");
    
    contDir = 2;
    
    return desc;
}

pontBloco *criaBloco()
{
    pontBloco *bloco = (pontBloco*) malloc(sizeof(pontBloco));
    
    bloco->prox = NULL;
    bloco->bloco = contDir++;
    bloco->dir.TL = 0;
    bloco->parteArquivo[0] = '\0';
    
    for(int i = 0; i< 5; i++)
    {
        strcmp(bloco->dir.vetArqDir[i], "\0");
        bloco->dir.vetl_node[i] = 0;
    }
    
    
    return bloco;
}

iNode *criaNode(int inode)
{
    iNode *node = (iNode*) malloc(sizeof(iNode));
    
    strcpy(node->atr, "rwxr-xr-x");
    strcpy(node->data, __DATE__);
    strcpy(node->hora, __TIME__);
    node->i_node = inode;
    node->prox = NULL;
    node->tam = 0;
    node->tipo = 'd';
    node->TL = 0;
    for(int i = 0; i<5; i++)
    {
        node->vetPontoBloco[i] = NULL;
        node->vetBloco[i] = 0;
    }
    
    return node;
}

listaGenArqDir *criaCaixagen(int inode, iNode *node)
{
    listaGenArqDir *caixa = (listaGenArqDir*) malloc(sizeof(listaGenArqDir));
    
    caixa->head = NULL;
    caixa->i_node = inode;
    caixa->pi_node = node;
    caixa->tail = NULL;
    
    return caixa;
}

pontBloco *buscaArqDir(pontBloco *arqDir, char nome[])
{
    while(arqDir != NULL)
    {
        for(int i = 0; i < arqDir->dir.TL; i++)
        {
            if(strcmp(arqDir->dir.vetArqDir[i], nome) == 0)
            {
                i_node = arqDir->dir.vetl_node[i];
                return arqDir;
            }
            
        }
        arqDir = arqDir->prox;
    }
    return NULL;
}


pontBloco *buscaDirRaiz(pontBloco *raiz, char nome[])
{
    while(raiz != NULL)
    {
        for(int i = 0; i<raiz->dir.TL; i++)
        {
            if(strcmp(raiz->dir.vetArqDir[i], nome) == 0)
            {
                i_node = raiz->dir.vetl_node[i];
                return raiz;
            }
            
        }
        raiz = raiz->prox;
    }
    return NULL;
}

iNode *buscaNode(iNode *node, int i_node)
{
    while(node != NULL)
    {
        if(node->i_node == i_node)
            return node;
        
        node = node->prox;
    }
    return NULL;
}

void buscaListagen(listaGenArqDir *listagen, int i_node, iNode **aux)
{
    if(listagen == NULL)
        return;
    else
    {
        if(listagen->i_node == i_node)
        {
            *aux = listagen->pi_node;
            return;
        }
        else
        {
            buscaListagen(listagen->head, i_node, &*aux);
            buscaListagen(listagen->tail, i_node, &*aux);
        }
    }
}


void insereCaixagen(listaGenArqDir **listagen, int i_node, iNode *node)
{
    if(*listagen == NULL)
        return;
    else
    {
        if((*listagen)->i_node == i_node)
        {
            listaGenArqDir *nova = criaCaixagen(node->i_node, node);
            nova->tail = (*listagen)->head;
            (*listagen)->head = nova;
            return;
        }
        
        else
        {
            insereCaixagen(&(*listagen)->head, i_node, node);
            insereCaixagen(&(*listagen)->tail, i_node, node);
        }
    }
}

char existeDir(Descritor desc, char path[])
{
    int len = (int)strlen(path);
    int flag = 0;
    
    for(int i = 0; i<len; i++)
        if(path[i] == '/')
            flag++;
    
    int auxFlag = flag;
    char *splitDir = NULL;
    splitDir = strtok(path, "/");
    while(splitDir)
    {
        pontBloco *bloco = desc.ArqDir->diretorio_raiz;
        while(bloco != NULL)
        {
            for(int i = 0; i<bloco->dir.TL; i++)
            {
                if(strcmp(bloco->dir.vetArqDir[i], splitDir) == 0)
                    flag--;
            }
            bloco = bloco->prox;
        }
        
        if(auxFlag == flag)
        {
            pontBloco *arqOrdir = desc.ArqDir->arquivos_diretorios;
            
            while(arqOrdir != NULL)
            {
                if(arqOrdir->parteArquivo[0] != '\0')
                    for(int i = 0; i<arqOrdir->dir.TL; i++)
                    {
                        if(strcmp(arqOrdir->dir.vetArqDir[i], splitDir) == 0)
                            flag--;
                    }
                
                arqOrdir = arqOrdir->prox;
            }
        }
        auxFlag = flag;
        splitDir = strtok(NULL, "/");
    }
    
    if(flag == 0)
        return 1;
    else
        return 0;
    
}

void criaDiretorio(Descritor *desc, char nome[], char path[])
{
    int cont = 1;
    pontBloco *blocoDir = (*desc).ArqDir->diretorio_raiz;
    int TL = blocoDir->dir.TL;
    
    if(strcmp(path, "fulvio@fippnux:~$") == 0)
    {
        if(blocoDir->bloco == 1 && TL < 5)
        {
            strcpy(blocoDir->dir.vetArqDir[TL], nome);
            blocoDir->dir.vetl_node[TL] = contDir;
        }
        else
        {
            while(blocoDir->dir.TL >= 5 && blocoDir->prox != NULL)
            {
                blocoDir = blocoDir->prox;
                cont++;
            }
            
            if(blocoDir->dir.TL < 5)
            {
                
                TL = blocoDir->dir.TL;
                strcpy(blocoDir->dir.vetArqDir[blocoDir->dir.TL], nome);
                blocoDir->dir.vetl_node[blocoDir->dir.TL] = contDir;
            }
            
            else
            {
                blocoDir->prox = criaBloco();
                blocoDir = blocoDir->prox;
                blocoDir->bloco = cont+1;
                TL = blocoDir->dir.TL;
                blocoDir->dir.vetl_node[blocoDir->dir.TL] = contDir;
                strcpy(blocoDir->dir.vetArqDir[blocoDir->dir.TL], nome);
            }
        }
        iNode *aux = NULL;
        iNode *nodes = (*desc).ArqDir->nodes;
        if(nodes == NULL)
            nodes = criaNode(contDir);
        else
        {
            aux = nodes;
            while(aux->prox != NULL)
                aux = aux->prox;
            
            aux->prox = criaNode(contDir);
        }
        
        listaGenArqDir *listagen = (*desc).ListGen->head;
        if(listagen == NULL)
            listagen = criaCaixagen(contDir, nodes);
        else
        {
            listaGenArqDir *auxgen = listagen;
            while(auxgen->tail != NULL)
                auxgen = auxgen->tail;
            
            auxgen->tail = criaCaixagen(contDir, aux);
        }
        
        (*desc).ArqDir->nodes = nodes;
        (*desc).ListGen->head = listagen;
        TL++;
        contDir++;
        blocoDir->dir.TL = TL;
    }
    else
    {
        char dirs[20];
        char finalDir[20];
        for(int i = 16, j = 0; path[i] != '$'; i++, j++)
            dirs[j] = path[i];
        
        Descritor aux = *desc;
        if(existeDir(aux, dirs))
        {
            int lenDirs = (int)strlen(dirs);
            int k = 0;
            
            for(int i = 0; i<lenDirs; i++)
                finalDir[i] = '\0';
            
            for(k = lenDirs; dirs[k] != '/'; k--);
            k++;
            for(int j = 0; dirs[k] != '\0'; k++, j++)
                finalDir[j] = dirs[k];
            
            
            pontBloco *bloco = buscaDirRaiz((*desc).ArqDir->diretorio_raiz, finalDir);
            
            if(bloco == NULL)
            {
                bloco = buscaArqDir((*desc).ArqDir->arquivos_diretorios, finalDir);
            }
            
            iNode *aux;
            buscaListagen((*desc).ListGen->head, i_node, &aux);
            
            pontBloco *novoDir = criaBloco();
            
            novoDir->bloco = contDir;
            strcpy(novoDir->dir.vetArqDir[novoDir->dir.TL], nome);
            novoDir->dir.vetl_node[novoDir->dir.TL] = contDir;
            novoDir->dir.TL++;
            novoDir->prox = (*desc).ArqDir->arquivos_diretorios;
            
            aux->vetBloco[aux->TL] = novoDir->bloco;
            aux->vetPontoBloco[aux->TL] = novoDir;
            aux->TL++;
            
            iNode *novoNode = criaNode(novoDir->bloco);
            listaGenArqDir *lista = (*desc).ListGen->head;
            novoNode->prox = (*desc).ArqDir->nodes;
            insereCaixagen(&lista, aux->i_node, novoNode);
            (*desc).ArqDir->nodes = novoNode;
            (*desc).ArqDir->arquivos_diretorios = novoDir;
            contDir++;
        }
    }
    
}


void criaArquivo(Descritor *desc, char absPath[], char nome[])
{
    char dirs[30], finalDir[15];
    Descritor aux = *desc;
    
    if(strcmp(absPath, "fulvio@fippnux:~$") != 0)
    {
        int j;
        for(j = (int)strlen(absPath)-2; absPath[j] != '/'; j--);
        
        for(int i = 0; i<30; i++)
            dirs[i] = '\0';
        
        for(int i = j, k = 0; absPath[i] != '$'; i++ , k++)
            dirs[k] = absPath[i];
        
        if(existeDir(aux, dirs))
        {
            int k;
            int lenDirs = (int)strlen(dirs);
            
            k = lenDirs;
            for(; dirs[k] != '/'; k--);
            
            k++;
            
            for(int i = 0; i<lenDirs; i++, k++)
                finalDir[i] = dirs[k];
            
            pontBloco *auxBloco;
            auxBloco = buscaDirRaiz(aux.ArqDir->diretorio_raiz, finalDir);
            if(auxBloco == NULL)
                auxBloco = buscaArqDir(aux.ArqDir->arquivos_diretorios, finalDir);
            
            iNode *auxNode = buscaNode(aux.ArqDir->nodes, i_node);
            
            iNode *novoNode = criaNode(contDir);
            
            pontBloco *blocoNode;
            
            if(auxNode->TL-1 == -1)
            {
                blocoNode = criaBloco();
                auxNode->vetPontoBloco[auxNode->TL] = blocoNode;
                auxNode->vetBloco[auxNode->TL] = blocoNode->bloco;
                auxNode->TL++;
                blocoNode->prox = (*desc).ArqDir->arquivos_diretorios;
                (*desc).ArqDir->arquivos_diretorios = blocoNode;
            }
            else
            {
                blocoNode = auxNode->vetPontoBloco[auxNode->TL-1];
            }
            
            strcpy(blocoNode->dir.vetArqDir[blocoNode->dir.TL], nome);
            blocoNode->dir.vetl_node[blocoNode->dir.TL] = novoNode->i_node;
            blocoNode->dir.TL++;
            
            listaGenArqDir * lista = (*desc).ListGen->head;
            insereCaixagen(&lista, auxNode->i_node, novoNode);
            
            pontBloco *arquivo = criaBloco();
            
            gets(arquivo->parteArquivo);
            
            arquivo->prox = aux.ArqDir->arquivos_diretorios;
            novoNode->vetPontoBloco[novoNode->TL] = arquivo;
            novoNode->TL++;
            novoNode->prox = aux.ArqDir->nodes;
            novoNode->tipo = '-';
            
            (*desc).ArqDir->arquivos_diretorios = arquivo;
            (*desc).ArqDir->nodes = novoNode;
            //contDir++;
        }
        else
            printf("Diretorio nao encontrado!\n");
    }
    else
        printf("Nao e possivel inserir arquivos na raiz\n");
}


char exibeArquivo(char nomeDir[], char nomeArq[], Descritor desc)
{
    pontBloco *aux = buscaArqDir(desc.ArqDir->arquivos_diretorios, nomeArq);
    if(aux == NULL)
        aux = buscaDirRaiz(desc.ArqDir->diretorio_raiz, nomeArq);
    
    iNode *auxNode = buscaNode(desc.ArqDir->nodes, i_node);
    
    if(auxNode != NULL)
    {
        for(int i = 0; i<auxNode->TL; i++)
        {
            pontBloco *arquivo = auxNode->vetPontoBloco[auxNode->TL-1];
            if(arquivo != NULL)
            {
                printf("%s", arquivo->parteArquivo);
                printf("\n");
                return 1;
            }
        }
    }
    return 0;
}


void listaDir(Descritor desc)
{
    pontBloco *blocoDir = desc.ArqDir->diretorio_raiz;
    pontBloco *arqDir = desc.ArqDir->arquivos_diretorios;
    //iNode *nodes = desc.ArqDir->nodes;
    printf("Diretorios raiz\n");
    while(blocoDir != NULL)
    {
        for(int i = 0; i<blocoDir->dir.TL; i++)
        {
            if(strcmp(blocoDir->dir.vetArqDir[i], ".") != 0)
                if(strcmp(blocoDir->dir.vetArqDir[i], "..") != 0)
                {
                    printf("%s - ", blocoDir->dir.vetArqDir[i]);
                    //                    printf("%s | ", nodes->data);
                    //                    nodes = nodes->prox;
                }
        }
        blocoDir = blocoDir->prox;
    }
    printf("\nOutros diretorios/Arquivos\n");
    while(arqDir != NULL)
    {
        for(int i = 0; i<arqDir->dir.TL; i++)
        {
            printf("%s, ", arqDir->dir.vetArqDir[i]);
        }
        arqDir = arqDir->prox;
    }
}

void execShell()
{
    Descritor desc = inicializaShell();
    int len;
    char op[50], nome[20], nomeArq[20];
    int opShell = 0;;
    char raiz[18];
    char caminho[30];
    
    strcpy(raiz, "fulvio@fippnux:~$");
    printf("%s", raiz);
    gets(op);
    
    while(strcmp(op, "exit") != 0)
    {
        if(strncmp(op, "cal", 3) == 0)
            opShell = 2;
        else
        if(strcmp(op, "clear") == 0)
            opShell = 12;
        else
        if(strncmp(op, "mkdir", 5) == 0)
            opShell = 6;
        else
        if(strncmp(op, "ls", 2)== 0)
            opShell = 3;
        else
        if(strncmp(op, "cd", 2) == 0)
            opShell = 4;
        else
        if(strncmp(op, "cat >", 5) == 0)
            opShell = 8;
        else
        if(strncmp(op, "cat <", 5) == 0)
            opShell = 9;
        else
            opShell = 10;
        for(int i = 0; i<30; i++)
            caminho[i] = '\0';
        
        len = (int)strlen(op);
        switch(opShell)
        {
            case 2:
                exibeCalendario(op);
                break;
                
            case 12:
                system("clear");
                break;
                
            case 6:
                for(int i = 6, j = 0; i < len; i++, j++)
                    nome[j] = op[i];
                
                criaDiretorio(&desc, nome, raiz);
                
                for(int i = 0; i<20; i++)
                    nome[i] = '\0';
                break;
                
            case 3:
                listaDir(desc);
                printf("\n");
                break;
                
            case 4:
                char absPath[30];
                
                for(int i = 3, j = 0; op[i] != '\0'; i++, j++)
                    absPath[j] = op[i];
                
                if(strcmp(absPath, "..") == 0)
                {
                    int raizLen = (int)strlen(raiz);
                    
                    int i = raizLen;
                    for( ;raiz[i] != '/'; i--)
                        raiz[i] = '\0';
                    
                    raiz[i] = '$';
                }
                else
                {
                    char auxp[30];
                    strcpy(auxp, absPath);
                    if(existeDir(desc, auxp))
                    {
                        if(strcmp(raiz, "fulvio@fippnux:~$") != 0)
                            strcpy(raiz, "fulvio@fippnux:~$");
                        raiz[strlen(raiz)-1] = '\0';
                        absPath[strlen(absPath)] = '$';
                        strcpy(caminho, absPath);
                    }
                    else
                        printf("Diretorio nao encontrado!\n");
                    
                    for(int i = 0; i<30; i++)
                    {
                        absPath[i] = '\0';
                        auxp[i] = '\0';
                    }
                }
                break;
                
            case 8:
                for(int i = 6, j = 0; i < len; i++, j++)
                    nomeArq[j] = op[i];
                
                criaArquivo(&desc, raiz, nomeArq);
                
                for(int i = 0; i<20; i++)
                    nomeArq[i] = '\0';
                
                break;
                
            case 9:
                for(int i = 6, j = 0; i < len; i++, j++)
                    nome[j] = op[i];
                 if(strcmp(raiz, "fulvio@fippnux:~$") != 0)
                 {
                     char diret[20];
                     for(int i = 0, j = RAIZ; raiz[j] != '$'; j++, i++)
                         diret[i] = raiz[j];
                     
                     if(exibeArquivo(diret, nome, desc) == 0)
                     {
                         printf("Arquivo nao encontrado!\n");
                     }
                 }
                else
                    printf("Arquivo nao encontrado!\n");
            break;
               
            case 10:
            break;
        }
        strcat(raiz,  caminho);
        printf("%s", raiz);
        gets(op);
    }
}



int main()
{
    execShell();
    return 0;
}

