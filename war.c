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
    char *missao;  // Ponteiro para armazenar dinamicamente a missão do jogador
} Territorio;

// Vetor global de missões disponíveis
char *missoes[] = {
    "Conquistar 2 territórios",
    "Acumular 20 tropas em um território",
    "Conquistar 3 territórios",
    "Conquistar mais de 50% dos territórios",
    "Alcançar 30 tropas no total"
};

int totalMissoes = 5;

// Protótipos de funções
void limparBufferEntrada();
void atacar(Territorio *atacante, Territorio *defensor);
void cadastrarTerritorios(Territorio *mapa, int total);
void exibirMapa(Territorio *mapa, int total);
void liberarMemoriaTotalizada(Territorio *territorios, int total);
void atribuirMissao(char** destino, char* missoes[], int totalMissoes);
void exibirMissoes(Territorio *territorios, int total);
int verificarMissao(char *missao, Territorio *mapa, int tamanho, char *corJogador);
int validarMissao(char *missao, Territorio *mapa, int tamanho, char *corJogador);
void exibirCabecalho();
int obterTotalTerritorios();
Territorio* alocarTerritorios(int total);
void exibirTerritorios(Territorio *territorios, int total);
int validarAtaque(int a, int d, int total, Territorio *territorios);
int executarTurno(int a, int d, Territorio *territorios, int total);
void executarBatalhas(Territorio *territorios, int total);
void exibirMissoesAtual(Territorio *territorios, int total);
void verificarStatusMissao(Territorio *territorios, int total);
int exibirMenuAcoes(int total);


/**
 * @brief Função principal do programa WAR Estruturado.
 * Inicializa o jogo, aloca memória, cadastra territórios,
 * atribui missões e executa o loop de batalhas.
 * @return 0 se o programa executou com sucesso, 1 em caso de erro.
 */
int main() {
    exibirCabecalho();

    int totalTerritorios = obterTotalTerritorios();
    if(totalTerritorios == -1)
        return 1;

    Territorio *territorios = alocarTerritorios(totalTerritorios);
    if(territorios == NULL)
        return 1;

    limparBufferEntrada();

    /* Inicializa gerador de números aleatórios para os dados de batalha */
    srand((unsigned)time(NULL));

    // cadastro dos territórios
    cadastrarTerritorios(territorios, totalTerritorios);

    // Atribui uma missão para cada jogador usando malloc
    for(int i = 0; i < totalTerritorios; i++) {
        atribuirMissao(&(territorios[i].missao), missoes, totalMissoes);
        
        // Se a missão não for viável, sorteia outra
        while(!validarMissao(territorios[i].missao, territorios, totalTerritorios, territorios[i].cor)) {
            atribuirMissao(&(territorios[i].missao), missoes, totalMissoes);
        }
    }

    // exibe o mapa do mundo
    exibirMapa(territorios, totalTerritorios);

    // Exibe as missões dos jogadores apenas uma vez no início
    exibirMissoes(territorios, totalTerritorios);

    // executa o loop de batalhas
    executarBatalhas(territorios, totalTerritorios);

    // libera a memória alocada
    liberarMemoriaTotalizada(territorios, totalTerritorios);
    return 0;
}

/**
 * @brief Limpa o buffer de entrada do teclado.
 * Remove caracteres pendentes até encontrar uma quebra de linha.
 */
void limparBufferEntrada() {
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}



/**
 * @brief Realiza um ataque entre dois territórios.
 * Simula um dado para atacante e defensor, determina o vencedor,
 * e atualiza as tropas e cores dos territórios.
 * @param atacante Ponteiro para o território atacante (modificável).
 * @param defensor Ponteiro para o território defensor (modificável).
 */
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

/**
 * @brief Cadastra territórios com dados do usuário.
 * Recebe nome, cor do exército e número de tropas para cada território.
 * @param mapa Array de territórios (modificável).
 * @param total Número total de territórios a cadastrar.
 */
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

/**
 * @brief Exibe o mapa do mundo com todos os territórios cadastrados.
 * Mostra nome, exército e número de tropas de cada território.
 * @param mapa Array de territórios.
 * @param total Número total de territórios.
 */
