#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Desafio Tetris Stack
// Tema 3 - Integração de Fila e Pilha
// Este código final implementa um sistema de controle de peças para o jogo Tetris Stack,
// utilizando fila circular para peças futuras e uma pilha linear para peças de reserva.

// --- Definição das Estruturas ---
#define FILA_CAPACIDADE 5
#define PILHA_CAPACIDADE 3

// Representa uma peça do jogo com seu tipo e um ID único
typedef struct {
    char tipo;
    int id;
} Peca;

// Estrutura para a fila circular de peças futuras
typedef struct {
    Peca pecas[FILA_CAPACIDADE];
    int frente;
    int tras;
    int tamanho;
} Fila;

// Estrutura para a pilha linear de peças de reserva
typedef struct {
    Peca pecas[PILHA_CAPACIDADE];
    int topo;
} Pilha;

// Variável global para gerar IDs sequenciais para as peças
int proximoId = 0;

// --- Funções Auxiliares ---

/**
 * @brief Gera uma nova peça com um tipo aleatório e um ID único.
 * @return Retorna a peça gerada.
 */
Peca gerarPeca() {
    Peca novaPeca;
    char tipos[] = {'I', 'O', 'T', 'L'};
    novaPeca.tipo = tipos[rand() % 4];
    novaPeca.id = proximoId++;
    return novaPeca;
}

/**
 * @brief Exibe o estado atual da fila de peças.
 * @param fila Ponteiro para a fila a ser exibida.
 */
void mostrarFila(Fila* fila) {
    printf("Fila de pecas: ");
    if (fila->tamanho == 0) {
        printf("[Vazia]\n");
        return;
    }
    printf("[");
    for (int i = 0; i < fila->tamanho; i++) {
        int indice = (fila->frente + i) % FILA_CAPACIDADE;
        printf("%c %d", fila->pecas[indice].tipo, fila->pecas[indice].id);
        if (i < fila->tamanho - 1) {
            printf("] [");
        }
    }
    printf("]\n");
}

/**
 * @brief Exibe o estado atual da pilha de reserva.
 * @param pilha Ponteiro para a pilha a ser exibida.
 */
void mostrarPilha(Pilha* pilha) {
    printf("Pilha de reserva (Topo -> Base): ");
    if (pilha->topo == -1) {
        printf("[Vazia]\n");
        return;
    }
    printf("[");
    for (int i = pilha->topo; i >= 0; i--) {
        printf("%c %d", pilha->pecas[i].tipo, pilha->pecas[i].id);
        if (i > 0) {
            printf("] [");
        }
    }
    printf("]\n");
}

// --- Funções da Fila Circular ---

/**
 * @brief Inicializa a fila.
 * @param fila Ponteiro para a fila a ser inicializada.
 */
void inicializarFila(Fila* fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->tamanho = 0;
}

/**
 * @brief Verifica se a fila está cheia.
 * @param fila Ponteiro para a fila.
 * @return Retorna 1 se estiver cheia, 0 caso contrário.
 */
int filaCheia(Fila* fila) {
    return fila->tamanho == FILA_CAPACIDADE;
}

/**
 * @brief Verifica se a fila está vazia.
 * @param fila Ponteiro para a fila.
 * @return Retorna 1 se estiver vazia, 0 caso contrário.
 */
int filaVazia(Fila* fila) {
    return fila->tamanho == 0;
}

/**
 * @brief Insere uma peça no final da fila.
 * @param fila Ponteiro para a fila.
 * @param peca A peça a ser inserida.
 */
void enqueue(Fila* fila, Peca peca) {
    if (filaCheia(fila)) {
        // Isso não deve ocorrer no fluxo principal, pois a fila é sempre mantida cheia
        // após uma remoção. No entanto, é uma boa prática de segurança.
        return;
    }
    fila->tras = (fila->tras + 1) % FILA_CAPACIDADE;
    fila->pecas[fila->tras] = peca;
    fila->tamanho++;
}

/**
 * @brief Remove a peça da frente da fila.
 * @param fila Ponteiro para a fila.
 * @return Retorna a peça removida.
 */
Peca dequeue(Fila* fila) {
    if (filaVazia(fila)) {
        Peca pecaVazia = {'\0', -1}; // Peça inválida para indicar erro
        return pecaVazia;
    }
    Peca pecaRemovida = fila->pecas[fila->frente];
    fila->frente = (fila->frente + 1) % FILA_CAPACIDADE;
    fila->tamanho--;
    return pecaRemovida;
}

// --- Funções da Pilha Linear ---

/**
 * @brief Inicializa a pilha.
 * @param pilha Ponteiro para a pilha a ser inicializada.
 */
void inicializarPilha(Pilha* pilha) {
    pilha->topo = -1;
}

/**
 * @brief Verifica se a pilha está cheia.
 * @param pilha Ponteiro para a pilha.
 * @return Retorna 1 se estiver cheia, 0 caso contrário.
 */
int pilhaCheia(Pilha* pilha) {
    return pilha->topo == PILHA_CAPACIDADE - 1;
}

