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
    struct EstadoMaquina historico[999];
    int passo_atual = 0;

    inicializar_registradores(registradores);
    
    do {
        printf("\n--- Menu do Simulador Mini MIPS ---\n");
        printf("1 - Carregar memoria de instrucoes (.mem)\n");
        printf("2 - Carregar memoria de dados (.dat)\n");
        printf("3 - Imprimir memorias (instrucoes e dados)\n");
        printf("4 - Imprimir banco de registradores\n");
        printf("5 - Imprimir todo o simulador (registradores e memorias)\n");
        printf("6 - Salvar programa em ASM\n");
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
            case 1:
                escolher_arquivo_mem(nome_mem);
                leitura_arquivo_mem(memoria_instrucao, nome_mem);
                break;
            case 2:
                escolher_arquivo_dat(nome_dat);
                leitura_arquivos_dados(memoria_dados, nome_dat);
                break;
            case 3:
                imprimir_memoria_instrucoes(memoria_instrucao);
                imprimir_memoria_dados(memoria_dados);
                break;
            case 4:
                imprimir_registradores(registradores, PC);
                break;
            case 5:
                imprimir_registradores(registradores, PC);
                imprimir_memoria_instrucoes(memoria_instrucao);
                imprimir_memoria_dados(memoria_dados);
                break;
            case 6:
                salvar_asm(memoria_instrucao);
                break;
            case 7:
                salvar_arquivo_dat(memoria_dados);
                printf("Arquivo saidaDados.dat salvo\n");
                break;
            case 8:
                run(memoria_instrucao, memoria_dados, registradores, &PC, &aritmeticas, &memoria_acesso, historico, &passo_atual);
                break;
            case 9:
                step(memoria_instrucao, memoria_dados, registradores, &PC, &aritmeticas, &memoria_acesso, historico, &passo_atual);
                break;
            case 10:
                back(registradores, memoria_dados, &PC, historico, &passo_atual);
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
