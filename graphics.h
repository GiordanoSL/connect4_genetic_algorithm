#ifndef GRAPHICS_H
    #define GRAPHICS_H

    #include <GL/glut.h>
    #include <iostream>
    #include <vector>
    #include <math.h>
    #include "glColors.h"
    #include "game.h"

    #define WIN_HEIGHT 900
    #define WIN_WIDTH  900

    void render(CONNECT4 * game);
    void draw_board(CONNECT4 * game);
    void draw_vencedor(CONNECT4 * game);
    void mouse_handler(int button, int state, int x, int y, CONNECT4 * game);
    void timer(int a);

#endif
