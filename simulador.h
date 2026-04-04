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
void leitura_arquivos_dados(int memoria_dados[]);
void inicializar_registradores(int registradores[]);
int ler_reg(int registradores[], int indice);
void escrever_reg(int registradores[], int indice, int valor);
int fetch(int memoria_instrucao[], int PC);
struct decode campos(int instrucao);
void execute(struct decode c, int registradores[], int memoria_dados[], int *PC, int *aritmeticas, int *memoria_acesso);
void executar_programa(int memoria_instrucao[], int memoria_dados[], int registradores[]);
int ULA(int A, int B, int controle, int *flag);
void imprimir_memoria(int memoria[]);
void salvar_arquivo_dat(int memoria_dados[]);
void salvar_asm(int memoria[]);
