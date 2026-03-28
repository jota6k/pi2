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
        memoria[i] = (int)strol(linha, NULL, 2);
        i++;
    }

    fclose(arquivo);
}

void inicializar_registradores(int registradores[]) {
    for (int i = 0; i < 8; i++) {
        registradores[i] = 0;
    }
}
