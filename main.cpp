#include "game.h"
#include "graphics.h"
#include "AG.h"
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <math.h>
#include "glColors.h"

#ifndef WIN_WIDTH
    #define WIN_WIDTH 900
#endif
#ifndef WIN_HEIGHT
    #define WIN_HEIGHT 900
#endif

enum{
    P_VS_Comp = 1,
    Comp_VS_P = 2,
    Comp_VS_Comp = 3,
}; 

CONNECT4 * game;
IND * p1, * p2;
int modo_de_jogo = Comp_VS_Comp, rodada = 1, fit1, fit2;

void display(){
    if(game == NULL)
        glClear(GL_COLOR_BUFFER_BIT);
    else{
        render(game);
    }
}

void mouse_input(int button, int state, int x, int y){
    if(game == NULL)
        return;
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        if (modo_de_jogo == Comp_VS_Comp)
            adicionar_peca(game, rodada == 1? output(p1, game): output(p2, game));
        else if (modo_de_jogo == P_VS_Comp){
            if(rodada == 1)
                mouse_handler(button, state, x, y, game);
            else 
                adicionar_peca(game, output(p2, game));
        }else{
            if(rodada == -1)
                mouse_handler(button, state, x, y, game);
            else 
                adicionar_peca(game, output(p1, game));
        }

        rodada *= -1;
    }
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
        fit1 = fit2 = 0;
        fit1 -= calcular_penalidade(1, game);
        fit1 += 49000*get_vencedor(game);
        fit2 -= calcular_penalidade(1, game);
        fit2 -= 49000*get_vencedor(game);

        printf("fit1: %d, fit2: %d\n", fit1, fit2);
        reset(game);
        rodada = 1;
    }

}

void handle_keyboard(unsigned char key, int x, int y){
    switch (key){
    case 'e':
    case 'E':
        printf("oi?");

        // evoluir(100);
        // get_best_ai(&p1, &p2);
        
        printf("tchau?");

        break;
    case '1':
        modo_de_jogo = P_VS_Comp;
        break;
    case '2':
        modo_de_jogo = Comp_VS_P;
        break;
    case '3':
        modo_de_jogo = Comp_VS_Comp;
        break;
    }
}


int main(int argc, char** argv) {
    int n;
    game = novo_jogo();
    init_populacao();
    get_best_ai(&p1, &p2);
    scanf(" %d", &n);
    while(n != -1){
        evoluir(n);
        scanf(" %d", &n);
    }
    finaliza_evolucao(&p1,&p2);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
    glutCreateWindow("Connect4 Game");

    glClearColor(0.137f, 0.137f, 0.180f, 1.0f);

    glutDisplayFunc(display);
    glutMouseFunc(mouse_input);
    glutKeyboardFunc(handle_keyboard);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();

    fim_jogo(&game);
    return 0;
}