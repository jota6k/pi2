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
    char nome_mem[50];
    char nome_dat[50];
    
    printf("--- Inicializacao do Mini MIPS ---\n");

    int status_mem = escolher_arquivo_mem(nome_mem);
    if (status_mem == -1) {
        printf("Encerrando o programa.\n");
        return 0;
    }

    int status_dat = escolher_arquivo_dat(nome_dat);
    if (status_dat == -1) {
        printf("Encerrando o programa.\n");
        return 0;
    }

    inicializar_registradores(registradores);

    if (status_mem == 1) {
        leitura_arquivo_mem(memoria_instrucao, nome_mem);
        printf("Memoria de instrucoes carregada com sucesso.\n");
    } else {
        printf("Memoria de instrucoes iniciada com zeros.\n");
    }

    if (status_dat == 1) {
        leitura_arquivos_dados(memoria_dados, nome_dat);
        printf("Memoria de dados carregada com sucesso.\n");
    } else {
        printf("Memoria de dados iniciada com zeros.\n");
    }

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
        
        if (scanf("%d", &op) != 1) {
            while (getchar() != '\n'); 
            op = -1; 
        }

        switch(op) {
            case 2:
                imprimir_memoria_instrucoes(memoria_instrucao);
                imprimir_memoria_dados(memoria_dados);
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
