#define BIBLIOTECA_H

// Estrutura de dados das instrucoes

struct instrucoes {
    int opcode;
    int rs;
    int rt;
    int rd;
    int funct;
    int imm;
    int addr;
};

void leitura_arquivo_mem(int memoria[]);

void inicializar_registradores(int registradores[]);
