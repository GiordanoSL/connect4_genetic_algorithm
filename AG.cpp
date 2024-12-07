#include "AG.h"
#include "math.h"

#define TAM_POP 100

struct _individuo{
    float pesos_input[49][21];
    float pesos_intermed[21][7];
    char comeco; // (0 - 6) bias
};

IND * populacao1[TAM_POP];
IND * populacao2[TAM_POP];

CONNECT4 * jogos[TAM_POP][TAM_POP]; // jogos[i][j] corresponde ao jogo em que o indivíduo populacao1[i] joga contra o indivíduo populacao2[j]

int fitness1[TAM_POP], fitness2[TAM_POP], max_fit_ant1 = 0, max_fit_ant2 = 0, ger_rep1 = 0, ger_rep2 = 0;
float freq_mut1 = 0.05, max_mut1 = 0.01, freq_mut2 = 0.05, max_mut2 = 0.01; 

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
        for (int j = 0; j < TAM_POP; j++){
            if((jogos[i][j] = novo_jogo()) == NULL){
                printf("Erro na alocação do jogo");
                exit(1);
            }
        }
    }
    
}

void avaliacao(){
    char vencedor, cor, seq;
    const char * tabuleiro;

    // fitness inicializa com 0
    for (int i = 0; i < TAM_POP; i++){
        fitness1[i] = 0;
        fitness2[i] = 0;
    }

    for (int i = 0; i < TAM_POP; i++){
        for (int j = 0; j < TAM_POP; j++){
            while (1){
                if(adicionar_peca(jogos[i][j], output(populacao1[i], jogos[i][j])) == -1){
                    printf("INDIVIDUO %d DA POPULAÇÃO 1 TENTOU INSERIR PEÇA EM LUGAR INDEVIDO\n", i);
                    exit(1);
                }
                if(acabou(jogos[i][j]))
                    break;
                if(adicionar_peca(jogos[i][j], output(populacao2[i], jogos[i][j])) == -1){
                    printf("INDIVIDUO %d DA POPULAÇÃO 2 TENTOU INSERIR PEÇA EM LUGAR INDEVIDO\n", i);
                    exit(1);
                }
                if(acabou(jogos[i][j]))
                    break;
            }

            fitness1[i] -= get_num_jogadas(jogos[i][j]) * 10;
            fitness2[j] -= get_num_jogadas(jogos[i][j]) * 10;

            vencedor = get_vencedor(jogos[i][j]);
            
            fitness1[i] += 49000*vencedor;
            fitness2[j] -= 49000*vencedor;

            tabuleiro = get_tabuleiro(jogos[i][j]);
            for (int k = 1; k < TAM_TAB - 1; k++){
                for (int l = 1; l < TAM_TAB - 1; l++){
                    cor = tabuleiro[k*TAM_TAB + l];
                    if(cor == 0)
                        continue;
                    if(cor == 1){
                        seq = 0;
                        if(tabuleiro[k*TAM_TAB + l] == tabuleiro[k*TAM_TAB + l + 1])
                            seq++;
                        if(tabuleiro[k*TAM_TAB + l] == tabuleiro[k*TAM_TAB + l - 1])
                            seq++;
                        
                        fitness2[j] -= pow(20, seq);

                        seq = 0;
                        if(tabuleiro[k*TAM_TAB + l] == tabuleiro[(k+1)*TAM_TAB + l])
                            seq++;
                        if(tabuleiro[k*TAM_TAB + l] == tabuleiro[(k-1)*TAM_TAB + l])
                            seq++;
                        
                        fitness2[j] -= pow(20, seq);

                        seq = 0;
                        if(tabuleiro[k*TAM_TAB + l] == tabuleiro[(k+1)*TAM_TAB + l + 1])
                            seq++;
                        if(tabuleiro[k*TAM_TAB + l] == tabuleiro[(k-1)*TAM_TAB + l - 1])
                            seq++;
                        
                        fitness2[j] -= pow(20, seq);

                        seq = 0;
                        if(tabuleiro[k*TAM_TAB + l] == tabuleiro[(k - 1)*TAM_TAB + l + 1])
                            seq++;
                        if(tabuleiro[k*TAM_TAB + l] == tabuleiro[(k + 1)*TAM_TAB + l - 1])
                            seq++;
                        
                        fitness2[j] -= pow(20, seq);
                        
                    }
                    else if(cor == -1){
                        seq = 0;
                        if(tabuleiro[k*TAM_TAB + l] == tabuleiro[k*TAM_TAB + l + 1])
                            seq++;
                        if(tabuleiro[k*TAM_TAB + l] == tabuleiro[k*TAM_TAB + l - 1])
                            seq++;
                        
                        fitness1[i] -= pow(20, seq);

                        seq = 0;
                        if(tabuleiro[k*TAM_TAB + l] == tabuleiro[(k+1)*TAM_TAB + l])
                            seq++;
                        if(tabuleiro[k*TAM_TAB + l] == tabuleiro[(k-1)*TAM_TAB + l])
                            seq++;
                        
                        fitness1[i] -= pow(20, seq);

                        seq = 0;
                        if(tabuleiro[k*TAM_TAB + l] == tabuleiro[(k+1)*TAM_TAB + l + 1])
                            seq++;
                        if(tabuleiro[k*TAM_TAB + l] == tabuleiro[(k-1)*TAM_TAB + l - 1])
                            seq++;
                        
                        fitness1[i] -= pow(20, seq);

                        seq = 0;
                        if(tabuleiro[k*TAM_TAB + l] == tabuleiro[(k - 1)*TAM_TAB + l + 1])
                            seq++;
                        if(tabuleiro[k*TAM_TAB + l] == tabuleiro[(k + 1)*TAM_TAB + l - 1])
                            seq++;
                        
                        fitness1[i] -= pow(20, seq);
                        
                    }
                }
            }
            
            seq = 0;
            for (int k = 1; k < TAM_TAB; k++) {
                // [0][k]
                if(tabuleiro[0 * TAM_TAB + k - 1] == tabuleiro[0 * TAM_TAB + k])
                    seq++;
                if(tabuleiro[0 * TAM_TAB + k] == tabuleiro[0 * TAM_TAB + k + 1])
                    seq++;
                    
                if(cor == 1)
                    fitness2[j] -= pow(20, seq);
                else if(cor == -1)
                    fitness1[i] -= pow(20, seq);
                
                seq = 0;
                    
                // [k][0]
                if(tabuleiro[(k - 1) * TAM_TAB] == tabuleiro[k * TAM_TAB])
                    seq++;
                if(tabuleiro[k * TAM_TAB] == tabuleiro[(k + 1) * TAM_TAB])
                    seq++;
                    
                if(cor == 1)
                    fitness2[j] -= pow(20, seq);
                else if(cor == -1)
                    fitness1[i] -= pow(20, seq);

                seq = 0;

                // [6][k]
                if(tabuleiro[6 * TAM_TAB + k - 1] == tabuleiro[6 * TAM_TAB + k])
                    seq++;
                if(tabuleiro[6 * TAM_TAB + k] == tabuleiro[6 * TAM_TAB + k + 1])
                    seq++;
                    
                if(cor == 1)
                    fitness2[j] -= pow(20, seq);
                else if(cor == -1)
                    fitness1[i] -= pow(20, seq);

                seq = 0;
                    
                // [k][6]
                if(tabuleiro[(k - 1) * TAM_TAB + 6] == tabuleiro[k * TAM_TAB + 6])
                    seq++;
                if(tabuleiro[k * TAM_TAB + 6] == tabuleiro[(k + 1) * TAM_TAB + 6])
                    seq++;
                    
                if(cor == 1)
                    fitness2[j] -= pow(20, seq);
                else if(cor == -1)
                    fitness1[i] -= pow(20, seq);

                seq = 0;
            }
            
            



            reset(jogos[i][j]);
        }
    }
    
}