void exibirMapa(Territorio *mapa, int total) {
    printf("====================================\n");
    printf("    MAPA DO MUNDO - ESTADO ATUAL    \n");
    printf("====================================\n\n");
    // loop para exibir cada território
    for(int j = 0; j < total; j++) {
        printf("%d. %s (Exercito %s, Tropas: %d)\n", j + 1, mapa[j].nome, mapa[j].cor, mapa[j].tropas);
    }
}

/**
 * @brief Libera toda a memória alocada dinamicamente.
 * Libera as strings de missões de cada território e o array de territórios.
 * @param territorios Array de territórios (será liberado).
 * @param total Número total de territórios.
 */
void liberarMemoriaTotalizada(Territorio *territorios, int total) {
    if(territorios != NULL) {
        // Libera a memória de cada missão de cada jogador
        for(int i = 0; i < total; i++) {
            if(territorios[i].missao != NULL) {
                free(territorios[i].missao);
                territorios[i].missao = NULL;
            }
        }
        // Libera o vetor de territórios
        free(territorios);
    }
}

/**
 * @brief Sorteia e atribui uma missão aleatória a um jogador.
 * Aloca memória dinâmica e copia a missão sorteada usando strcpy.
 * @param destino Ponteiro para a string de missão (será alocada).
 * @param missoes Array de strings com as missões disponíveis.
 * @param totalMissoes Número total de missões disponíveis.
 */
void atribuirMissao(char** destino, char* missoes[], int totalMissoes) {
    if(destino == NULL)
        return;

    // Sorteia um índice aleatório
    int indice = rand() % totalMissoes;

    // Aloca memória dinâmica para a missão
    *destino = (char *)malloc((strlen(missoes[indice]) + 1) * sizeof(char));
    if(*destino == NULL) {
        perror("malloc");
        return;
    }

    // Copia a missão para o destino usando strcpy
    strcpy(*destino, missoes[indice]);
}

/**
 * @brief Exibe as missões atribuídas a cada jogador no início do jogo.
 * Mostra o território, exército e missão de cada jogador.
 * @param territorios Array de territórios com as missões atribuídas.
 * @param total Número total de territórios.
 */
void exibirMissoes(Territorio *territorios, int total) {
    printf("\n====================================\n");
    printf("     MISSÕES DOS JOGADORES         \n");
    printf("====================================\n\n");
    for(int i = 0; i < total; i++) {
        if(territorios[i].missao != NULL) {
            printf("%s (Exército: %s): %s\n", territorios[i].nome, territorios[i].cor, territorios[i].missao);
        } else {
            printf("%s (Exército: %s): Sem missão atribuída\n", territorios[i].nome, territorios[i].cor);
        }
    }
    printf("\n====================================\n\n");
}

/**
 * @brief Verifica se a missão de um jogador foi cumprida.
 * Analisa o estado atual do mapa e retorna 1 se a condição foi atingida.
 * @param missao String contendo a descrição da missão.
 * @param mapa Array de territórios para análise.
 * @param tamanho Número total de territórios.
 * @param corJogador Cor do exército do jogador.
 * @return 1 se a missão foi cumprida, 0 caso contrário.
 */
