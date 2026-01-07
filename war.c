#include <stdio.h>
#include <string.h>

// definições de constantes
#define MAX_TERRITORIO 5

// definição da estrutura Territorio
typedef struct
{
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// função para limpar o buffer de entrada
void limparBufferEntrada()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// função principal
int main()
{
    // array para armazenar os territórios
    Territorio territorios[MAX_TERRITORIO];
    int totalTerritorios = 0;

    printf("===========================================================\n\n");
    printf(" Vamos cadastrar os 5 territorios iniciais do nosso mundo.\n");

    // loop para cadastrar os territórios
    for (int i = 0; i < MAX_TERRITORIO; i++)
    {
            // coleta o nome do território
            printf("\n--- Cadastrando Territorio %d ---\n", i + 1);
            printf("Nome do Territorio: ");
            fgets(territorios[totalTerritorios].nome, 30, stdin);
            territorios[totalTerritorios].nome[strcspn(territorios[totalTerritorios].nome, "\n")] = 0;

            // coleta a cor do exército
            printf("Cor do Exercito (ex: Azul, verde): ");
            fgets(territorios[totalTerritorios].cor, 10, stdin);
            territorios[totalTerritorios].cor[strcspn(territorios[totalTerritorios].cor, "\n")] = 0;

            // coleta o número de tropas    
            printf("Numero de Tropas: ");
            scanf("%d", &territorios[totalTerritorios].tropas);
            limparBufferEntrada();

            // incrementa o total de territórios cadastrados
            totalTerritorios++;
    }

    // exibe a mensagem de conclusão do cadastro
    printf("\nCadastro inicial concluido com sucesso!\n\n");

    // exibe o mapa do mundo
    printf("====================================\n");
    printf("    MAPA DO MUNDO - ESTADO ATUAL    \n");
    printf("====================================\n\n");

    // loop para exibir os territórios
    for (int j = 0; j < totalTerritorios; j++)
    {
        // exibe os detalhes de cada território
        printf("TERRITORIO %d:\n", j + 1);
        printf(" - Nome: %s\n", territorios[j].nome);
        printf(" - Dominado por: Exercito %s\n", territorios[j].cor);
        printf(" - Tropas: %d\n\n", territorios[j].tropas);
    }
}