#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simulador.h"

void escolher_arquivo_mem(char nome_arquivo[]){
    FILE *arquivo;

    printf("\nDigite o nome do arquivo .mem: ");
    scanf("%s", nome_arquivo);
    arquivo = fopen(nome_arquivo, "r");
    printf("Arquivo carregado.\n");
    if (arquivo == NULL) {
        printf("Erro: o arquivo %s nao foi encontrado.\n", nome_arquivo);
    }

    fclose(arquivo);
}

void escolher_arquivo_dat(char nome_arquivo[]){
    FILE *arquivo;

    printf("\nDigite o nome do arquivo .dat: ");
    scanf("%s", nome_arquivo);
    arquivo = fopen(nome_arquivo, "r");
    printf("Arquivo carregado.\n");
    if (arquivo == NULL) {
        printf("Erro: o arquivo %s nao foi encontrado.\n", nome_arquivo);
    }

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

    if (c.imm >= 32) { // extensao de sinal
        c.imm -= 64; 
    }
    
    return c;
}

void execute(struct decode c, int registradores[], int memoria_dados[], int *PC, int *aritmeticas, int *memoria_acesso){
    int flag_zero = 0;
    int endereco;
    int ctrl = controle_ULA(c.opcode, c.funct);

    switch(c.opcode) {
        case 0: // R-type
            (*aritmeticas)++;

            if (ctrl == -1) {
                printf("Operacao invalida!\n");
                break;
            }
            registradores[c.rd] = ULA(registradores[c.rs], registradores[c.rt], ctrl, &flag_zero);

            switch(c.funct) {
                case 0: printf("ADD r%d = r%d + r%d\n", c.rd, c.rs, c.rt); break;
                case 2: printf("SUB r%d = r%d - r%d\n", c.rd, c.rs, c.rt); break;
                case 4: printf("AND r%d = r%d & r%d\n", c.rd, c.rs, c.rt); break;
                case 5: printf("OR r%d = r%d | r%d\n", c.rd, c.rs, c.rt); break;
            }
            break;
        case 2: // JUMP
            *PC = c.addr;
            printf("JUMP para %d\n", c.addr);
            return;
        case 4: // ADDI
            (*aritmeticas)++;
            registradores[c.rt] = ULA(registradores[c.rs], c.imm, ctrl, &flag_zero);
            printf("ADDI r%d = r%d + %d\n", c.rt, c.rs, c.imm);
            break;
        case 8: // BEQ
            ULA(registradores[c.rs], registradores[c.rt], ctrl, &flag_zero);

            if(flag_zero) {
                *PC = (*PC + 1) + c.imm;
                printf("BEQ verdadeiro -> salto para %d\n", *PC);
                return;
            }

            printf("BEQ falso\n");
            break;
        case 11: // LW
            endereco = ULA(registradores[c.rs], c.imm, ctrl, &flag_zero);

            if (endereco >= 0 && endereco < 256) {
                (*memoria_acesso)++;
                registradores[c.rt] = memoria_dados[endereco];
                printf("LW r%d = MEM[%d]\n", c.rt, endereco);
            } else {
                printf("Erro de memoria (LW)\n");
            }
            break;
        case 15: // SW
            endereco = ULA(registradores[c.rs], c.imm, ctrl, &flag_zero);

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
int controle_ULA(int opcode, int funct) {
    switch(opcode) {
    case 0: // tipo R
        switch(funct) {
            case 0: return 0; // ADD
            case 2: return 2; // SUB
            case 4: return 4; // AND
            case 5: return 5; // OR
            default: return -1;
            }
    case 11: // LW
    case 15: // SW
    case 4:  // ADDI
        return 0;
    case 8: // BEQ
        return 2; // sub para fazer a comparação
    default:
        return -1;
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
    }
    
    *flag = (resultado == 0);
    
    if (resultado > 127 || resultado < -128) {
        printf("Overflow.\n");
    }
    
    return resultado;
}

void imprimir_memoria_instrucoes(int memoria[]) {
    printf("\n--- Memoria de Instrucoes (Binario) ---\n");
    for(int i = 0; i < 256; i++) {
        int instrucao = memoria[i];
        printf("Mem[%d] = ", i); 
        for (int b = 15; b >= 0; b--) {
            int bit = (memoria[i] >> b) & 1;
            printf("%d", bit);
        }
        struct decode c = campos(instrucao);

printf("  |  ");
    switch(c.opcode) {
        case 0:
    switch(c.funct) {
        case 0:
            printf("ADD r%d = r%d + r%d", c.rd, c.rs, c.rt);
            break;
        case 2:
            printf("SUB r%d = r%d - r%d", c.rd, c.rs, c.rt);
            break;
        case 4:
            printf("AND r%d = r%d & r%d", c.rd, c.rs, c.rt);
            break;
        case 5:
            printf("OR r%d = r%d | r%d", c.rd, c.rs, c.rt);
            break;
        default:
            printf("Tipo R invalido");
}
            break;
        case 4:
            printf("ADDI r%d = r%d + %d", c.rt, c.rs, c.imm);
            break;

        case 11:
            printf("LW r%d = MEM[%d]", c.rt, c.rs + c.imm);
            break;

        case 15:
            printf("SW MEM[%d] = r%d", c.rs + c.imm, c.rt);
            break;

        case 8:
            printf("BEQ r%d == r%d (salto %d)", c.rs, c.rt, c.imm);
            break;

        case 2:
            printf("JUMP %d", c.addr);
            break;
        default:
                printf("Instrucao invalida");
        }
        printf("\n");
    }
}
void imprimir_memoria_dados(int memoria[]) {
    printf("\n--- Memoria de Dados (Decimal) ---\n");
    for(int i = 0; i < 256; i++) {
        printf("Mem[%d] = %d\n", i, memoria[i]);
    }
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

void imprimir_registradores(int registradores[], int PC) {
    printf("\nBanco de Registradores\n");
    for (int i = 0; i < 8; i++) {
        printf("$%d = %d\n", i, registradores[i]);
    }
    printf("PC = %d\n", PC);
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
        
        if (imm >= 32) {
            imm -= 64;
        }
        
        switch(opcode) {
            case 0:
                switch(funct) {
                    case 0: 
                        fprintf(arquivo, "add r%d, r%d, r%d\n", rd, rs, rt); 
                        break;
                    case 2: 
                        fprintf(arquivo, "sub r%d, r%d, r%d\n", rd, rs, rt); 
                        break;
                    case 4: 
                        fprintf(arquivo, "and r%d, r%d, r%d\n", rd, rs, rt); 
                        break;
                    case 5: 
                        fprintf(arquivo, "or r%d, r%d, r%d\n", rd, rs, rt); 
                        break;
                    default: 
                        fprintf(arquivo, "NOP\n");
                }
                break;
                
            case 4: 
                fprintf(arquivo, "addi r%d, r%d, %d\n", rt, rs, imm); 
                break;
                
            case 11: 
                fprintf(arquivo, "lw r%d, %d(%d)\n", rt, imm, rs); 
                break;
                
            case 15: 
                fprintf(arquivo, "sw r%d, %d(%d)\n", rt, imm, rs); 
                break;
                
            case 8: 
                fprintf(arquivo, "beq r%d, r%d, %d\n", rs, rt, imm); 
                break;
                
            case 2: 
                fprintf(arquivo, "j %d\n", addr); 
                break;
                
            default: 
                fprintf(arquivo, "nop\n");
        }
    }
    
    fclose(arquivo);
    printf("Arquivo programa.asm salvo!\n");
}

struct EstadoMaquina historico[999]; 
int passo_atual = 0; 

void salvar_estado(int PC_atual, int registradores[], int memoria_dados[]) {
    if (passo_atual >= 998) {
        return;
    }
    
    historico[passo_atual].PC = PC_atual;
    
    for(int i = 0; i < 8; i++) {
        historico[passo_atual].registradores[i] = registradores[i];
    }
    
    for(int i = 0; i < 256; i++) {
        historico[passo_atual].memoria_dados[i] = memoria_dados[i];
    }
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
    printf("\n[STEP] PC=%d | Opcode=%d\n", *PC, c.opcode);
    execute(c, registradores, memoria_dados, PC, arit, mem);
}

void back(int registradores[], int memoria_dados[], int *PC) {
    if (passo_atual > 0) {
        passo_atual--;
        
        *PC = historico[passo_atual].PC;
        
        for(int i = 0; i < 8; i++) {
            registradores[i] = historico[passo_atual].registradores[i];
        }
        
        for(int i = 0; i < 256; i++) {
            memoria_dados[i] = historico[passo_atual].memoria_dados[i];
        }
        
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