/**
 * @brief Verifica se a pilha está vazia.
 * @param pilha Ponteiro para a pilha.
 * @return Retorna 1 se estiver vazia, 0 caso contrário.
 */
int pilhaVazia(Pilha* pilha) {
    return pilha->topo == -1;
}

/**
 * @brief Insere uma peça no topo da pilha.
 * @param pilha Ponteiro para a pilha.
 * @param peca A peça a ser inserida.
 */
void push(Pilha* pilha, Peca peca) {
    if (pilhaCheia(pilha)) {
        return;
    }
    pilha->pecas[++pilha->topo] = peca;
}

/**
 * @brief Remove a peça do topo da pilha.
 * @param pilha Ponteiro para a pilha.
 * @return Retorna a peça removida.
 */
Peca pop(Pilha* pilha) {
    if (pilhaVazia(pilha)) {
        Peca pecaVazia = {'\0', -1};
        return pecaVazia;
    }
    return pilha->pecas[pilha->topo--];
}

// --- Funções do Jogo ---

/**
 * @brief Troca a peça da frente da fila com a peça do topo da pilha.
 * @param fila Ponteiro para a fila.
 * @param pilha Ponteiro para a pilha.
 */
void trocarFilaComPilha(Fila* fila, Pilha* pilha) {
    if (filaVazia(fila) || pilhaVazia(pilha)) {
        printf("Nao e possivel realizar a troca. A fila ou a pilha estao vazias.\n");
        return;
    }
    
    Peca temp = fila->pecas[fila->frente];
    fila->pecas[fila->frente] = pilha->pecas[pilha->topo];
    pilha->pecas[pilha->topo] = temp;
    
    printf("Troca realizada entre a peca da frente da fila e o topo da pilha.\n");
}

/**
 * @brief Troca as 3 primeiras peças da fila com as 3 peças da pilha.
 * @param fila Ponteiro para a fila.
 * @param pilha Ponteiro para a pilha.
 */
void trocar3Com3(Fila* fila, Pilha* pilha) {
    if (pilha->topo != 2 || fila->tamanho < 3) {
        printf("Nao e possivel realizar a troca. A pilha nao tem 3 pecas ou a fila nao tem 3 pecas disponiveis.\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        Peca temp = fila->pecas[(fila->frente + i) % FILA_CAPACIDADE];
        fila->pecas[(fila->frente + i) % FILA_CAPACIDADE] = pilha->pecas[pilha->topo - i];
        pilha->pecas[pilha->topo - i] = temp;
    }

    printf("Troca realizada entre as 3 primeiras pecas da fila e as 3 da pilha.\n");
}


// --- Função Principal ---
int main() {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    Fila fila;
    Pilha pilha;
    int opcao;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche a fila com 5 peças iniciais
    for (int i = 0; i < FILA_CAPACIDADE; i++) {
        enqueue(&fila, gerarPeca());
    }

    // Loop principal do jogo
    do {
        printf("\n--- Estado Atual ---\n");
        mostrarFila(&fila);
        mostrarPilha(&pilha);
        printf("--------------------\n");
        printf("Opcoes disponiveis:\n");
        printf("1 - Jogar peca da frente da fila\n");
        printf("2 - Enviar peca da fila para a pilha de reserva\n");
        printf("3 - Usar peca da pilha de reserva\n");
        printf("4 - Trocar peca da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 pecas da pilha\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                if (!filaVazia(&fila)) {
                    Peca pecaJogada = dequeue(&fila);
                    printf("Peca [%c %d] jogada da fila.\n", pecaJogada.tipo, pecaJogada.id);
                    enqueue(&fila, gerarPeca());
                } else {
                    printf("A fila esta vazia. Nao ha pecas para jogar.\n");
                }
                break;
            }
            case 2: {
                if (!filaVazia(&fila) && !pilhaCheia(&pilha)) {
                    Peca pecaReservada = dequeue(&fila);
                    push(&pilha, pecaReservada);
                    printf("Peca [%c %d] enviada para a reserva.\n", pecaReservada.tipo, pecaReservada.id);
                    enqueue(&fila, gerarPeca());
                } else if (filaVazia(&fila)) {
                    printf("A fila esta vazia. Nao ha pecas para reservar.\n");
                } else {
                    printf("A pilha de reserva esta cheia. Nao e possivel reservar mais pecas.\n");
                }
                break;
            }
            case 3: {
                if (!pilhaVazia(&pilha)) {
                    Peca pecaUsada = pop(&pilha);
                    printf("Peca [%c %d] usada da reserva.\n", pecaUsada.tipo, pecaUsada.id);
                } else {
                    printf("A pilha de reserva esta vazia. Nao ha pecas para usar.\n");
                }
                break;
            }
            case 4: {
                trocarFilaComPilha(&fila, &pilha);
                break;
            }
            case 5: {
                trocar3Com3(&fila, &pilha);
                break;
            }
            case 0: {
                printf("Saindo do jogo. Adeus!\n");
                break;
            }
            default: {
                printf("Opcao invalida. Tente novamente.\n");
                break;
            }
        }
    } while (opcao != 0);

    return 0;
}
