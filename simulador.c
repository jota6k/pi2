#include <stdio.h>
#include "simulador.h"

void leitura_arquivo_mem(int memoria[]) {

    FILE *arquivo = fopen("memoria.mem", "r");

    if(arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    int i = 0;
    char linha[17];

    while (fscanf(arquivo, "%s", &memoria[i]) != EOF) {
        memoria[i] = (int)strtol(linha, NULL, 2);
        i++;
    }

    fclose(arquivo);
}

void inicializar_registradores(int registradores[]) {
    for (int i = 0; i < 8; i++) {
        registradores[i] = 0;
    }
}
int ler_reg(int registradores[], int indice) {
    return registradores[indice];
}
void escrever_reg(int registradores[], int indice, int valor) {
    registradores[indice] = valor;
}

void executar_programa(int memoria[], int registradores[]) {         //Coloquei como uma variável que controla qual instrução vai ser executada.
    int PC = 0; 
    while (PC < 256) {
        int instrucao = memoria[PC];
        printf("PC = %d | Instrucao = %d\n", PC, instrucao);
        PC++;
    }
}


