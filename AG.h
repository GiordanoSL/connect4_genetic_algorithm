#ifndef NEURAL_NETWORK_H
    #define NEURAL_NETWORK_H
    
    #include <algorithm>
    #include <time.h>
    #include "game.h"

    typedef struct _individuo IND;
    
    int output(IND * individuo, CONNECT4 * input);

    void init_populacao();

    void avaliacao();

    void reproducao();

    void finaliza_evolucao(IND ** player1, IND ** player2);

    void evoluir(int gen);

    void write_ind(const char * pesos_file, IND * ind);
    
    void get_best_ai(IND ** p1, IND ** p2);

    int calcular_penalidade(int cor, CONNECT4 * game);



#endif