int verificarMissao(char *missao, Territorio *mapa, int tamanho, char *corJogador) {
    if(missao == NULL)
        return 0;

    // Verificação 1: "Conquistar 2 territórios"
    // Conta quantos territórios o jogador controla
    if(strstr(missao, "Conquistar 2") != NULL) {
        int contador = 0;
        for(int i = 0; i < tamanho; i++) {
            if(strcmp(mapa[i].cor, corJogador) == 0) {
                contador++;
            }
        }
        // Retorna verdadeiro se tem 2 ou mais territórios
        if(contador >= 2)
            return 1;
    }

    // Verificação 2: "Acumular 20 tropas em um território"
    // Verifica se há pelo menos um território com 20+ tropas
    if(strstr(missao, "Acumular 20") != NULL) {
        for(int i = 0; i < tamanho; i++) {
            if(strcmp(mapa[i].cor, corJogador) == 0 && mapa[i].tropas >= 20)
                return 1;
        }
    }

    // Verificação 3: "Conquistar 3 territórios"
    // Conta quantos territórios o jogador controla
    if(strstr(missao, "Conquistar 3") != NULL) {
        int contador = 0;
        for(int i = 0; i < tamanho; i++) {
            if(strcmp(mapa[i].cor, corJogador) == 0) {
                contador++;
            }
        }
        // Retorna verdadeiro se tem 3 ou mais territórios
        if(contador >= 3)
            return 1;
    }

    // Verificação 4: "Conquistar mais de 50% dos territórios"
    // Verifica se o jogador controla mais da metade dos territórios
    if(strstr(missao, "mais de 50%") != NULL) {
        int contador = 0;
        for(int i = 0; i < tamanho; i++) {
            if(strcmp(mapa[i].cor, corJogador) == 0) {
                contador++;
            }
        }
        // Retorna verdadeiro se tem mais de 50% dos territórios
        if(contador > tamanho / 2)
            return 1;
    }

    // Verificação 5: "Alcançar 30 tropas no total"
    // Soma todas as tropas que o jogador controla
    if(strstr(missao, "30 tropas") != NULL) {
        int totalTropas = 0;
        for(int i = 0; i < tamanho; i++) {
            if(strcmp(mapa[i].cor, corJogador) == 0) {
                totalTropas += mapa[i].tropas;
            }
        }
        // Retorna verdadeiro se tem 30 ou mais tropas no total
        if(totalTropas >= 30)
            return 1;
    }

    return 0;
}

/**
 * @brief Valida se uma missão é viável antes de ser atribuída.
 * Verifica se as condições do jogo permitem a realização da missão.
 * @param missao String contendo a descrição da missão.
 * @param mapa Array de territórios para análise.
 * @param tamanho Número total de territórios.
 * @param corJogador Cor do exército do jogador.
 * @return 1 se a missão é viável, 0 caso contrário.
 */
int validarMissao(char *missao, Territorio *mapa, int tamanho, char *corJogador) {
    if(missao == NULL)
        return 0;

    // Verificação 1: "Conquistar 2 territórios" - viável se houver pelo menos 2 territórios
    if(strstr(missao, "Conquistar 2") != NULL) {
        return tamanho >= 2 ? 1 : 0;
    }

    // Verificação 2: "Acumular 20 tropas" - sempre viável
    if(strstr(missao, "Acumular 20") != NULL) {
        return 1;
    }

    // Verificação 3: "Conquistar 3 territórios" - viável se houver pelo menos 3 territórios
    if(strstr(missao, "Conquistar 3") != NULL) {
        return tamanho >= 3 ? 1 : 0;
    }

    // Verificação 4: "Conquistar 50% dos territórios" - viável se houver pelo menos 2 territórios
    if(strstr(missao, "mais de 50%") != NULL) {
        return tamanho >= 2 ? 1 : 0;
    }

    // Verificação 5: "Alcançar 30 tropas" - sempre viável
    if(strstr(missao, "30 tropas") != NULL) {
        return 1;
    }

    return 1;  // por padrão, assume que é viável
}

/**
 * @brief Exibe o cabeçalho do programa no início da execução.
 */
void exibirCabecalho() {
    printf("===========================================================\n");
    printf("            WAR ESTRUTURADO - CADASTRO INICIAL             \n");
    printf("===========================================================\n\n");
}

/**
 * @brief Exibe as missões atuais de todos os jogadores durante os turnos.
 * @param territorios Array de territórios com as missões atuais.
 * @param total Número total de territórios.
 */
void exibirMissoesAtual(Territorio *territorios, int total) {
    printf("\n====================================\n");
    printf("     MISSÕES ATUAIS DOS JOGADORES   \n");
    printf("====================================\n\n");
    for(int i = 0; i < total; i++) {
        if(territorios[i].missao != NULL) {
            printf("%s (Exército: %s): %s\n", territorios[i].nome, territorios[i].cor, territorios[i].missao);
        } else {
            printf("%s (Exército: %s): Sem missão atribuída\n", territorios[i].nome, territorios[i].cor);
        }
    }
    printf("\n====================================\n\n");
}

/**
 * @brief Permite ao usuário verificar o status de uma missão específica.
 * Solicita qual território verificar e exibe o status da missão.
 * @param territorios Array de territórios.
 * @param total Número total de territórios.
 */
