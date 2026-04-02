#include <stdio.h>
#include <stdlib.h>
#include "simulador.h"

void leitura_arquivo_mem(int memoria[]) {
    FILE *arquivo = fopen("memoria.mem", "r");
    if(arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    int i = 0;
    char linha[17];

    while (fscanf(arquivo, "%s", linha) != EOF && i < 256) {
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

    while(fscanf(arquivo, "%d", &memoria_dados[i]) != EOF && i < 256) {
        i++;
    }

    fclose(arquivo);
}
int fetch(int memoria_instrucao[], int PC){
    return memoria_instrucao[PC];
}
struct decode{
    int opcode;
    int rs, rt, rd;
    int funct, imm, addr;
};
struct decode campos(int instrucao){
    struct decode c;

    c.opcode = (instrucao >> 12) & 0xF;
    c.rs = (instrucao >> 9) & 0x7;
    c.rt = (instrucao >> 6) & 0x7;
    c.rd = (instrucao >> 3) & 0x7;
    c.funct = instrucao & 0x7;
    c.imm = instrucao & 0x3F;
    c.addr = instrucao & 0xFF;

    if (c.imm >= 32) {
        c.imm -= 64;
    }
    return c;
}
void execute(struct decode c, int registradores[], int memoria_dados[], int *PC, int *aritmeticas, int *memoria_acesso){
    int flag_zero = 0;
    int endereco;
    switch(c.opcode) {
        case 0:
            (*aritmeticas)++;
            switch(c.funct) {
                case 0:
                    registradores[c.rd] = registradores[c.rs] + registradores[c.rt];
                    printf("ADD r%d = r%d + r%d\n", c.rd, c.rs, c.rt);
                    break;
                case 2:
                    registradores[c.rd] = registradores[c.rs] - registradores[c.rt];
                    printf("SUB r%d = r%d - r%d\n", c.rd, c.rs, c.rt);
                    break;
                case 4:
                    registradores[c.rd] = registradores[c.rs] & registradores[c.rt];
                    printf("AND r%d = r%d & r%d\n", c.rd, c.rs, c.rt);
                    break;
                case 5:
                    registradores[c.rd] = registradores[c.rs] | registradores[c.rt];
                    printf("OR r%d = r%d | r%d\n", c.rd, c.rs, c.rt);
                    break;
                default:
                    printf("FUNCT invalido!\n");
            }
            break;
        case 4:
            (*aritmeticas)++;
            registradores[c.rt] = registradores[c.rs] + c.imm;
            printf("ADDI r%d = r%d + %d\n", c.rt, c.rs, c.imm);
            break;
        case 11:
            endereco = registradores[c.rs] + c.imm;
            if (endereco >= 0 && endereco < 256) {
                (*memoria_acesso)++;
                registradores[c.rt] = memoria_dados[endereco];
                printf("LW r%d = MEM[%d]\n", c.rt, endereco);
            } else {
                printf("Erro de memoria (LW)\n");
            }
            break;
        case 15:
            endereco = registradores[c.rs] + c.imm;
            if (endereco >= 0 && endereco < 256) {
                (*memoria_acesso)++;
                memoria_dados[endereco] = registradores[c.rt];
                printf("SW MEM[%d] = r%d\n", endereco, c.rt);
            } else {
                printf("Erro de memoria (SW)\n");
            }
            break;
        case 8:
            int resultado = ULA(registradores[c.rs], registradores[c.rt], 2, &flag_zero);
            if(flag_zero) {
                *PC = *PC + c.imm + 1;
                printf("BEQ verdadeiro -> salto para %d\n", *PC);
                return;
            }
            printf("BEQ falso\n");
            break;
        case 2:
            *PC = c.addr;
            printf("JUMP para %d\n", c.addr);
            return; 
        default:
            printf("Instrucao invalida!\n");
    }
    (*PC)++;
}
void executar_programa(int memoria_instrucao[], int memoria_dados[], int registradores[]) {
    int PC = 0, total = 0, arit = 0, mem = 0;

    while (PC < 256) {
        int instrucao_atual = fetch(memoria_instrucao, PC);
        struct decode c = campos(instrucao_atual);
        printf("\nPC=%d | opcode=%d\n", PC, c.opcode);
        total++;
        execute(c, registradores, memoria_dados, &PC, &arit, &mem);
    }
    printf("\n-Estatisticas-\n");
    printf("Total: %d | Arit: %d | Mem: %d\n", total, arit, mem);
}
int ULA(int A, int B, int controle, int *flag) {
    int resultado = 0;
    switch(controle) {
        case 0: resultado = A + B; break;
        case 2: resultado = A - B; break;
        case 4: resultado = A & B; break;
        case 5: resultado = A | B; break;
        default: resultado = 0;
    }
    *flag = (resultado == 0);
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
    fclose(arquivo);
}
void imprimir_memoria(int memoria[]) {
    printf("\n- Conteudo da Memoria -\n");
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
        int funct = instrucao & 0x7;
        int imm = instrucao & 0x3F;
        int addr = instrucao & 0xFF;
        if (imm >= 32) imm -= 64;
        
     switch(opcode) {
            case 0:
                switch(funct) {
                    case 0: fprintf(arquivo, "ADD R%d R%d R%d\n", rd, rs, rt); break;
                    case 2: fprintf(arquivo, "SUB R%d R%d R%d\n", rd, rs, rt); break;
                    case 4: fprintf(arquivo, "AND R%d R%d R%d\n", rd, rs, rt); break;
                    case 5: fprintf(arquivo, "OR R%d R%d R%d\n", rd, rs, rt); break;
                    default: fprintf(arquivo, "NOP\n");
                }
                break;

            case 4:
                fprintf(arquivo, "ADDI R%d R%d %d\n", rt, rs, imm);
                break;

            case 11:
                fprintf(arquivo, "LW R%d %d(R%d)\n", rt, imm, rs);
                break;

            case 15:
                fprintf(arquivo, "SW R%d %d(R%d)\n", rt, imm, rs);
                break;

            case 8:
                fprintf(arquivo, "BEQ R%d R%d %d\n", rs, rt, imm);
                break;

            case 2:
                fprintf(arquivo, "J %d\n", addr);
                break;

            default:
                fprintf(arquivo, "NOP\n");
        }
    }
    fclose(arquivo);
    printf("Arquivo programa.asm salvo!\n");
}
