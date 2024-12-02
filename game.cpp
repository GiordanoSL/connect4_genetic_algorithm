#include "game.h"

struct connect4_{
    unsigned char tabuleiro[TAM_TAB][TAM_TAB],  // tabuleiro 7x7
        rodada,                     // 1 = vermelho, -1 = azul (obs: como é unsigned char -1 = 255)
        vencedor,                   // 1 = vermelho, -1 = azul, 0 = empate
        num_jogadas;                // nro de jogadas
};



CONNECT4 * novo_jogo(){
    CONNECT4 * game = (CONNECT4 *) malloc(sizeof(CONNECT4)); // alocação de espaço para o novo tabuleiro

    if(game != NULL)
        reset(game); // inicializa o jogo

    return game;    
}

void fim_jogo(CONNECT4 ** game){
    free(*game);  // libera a memória usada pelo jogo
    *game = NULL;
}

void reset(CONNECT4 * game){
    for (int i = 0; i < TAM_TAB; i++){
        for (int j = 0; j < TAM_TAB; j++)    
            game->tabuleiro[i][j] = 0; // inicialmente todos os espaços estao vazios
    }
    game->rodada = 1;   // o primeiro a fazer um movimento é o vermelho
    game->vencedor = 0; // o jogo inicia empatado
    game->num_jogadas = 0; // nro de jogadas se for 49 o tabuleiro está cheio

}

void print_tabuleiro(CONNECT4 * game){
    if(game == NULL)
        return;

    printf("-----------------------------\n");
    for (int i = 0; i < TAM_TAB; i++){

        for (int j = 0; j < TAM_TAB; j++)
            printf("|%2c ", game->tabuleiro[i][j] == 1? 'o' : game->tabuleiro[i][j] == 0? ' ' : 'x');

        printf("|\n-----------------------------\n");
    }
}

unsigned char venceu(CONNECT4 * game, int lin, int col){
    if(game == NULL)
        return -2;

    unsigned char cor = game->tabuleiro[lin][col];
    int i = lin, j = col, seq; // i, j = variáveis auxiliares; seq = sequencia de peças

    seq = 0;
    i = lin, j = col;
    
    while(i - 1 >= 0){
        if(game->tabuleiro[i - 1][j] == cor){
            seq++;
            i--;
            continue;
        }
        break;
    }
    i = lin, j = col;
    // verifica a sequencia de peças para cima 
    while(i + 1 < TAM_TAB){
        if(game->tabuleiro[i + 1][j] == cor){
            seq++;
            i++;
            continue;
        }
        break;
    }

    if (seq >= 3)
        return cor;

    seq = 0;
    i= lin, j = col;
    // verifica a sequencia de peças na diagonal principal
    while(i - 1 >= 0 && j - 1 >= 0){
        if(game->tabuleiro[i - 1][j - 1] == cor){
            seq++;
            i--;
            j--;
            continue;
        }
        break;
    }
    i = lin, j = col;
    // verifica a sequencia de peças na diagonal principal
    while(i + 1 < TAM_TAB && j + 1 < TAM_TAB){
        if(game->tabuleiro[i + 1][j + 1] == cor){
            seq++;
            i++;
            j++;
            continue;
        }
        break;
    }

    if (seq >= 3)
        return cor;

    seq = 0;
    i= lin, j = col;
    // verifica a sequencia de peças na diagonal secundária
    while(i - 1 >= 0 && j + 1 < TAM_TAB){
        if(game->tabuleiro[i - 1][j + 1] == cor){
            seq++;
            i--;
            j++;
            continue;
        }
        break;
    }
    i = lin, j = col;
    // verifica a sequencia de peças na diagonal secundária
    while(i + 1 < TAM_TAB && j - 1 >= 0){
        if(game->tabuleiro[i + 1][j - 1] == cor){
            seq++;
            i++;
            j--;
            continue;
        }
        break;
    }
    
    if (seq >= 3)
        return cor;

    seq = 0;
    i = lin, j = col;
    // verifica a sequencia de peças da direita para esquerda
    while(j - 1 >= 0){
        if(game->tabuleiro[i][j - 1] == cor){
            seq++;
            j--;
            continue;
        }
        break;
    }
    i = lin, j = col;
    // verifica a sequencia de peças da esquerda para direita
    while(j + 1 < TAM_TAB){
        if(game->tabuleiro[i][j + 1] == cor){
            seq++;
            j++;
            continue;
        }
        break;
    }

    if (seq >= 3)
        return cor;

    return 0;
}

int adicionar_peca(CONNECT4 * game, int coluna){
    if(game == NULL)
        return -1;

    if(game->vencedor != 0 || game->num_jogadas >= 49) // verifica se o jogo já acabou
        return -1;
    if(coluna < 0 || coluna >= TAM_TAB) // verifica se a coluna é válida
        return -1;
    
    int linha = 0;

    // encontra a primeira linha sem nenhuma peça
    while(linha < TAM_TAB){
        if(game->tabuleiro[linha][coluna] == 0)
            break;
        linha++;
    }
    if(linha >= TAM_TAB) // coluna cheia -> não é possível adicionar peça
        return -1;

    game->tabuleiro[linha][coluna] = game->rodada; // adiciona a peça
    game->rodada *= -1; // troca a rodada
    game->num_jogadas++; // atualiza o numero de jogadas feitas

    game->vencedor = venceu(game, linha, coluna); // verifica se algum dos jogadores venceu

    return linha; // peça adicionada com sucesso -> retorna a linha em que foi adicionada
}

bool acabou(CONNECT4 * game){
    if(game == NULL)
        return false;
    return game->num_jogadas == TAM_TAB*TAM_TAB || game->vencedor != 0;
}

const unsigned char * get_tabuleiro(CONNECT4 * game){
    if(game == NULL)
        return NULL;
    return (const unsigned char *) &(game->tabuleiro[0][0]);
}
unsigned char get_vencedor(CONNECT4 * game){
    if(game == NULL)
        return -2;
    return game->vencedor;
}

// TESTE UTILIZANDO APENAS O JOGO

// int main (void){
//     CONNECT4 * jogo1 = novo_jogo();
//     CONNECT4 * jogo2 = novo_jogo();

//     int n;

//     print_tabuleiro(jogo1);
//     while (!acabou(jogo1)){
//         scanf(" %d", &n);
//         if(n == -1)
//             break;
//         adicionar_peca(jogo1, n-1);
//         print_tabuleiro(jogo1);
//     }

//     while (!acabou(jogo2)){
//         adicionar_peca(jogo2, rand()%TAM_TAB);
//         print_tabuleiro(jogo2);
//         scanf(" %d", &n);
//         if(n == -1)
//             break;
//         adicionar_peca(jogo2, n-1);
//     }
    
//     print_tabuleiro(jogo1);
//     print_tabuleiro(jogo2);
    
//     fim_jogo(&jogo1);
//     fim_jogo(&jogo2);
    
//     return 0;
// }