void reproducao(){
    int max_fit_1 = 0, max_fit_2 = 0;

    for (int i = 0; i < TAM_POP; i++){
        if (fitness1[i] > fitness1[max_fit_1]) max_fit_1 = i;
        if (fitness2[i] > fitness2[max_fit_2]) max_fit_2 = i;
    }

    if(max_fit_ant1 == fitness1[max_fit_1]) {
        ger_rep1++;
    } else {
        ger_rep1 = 0;
        freq_mut2 = 0.05;
    }

    if(max_fit_ant2 == fitness2[max_fit_2]) {
        ger_rep2 ++;
    } else {
        ger_rep2 = 0;
        freq_mut1 = 0.05;
    }

    max_fit_ant1 = fitness1[max_fit_1];
    max_fit_ant2 = fitness2[max_fit_2];
    
    if(ger_rep1 > 50 && freq_mut2 < 0.5){
        freq_mut2 += 0.02;
    }
    if(ger_rep2 > 50 && freq_mut1 < 0.5){
        freq_mut1 += 0.02;
    }

    std::swap(populacao1[0], populacao1[max_fit_1]); max_fit_1 = 0;
    std::swap(populacao2[0], populacao2[max_fit_2]); max_fit_2 = 0;
    
    for (int i = 1; i < TAM_POP; i++){
        for (int j = 0; j < 49; j++){
            for (int k = 0; k < 21; k++){
                populacao1[i]->pesos_input[j][k] = populacao1[0]->pesos_input[j][k];
                if((rand()%100)/100.0 < freq_mut1)
                    populacao1[i]->pesos_input[j][k] += ((rand()%(int) (max_mut1*2000)) - (max_mut1*1000))/1000.0;
                populacao2[i]->pesos_input[j][k] = populacao2[0]->pesos_input[j][k];
                if((rand()%100)/100.0 < freq_mut2)
                    populacao2[i]->pesos_input[j][k] += ((rand()%(int) (max_mut2*2000)) - (max_mut2*1000))/1000.0;
            }
        }
        for (int k = 0; k < 21; k++){
            for (int l = 0; l < 7; l++) {
                populacao1[i]->pesos_intermed[k][l] = populacao1[0]->pesos_intermed[k][l];
                if((rand()%100)/100.0 < freq_mut1)
                    populacao1[i]->pesos_intermed[k][l] += ((rand()%(int) (max_mut1*2000)) - (max_mut1*1000))/1000.0;
                populacao2[i]->pesos_intermed[k][l] = populacao2[0]->pesos_intermed[k][l];
                if((rand()%100)/100.0 < freq_mut2)
                    populacao2[i]->pesos_intermed[k][l] += ((rand()%(int) (max_mut2*2000)) - (max_mut2*1000))/1000.0;
            }
        }
        if((rand()%100)/100.0 < freq_mut1)
            populacao1[i]->comeco = rand()%7;
        if((rand()%100)/100.0 < freq_mut2)
            populacao2[i]->comeco = rand()%7;
    }
    printf("max_fit_1: %d, max_fit_2: %d\n", fitness1[max_fit_1], fitness2[max_fit_2]);
}

void evoluir(int gen){
    init_populacao();
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
        for (int j = 0; j < TAM_POP; j++)
            fim_jogo(&jogos[i][j]);   
            
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
    