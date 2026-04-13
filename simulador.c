#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simulador.h"

void escolher_arquivo_mem(char nome_arquivo[]){
    FILE *arquivo;
    do {
        printf("Digite o nome do arquivo .mem: ");
        scanf("%s", nome_arquivo);

        arquivo = fopen(nome_arquivo, "r");
        if (arquivo == NULL){
            printf("Arquivo .mem não encontrado.\n");
        }
    } while (arquivo == NULL);
    printf("Arquivo .mem carregado...\n");
    fclose(arquivo);
}

void escolher_arquivo_dat(char nome_arquivo[]){
    FILE *arquivo;
    do {
        printf("Digite o nome do arquivo .dat: ");
        scanf("%s", nome_arquivo);

        arquivo = fopen(nome_arquivo, "r");
        if (arquivo == NULL) {
            printf("Arquivo .dat nao encontrado\n");
        }
    } while (arquivo == NULL);
    printf("Arquivo .dat carregado...\n");
    fclose(arquivo);
}

void leitura_arquivo_mem(int memoria[], char nome_arquivo[]) {
    FILE *arquivo = fopen(nome_arquivo, "r");
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

void leitura_arquivos_dados(int memoria_dados[], char nome_arquivo[]) {
    int i = 0;
    FILE *arquivo = fopen(nome_arquivo, "r");

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

struct decode campos(int instrucao){
    struct decode c;

    c.opcode = (instrucao >> 12) & 0xF;
    c.rs = (instrucao >> 9) & 0x7;
    c.rt = (instrucao >> 6) & 0x7;
    c.rd = (instrucao >> 3) & 0x7;
    c.funct = instrucao & 0x7;
    c.imm = instrucao & 0x3F;
    c.addr = instrucao & 0xFF;

    if (c.imm >= 32) { // < ------------ extensao de sinal
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
                    registradores[c.rd] = ULA(registradores[c.rs], registradores[c.rt], 0, &flag_zero);
                    printf("ADD r%d = r%d + r%d\n", c.rd, c.rs, c.rt);
                    break;
                case 2:
                    registradores[c.rd] = ULA(registradores[c.rs], registradores[c.rt], 2, &flag_zero);
                    printf("SUB r%d = r%d - r%d\n", c.rd, c.rs, c.rt);
                    break;
                case 4:
                    registradores[c.rd] = ULA(registradores[c.rs], registradores[c.rt], 4, &flag_zero);
                    printf("AND r%d = r%d & r%d\n", c.rd, c.rs, c.rt);
                    break;
                case 5:
                    registradores[c.rd] = ULA(registradores[c.rs], registradores[c.rt], 5, &flag_zero);
                    printf("OR r%d = r%d | r%d\n", c.rd, c.rs, c.rt);
                    break;
                default:
                    printf("FUNCT invalido!\n");
            }
            break;
        case 2:
            *PC = c.addr;
            printf("JUMP para %d\n", c.addr);
            return; 
        case 4:
            (*aritmeticas)++;
            registradores[c.rt] = registradores[c.rs] + c.imm;
            printf("ADDI r%d = r%d + %d\n", c.rt, c.rs, c.imm);
            break;
        case 8: {
            ULA(registradores[c.rs], registradores[c.rt], 2, &flag_zero);
            if(flag_zero) {
                *PC = *PC + c.imm + 1;
                printf("BEQ verdadeiro -> salto para %d\n", *PC);
                return;
            }
        }
            printf("BEQ falso\n");
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
        default:
            printf("Instrucao invalida!\n");
    }
    (*PC)++;
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

struct EstadoMaquina historico[999]; 
int passo_atual = 0; 

void salvar_estado(int PC_atual, int registradores[], int memoria_dados[]) {
    historico[passo_atual].PC = PC_atual;
    for(int i = 0; i < 8; i++) historico[passo_atual].registradores[i] = registradores[i];
    for(int i = 0; i < 256; i++) historico[passo_atual].memoria_dados[i] = memoria_dados[i];
}

void step(int memoria_instrucao[], int memoria_dados[], int registradores[], int *PC, int *arit, int *mem) {
    int instrucao_atual = fetch(memoria_instrucao, *PC);

    if (instrucao_atual == 0 || *PC >= 256) {
        printf("Nao ha mais instrucoes para executar.\n");
        return;
    }

    salvar_estado(*PC, registradores, memoria_dados);
    passo_atual++;

    struct decode c = campos(instrucao_atual);
    printf("\n===== STEP =====\n");
    printf("\n===== STEP ATUAL =====\n");
    printf("Instrucao atual (PC=%d): %d\n", *PC, instrucao_atual);
    printf("Opcode: %d\n", c.opcode);
    execute(c, registradores, memoria_dados, PC, arit, mem);
}

void back(int registradores[], int memoria_dados[], int *PC) {
    if (passo_atual > 0) {
        passo_atual--;
        
        *PC = historico[passo_atual].PC;
        for(int i = 0; i < 8; i++) registradores[i] = historico[passo_atual].registradores[i];
        for(int i = 0; i < 256; i++) memoria_dados[i] = historico[passo_atual].memoria_dados[i];
        
        printf("\nBack executado, PC restaurado para %d\n", *PC);
    } else {
        printf("\nErro: vc esta no inicio do programa!\n");
    }
}
void run(int memoria_instrucao[], int memoria_dados[], int registradores[], int *PC, int *arit, int *mem) {
    int total = 0;
    printf("\n--- Executando ---\n");

    while (*PC < 256 && fetch(memoria_instrucao, *PC) != 0) {
        if (passo_atual >= 998) {
            printf("\nO programa esta em loop, forcando parada.");
            break;
        }

        step(memoria_instrucao, memoria_dados, registradores, PC, arit, mem);
        total++;
    }
    printf("\nTotal de Instrucoes: %d", total);
    printf("\nAritmeticas: %d", *arit);
    printf("\nMemoria: %d", *mem);
    printf("\nPC Final: %d\n", *PC);
}
