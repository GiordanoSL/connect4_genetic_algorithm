#include "AG.h"
#include "math.h"

#define TAM_POP 100
#define POP1 0
#define POP2 1

struct _individuo{
    float pesos_input[49][21];
    float pesos_intermed[21][7];
    char comeco; // (0 - 6) bias
};

IND * populacao1[TAM_POP];
IND * populacao2[TAM_POP];

CONNECT4 * jogos[TAM_POP]; // jogos[i][j] corresponde ao jogo em que o indivíduo populacao1[i] joga contra o indivíduo populacao2[j]

int fitness1[TAM_POP], 
    fitness2[TAM_POP],
    max_fit_ant1 = 0, 
    max_fit_ant2 = 0, 
    ger_rep1 = 0, 
    ger_rep2 = 0,
    penalidade[3] = {0, 1000, 15000};
float freq_mut1 = 0.05, max_mut1 = 0.01, freq_mut2 = 0.05, max_mut2 = 0.01; 

int quem_evolui = POP1;

int output(IND * individuo, CONNECT4 * jogo){
    const char * input = get_tabuleiro(jogo);
    float intermed[21];
    float output[7];
    int max_i = 0;

    for (int i = 0; i < 21; i++) {
        intermed[i] = 0;

        for (int j = 0; j < TAM_TAB*TAM_TAB; j++){
            intermed[i] += input[j]*individuo->pesos_input[j][i];   
        }
        
        intermed[i] = tanh(intermed[i]);// funcao de ativação
    }
    
    for (int i = 0; i < TAM_TAB; i++){
        output[i] = 0; // inicializa com 0 (neutro)

        // verifica se a coluna i do tabuleiro ta cheia
        if(input[42 + (i + individuo->comeco)%7] != 0){
            output[i] = (-__INT_MAX__); 
            continue;
        }

        for (int j = 0; j < 21; j++){
            output[i] += intermed[j]*individuo->pesos_intermed[j][i];   
        }

        if(output[i] > output[max_i]){
            max_i = i;
        }

    }

    if(input[42 + (max_i+individuo->comeco)%7] != 0){
        printf("[%.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f] max:, %d\n", output[0], output[1], output[2], output[3], output[4], output[5], output[6], max_i);
        print_tabuleiro(jogo);
        printf("%d\n", acabou(jogo));
        getchar();

    }
    return (max_i + individuo->comeco)%7;
}

IND * alloc_ind(){
    IND * ind = (IND *) malloc(sizeof(IND));
    if(ind == NULL){
        printf("Erro na alocação");
        exit(1);
    }
    srand(time(NULL));
    
    for (int i = 0; i < 49; i++)
        for (int j = 0; j < 21; j++)
            ind->pesos_input[i][j] = (rand()/(float)RAND_MAX) * 0.1;

    for (int i = 0; i < 21; i++)
        for (int j = 0; j < 7; j++)
            ind->pesos_intermed[i][j] = (rand()/(float)RAND_MAX) * 0.1;
        
    ind->comeco = rand()%7;

    return ind;
}

void init_populacao(){
    for (int i = 0; i < TAM_POP; i++){
        populacao1[i] = alloc_ind();
        populacao2[i] = alloc_ind();
        if((jogos[i] = novo_jogo()) == NULL){
            printf("Erro na alocação do jogo");
            exit(1);
        }
    }
    
}
void read_pesos_input(FILE * fp, IND ** ind){
    for (int i = 0; i < 49; i++)
    {
        size_t result = fread(&((*ind)->pesos_input[i]), sizeof(float), 21, fp);
        if (result != 21) {
            printf("Erro ao ler pesos_input[%d], esperado 21 elementos, mas leu %zu\n", i, result);
            // Tratamento de erro, como retornar ou abortar
        }
    }
}

void read_pesos_intermed(FILE * fp, IND ** ind){
    for (int i = 0; i < 21; i++)
    {
        size_t result = fread(&((*ind)->pesos_intermed[i]), sizeof(float), 7, fp);
        if (result != 7) {
            printf("Erro ao ler pesos_intermed[%d], esperado 7 elementos, mas leu %zu\n", i, result);
            // Tratamento de erro, como retornar ou abortar
        }
    }
}

void read_ind(const char * pesos_file, IND ** ind){
    FILE * file = fopen(pesos_file, "rb");

    if(file == NULL){
        printf("ERRO AO ABRIR ARQUIVO\n");
        return;
    }

    read_pesos_input(file, ind);
    read_pesos_intermed(file, ind);

    size_t result = fread(&((*ind)->comeco), sizeof(int), 1, file);
    if (result != 1) {
        printf("Erro ao ler o campo 'comeco', esperado 1 elemento, mas leu %zu\n", result);
        // Tratamento de erro, como retornar ou abortar
    }

    fclose(file);
}

