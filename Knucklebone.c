#include <stdio.h>

#include <stdlib.h>

#include <time.h>

#include <unistd.h>



#define QTD_LINHAS 3

#define QTD_COLUNAS 3

#define LADOS_DADO 6

#define JOGADAS_MAX (QTD_LINHAS * QTD_COLUNAS)

typedef struct {

    int tabuleiro[QTD_LINHAS][QTD_COLUNAS]; // Tabuleiro do jogador

    int pontuacao;

} Jogador;

typedef struct {

    Jogador jogadores[2]; // Jogadores

} Jogo;

void inicializaJogo(Jogo *jogo) {

    for (int jogador = 0; jogador < 2; jogador++) {

        for (int linha = 0; linha < QTD_LINHAS; linha++) {

            for (int coluna = 0; coluna < QTD_COLUNAS; coluna++) {

                jogo->jogadores[jogador].tabuleiro[linha][coluna] = 0;

            }

        }

        jogo->jogadores[jogador].pontuacao = 0;

    }

}

int rolarDado() {

    return rand() % LADOS_DADO + 1;

}

void imprimirTabuleiro(Jogo *jogo, int jogador) {

    printf("Tabuleiro do Jogador %d:\n", jogador + 1);

    for (int linha = 0; linha < QTD_LINHAS; linha++) {

        for (int coluna = 0; coluna < QTD_COLUNAS; coluna++) {

            if (jogo->jogadores[jogador].tabuleiro[linha][coluna] == 0) {

                printf("_ ");

            } else {

                printf("%d ", jogo->jogadores[jogador].tabuleiro[linha][coluna]);

            }

        }

        printf("\n");

    }

    printf("Pontuação: %d\n\n", jogo->jogadores[jogador].pontuacao);

}

int jogadaBot(int tabuleiro[QTD_LINHAS][QTD_COLUNAS]) {

    int linha, coluna;

    do {

        linha = rand() % QTD_LINHAS;

        coluna = rand() % QTD_COLUNAS;

    } while (tabuleiro[linha][coluna] != 0);

    return linha * QTD_COLUNAS + coluna;

}

int main() {

    srand(time(NULL));

    int jogador1_para_jogador2[2];

    int jogador2_para_jogador1[2];

    if (pipe(jogador1_para_jogador2) == -1 || pipe(jogador2_para_jogador1) == -1) {

        perror("pipe");

        exit(EXIT_FAILURE);

    }

    pid_t pid = fork();

    if (pid == -1) {

        perror("fork");

        exit(EXIT_FAILURE);

    }

    if (pid == 0) { // Processo filho (Jogador 2)

        close(jogador1_para_jogador2[1]); // Fecha a escrita no pipe jogador1_para_jogador2

        close(jogador2_para_jogador1[0]); // Fecha a leitura do pipe jogador2_para_jogador1

        Jogo jogo;

        inicializaJogo(&jogo);

        for (int turno = 0; turno < JOGADAS_MAX; turno++) {

            read(jogador1_para_jogador2[0], &jogo, sizeof(Jogo)); // Lê o estado do jogo do pipe

            int valorDado = rolarDado();

            printf("Jogador 2 rolou um %d\n", valorDado);

            int posicao = jogadaBot(jogo.jogadores[1].tabuleiro);

            int linha = posicao / QTD_COLUNAS;

            int coluna = posicao % QTD_COLUNAS;

            printf("Jogador 2 escolheu a posição (%d, %d)\n", linha + 1, coluna + 1);

            jogo.jogadores[1].tabuleiro[linha][coluna] = valorDado;

            jogo.jogadores[1].pontuacao += valorDado;

            imprimirTabuleiro(&jogo, 1);

            write(jogador2_para_jogador1[1], &jogo, sizeof(Jogo)); // Envia o estado atualizado do jogo para o jogador 1

        }

        close(jogador1_para_jogador2[0]);

        close(jogador2_para_jogador1[1]);

   if (jogo.jogadores[0].pontuacao > jogo.jogadores[1].pontuacao) {

            printf("Jogador 1 vence!\n");

        } else if (jogo.jogadores[0].pontuacao < jogo.jogadores[1].pontuacao) {

            printf("Jogador 2 vence!\n");

        } else {

            printf("Empate!\n");

        }

    } else { // Processo pai (Jogador 1)

        close(jogador1_para_jogador2[0]); // Fecha a leitura do pipe jogador1_para_jogador2

        close(jogador2_para_jogador1[1]); // Fecha a escrita no pipe jogador2_para_jogador1

        Jogo jogo;

        inicializaJogo(&jogo);

        for (int turno = 0; turno < JOGADAS_MAX; turno++) {

            int valorDado = rolarDado();

            printf("Jogador 1 rolou um %d\n", valorDado);

            int linha, coluna;

            do {

                printf("Jogador 1, escolha uma linha (1-%d) e coluna (1-%d): ", QTD_LINHAS, QTD_COLUNAS);

                scanf("%d %d", &linha, &coluna);

                linha--; // Convertendo para índice 0-based

                coluna--;

            } while (linha < 0 || linha >= QTD_LINHAS || coluna < 0 || coluna >= QTD_COLUNAS || jogo.jogadores[0].tabuleiro[linha][coluna] != 0);

            printf("Jogador 1 escolheu a posição (%d, %d)\n", linha + 1, coluna + 1);

            jogo.jogadores[0].tabuleiro[linha][coluna] = valorDado;

            jogo.jogadores[0].pontuacao += valorDado;

            imprimirTabuleiro(&jogo, 0);


            write(jogador1_para_jogador2[1], &jogo, sizeof(Jogo)); // Envia o estado atualizado do jogo para o jogador 2


            read(jogador2_para_jogador1[0], &jogo, sizeof(Jogo)); // Lê o estado do jogo do jogador 2

        }

        close(jogador1_para_jogador2[1]);

        close(jogador2_para_jogador1[0]);

    }

    return 0;

}

