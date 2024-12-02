#include "graphics.h"

#define CLEAR_COLOR CINZA_ESCURO
#define EMPATE_COLOR CINZA
#define PLAYER1_COLOR VERMELHO
#define PLAYER2_COLOR AZUL

void draw_board(CONNECT4 * game){
    if(game == NULL)
        return;

    const unsigned char * tabuleiro = get_tabuleiro(game);
    float x, y;
    COR cor;

    glColor3ubv(BEGE);
    glBegin(GL_POLYGON);
        glVertex2d(0.7, 0.7);
        glVertex2d(0.7, -0.7);
        glVertex2d(-0.7, -0.7);
        glVertex2d(-0.7, 0.7);
    glEnd();

    for (int i = 0; i < TAM_TAB; i++){
        for (int j = 0; j < TAM_TAB; j++){
            x = -0.6 + j*0.2;
            y = -0.6 + i*0.2;

            cor = tabuleiro[i * TAM_TAB + j] == 1? PLAYER1_COLOR : tabuleiro[i * TAM_TAB + j] == 0? CLEAR_COLOR : PLAYER2_COLOR;

            glColor3ubv(cor);
            glBegin(GL_POLYGON);
                for (float i = 0; i < 360.0; i+= 5){
                        glVertex2d(0.07*cos((i/180.0)*M_PI) + x, 0.07*sin((i/180.0)*M_PI) + y);
                }
            glEnd();

        }
    }
    
}

void draw_vencedor(CONNECT4 * game){
    if(!acabou(game))
        return;
    unsigned char vencedor = get_vencedor(game);
    COR c = vencedor == 1 ? PLAYER1_COLOR : vencedor == 0 ? EMPATE_COLOR : PLAYER2_COLOR;

    glColor3ubv(c);
    glBegin(GL_POLYGON);
        glVertex2d(-0.6, 0.9);
        glVertex2d(0.6, 0.9);
        glVertex2d(0.6, 0.75);
        glVertex2d(-0.6, 0.75);
    glEnd();
}

void mouse_handler(int button, int state, int x, int y, CONNECT4 * game){
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        int coluna;
        float mouseX, mouseY;
        // Converte a entrada para pontos (x,y) na tela
        mouseX = 2*((float(x)/WIN_WIDTH)-0.5);
        mouseY = -2*((float(y)/WIN_HEIGHT)-0.5);

        if(mouseX < -0.7 || mouseX > 0.7 || mouseY < -0.7 || mouseY > 0.7)  // mouse fora do tabuleiro, descarta
            return;
        
        coluna = (mouseX*10 + 7)/2; 

        adicionar_peca(game, coluna);
    }
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
        reset(game);
    }

}


void render(CONNECT4 * game){
    // Clear the window with the specified color
    glClear(GL_COLOR_BUFFER_BIT);

    draw_vencedor(game);

    draw_board(game);

    glutSwapBuffers();
}

void timer(int a){
    glutPostRedisplay();

    glutTimerFunc(1000/60, timer, 0);
}