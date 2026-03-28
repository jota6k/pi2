#include <stdio.h>
#include "simulador.h"

int main() {
    int memoria[256];
    int registradores[8];

    inicializar_registradores(registradores);

    leitura_arquivo_mem(memoria);

    return 0;
}
