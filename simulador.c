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

void executar_programa(int memoria[], int registradores[]) {
    int PC = 0;
    while (PC < 256) {
        int instrucao = memoria[PC];
        int opcode = (instrucao >> 12) & 0xF;
        int rs = (instrucao >> 9)  & 0x7;
        int rt = (instrucao >> 6)  & 0x7;
        int rd = (instrucao >> 3)  & 0x7;
        int imm = instrucao & 0x3F;

        printf("\nPC=%d | opcode=%d\n", PC, opcode);
        switch(opcode) {                                //switch para add, sub, addi, lw e sw
            case 0:
                registradores[rd] = registradores[rs] + registradores[rt];
                printf("ADD r%d = r%d + r%d\n", rd, rs, rt);
                break;
            case 1: 
                registradores[rd] = registradores[rs] - registradores[rt];
                printf("SUB r%d = r%d - r%d\n", rd, rs, rt);
                break;
            case 2:
                registradores[rt] = registradores[rs] + imm;
                printf("ADDI r%d = r%d + %d\n", rt, rs, imm);
                break;
            case 3:
                registradores[rt] = memoria[registradores[rs] + imm];
                printf("LW r%d = MEM[%d]\n", rt, registradores[rs] + imm);
                break;
            case 4: 
                memoria[registradores[rs] + imm] = registradores[rt];
                printf("SW MEM[%d] = r%d\n", registradores[rs] + imm, rt);
                break;
            default:
                printf("Instrucao invalida!\n");
                break;
        }
        PC++;
    }
}
int ULA(int A, int B, int controle, int *flag) {
    int resultado = 0;

    switch(controle) {
        case 0:
            resultado = A + B;
            break;
        case 2:
            resultado = A - B;
            break;
        case 4:
            resultado = A & B;
            break;
        case 5:
            resultado = A | B;
            break;
        default:
            resultado = 0;
            break;
    }

    if (resultado == 0) {
        *flag = 1;
    } else {
        *flag = 0;
    }

    if (resultado > 127 || resultado < -128) {
        printf("Overflow.\n");
    }

    return resultado;
}
