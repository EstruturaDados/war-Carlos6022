#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// definição da estrutura Territorio
typedef struct
{
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// função para limpar o buffer de entrada
void limparBufferEntrada() {
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

// função para realizar o ataque entre dois territórios
void atacar(Territorio *atacante, Territorio *defensor) {
    // validação básica
    if(atacante == NULL || defensor == NULL) {
        return;
    }
    // condições de ataque
    if(atacante->tropas <= 1) {
        printf("Atacante não tem tropas suficientes para atacar (é necessário deixar 1 no território).\n");
        return;
    }
    // defensor deve ter tropas
    if(defensor->tropas <= 0) {
        printf("Defensor já não possui tropas.\n");
        return;
    }

    // rolagem de dados
    int atkRoll = rand() % 6 + 1;
    int defRoll = rand() % 6 + 1;

    // exibe resultados da rolagem
    printf("\n--- RESULTADO DA BATALHA ---\n");
    printf("\nO Atacante %s rolou um dado e tirou: %d\n", atacante->nome, atkRoll);
    printf("O Defensor %s rolou um dado e tirou: %d\n\n", defensor->nome, defRoll);

    // determina o vencedor
    if(atkRoll > defRoll) {
        // atacante vence: defensor é conquistado
        printf("VITÓRIA DO ATACANTE! %s foi conquistado por %s.\n", defensor->nome, atacante->nome);

        // transfere metade das tropas do atacante para o defensor
        int transferencia = atacante->tropas / 2; // metade das tropas do atacante
        // ajustes para garantir pelo menos 1 tropa transferida e deixar 1 no atacante
        if(transferencia < 1 && atacante->tropas > 1)
            transferencia = 1;
        // garante que o atacante deixe pelo menos 1 tropa
        if(atacante->tropas - transferencia < 1)
            transferencia = atacante->tropas - 1;
        // garante que a transferência não seja negativa
        if(transferencia < 0)
            transferencia = 0;
        // atualiza o defensor
        strncpy(defensor->cor, atacante->cor, sizeof(defensor->cor) - 1);
        defensor->cor[sizeof(defensor->cor) - 1] = '\0';
        defensor->tropas = transferencia;
        atacante->tropas -= transferencia;

        // exibe o resultado da transferência
        printf("%d tropas foram transferidas de %s para %s.\n\n", transferencia, atacante->nome, defensor->nome);
        printf("%s: %d tropas (Exército: %s)\n", defensor->nome, defensor->tropas, defensor->cor);
        printf("%s: %d tropas (Exército: %s)\n", atacante->nome, atacante->tropas, atacante->cor);
    } else {
        // atacante perde 1 tropa
        atacante->tropas--;
        printf("VITÓRIA DO DEFENSOR! %s perde 1 tropa (restam %d).\n", atacante->nome, atacante->tropas);
    }
}

// cadastra territórios usando ponteiros e alocação dinâmica
void cadastrarTerritorios(Territorio *mapa, int total) {
    // loop para cadastro
    for(int i = 0; i < total; i++) {
        printf("\n--- Cadastrando Territorio %d ---\n", i + 1);
        printf("Nome do Territorio: ");
        fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        printf("Cor do Exercito (ex: Azul, verde): ");
        fgets(mapa[i].cor, sizeof(mapa[i].cor), stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        printf("Numero de Tropas: ");
        scanf("%d", &mapa[i].tropas);
        limparBufferEntrada();
    }
}

// exibe o mapa do mundo
void exibirMapa(Territorio *mapa, int total) {
    printf("====================================\n");
    printf("    MAPA DO MUNDO - ESTADO ATUAL    \n");
    printf("====================================\n\n");
    // loop para exibir cada território
    for(int j = 0; j < total; j++) {
        printf("%d. %s (Exercito %s, Tropas: %d)\n", j + 1, mapa[j].nome, mapa[j].cor, mapa[j].tropas);
    }
}

// libera a memória alocada para o mapa
void liberarMemoria(Territorio *mapa) {
    if(mapa != NULL)
        free(mapa);
}

// função principal
int main() {
    // array para armazenar os territórios
    Territorio *territorios;
    int totalTerritorios;

    // cabeçalho do programa
    printf("===========================================================\n");
    printf("            WAR ESTRUTURADO - CADASTRO INICIAL             \n");
    printf("===========================================================\n\n");

    // solicita o número de territórios
    printf("Quantos territórios deseja cadastrar: ");
    scanf("%d", &totalTerritorios);

    // validação do número de territórios
    if(totalTerritorios <= 0) {
        fprintf(stderr, "Número de territórios inválido\n");
        return 1;
    }

    // alocação dinâmica de memória para os territórios
    territorios = calloc((size_t)totalTerritorios, sizeof(Territorio));
    if(territorios == NULL) {
        perror("calloc");
        return 1;
    }
    limparBufferEntrada();

    /* Inicializa gerador de números aleatórios para os dados de batalha */
    srand((unsigned)time(NULL));

    // cadastro dos territórios (modularizado)
    cadastrarTerritorios(territorios, totalTerritorios);

    // exibe o mapa do mundo (modularizado)
    exibirMapa(territorios, totalTerritorios);

    // interação para simular ataques
    while(1) {
        // mostra territórios (índice, nome, cor, tropas)
        printf("\nTerritórios:\n");
        for(int i = 0; i < totalTerritorios; i++) {
            printf("%d - %s (Exército: %s) - Tropas: %d%s\n", i + 1, territorios[i].nome, territorios[i].cor, territorios[i].tropas, territorios[i].tropas > 1 ? " [pode atacar]" : "");
        }

        // fase de ataque
        printf("\n\n--- FASE DE ATAQUE ---\n");

        // escolhe atacante
        int a;
        printf("Escolha o territorio atacante (1 a %d, ou 0 para sair): ", totalTerritorios);
        if(scanf("%d", &a) != 1) {
            limparBufferEntrada();
            printf("Entrada inválida. Tente novamente.\n");
            continue;
        }
        limparBufferEntrada();
        if(a == 0)
            break;

        // escolhe defensor
        int d;
        printf("Escolha o territorio defensor (1 a %d): ", totalTerritorios);
        if(scanf("%d", &d) != 1) {
            limparBufferEntrada();
            printf("Entrada inválida. Tente novamente.\n");
            continue;
        }
        limparBufferEntrada();

        // valida índices
        if(a < 1 || a > totalTerritorios || d < 1 || d > totalTerritorios) {
            printf("Índices inválidos. Tente novamente.\n");
            continue;
        }
        // evita auto-ataque
        if(a == d) {
            printf("Um território não pode atacar a si mesmo.\n");
            continue;
        }

        // realiza o ataque
        Territorio *atk = &territorios[a - 1];
        Territorio *def = &territorios[d - 1];

        // valida condições de ataque
        if(atk->tropas <= 1) {
            printf("Esse território não possui tropas suficientes para atacar (é necessário deixar 1).\n");
            continue;
        }

        // evita ataque entre territórios da mesma cor
        if(strcmp(atk->cor, def->cor) == 0) {
            printf("Ataque inválido: não é permitido atacar um território da mesma cor.\n");
            continue;
        }

        // executa o ataque
        atacar(atk, def);
        // exibe o estado atualizado dos territórios
        printf("\nEstado atualizado:\n");
        printf(" - %s: %d tropas (Exército: %s)\n", atk->nome, atk->tropas, atk->cor);
        printf(" - %s: %d tropas (Exército: %s)\n\n", def->nome, def->tropas, def->cor);

        // aguarda apenas ENTER para continuar ao próximo turno
        printf("Presione Enter para continuar para o proximo turno...");
        char _cont[16];
        if(fgets(_cont, sizeof(_cont), stdin) == NULL)
            break;
        // ao pressionar ENTER (linha vazia) ou qualquer entrada seguida de ENTER, o loop continua e volta à fase de ataque
    }

    // libera a memória alocada
    liberarMemoria(territorios);
    return 0;
}