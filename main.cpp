#include "game.h"
#include "graphics.h"
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

void display(){
    if(game == NULL)
        glClear(GL_COLOR_BUFFER_BIT);
    else
        render(game);
}

void mouse_input(int button, int state, int x, int y){
    if(game != NULL)
        mouse_handler(button, state, x, y, game);
}


int main(int argc, char** argv) {
    game = novo_jogo();

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