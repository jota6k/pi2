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

    while (fscanf(arquivo, "%s", linha) != EOF) {
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

void leitura_arquivos_dados(int memoria_dados[]) {
int i = 0;
    FILE *arquivo = fopen("dados.dat", "r");
if(arquivo == NULL){
    printf("Erro ao abrir arquivo.\n");
    return;
}
while(fscanf(arquivo, "%d", &memoria_dados[i]) != EOF) {
    i++;
}
fclose(arquivo);
}

void executar_programa(int memoria_instrucao[], int memoria_dados[], int registradores[]) {
    int PC = 0;
    int total = 0, aritmeticas = 0, memoria_acesso = 0;
    while (PC < 256) {
        int instrucao = memoria_instrucao[PC];
        int opcode = (instrucao >> 12) & 0xF;
        int rs = (instrucao >> 9)  & 0x7;
        int rt = (instrucao >> 6)  & 0x7;
        int rd = (instrucao >> 3)  & 0x7;
        int imm = instrucao & 0x3F;
        int addr = instrucao & 0xFF;

        if (imm >= 32) { // <---------- extensao de sinal
            imm = imm - 64; 
        }

        total++;    
        printf("\nPC=%d | opcode=%d\n", PC, opcode);

        int flag_zero = 0;
    
        switch(opcode) {
            case 0:
                aritmeticas++;
                registradores[rd] = registradores[rs] + registradores[rt];
                printf("ADD r%d = r%d + r%d\n", rd, rs, rt);
                break;
            case 1:
                aritmeticas++;
                registradores[rd] = registradores[rs] - registradores[rt];
                printf("SUB r%d = r%d - r%d\n", rd, rs, rt);
                break;
            case 2:
                aritmeticas++;
                registradores[rt] = registradores[rs] + imm;
                printf("ADDI r%d = r%d + %d\n", rt, rs, imm);
                break;
            case 3:
                memoria_acesso++;
                registradores[rt] = memoria_dados[registradores[rs] + imm];
                printf("LW r%d = MEM[%d]\n", rt, registradores[rs] + imm);
                break;
            case 4: 
                memoria_acesso++;
                memoria_dados[registradores[rs] + imm] = registradores[rt];
                printf("SW MEM[%d] = r%d\n", registradores[rs] + imm, rt);
                break;
            case 8:
                ULA(registradores[rs], registradores[rt], 2, &flag_zero);
                if (flag_zero) {
                    PC += imm;
                }
                break;
            case 2:
                PC = addr - 1;
                break;
            default:
                printf("Instrucao invalida!\n");
                break;
        }
        PC++;
    }
    printf("\n -Estatisticas- \n");
    printf("Total de instrucoes: %d\nAritmeticas: %d\nAcesso a memoria: %d\n", total, aritmeticas, memoria_acesso);
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

void salvar_arquivo_dat(int memoria_dados[]) {
    FILE *arquivo = fopen("saidaDados.dat", "w");
    
    if(arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    
    for (int i = 0; i < 256; i++) {
        fprintf(arquivo, "%d\n", memoria_dados[i]);
    }
void imprimir_memoria(int memoria[]) {

    printf("\n--- Conteudo da Memoria ---\n");

    for(int i = 0; i < 256; i++) {
        printf("Mem[%d] = %d\n", i, memoria[i]);
    }
}
    void salvar_asm(int memoria[]) {

    FILE *arquivo = fopen("programa.asm", "w");

    if (arquivo == NULL) {
        printf("Erro ao criar arquivo ASM\n");
        return;
    }

    for(int PC = 0; PC < 256; PC++) {

        int instrucao = memoria[PC];

        int opcode = (instrucao >> 12) & 0xF;
        int rs = (instrucao >> 9) & 0x7;
        int rt = (instrucao >> 6) & 0x7;
        int rd = (instrucao >> 3) & 0x7;
        int imm = instrucao & 0x3F;
        int addr = instrucao & 0xFF;

        switch(opcode) {

        case 0:
            fprintf(arquivo, "ADD R%d R%d R%d\n", rd, rs, rt);
            break;

        case 1:
            fprintf(arquivo, "SUB R%d R%d R%d\n", rd, rs, rt);
            break;

        case 2:
            fprintf(arquivo, "ADDI R%d R%d %d\n", rt, rs, imm);
            break;

        case 3:
            fprintf(arquivo, "LW R%d %d(R%d)\n", rt, imm, rs);
            break;

        case 4:
            fprintf(arquivo, "SW R%d %d(R%d)\n", rt, imm, rs);
            break;

        case 8:
            fprintf(arquivo, "BEQ R%d R%d %d\n", rs, rt, imm);
            break;

        default:
            fprintf(arquivo, "NOP\n");
        }
    }

    fclose(arquivo);

    printf("Arquivo programa.asm salvo!\n");
}
    fclose(arquivo);
}
