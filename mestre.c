#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 101

typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esq;
    struct Sala *dir;
} Sala;

typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esq;
    struct PistaNode *dir;
} PistaNode;

typedef struct HashNode {
    char chave[100];
    char suspeito[50];
    struct HashNode *prox;
} HashNode;

Sala* criarSala(char *nome, char *pista) {
    Sala *s = (Sala*) malloc(sizeof(Sala));
    strcpy(s->nome, nome);
    if(pista) strcpy(s->pista, pista); else s->pista[0] = '\0';
    s->esq = s->dir = NULL;
    return s;
}

PistaNode* novoPistaNode(char *p) {
    PistaNode *n = (PistaNode*) malloc(sizeof(PistaNode));
    strcpy(n->pista, p);
    n->esq = n->dir = NULL;
    return n;
}

PistaNode* inserirPistaBST(PistaNode *r, char *p) {
    if(r == NULL) return novoPistaNode(p);
    if(strcmp(p, r->pista) < 0) r->esq = inserirPistaBST(r->esq, p);
    else r->dir = inserirPistaBST(r->dir, p);
    return r;
}

void exibirPistasInOrder(PistaNode *r) {
    if(r == NULL) return;
    exibirPistasInOrder(r->esq);
    printf("- %s\n", r->pista);
    exibirPistasInOrder(r->dir);
}

unsigned int hashFunc(char *s) {
    unsigned int h = 0;
    while(*s) h = (h * 31) + (unsigned char)(*s++);
    return h % HASH_SIZE;
}

void inserirHash(HashNode *tabela[], char *chave, char *suspeito) {
    unsigned int h = hashFunc(chave);
    HashNode *n = (HashNode*) malloc(sizeof(HashNode));
    strcpy(n->chave, chave);
    strcpy(n->suspeito, suspeito);
    n->prox = tabela[h];
    tabela[h] = n;
}

char* encontrarSuspeito(HashNode *tabela[], char *chave) {
    unsigned int h = hashFunc(chave);
    HashNode *cur = tabela[h];
    while(cur) {
        if(strcmp(cur->chave, chave) == 0) return cur->suspeito;
        cur = cur->prox;
    }
    return NULL;
}

PistaNode* explorarSalas(Sala *atual, PistaNode *bst, HashNode *tabela[]) {
    char op;
    while(atual != NULL) {
        printf("\nVoce esta em: %s\n", atual->nome);
        if(strlen(atual->pista) > 0) {
            printf("Pista encontrada: %s\n", atual->pista);
            bst = inserirPistaBST(bst, atual->pista);
        } else {
            printf("Nenhuma pista neste local.\n");
        }
        printf("Escolha (e = esquerda, d = direita, s = sair): ");
        if(scanf(" %c", &op) != 1) { op = 's'; }
        if(op == 'e') atual = atual->esq;
        else if(op == 'd') atual = atual->dir;
        else if(op == 's') return bst;
        else printf("Opcao invalida.\n");
    }
    return bst;
}

int contarPistasPorSuspeito(PistaNode *r, HashNode *tabela[], char *acusado) {
    if(r == NULL) return 0;
    int cnt = 0;
    cnt += contarPistasPorSuspeito(r->esq, tabela, acusado);
    char *s = encontrarSuspeito(tabela, r->pista);
    if(s != NULL && strcmp(s, acusado) == 0) cnt++;
    cnt += contarPistasPorSuspeito(r->dir, tabela, acusado);
    return cnt;
}

int main() {
    Sala *hall = criarSala("Hall de Entrada", "Pegada de lama");
    Sala *salaEstar = criarSala("Sala de Estar", "Copo quebrado");
    Sala *cozinha = criarSala("Cozinha", "Marca de faca");
    Sala *jardim = criarSala("Jardim", "Ramo de flor arrancado");
    Sala *biblioteca = criarSala("Biblioteca", "Livro rasgado");
    Sala *escritorio = criarSala("Escritorio", "Bilhete ameaçador");
    Sala *porao = criarSala("Porao", "Corda cortada");

    hall->esq = salaEstar;
    hall->dir = cozinha;
    salaEstar->esq = jardim;
    salaEstar->dir = biblioteca;
    cozinha->esq = escritorio;
    cozinha->dir = porao;

    HashNode *tabela[HASH_SIZE];
    for(int i=0;i<HASH_SIZE;i++) tabela[i] = NULL;

    inserirHash(tabela, "Pegada de lama", "Sr. Silva");
    inserirHash(tabela, "Copo quebrado", "Sra. Almeida");
    inserirHash(tabela, "Marca de faca", "Sr. Silva");
    inserirHash(tabela, "Ramo de flor arrancado", "Srta. Costa");
    inserirHash(tabela, "Livro rasgado", "Sra. Almeida");
    inserirHash(tabela, "Bilhete ameaçador", "Sr. Mendes");
    inserirHash(tabela, "Corda cortada", "Srta. Costa");

    PistaNode *bst = NULL;
    bst = explorarSalas(hall, bst, tabela);

    printf("\nPistas coletadas (ordem alfabetica):\n");
    exibirPistasInOrder(bst);

    char acusado[50];
    printf("\nQuem voce acusa? (nome exato do suspeito): ");
    scanf(" %49[^\n]", acusado);

    int ocorrencias = contarPistasPorSuspeito(bst, tabela, acusado);

    printf("\nPistas que apontam para %s: %d\n", acusado, ocorrencias);
    if(ocorrencias >= 2) {
        printf("Conclusao: Ha pistas suficientes. Acusacao valida. Caso encerrado.\n");
    } else {
        printf("Conclusao: Nao ha pistas suficientes para sustentar a acusacao.\n");
    }

    return 0;
}
