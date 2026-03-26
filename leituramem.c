#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAM_MEM 256

enum classe_inst{                         
    tipo_I,
    tipo_J,
    tipo_R,
    tipo_OUTROS
};

struct instrucao{                          
    enum classe_inst tipo_inst;

    char inst_char[17];
    int opcode;
    int rs;
    int rt;
    int rd;
    int funct;
    int imm;
    int addr;
};
struct instrucao novainst(){                         
    struct instrucao inst;

    inst.tipo_inst = tipo_OUTROS;
    inst.opcode = 0;
    inst.rs = 0;
    inst.rt = 0;
    inst.rd = 0;
    inst.funct = 0;
    inst.imm = 0;
    inst.addr = 0;

    strcpy(inst.inst_char, "");                
    return inst;        
}
struct memoria_instrucao{                       //Vetor de instruções
    struct instrucao *mem_inst;
    int tamanho;
};
struct memoria_instrucao criar_mem(){           //Faz a alocação da mem de instruções
    struct memoria_instrucao mem;

    mem.mem_inst = (struct instrucao*) malloc(sizeof(struct instrucao)*TAM_MEM);
    mem.tamanho = TAM_MEM;

    return mem;
}
void iniciar_mem(struct memoria_instrucao *mem){
    for(int i = 0; i < mem->tamanho; i++){
    mem->mem_inst[i] = novainst();
    }
}
int inserir_inst(struct memoria_instrucao *mem, struct instrucao inst, int pos){
    if(pos >= mem->tamanho){
        printf("Memoria cheia!\n");
        return 0;
    }
    mem->mem_inst[pos] = inst;
    return 1;
}
void carregar_mem(struct memoria_instrucao *mem, char *arquivo){        //Le o arquivo .mem e carrega as instruções na mem
    FILE *f = fopen(arquivo, "r");
    if(f == NULL){
        printf("Erro ao abrir!\n");
        return;
    }
char linha[20];
int i = 0;

while(fgets(linha, sizeof(linha), f)){
    linha[strcspn(linha, "\n")] = '\0';

    struct instrucao inst = novainst();
    strcpy(inst.inst_char, linha);
    inserir_inst(mem, inst, i);
    i++;
}
    fclose(f);
}

int main(){

    struct memoria_instrucao mem;

    mem = criar_mem();
    iniciar_mem(&mem);

    carregar_mem(&mem, "teste.mem");

    for(int i = 0; i < 5; i++){
        printf("Pos %d: %s\n", i, mem.mem_inst[i].inst_char);
    }

    return 0;
}

/*void imprimir(struct instrucao inst){
    printf("-- INSTRUCAO --\n");
    printf("Binario: %s\n", inst.inst_char);
    printf("Opcode: %d\n", inst.opcode);
    printf("rs: %d rt: %d rd: %d\n", inst.rs, inst.rt, inst.rd);
    printf("Funct: %d", inst.funct);
    printf("Imediato: %d\n", inst.imm);
    printf("Addr: %d", inst.addr);
}*/