int calcular_penalidade(int cor, CONNECT4 * game){
    const char * tabuleiro = get_tabuleiro(game);
    int x, y, result = 0, seq;

    for (x = 0; x < TAM_TAB;x++){
        y = 0;
        while (y < TAM_TAB){
            if (tabuleiro[y*TAM_TAB + x] == 0)
                break;
            if (tabuleiro[y*TAM_TAB + x] == cor){
                y++;
                continue;
            }

            seq = 0;
            if(y < TAM_TAB - 2){
                if(tabuleiro[y*TAM_TAB + x] == tabuleiro[(y+1)*TAM_TAB + x]){
                    seq++;
                    if(tabuleiro[y*TAM_TAB + x] == tabuleiro[(y+2)*TAM_TAB + x])
                        seq++;
                }
            }else if(y < TAM_TAB -1){
                if(tabuleiro[y*TAM_TAB + x] == tabuleiro[(y+1)*TAM_TAB + x])
                    seq++;
            }
            result += penalidade[seq];

            seq = 0;
            if(x < TAM_TAB - 2){
                if(tabuleiro[y*TAM_TAB + x] == tabuleiro[y*TAM_TAB + x+1]){
                    seq++;
                    if(tabuleiro[y*TAM_TAB + x] == tabuleiro[y*TAM_TAB + x+2])
                        seq++;
                }
            }else if(x < TAM_TAB -1){
                if(tabuleiro[y*TAM_TAB + x] == tabuleiro[y*TAM_TAB + x+1])
                    seq++;
            }
            result += penalidade[seq];


            seq = 0;
            if(x < TAM_TAB - 2 && y < TAM_TAB - 2){
                if(tabuleiro[y*TAM_TAB + x] == tabuleiro[(y+1)*TAM_TAB + x+1]){
                    seq++;
                    if(tabuleiro[y*TAM_TAB + x] == tabuleiro[(y+2)*TAM_TAB + x+2])
                        seq++;
                }
            }else if(x < TAM_TAB -1 && y < TAM_TAB - 1){
                if(tabuleiro[y*TAM_TAB + x] == tabuleiro[(y+1)*TAM_TAB + x+1])
                    seq++;
            }
            result += penalidade[seq];

            seq = 0;
            if(x < TAM_TAB - 2 && y > 1){
                if(tabuleiro[y*TAM_TAB + x] == tabuleiro[(y-1)*TAM_TAB + x+1]){
                    seq++;
                    if(tabuleiro[y*TAM_TAB + x] == tabuleiro[(y-2)*TAM_TAB + x+2])
                        seq++;
                }

            }else if(x < TAM_TAB -1 && y > 0){
                if(tabuleiro[y*TAM_TAB + x] == tabuleiro[(y-1)*TAM_TAB + x+1])
                    seq++;
            }
            result += penalidade[seq];

            y++;
        }
    }
    return result;
}

void avaliacao(){

    if(quem_evolui == POP1){
        for (int i = 0; i < TAM_POP; i++){
            fitness1[i] = 0;
            reset(jogos[i]);
            while (1){
                if(adicionar_peca(jogos[i], output(populacao1[i], jogos[i])) == -1){
                    printf("INDIVIDUO %d DA POPULAÇÃO 1 TENTOU INSERIR PEÇA EM LUGAR INDEVIDO\n", i);
                    exit(1);
                }
                if(acabou(jogos[i]))
                    break;
                if(adicionar_peca(jogos[i], output(populacao2[0], jogos[i])) == -1){
                    printf("INDIVIDUO %d DA POPULAÇÃO 2 TENTOU INSERIR PEÇA EM LUGAR INDEVIDO\n", i);
                    exit(1);
                }
                if(acabou(jogos[i]))
                    break;
            }

            fitness1[i] += calcular_penalidade(-1, jogos[i]);
            fitness1[i] -= calcular_penalidade(1, jogos[i]);
            fitness1[i] += 49000*get_vencedor(jogos[i]);
                    
        }
    }else{
        for (int i = 0; i < TAM_POP; i++){
            fitness2[i] = 0;
            reset(jogos[i]);
            while (1){
                if(adicionar_peca(jogos[i], output(populacao1[0], jogos[i])) == -1){
                    printf("INDIVIDUO %d DA POPULAÇÃO 1 TENTOU INSERIR PEÇA EM LUGAR INDEVIDO\n", i);
                    exit(1);
                }
                if(acabou(jogos[i]))
                    break;
                if(adicionar_peca(jogos[i], output(populacao2[i], jogos[i])) == -1){
                    printf("INDIVIDUO %d DA POPULAÇÃO 2 TENTOU INSERIR PEÇA EM LUGAR INDEVIDO\n", i);
                    exit(1);
                }
                if(acabou(jogos[i]))
                    break;
            }

            fitness2[i] += calcular_penalidade(1, jogos[i]);
            fitness2[i] -= calcular_penalidade(-1, jogos[i]);
            fitness2[i] -= 49000*get_vencedor(jogos[i]);        
        }
    }
}

