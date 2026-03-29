#include <stdio.h>
#include "simulador.h"

int main() {
    int memoria[256];
    int registradores[8];
    int op;

    inicializar_registradores(registradores);
    leitura_arquivo_mem(memoria);

do{
    printf("\n  - Menu do Simulador \n");
    printf("1 - Executar programa\n");
    printf("2 - Mostrar memoria\n");
    printf("3 - Mostrar registradores\n");
    printf("0 - Sair\n");
    printf("Escolha: ");
    scanf("%d", &op);
switch(op) {
    case 1:
        executar_programa(memoria, registradores);
        break;
    case 2:
        for (int i = 0; i < 256; i++) {
           printf("Mem[%d] = %d\n", i, memoria[i]);
        }
           break;
    case 3:
        for (int i = 0; i < 8; i++) {
           printf("R%d = %d\n", i, registradores[i]);
        }
        break;
    case 0:
           printf("Programa encerrado!\n");
            break;
    default:
       printf("Opcao invalida!\n");
        }
    } while (op != 0);
    return 0;
}
