#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#define NUM_REGISTRADORES 8

struct instrucao {
    char inst_char[17]; // 16 bits da instrução + "\0"
    int opcode;         // Operação principal
    int rs;             // Registrador Source 1
    int rt;             // Registrador Target / Source 2
    int rd;             // Registrador Destination
    int funct;          // Código da função (para instruções tipo R)
    int imm;            // Valor imediato (para instruções tipo I)
    int addr;           // Endereço de salto (para instruções tipo J)
};

// Estrutura para a memória de dados (Para uso futuro)
struct mem_dados {
    int dado;
};

// Estrutura para salvar o estado do simulador (Step/Back)
struct estado_salvo {
    int copia_PC;
    int copia_banco_de_registradores[NUM_REGISTRADORES];
    struct mem_dados copia_memoria[256];
};

// FUNÇÕES

// Função para ler o arquivo .mem
void Ler_Arquivo_Memoria(struct instrucao *memoria_instrucoes);

// Funções do Banco de Registradores
void inicializar_registradores(int banco_registradores[]);
int ler_registrador(const int banco_registradores[], int indice);
void escrever_registrador(int banco_registradores[], int indice, int valor);
void imprimir_registradores(const int banco_registradores[]);

// (As próximas etapas entrarão aqui conforme formos avançando)

#endif
