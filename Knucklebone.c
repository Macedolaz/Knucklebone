#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define QTD_LINHAS 3
#define QTD_COLUNAS 3
#define LADOS_DADO 6
#define JOGADAS_MAX (QTD_LINHAS * QTD_COLUNAS)

typedef struct {
    int tabuleiro[2][QTD_LINHAS][QTD_COLUNAS]; // Tabuleiro dos jogadores
    int pontuacao[2]; // Pontuação dos jogadores
} Jogo;

void inicializaJogo(Jogo *jogo) {
    for (int jogador = 0; jogador < 2; jogador++) {
        for (int linha = 0; linha < QTD_LINHAS; linha++) {
            for (int coluna = 0; coluna < QTD_COLUNAS; coluna++) {
                jogo->tabuleiro[jogador][linha][coluna] = 0;
            }
        }
        jogo->pontuacao[jogador] = 0;
    }
}

int rolarDado() {
    return rand() % LADOS_DADO + 1;
}

void colocarDado(Jogo *jogo, int jogador, int linha, int coluna, int valorDado) {
    jogo->tabuleiro[jogador][linha][coluna] = valorDado;

    jogo->pontuacao[jogador] = 0;
    for (int l = 0; l < QTD_LINHAS; l++) {
        for (int c = 0; c < QTD_COLUNAS; c++) {
            if (jogo->tabuleiro[jogador][l][c] != 0) {
                jogo->pontuacao[jogador] += jogo->tabuleiro[jogador][l][c];
            }
        }
    }
}

void imprimirTabuleiro(Jogo *jogo) {
    for (int jogador = 0; jogador < 2; jogador++) {
        printf("Tabuleiro do Jogador %d:\n", jogador + 1);
        for (int linha = 0; linha < QTD_LINHAS; linha++) {
            for (int coluna = 0; coluna < QTD_COLUNAS; coluna++) {
                if (jogo->tabuleiro[jogador][linha][coluna] == 0) {
                    printf("_ ");
                } else {
                    printf("%d ", jogo->tabuleiro[jogador][linha][coluna]);
                }
            }
            printf("\n");
        }
        printf("Pontuação: %d\n\n", jogo->pontuacao[jogador]);
    }
}

int main() {
    srand(time(NULL));

    Jogo jogo;
    inicializaJogo(&jogo);

    int jogadorAtual = 0;
    for (int turno = 0; turno < 18; turno++) {
        int valorDado = rolarDado();
        printf("Jogador %d rolou um %d\n", jogadorAtual + 1, valorDado);
        
        int linha, coluna;
        do {
            printf("Jogador %d, escolha uma linha (1-%d) e coluna (1-%d): ", jogadorAtual + 1, QTD_LINHAS, QTD_COLUNAS);
            scanf("%d %d", &linha, &coluna);
        } while (linha < 1 || linha > QTD_LINHAS || coluna < 1 || coluna > QTD_COLUNAS || jogo.tabuleiro[jogadorAtual][linha-1][coluna-1] != 0);
        
        colocarDado(&jogo, jogadorAtual, linha - 1, coluna - 1, valorDado);
        imprimirTabuleiro(&jogo);

        jogadorAtual = 1 - jogadorAtual;
    }

    if (jogo.pontuacao[0] > jogo.pontuacao[1]) {
        printf("Jogador 1 vence!\n");
    } else if (jogo.pontuacao[0] < jogo.pontuacao[1]) {
        printf("Jogador 2 vence!\n");
    } else {
        printf("Empate!\n");
    }

    return 0;
}
