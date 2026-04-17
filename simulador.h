#ifndef SIMULADOR_H
#define SIMULADOR_H

struct decode {
    int opcode;
    int rs;
    int rt;
    int rd;
    int funct;
    int imm;
    int addr;
};

struct EstadoMaquina {
    int PC;
    int registradores[8];
    int memoria_dados[256];
};

void escolher_arquivo_mem(char nome_arquivo[]);
void escolher_arquivo_dat(char nome_arquivo[]);
void leitura_arquivo_mem(int memoria[], char nome_arquivo[]);
void leitura_arquivos_dados(int memoria_dados[], char nome_arquivo[]);
void inicializar_registradores(int registradores[]);

int fetch(int memoria_instrucao[], int PC);
struct decode campos(int instrucao);
void execute(struct decode c, int registradores[], int memoria_dados[], int *PC, int *aritmeticas, int *memoria_acessom, int *jumps);
int ULA(int A, int B, int controle, int *flag);
int controle_ULA(int opcode, int funct);
void imprimir_memoria_instrucoes(int memoria[]);
void imprimir_memoria_dados(int memoria[]);
void salvar_arquivo_dat(int memoria_dados[]);
void salvar_asm(int memoria[]);
void imprimir_registradores(int registradores[], int PC);

void salvar_estado(int PC_atual, int registradores[], int memoria_dados[], struct EstadoMaquina historico[], int *passo_atual);
void step(int memoria_instrucao[], int memoria_dados[], int registradores[], int *PC, int *arit, int *mem, struct EstadoMaquina historico[], int *passo_atual, int *jumps);
void back(int registradores[], int memoria_dados[], int *PC, struct EstadoMaquina historico[], int *passo_atual);
void run(int memoria_instrucao[], int memoria_dados[], int registradores[], int *PC, int *arit, int *mem, struct EstadoMaquina historico[], int *passo_atual, int *jumps);

#endif
