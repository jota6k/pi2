#include <stdio.h>
#include <stdlib.h>
#include "simulador.h"

int main() {
    // Estruturas e variáveis principais
    struct instrucao memoria_instrucoes[256];
    int banco_registradores[NUM_REGISTRADORES];
    int PC;

    // Inicialização do estado do simulador
    inicializar_registradores(banco_registradores);
    inicializar_PC(&PC);
    
    // Inicializa a memória de instruções como vazia (opcode = -1)
    for (int i = 0; i < 256; i++) {
        memoria_instrucoes[i].opcode = -1;
    }

    int opcao = -1;

    // Menu
    while (opcao != 0) {
        printf("\n-------------------------------------------------\n");
        printf("          SIMULADOR MINI-MIPS 8 BITS             \n");
        printf("-------------------------------------------------\n");
        printf(" 1. Carregar memoria de instrucoes (.mem)\n");
        printf(" 2. Carregar memoria de dados (.dat)\n");
        printf(" 3. Imprimir memorias (instrucoes e dados)\n");
        printf(" 4. Imprimir banco de registradores\n");
        printf(" 5. Imprimir PC (Program Counter)\n");
        printf(" 6. Executa Programa (Run)\n");
        printf(" 7. Executa uma instrucao (Step)\n");
        printf(" 8. Volta uma instrucao (Back)\n");
        printf(" 0. Sair do simulador\n");
        printf("-------------------------------------------------\n");
        printf("Escolha uma opcao: ");
        
        // Evitar loop infinito se o usuário digitar uma letra
        if (scanf("%d", &opcao) != 1) {
            while(getchar() != '\n'); // Limpa o buffer do teclado
            opcao = -1;
        }

        printf("\n");

        // Fluxo de opções
        switch (opcao) {
            case 1:
                // Função de leitura do .mem
                Ler_Arquivo_Memoria(memoria_instrucoes);
                break;
                
            case 4:
                // Imprime o estado atual dos registradores
                imprimir_registradores(banco_registradores);
                break;
                
            case 5:
                // Imprime qual a linha que o simulador está
                imprimir_PC(PC);
                break;
            case 2:
            case 3:
                printf("Memoria de Dados\n");
                break;
            case 6:
            case 7:
                printf("ULA\n");
                break;
            case 8:
                printf("Instrucao Back\n");
                break;
            case 0:
                printf("Simulador encerrado!\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }
    }

    return 0;
}
