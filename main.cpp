#include "main.hpp"
#include "include/GL/freeglut_std.h"
#include "interface/interface.hpp"

#include <chrono>
#include <future>
#include <thread>
#include <vector>

void Render();

pInterface interface(Render);

pGraphics::pButon restart({ (17 * 40) / 2 - 4 * 20, 9 * 40 }, { 4 * 40, 40 }, interface.graphics.createNewColor(0, 255, 0), interface.graphics.createNewColor(0, 255, 0), GLUT_BITMAP_HELVETICA_18, interface.graphics.black, "Restart", [](bool active) {
    if (interface.screen.gameOver) {
        interface.border.clear();
        interface.pieces.clear();
        active = false;
        interface.bInit = false;
        interface.init();
        interface.screen.score = 0;
        interface.screen.level = 0;
        Render();
        interface.screen.gameOver = false;
    }
});

void Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_TEXTURE_2D);
    
    interface.drawFrame();
    interface.drawPieces();
    interface.drawNextPiece();
    //interface.drawBorder();
    interface.drawGameOverScreen();

    if (interface.screen.gameOver)
        restart.draw(interface);

    glutSwapBuffers();
}

void Resize(GLint newWidth, GLint newHeight) {
    if (newWidth >= 8 && newHeight >= 8) {
        interface.screen.size = { newWidth, newHeight };

        glViewport( 0, 0, newWidth, newHeight );
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        glOrtho( 0, GLdouble (newWidth), GLdouble (newHeight), 0, 0, 100);
        Render();
        glutPostRedisplay();
    }
}

void ProcessSpecialInput(int key, int x, int y) {
}

void ProcessInput(unsigned char key, int x, int y) {
    if (!interface.screen.gameOver)
        interface.onKeyPress(key);
}

void HandleMouseKeys(int button, int state, int x, int y) {
    switch(button) {
        case GLUT_LEFT_BUTTON: {
            interface.screen.leftClick = state;
            restart.checkClick(interface);
            if (state != GLUT_DOWN)
                break;

            break;
        }
        case GLUT_RIGHT_BUTTON: {
            interface.screen.rightClick = state;

            if (state != GLUT_DOWN)
                break;

            break;
        }
    }
    Render();
}

void HandleMouseMovement(int x, int y) {
    interface.screen.mousePointer = { x, y };
}

void HandleMouseDrag(int x, int y) {
    interface.screen.mousePointer = { x, y };
    if (interface.screen.leftClick != interface.screen.leftClickDrag)
        interface.screen.leftClickDrag = interface.screen.leftClick;
}

void automaticMove() {
    interface.screen.automaticMove = true;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    automaticMove();
}

void idleFunc() {
    if (interface.screen.automaticMove && !interface.screen.gameOver) {
        interface.automaticMove();
        Render();
        interface.screen.automaticMove = false;
    }
}

void GL(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowPosition(100,100);
    glutInitWindowSize(interface.screen.initialSize.first, interface.screen.initialSize.second);
    glutCreateWindow (interface.screen.windowName.c_str());

    glEnable(GL_TEXTURE_2D);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho( 0, 640, 640, 0, 0, 100);
    glClearColor(1.0, 1.0, 1.0, 1.0);

    ilInit();

    glutDisplayFunc(Render);
    glutKeyboardFunc(ProcessInput);
    glutSpecialFunc(ProcessSpecialInput);
    glutReshapeFunc(Resize);
    glutMouseFunc(HandleMouseKeys);
    glutPassiveMotionFunc(HandleMouseMovement);
    glutMotionFunc(HandleMouseDrag);
    
    glutIdleFunc(idleFunc);

    glutMainLoop();
}

int main(int argc, char **argv) {
    auto mainProc = std::async(std::launch::async, GL, argc, argv);
    auto automaticMoveProc = std::async(std::launch::async, automaticMove);
    mainProc.get(), automaticMoveProc.get();
    return 0;
}