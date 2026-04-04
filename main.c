#include <stdio.h>
#include <stdlib.h>
#include "simulador.h"

int main() {
    int memoria_instrucao[256] = {0}; 
    int memoria_dados[256] = {0};
    int registradores[8] = {0};
    
    int PC = 0;
    int aritmeticas = 0;
    int memoria_acesso = 0;
    int op;

    inicializar_registradores(registradores);
    leitura_arquivo_mem(memoria_instrucao);
    leitura_arquivos_dados(memoria_dados);

    do {
        printf("\n--- Menu do Simulador Mini MIPS ---\n");
        printf("2 - Mostrar memorias\n");
        printf("3 - Mostrar registradores\n");
        printf("6 - Salvar .asm\n");
        printf("7 - Salvar saidaDados.dat\n");
        printf("8 - Executar programa (Run)\n");
        printf("9 - Executar uma instrucao (Step)\n");
        printf("10 - Voltar uma instrucao (Back)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &op);

        switch(op) {
            case 2:
                printf("\nMemoria de Instrucoes");
                imprimir_memoria(memoria_instrucao);
                printf("\nMemoria de Dados");
                imprimir_memoria(memoria_dados);
                break;
            case 3:
                printf("\nBanco de Registradores\n");
                for (int i = 0; i < 8; i++) {
                    printf("R%d = %d\n", i, registradores[i]);
                }
                printf("PC = %d\n", PC);
                break;
            case 6:
                salvar_asm(memoria_instrucao);
                break;
            case 7:
                salvar_arquivo_dat(memoria_dados);
                printf("Arquivo saidaDados.dat salvo\n");
                break;
            case 8:
                run(memoria_instrucao, memoria_dados, registradores, &PC, &aritmeticas, &memoria_acesso);
                break;
            case 9:
                step(memoria_instrucao, memoria_dados, registradores, &PC, &aritmeticas, &memoria_acesso);
                break;
            case 10:
                back(registradores, memoria_dados, &PC);
                break;
            case 0:
                printf("Programa encerrado\n");
                break;
            default:
                printf("Opcao invalida\n");
        }
    } while (op != 0);

    return 0;
}
