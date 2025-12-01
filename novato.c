#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sala {
    char nome[50];
    struct sala *esq;
    struct sala *dir;
} Sala;

Sala* criarSala(char *nome) {
    Sala *s = (Sala*) malloc(sizeof(Sala));
    strcpy(s->nome, nome);
    s->esq = NULL;
    s->dir = NULL;
    return s;
}

void explorarSalas(Sala *atual) {
    char op;
    while (atual != NULL) {
        printf("\nVoce esta em: %s\n", atual->nome);

        if (atual->esq == NULL && atual->dir == NULL) {
            printf("Nao ha mais caminhos. Fim da exploracao.\n");
            return;
        }

        printf("Escolha (e = esquerda, d = direita, s = sair): ");
        scanf(" %c", &op);

        if (op == 'e') atual = atual->esq;
        else if (op == 'd') atual = atual->dir;
        else if (op == 's') return;
        else printf("Opcao invalida.\n");
    }
}

int main() {
    Sala *hall = criarSala("Hall de Entrada");
    Sala *salaEstar = criarSala("Sala de Estar");
    Sala *cozinha = criarSala("Cozinha");
    Sala *jardim = criarSala("Jardim");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *porao = criarSala("Porao");

    hall->esq = salaEstar;
    hall->dir = cozinha;

    salaEstar->esq = jardim;
    salaEstar->dir = biblioteca;

    cozinha->dir = porao;

    explorarSalas(hall);

    return 0;
}