void verificarStatusMissao(Territorio *territorios, int total) {
    printf("\nQual territorio deseja verificar a missão? (1 a %d): ", total);
    int territorio;
    if(scanf("%d", &territorio) != 1) {
        limparBufferEntrada();
        printf("Entrada inválida. Tente novamente.\n");
        return;
    }
    limparBufferEntrada();
    
    if(territorio < 1 || territorio > total) {
        printf("Índice inválido. Tente novamente.\n");
        return;
    }
    
    Territorio *terr = &territorios[territorio - 1];
    printf("\n--- STATUS DA MISSÃO ---\n");
    printf("Território: %s\n", terr->nome);
    printf("Exército: %s\n", terr->cor);
    printf("Missão: %s\n", terr->missao != NULL ? terr->missao : "Sem missão");
    
    if(terr->missao != NULL) {
        if(verificarMissao(terr->missao, territorios, total, terr->cor)) {
            printf("Status: ✅ MISSÃO CUMPRIDA!\n");
        } else {
            printf("Status: ⏳ Missão em progresso\n");
        }
    }
    printf("---\n\n");
}

/**
 * @brief Exibe o menu de ações e retorna a escolha do usuário.
 * Menu com opções: 1 - Atacar, 2 - Verificar missão, 0 - Sair.
 * @param total Número total de territórios (para referência).
 * @return Número da ação escolhida (0, 1, 2) ou -1 em caso de entrada inválida.
 */
int exibirMenuAcoes(int total) {
    int acao;
    printf("\n--- MENU DE AÇÕES ---\n");
    printf("1 - Atacar\n");
    printf("2 - Verificar missão\n");
    printf("0 - Sair\n");
    printf("Escolha sua ação: ");
    
    if(scanf("%d", &acao) != 1) {
        limparBufferEntrada();
        printf("Entrada inválida. Tente novamente.\n");
        return -1;
    }
    limparBufferEntrada();
    return acao;
}

/**
 * @brief Obtém e valida o número de territórios a cadastrar.
 * @return Número de territórios se válido, -1 em caso de erro.
 */
int obterTotalTerritorios() {
    int total;
    printf("Quantos territórios deseja cadastrar: ");
    scanf("%d", &total);

    if(total <= 0) {
        fprintf(stderr, "Número de territórios inválido\n");
        return -1;
    }
    return total;
}

/**
 * @brief Aloca memória dinâmica para um array de territórios.
 * @param total Número de territórios a alocar.
 * @return Ponteiro para o array alocado, ou NULL em caso de erro.
 */
Territorio* alocarTerritorios(int total) {
    Territorio *territorios = (Territorio *)calloc((size_t)total, sizeof(Territorio));
    if(territorios == NULL) {
        perror("calloc");
        return NULL;
    }
    return territorios;
}

/**
 * @brief Exibe os territórios disponíveis com suas informações atuais.
 * Mostra índice, nome, exército, tropas e se pode atacar.
 * @param territorios Array de territórios.
 * @param total Número total de territórios.
 */
void exibirTerritorios(Territorio *territorios, int total) {
    printf("\nTerritórios:\n");
    for(int i = 0; i < total; i++) {
        printf("%d - %s (Exército: %s) - Tropas: %d%s\n", i + 1, territorios[i].nome, territorios[i].cor, territorios[i].tropas, territorios[i].tropas > 1 ? " [pode atacar]" : "");
    }
}

/**
 * @brief Valida se um ataque é possível segundo as regras do jogo.
 * Verifica índices, auto-ataque, tropas suficientes e cores diferentes.
 * @param a Índice do território atacante.
 * @param d Índice do território defensor.
 * @param total Número total de territórios.
 * @param territorios Array de territórios.
 * @return 1 se o ataque é válido, 0 caso contrário.
 */
int validarAtaque(int a, int d, int total, Territorio *territorios) {
    // valida índices
    if(a < 1 || a > total || d < 1 || d > total) {
        printf("Índices inválidos. Tente novamente.\n");
        return 0;
    }
    // evita auto-ataque
    if(a == d) {
        printf("Um território não pode atacar a si mesmo.\n");
        return 0;
    }

    Territorio *atk = &territorios[a - 1];
    Territorio *def = &territorios[d - 1];

    // valida condições de ataque
    if(atk->tropas <= 1) {
        printf("Esse território não possui tropas suficientes para atacar (é necessário deixar 1).\n");
        return 0;
    }

    // evita ataque entre territórios da mesma cor
    if(strcmp(atk->cor, def->cor) == 0) {
        printf("Ataque inválido: não é permitido atacar um território da mesma cor.\n");
        return 0;
    }

    return 1;
}

