#ifndef GAME_H
    #define GAME_H

    #include <stdio.h>
    #include <stdlib.h>

    #define TAM_TAB 7    

    typedef struct connect4_ CONNECT4;

    // aloca o espaço para um novo jogo e inicializa os atributos
    CONNECT4 * novo_jogo();

    // libera o espaço alocado pelo jogo
    void fim_jogo(CONNECT4 ** game);

    // reinicializa os valores dos atributos do jogo
    void reset(CONNECT4 * game);

    // printa o tabuleiro no terminal representando o vermelho como 'o' e o azul como 'x'
    void print_tabuleiro(CONNECT4 * game);    

    // adiciona uma peça na primeira linha disponível (de 0 até 6) e verifica se algum dos jogadores venceu
    int adicionar_peca(CONNECT4 * game, int coluna);

    // retorna true se alguem já venceu ou se o tabuleiro está cheio
    bool acabou(CONNECT4 * game);

    // retorna o tabuleiro como um vetor de 1 dimensão (read-only)
    const char * get_tabuleiro(CONNECT4 * game);

    // retorna 1 se PLAYER1 venceu, -1 se PLAYER2 venceu, 0 se está empatado (0 não significa que o jogo acabou)
    char get_vencedor(CONNECT4 * game);

    int get_num_jogadas(CONNECT4 * game);

#endif
