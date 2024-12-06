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

CONNECT4 * game;
IND * p1, * p2;
int sinal = 1;

void display(){
    if(game == NULL)
        glClear(GL_COLOR_BUFFER_BIT);
    else
        render(game);
}

void mouse_input(int button, int state, int x, int y){
    if(game == NULL)
        return;
    if(button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN){
        //int mov = nn_output(p1);
        //adicionar_peca(game, mov);
       // printf("[movimento: %d];  ", mov);
    }
    mouse_handler(button, state, x, y, game);
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        // if(sinal == 1)
        //     adicionar_peca(game, output(p1, game));
        // else
            adicionar_peca(game, output(p2, game));
        sinal *= -1;
        //printf("[movimento: %d];  ", nn_output(p2));
    }
}


int main(int argc, char** argv) {
    game = novo_jogo();
    evoluir(1000000);
    finaliza_evolucao(&p1, &p2);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
    glutCreateWindow("Connect4 Game");

    glClearColor(0.137f, 0.137f, 0.180f, 1.0f);

    glutDisplayFunc(display);
    glutMouseFunc(mouse_input);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();

    fim_jogo(&game);
    return 0;
}