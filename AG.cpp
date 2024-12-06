#include "AG.h"
#include "math.h"

#define TAM_POP 100

struct _individuo{
    float genes[TAM_TAB*TAM_TAB][TAM_TAB];
};

IND * populacao1[TAM_POP];
IND * populacao2[TAM_POP];

CONNECT4 * jogos[TAM_POP][TAM_POP]; // jogos[i][j] corresponde ao jogo em que o indivíduo populacao1[i] joga contra o indivíduo populacao2[j]

int fitness1[TAM_POP], fitness2[TAM_POP];
float mut_tax = 0.5, max_mut = 2000.0; 

int output(IND * individuo, CONNECT4 * input){
    const char * tabuleiro = get_tabuleiro(input);
    float output[TAM_TAB]; int max_i = 0;
    for (int i = 0; i < TAM_TAB; i++){
        output[i] = 0;

        if(tabuleiro[42 + i] != 0){
            output[i] = 0;
            continue;
        }

        for (int j = 0; j < TAM_TAB*TAM_TAB; j++){
            output[i] += tabuleiro[j]* individuo->genes[j][i];   
        }

        if(fabs(output[i]) > fabs(output[max_i])){
            max_i = i;
        }
    }
    if(tabuleiro[42 + max_i] != 0){
        printf("[%.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f] max:, %d\n", output[0], output[1], output[2], output[3], output[4], output[5], output[6], max_i);
        print_tabuleiro(input);
        printf("%d\n", acabou(input));
        getchar();

    }
    return max_i;
}

IND * alloc_ind(){
    IND * ind = (IND *) malloc(sizeof(IND));
    if(ind == NULL){
        printf("Erro na alocação");
        exit(1);
    }
    srand(time(NULL));
    
    for (int i = 0; i < 49; i++)
        for (int j = 0; j < 7; j++)
            ind->genes[i][j] = (rand()%2000000 - 1000000)/100;

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
    char vencedor;
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
            fitness2[i] -= get_num_jogadas(jogos[i][j]) * 10;

            vencedor = get_vencedor(jogos[i][j]);
            
            if(vencedor == 1)
                fitness1[i] += 49000;
            if(vencedor == -1)
                fitness2[i] += 49000;

            tabuleiro = get_tabuleiro(jogos[i][j]);
            for (int k = 0; k < TAM_TAB - 1; k++){
                if(tabuleiro[TAM_TAB*(TAM_TAB - 1) + k] == tabuleiro[TAM_TAB*(TAM_TAB - 1) + k + 1]){
                        fitness1[i] += 240*tabuleiro[k];
                        fitness2[i] -= 240*tabuleiro[k];
                }
                if(tabuleiro[k*TAM_TAB+TAM_TAB-1] == tabuleiro[(k + 1)*TAM_TAB+TAM_TAB-1]){
                        fitness1[i] += 240*tabuleiro[k];
                        fitness2[i] -= 240*tabuleiro[k];
                }
                for (int l = 0; l < TAM_TAB - 1; l++){
                    if(tabuleiro[k * TAM_TAB + l] == tabuleiro[k * TAM_TAB + (l + 1)]){
                        fitness1[i] += 240*tabuleiro[k*TAM_TAB + l];
                        fitness2[i] -= 240*tabuleiro[k*TAM_TAB + l];
                    }

                    if(tabuleiro[k * TAM_TAB + l] == tabuleiro[(k + 1)* TAM_TAB + (l + 1)]){
                        fitness1[i] += 240*tabuleiro[k*TAM_TAB + l];
                        fitness2[i] -= 240*tabuleiro[k*TAM_TAB + l];
                    }

                    if(tabuleiro[k * TAM_TAB + l] == tabuleiro[(k + 1)* TAM_TAB]){
                        fitness1[i] += 240*tabuleiro[k*TAM_TAB + l];
                        fitness2[i] -= 240*tabuleiro[k*TAM_TAB + l];
                    }
                }
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
    
    std::swap(populacao1[0], populacao1[max_fit_1]); max_fit_1 = 0;
    std::swap(populacao2[0], populacao2[max_fit_2]); max_fit_2 = 0;
    
    for (int i = 1; i < TAM_POP; i++){
        for (int j = 0; j < 49; j++){
            for (int k = 0; k < 7; k++){
                populacao1[i]->genes[j][k] = populacao1[0]->genes[j][k];
                if((rand()%100)/100.0 < mut_tax)
                    populacao1[i]->genes[j][k] += ((rand()%(int) max_mut) - (max_mut/2.0))*mut_tax;
                populacao2[i]->genes[j][k] = populacao2[0]->genes[j][k];
                if((rand()%100)/100.0 < mut_tax)
                    populacao2[i]->genes[j][k] += ((rand()%(int) max_mut) - (max_mut/2.0))*mut_tax;
            }
        }
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

    FILE * fp = fopen("pesos_salvos.txt", "w");

    for (int i = 0; i < 49; i++){
        for (int j = 0; j < 7; j++)
            fprintf(fp, "%f,", (*player1)->genes[i][j]); 
        fprintf(fp, "\n"); 
    }

        fprintf(fp, "\n"); 
        fprintf(fp, "\n"); 
        fprintf(fp, "\n"); 

    for (int i = 0; i < 49; i++){
        for (int j = 0; j < 7; j++)
            fprintf(fp, "%f,", (*player2)->genes[i][j]); 
        fprintf(fp, "\n"); 
    }

    fclose(fp);


    
}