/**
 * @brief Executa um turno de batalha entre dois territórios.
 * Realiza o ataque e verifica se alguma missão foi cumprida.
 * @param a Índice do território atacante.
 * @param d Índice do território defensor.
 * @param territorios Array de territórios (modificável).
 * @param total Número total de territórios.
 * @return 1 se uma missão foi cumprida (jogo termina), 0 para continuar, -1 em erro.
 */
int executarTurno(int a, int d, Territorio *territorios, int total) {
    Territorio *atk = &territorios[a - 1];
    Territorio *def = &territorios[d - 1];

    // executa o ataque
    atacar(atk, def);
    
    // exibe o estado atualizado dos territórios
    printf("\nEstado atualizado:\n");
    printf(" - %s: %d tropas (Exército: %s)\n", atk->nome, atk->tropas, atk->cor);
    printf(" - %s: %d tropas (Exército: %s)\n\n", def->nome, def->tropas, def->cor);

    // Verifica silenciosamente se algum jogador cumpriu a missão
    for(int i = 0; i < total; i++) {
        if(verificarMissao(territorios[i].missao, territorios, total, territorios[i].cor)) {
            printf("\n🎉 MISSÃO CUMPRIDA! 🎉\n");
            printf("%s (Exército: %s) completou a missão: %s\n", territorios[i].nome, territorios[i].cor, territorios[i].missao);
            printf("\n*** %s É O VENCEDOR! ***\n\n", territorios[i].nome);
            return 1;  // retorna 1 para indicar que o jogo deve terminar
        }
    }

    // aguarda apenas ENTER para continuar ao próximo turno
    printf("Presione Enter para continuar para o proximo turno...");
    char _cont[16];
    if(fgets(_cont, sizeof(_cont), stdin) == NULL) {
        return -1;
    }
    return 0;  // retorna 0 para continuar o jogo
}

/**
 * @brief Executa o loop principal de batalhas do jogo.
 * Exibe territórios, missões, menu de ações e processa a escolha do usuário.
 * @param territorios Array de territórios (modificável durante o jogo).
 * @param total Número total de territórios.
 */
void executarBatalhas(Territorio *territorios, int total) {
    while(1) {
        exibirTerritorios(territorios, total);
        exibirMissoesAtual(territorios, total);
        
        int acao = exibirMenuAcoes(total);
        
        if(acao == 0) {
            break;  // Sair do jogo
        } else if(acao == 1) {
            // Opção: Atacar
            printf("\n--- FASE DE ATAQUE ---\n");
            printf("Escolha o territorio atacante (1 a %d, ou 0 para voltar ao menu): ", total);
            int a;
            if(scanf("%d", &a) != 1) {
                limparBufferEntrada();
                printf("Entrada inválida. Tente novamente.\n");
                continue;
            }
            limparBufferEntrada();
            
            if(a == 0)
                continue;  // Volta ao menu
            if(a < 1 || a > total) {
                printf("Índice inválido. Tente novamente.\n");
                continue;
            }
            
            printf("Escolha o territorio defensor (1 a %d): ", total);
            int d;
            if(scanf("%d", &d) != 1) {
                limparBufferEntrada();
                printf("Entrada inválida. Tente novamente.\n");
                continue;
            }
            limparBufferEntrada();
            
            if(d < 1 || d > total) {
                printf("Índice inválido. Tente novamente.\n");
                continue;
            }
            
            if(!validarAtaque(a, d, total, territorios))
                continue;
            
            int resultado = executarTurno(a, d, territorios, total);
            if(resultado == 1)  // uma missão foi cumprida
                break;  // encerra o jogo
            if(resultado == -1)  // houve erro na entrada
                break;
        } else if(acao == 2) {
            // Opção: Verificar missão
            verificarStatusMissao(territorios, total);
            printf("Presione Enter para voltar ao menu...");
            char _cont[16];
            if(fgets(_cont, sizeof(_cont), stdin) == NULL) {
                break;
            }
        } else {
            printf("Ação inválida. Tente novamente.\n");
        }
    }
}