void reproducao(){
    int max_i_1 = 0, max_i_2 = 0;

    for (int i = 0; i < TAM_POP; i++){
        if (fitness1[i] > fitness1[max_i_1]) max_i_1 = i;
        if (fitness2[i] > fitness2[max_i_2]) max_i_2 = i;
    }

    if(max_i_1 == 0)
        ger_rep1++;
    else
        ger_rep1 = 0;

    if(max_i_2 == 0)
        ger_rep2++;
    else    
        ger_rep2 = 0;
    
    if(ger_rep1 > 3 && quem_evolui == POP1){
        if(fitness1[max_i_1] > 0){
            printf("\nAAA\n");
            quem_evolui = POP2;
            ger_rep2 = 0;
            freq_mut2 = 0.05;
        }else if(freq_mut1 < 0.5)
            freq_mut1 += 0.02;
        else if(max_mut1 < 0.1)
            max_mut1 += 0.01;
    }
    if(ger_rep2 > 3 && quem_evolui == POP2){
            printf("\nBBB\n");
        if(fitness2[max_i_2] > 0){
            quem_evolui = POP1;
            ger_rep1 = 0;
            freq_mut1 = 0.05;
            max_mut1 = 0.01;
        }else if(freq_mut2 < 0.5)
            freq_mut2 += 0.02;
        else if(max_mut2 < 0.1)
            max_mut2 += 0.01;
    }

    std::swap(populacao1[0], populacao1[max_i_1]); max_i_1 = 0;
    std::swap(populacao2[0], populacao2[max_i_2]); max_i_2 = 0;
    
    for (int i = 1; i < TAM_POP; i++){
        if(quem_evolui == POP1){
            for (int j = 0; j < 49; j++){
                for (int k = 0; k < 21; k++){
                    populacao1[i]->pesos_input[j][k] = populacao1[0]->pesos_input[j][k];
                    if((rand()%100)/100.0 < freq_mut1)
                        populacao1[i]->pesos_input[j][k] += ((rand()%(int) (max_mut1*2000)) - (max_mut1*1000))/1000.0;
                }
            }
            for (int j = 0; j < 21; j++){
                for (int k = 1; k < 7; k++) {
                    populacao1[i]->pesos_intermed[j][k] = populacao1[0]->pesos_intermed[j][k];
                    if((rand()%100)/100.0 < freq_mut1)
                        populacao1[i]->pesos_intermed[j][k] += ((rand()%(int) (max_mut1*2000)) - (max_mut1*1000))/1000.0;
                }
            }
            if((rand()%100)/100.0 < freq_mut1)
                populacao1[i]->comeco = rand()%7;
            
        }else if(quem_evolui == POP2){
            for (int j = 0; j < 49; j++){
                for (int k = 0; k < 21; k++){
                    populacao2[i]->pesos_input[j][k] = populacao2[0]->pesos_input[j][k];
                    if((rand()%100)/100.0 < freq_mut2)
                        populacao2[i]->pesos_input[j][k] += ((rand()%(int) (max_mut2*2000)) - (max_mut2*1000))/1000.0;
                }
            }
            for (int j = 0; j < 21; j++){
                for (int k = 1; k < 7; k++) {
                    populacao2[i]->pesos_intermed[j][k] = populacao2[0]->pesos_intermed[j][k];
                    if((rand()%100)/100.0 < freq_mut2)
                        populacao2[i]->pesos_intermed[j][k] += ((rand()%(int) (max_mut1*2000)) - (max_mut1*1000))/1000.0;
                }
            }
            
            if((rand()%100)/100.0 < freq_mut2)
                populacao2[i]->comeco = rand()%7;
        }
    }
    printf("max_i_1: %d, max_i_2: %d\n", fitness1[max_i_1], fitness2[max_i_2]);
}

void evoluir(int gen){
    for (int i = 0; i < gen; i++){
        printf("gen %d = ", i);
        avaliacao();
        reproducao();
    }
}

void finaliza_evolucao(IND ** player1, IND ** player2){
    *player1 = populacao1[0];
    *player2 = populacao2[0];

    for (int i = 1; i < TAM_POP; i++){
        free(populacao1[i]);
        free(populacao2[i]);
    }

    for (int i = 0; i < TAM_POP; i++)
        fim_jogo(&jogos[i]);   
            
    write_ind("player1.bin", *player1);
    write_ind("player2.bin", *player2);
}

void write_pesos_input(FILE * fp, IND * ind){
    for (int i = 0; i < 49; i++)
    {
        fwrite(ind->pesos_input[i], sizeof(float), 21, fp);
    }
}

void write_pesos_intermed(FILE * fp, IND * ind){
    for (int i = 0; i < 21; i++)
    {
        fwrite(ind->pesos_intermed[i], sizeof(float), 7, fp);
    }
}

void write_ind(const char * pesos_file, IND * ind){
    FILE * file = fopen(pesos_file, "wb");

    if(file == NULL){
        printf("ERRO AO ABRIR ARQUIVO");
        return;
    }

    write_pesos_input(file, ind);
    write_pesos_intermed(file, ind);

    fwrite(&(ind->comeco), sizeof(int), 1, file);

    fclose(file);
}

void get_best_ai(IND ** p1, IND ** p2){
    *p1 = populacao1[0];
    *p2 = populacao2[0];
}
    