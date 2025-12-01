#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sala {
    char nome[50];
    char pista[100];
    struct sala *esq;
    struct sala *dir;
} Sala;

typedef struct pistaNode {
    char pista[100];
    struct pistaNode *esq;
    struct pistaNode *dir;
} PistaNode;

Sala* criarSala(char *nome, char *pista) {
    Sala *s = (Sala*) malloc(sizeof(Sala));
    strcpy(s->nome, nome);
    if(pista) strcpy(s->pista, pista);
    else strcpy(s->pista, "");
    s->esq = NULL;
    s->dir = NULL;
    return s;
}

PistaNode* novaPista(char *p) {
    PistaNode *n = (PistaNode*) malloc(sizeof(PistaNode));
    strcpy(n->pista, p);
    n->esq = NULL;
    n->dir = NULL;
    return n;
}

PistaNode* inserirPista(PistaNode *raiz, char *p) {
    if(raiz == NULL) return novaPista(p);
    if(strcmp(p, raiz->pista) < 0) raiz->esq = inserirPista(raiz->esq, p);
    else raiz->dir = inserirPista(raiz->dir, p);
    return raiz;
}

void exibirPistas(PistaNode *r) {
    if(r == NULL) return;
    exibirPistas(r->esq);
    printf("- %s\n", r->pista);
    exibirPistas(r->dir);
}

PistaNode* explorarSalasComPistas(Sala *atual, PistaNode *bst) {
    char op;
    while(atual != NULL) {
        printf("\nVoce esta em: %s\n", atual->nome);

        if(strlen(atual->pista) > 0) {
            printf("Pista encontrada: %s\n", atual->pista);
            bst = inserirPista(bst, atual->pista);
        }

        printf("Escolha (e = esquerda, d = direita, s = sair): ");
        scanf(" %c", &op);

        if(op == 'e') atual = atual->esq;
        else if(op == 'd') atual = atual->dir;
        else if(op == 's') return bst;
        else printf("Opcao invalida.\n");
    }
    return bst;
}

int main() {
    Sala *hall = criarSala("Hall de Entrada", "Pegada de bota");
    Sala *salaEstar = criarSala("Sala de Estar", "Copo quebrado");
    Sala *cozinha = criarSala("Cozinha", "");
    Sala *jardim = criarSala("Jardim", "Ramo de flor arrancado");
    Sala *biblioteca = criarSala("Biblioteca", "Livro fora do lugar");
    Sala *porao = criarSala("Porao", "Corda cortada");

    hall->esq = salaEstar;
    hall->dir = cozinha;

    salaEstar->esq = jardim;
    salaEstar->dir = biblioteca;

    cozinha->dir = porao;

    PistaNode *bst = NULL;
    bst = explorarSalasComPistas(hall, bst);

    printf("\nPistas coletadas (ordem alfabetica):\n");
    exibirPistas(bst);

    return 0;
}
