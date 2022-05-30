/*
 * UFC - Universidade Federal do Ceará
 *
 * Aluno: Mateus Fonseca Lima
 * Matrícula: 471855
 * Disciplina: Estruturas de dados avançadas
 * Horário: 08:00 às 10:00
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct no{
    int chave;
    struct no *esq;
    struct no *dir;
    int bal;
} NO;

#define VERDADEIRO 1
#define FALSO 0

// Testes.
void realizacaoDosTestes();
void testeUnitario();

// Funções solicitadas.
void inserirAvl(int x, NO** pt, int* h);
void removerAVL(int x, NO** pt, int* h);
int isAvl(NO* pt);
int alturaAvl(NO *pt);
int quantNoAvl(NO* pt);

// Funções de rotação.
void balancear(NO** pt, char r, int* h);
void caso1(NO** pt, int* h);
void caso2(NO** pt, int* h);
void caso1R(NO** pt, int* h);
void caso2R(NO** pt, int* h);

// Funções utilitárias.
int chaveAleatoria(int valorMaximo);
NO* buscarElemento(int x, NO* pt);
void iniciarNo(NO** avl, int elemento);
void trocar(NO** a, NO** b);
void printar(NO* pt);
void preOrdem(NO* pt);

// MAIN --------------------------------------------------------------------------

int main(int argc, char *argv[]){
    srand(time(NULL));

    realizacaoDosTestes();
    // testeUnitario();

    return 0;
}

// TESTES ------------------------------------------------------------------------

void realizacaoDosTestes(){
    int totalAvls = 1000;
    int totalDeNos = 10000;
    int valorMaximoChaveAleatoria = 100000;
    int totalDeNosParaRemover = 1000;
    int i,j,h, novoValor;

    // Armazena todos os valores dos nós inseridos, para posteriormente remover 1000 destes.
    int valoresInseridos[totalDeNos];

    for(i = 1; i <= totalAvls; i++){
        NO* avl = NULL;

        printf("\n--------ARVORE %d INICIADA--------\n\n", i);
        
        for(j = 0; j < totalDeNos; j++){
            novoValor = chaveAleatoria(valorMaximoChaveAleatoria);
            if(buscarElemento(novoValor, avl) == NULL){
                valoresInseridos[j] = novoValor;
                inserirAvl(novoValor, &avl, &h);
            }else{
                j--; // Repete o for.
            }
        }

        printar(avl);

        while(quantNoAvl(avl) != totalDeNos-totalDeNosParaRemover){
            removerAVL(valoresInseridos[--j], &avl, &h);
        }

        printar(avl);
    }
}

void testeUnitario(){
    int totalDeNos = 10;
    int valorMaximoChaveAleatoria = 100;
    int totalDeNosParaRemover = 3;
    int j,h, novoValor;

    // Armazena todos os valores dos nós inseridos, para posteriormente remover 1000 destes.
    int valoresInseridos[totalDeNos];

    NO* avl = NULL;

    printf("\n--------ARVORE INICIADA--------\n\n");
    
    for(j = 0; j < totalDeNos; j++){
        novoValor = chaveAleatoria(valorMaximoChaveAleatoria);
        if(buscarElemento(novoValor, avl) == NULL){
            valoresInseridos[j] = novoValor;
            inserirAvl(novoValor, &avl, &h);
        }else{
            j--; // Repete o for.
        }
    }

    printar(avl);
    printf("\n");
    preOrdem(avl);
    printf("\n");

    while(quantNoAvl(avl) != totalDeNos-totalDeNosParaRemover){
        removerAVL(valoresInseridos[--j], &avl, &h);
    }

    printar(avl);
    printf("\n");
    preOrdem(avl);
}

// SOLICITADAS -------------------------------------------------------------------

// OK: Função usada para inserir um novo elemento na árvore.
void inserirAvl(int x, NO** pt, int* h){
    if (*pt == NULL) {
        iniciarNo(pt, x);
        (*h) = VERDADEIRO;
    }else{        
        if (x < (*pt)->chave){
            inserirAvl(x, &(*pt)->esq, h);
            if((*h) == VERDADEIRO){
                switch((*pt)->bal){
                    case 1: (*pt)->bal = 0; (*h) = FALSO; break;
                    case 0: (*pt)->bal = -1; break;
                    case -1: caso1(pt, h); break;
                    default: break;
                }
            }
        } else{
            inserirAvl(x, &(*pt)->dir, h);
            if((*h) == VERDADEIRO){
                switch((*pt)->bal){
                    case -1: (*pt)->bal = 0; (*h)= FALSO; break;
                    case 0: (*pt)->bal = 1; break;
                    case 1: caso2(pt, h); break;
                    default: break;
                }
            }  
        }
    }
}

// OK: Função usada para remover.
void removerAVL(int x, NO** pt, int* altura){
    NO** ptRaiz= pt; 
    if ((*ptRaiz) == NULL) { 
        printf("\nELEMENTO %d NÃO EXISTE\n", x);
        (*altura) = FALSO;
        return;
    }else{
        if (x < (*ptRaiz)->chave) {
            removerAVL(x, &(*ptRaiz)->esq, altura);
            balancear(&(*ptRaiz), 'E', altura);
        } else {
            if (x > (*ptRaiz)->chave){
                removerAVL(x, &(*ptRaiz)->dir, altura);
                balancear(&(*ptRaiz), 'D', altura);
            } else {
                NO* aux = (*ptRaiz);
                if((*ptRaiz)->esq == NULL){
                    (*ptRaiz) = (*ptRaiz)->dir;
                    (*altura) = VERDADEIRO;
                }
                else{
                    if((*ptRaiz)->dir == NULL){
                        (*ptRaiz) = (*ptRaiz)->esq;
                        (*altura) = VERDADEIRO;
                    }
                    else{
                        NO* s= (*ptRaiz)->dir;
                        if (s->esq == NULL) {
                            s->esq = (*ptRaiz)->esq;
                            s->bal = (*ptRaiz)->bal;
                            (*ptRaiz) = s;
                            (*altura) = VERDADEIRO;
                        } else {
                            NO* paiS=s;
                            while(s->esq != NULL){
                                paiS = s;
                                s = s->esq; 
                            }
                            trocar(ptRaiz, &(paiS->esq));
                            removerAVL(x, &(*ptRaiz)->dir, altura);
                        }
                        balancear(ptRaiz, 'D', altura);
                    }
                }
                free(aux);
            }
        }
    }
}

// OK: Informa se uma árvore é AVL.
int isAvl(NO* pt){
    NO* aux = pt;
    
    if (aux != NULL){
        int arvEsq = isAvl(aux->esq);
        int arvDir = isAvl(aux->dir);

        if(arvEsq == FALSO || arvDir == FALSO){
            return FALSO;
        }

        int result = (alturaAvl(pt->dir) - alturaAvl(pt->esq));
        int isAvl = result >= -1 && result <= 1 ? VERDADEIRO : FALSO;
        
        if (result != pt->bal) {
            printf("BALANCEAMENTO DO ELEMENTO %d ESTÁ INCORRETO: \n\tBALANCEAMENTO ATUAL = %d \n\tBALANCEAMENTO CORRETO = %d\n\n", pt->chave, pt->bal, result);
            return  FALSO;
        }

        return isAvl;
    } 

    return VERDADEIRO;
}

// OK: Calcula recursivamente a altura de uma AVL.
int alturaAvl(NO *pt)
{
    if (!pt){
        return 0;
    }

    int alturaEsquerda = alturaAvl(pt->esq);
    int alturaDireita = alturaAvl(pt->dir);

    return (alturaEsquerda > alturaDireita) ? alturaEsquerda + 1 : alturaDireita + 1;
}

// OK: Calcula recursivamente a quantidade de nós de uma AVL.
int quantNoAvl(NO* pt){
    if(pt == NULL){
        return 0;
    }
    
    return 1 + quantNoAvl(pt->esq) + quantNoAvl(pt->dir);
}

// ROTAÇÕES ----------------------------------------------------------------------

// OK: Função usada para balancear a árvore.
void balancear(NO** pt, char rotacao, int* altura)  {
    if ((*altura) == VERDADEIRO) {
        if (rotacao == 'D') {
            switch((*pt)->bal){
                case 1: (*pt)->bal = 0; break;
                case 0: (*pt)->bal = -1; (*altura) = FALSO; break;
                case -1: caso1R(&(*pt), altura); break;
                default: break;
            }
        } else {
            switch ((*pt)->bal){
                case -1:(*pt)->bal = 0; break;
                case 0:(*pt)->bal = 1; (*altura) = FALSO; break;
                case 1: caso2R(&(*pt), altura); break;
                default: break;
            }
        }
    }
}

// OK: Rotação simples e dupla a direita.
void caso1(NO** pt, int* h){
   NO* ptu = (*pt)->esq;
   
   if(ptu->bal == -1){
        (*pt)->esq = ptu->dir;
        ptu->dir = *pt;
        (*pt)->bal = 0; 
        *pt = ptu;
   }else{
        NO* ptv = ptu->dir;
        ptu->dir = ptv->esq;
        ptv->esq = ptu;
        (*pt)->esq = ptv->dir;
        ptv->dir = *pt;

        if (ptv->bal == -1){
            (*pt)->bal = 1;
        }else{
            (*pt)->bal = 0;
        }

        if (ptv->bal == 1){
            ptu->bal = -1;
        }else{
            ptu->bal = 0;
        }

        *pt = ptv;
   }
   (*pt)->bal = 0;
   (*h) = FALSO;
}

// OK: Rotação simples e dupla a esquerda.
void caso2(NO** pt, int* h){
    NO* ptu = (*pt)->dir;

   if(ptu->bal == 1){
       (*pt)->dir = ptu->esq;
       ptu->esq = *pt;
       (*pt)->bal = 0;
       (*pt) = ptu;
   }else{
       NO* ptv = ptu->esq;
       ptu->esq = ptv->dir;
       ptv->dir = ptu;
       (*pt)->dir = ptv->esq;
       ptv->esq = *pt;
       
       if (ptv->bal == 1){
           (*pt)->bal = -1;
       }else{
           (*pt)->bal = 0;
       }

       if(ptv->bal == -1){
           ptu->bal = 1;
       }else{
           ptu->bal = 0;
       }

       *pt = ptv;
   }
   (*pt)->bal =0;
   (*h) = FALSO;
}

// OK: Rotação simples e dupla a direita.
void caso1R(NO** pt, int* h){
    NO* ptu = (*pt)->esq;

    if (ptu->bal <= 0) {
        (*pt)->esq = ptu->dir;
        ptu->dir =(*pt);
        (*pt)= ptu;
        if (ptu->bal == -1) {
            ptu->bal =(*pt)->dir->bal = 0;         
            (*h) = VERDADEIRO;
        } else {
            ptu->bal = 1;
            (*pt)->dir->bal=-1;
            (*h) = FALSO;
        }
    } else {
        NO* ptv = ptu->dir;
        ptu->dir = ptv->esq;
        (*pt)->esq=ptv->dir;
        ptv->esq=ptu;
        ptv->dir=(*pt);
        (*pt)=ptv;

        switch (ptv->bal) {
            case -1: ptu->bal = 0; (*pt)->dir->bal = 1; break;
            case 0: ptu->bal = 0; (*pt)->dir->bal = 0; break;
            case 1: ptu->bal = -1; (*pt)->dir->bal=0; break;
            default: break;
        }
        (*pt)->bal = 0;
        (*h) = VERDADEIRO;
    }
}

// OK: Rotação simples e dupla a esquerda.
void caso2R(NO** pt, int* h){
    NO* ptu = (*pt)->dir;

    if (ptu->bal >= 0) {
        (*pt)->dir = ptu->esq;
        ptu->esq = (*pt);
        (*pt)= ptu;
        if (ptu->bal == 1) {
            ptu->bal=(*pt)->esq->bal = 0;
            (*h) = VERDADEIRO;
        } else {
            ptu->bal = -1;
            (*pt)->esq->bal=1;
            (*h) = FALSO;
        }
    } else {
        NO* ptv = ptu->esq;
        ptu->esq = ptv->dir;
        (*pt)->dir=ptv->esq;
        ptv->dir=ptu;
        ptv->esq=(*pt);
        (*pt)=ptv;

        switch (ptv->bal) {
            case 1: ptu->bal = 0; (*pt)->esq->bal = -1; break;
            case 0: ptu->bal = 0; (*pt)->esq->bal = 0; break;
            case -1: ptu->bal = 1; (*pt)->esq->bal= 0; break;
            default: break;
        }
        (*pt)->bal = 0;
        (*h) = VERDADEIRO;
    }
}

// UTILITÁRIAS -------------------------------------------------------------------

// OK: Gera uma chave aletória entre 0 e valorMaximo.
int chaveAleatoria(int valorMaximo){
    return (rand() % valorMaximo) + 1;
}

// OK: Verifica se um elemento x existe em uma árvore.
NO* buscarElemento(int x, NO* pt){
    while(pt != NULL && pt->chave != x){
        if(x > pt->chave){
            pt = pt->dir;
        }else{
            pt = pt->esq;
        }
    }

    return pt;
}

// OK: Inicializa um novo nó na árvore.
void iniciarNo(NO** pt, int elemento){
    NO* no = (NO*)malloc(sizeof(NO));
    no->chave = elemento;
    no->esq = no->dir = NULL;
    no->bal = 0;
    (*pt) = no;
}

// OK: Troca as posições de dois nó na árvore.
void trocar(NO** a, NO** b){
    int aux = (*a)->chave;
    (*a)->chave = (*b)->chave;
    (*b)->chave = aux;
}

// OK: Printa as informações da AVL.
void printar(NO* pt){
  printf("\nQUANTIDADE DE NÓS: %d\n", quantNoAvl(pt));
  printf("ALTURA DA AVL: %d\n", alturaAvl(pt));
  isAvl(pt) == VERDADEIRO ? printf("É AVL? sim\n") : printf("É AVL? não\n");
}

// OK: Imprime uma arvore no pré-ordem.
void preOrdem(NO* pt){
    printf("%d ", pt->chave);
    
    if(pt->esq != NULL)
        preOrdem(pt->esq);
    if(pt->dir != NULL)
        preOrdem(pt->dir);
}