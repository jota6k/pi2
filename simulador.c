#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simulador.h"

void Ler_Arquivo_Memoria(struct instrucao *memoria_instrucoes) {
    char caminho_arquivo[128];
    printf("Informe o nome/caminho do arquivo .mem: ");
    scanf("%127s", caminho_arquivo);

    FILE *ptr_arquivo = fopen(caminho_arquivo, "r");
    if (ptr_arquivo == NULL) {
        printf("Arquivo '%s' nao foi localizado.\n", caminho_arquivo);
        return;
    }

    printf("Arquivo aberto. Carregando instrucoes...\n");

    for (int i = 0; i < 256; i++) {
        memoria_instrucoes[i].opcode = -1;
    }

    char linha_buffer[32];
    int indice_atual = 0;

    while (fgets(linha_buffer, sizeof(linha_buffer), ptr_arquivo)) {
        linha_buffer[strcspn(linha_buffer, "\r\n")] = '\0';

        if (strlen(linha_buffer) != 16) {
            continue;
        }

        strcpy(memoria_instrucoes[indice_atual].inst_char, linha_buffer);

        char str_opcode[5] = {0};
        char str_rs[4] = {0}, str_rt[4] = {0}, str_rd[4] = {0}, str_funct[4] = {0};
        char str_imm[7] = {0};
        char str_addr[8] = {0}; 

        strncpy(str_opcode, linha_buffer, 4);
        strncpy(str_rs, linha_buffer + 4, 3);
        strncpy(str_rt, linha_buffer + 7, 3);
        strncpy(str_rd, linha_buffer + 10, 3);
        strncpy(str_funct, linha_buffer + 13, 3);
        strncpy(str_imm, linha_buffer + 10, 6);
        strncpy(str_addr, linha_buffer + 9, 7);

        int valor_opcode = (int)strtol(str_opcode, NULL, 2);
        memoria_instrucoes[indice_atual].opcode = valor_opcode;

        switch (valor_opcode) {
            case 0: // Instruções do TIPO R (ADD, SUB, AND, OR)
                memoria_instrucoes[indice_atual].rs = (int)strtol(str_rs, NULL, 2);
                memoria_instrucoes[indice_atual].rt = (int)strtol(str_rt, NULL, 2);
                memoria_instrucoes[indice_atual].rd = (int)strtol(str_rd, NULL, 2);
                memoria_instrucoes[indice_atual].funct = (int)strtol(str_funct, NULL, 2);
                break;

            case 2: // Instruções do TIPO J (JUMP)
                memoria_instrucoes[indice_atual].addr = (int)strtol(str_addr, NULL, 2);
                break;

            case 4:  // ADDI (TIPO I)
            case 8:  // BEQ  (TIPO I)
            case 11: // LW   (TIPO I)
            case 15: // SW   (TIPO I)
                memoria_instrucoes[indice_atual].rs = (int)strtol(str_rs, NULL, 2);
                memoria_instrucoes[indice_atual].rt = (int)strtol(str_rt, NULL, 2);
                
                int valor_imediato = (int)strtol(str_imm, NULL, 2);
                
                if (str_imm[0] == '1') {
                    valor_imediato = valor_imediato ^ 63; 
                    valor_imediato = (valor_imediato + 1) * (-1);
                }
                
                memoria_instrucoes[indice_atual].imm = valor_imediato;
                break;

            default:
                printf("Opcode nao reconhecido: %d na linha %d\n", valor_opcode, indice_atual + 1);
                break;
        }

        indice_atual++;

        if (indice_atual >= 256) {
            printf("Aviso: Memoria cheia. Limite de 256 instrucoes atingido.\n");
            break;
        }
    }

    fclose(ptr_arquivo);
    printf("Memoria de instrucoes processada com exito.\n");
}
