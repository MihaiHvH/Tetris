#include "main.hpp"
#include "include/GL/freeglut_std.h"
#include "interface/interface.hpp"

#include <chrono>
#include <future>
#include <thread>
#include <vector>

void Render();

pInterface interface(Render);

pGraphics::pButon restart({ (17 * interface.screen.pieceSize.first) / 2 - 4 * interface.screen.pieceSize.first / 2, 9 * interface.screen.pieceSize.second }, { 4 * interface.screen.pieceSize.first, interface.screen.pieceSize.second }, interface.graphics.createNewColor(0, 255, 0), interface.graphics.createNewColor(0, 255, 0), GLUT_BITMAP_HELVETICA_18, interface.graphics.black, "Restart", [](bool active) {
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

    if (!interface.screen.gameOver) { //draw score
        pColor red = interface.graphics.createNewColor(255, 0, 0);
        void* font = GLUT_BITMAP_HELVETICA_18;        
        std::pair<double, double> scoreTextSize = interface.graphics.getTextSize(std::string("Score: ").append(std::to_string(interface.screen.score)).c_str(), font);
        std::pair<double, double> levelTextSize = interface.graphics.getTextSize(std::string("Level: ").append(std::to_string(interface.screen.level)).c_str(), font);
        std::pair<double, double> linesTextSize = interface.graphics.getTextSize(std::string("Lines: ").append(std::to_string(interface.screen.lines)).c_str(), font);
        interface.graphics.drawText({ (4 * interface.screen.pieceSize.first) / 2 + 12 * interface.screen.pieceSize.first - scoreTextSize.first / 2, 6 * interface.screen.pieceSize.second + interface.screen.pieceSize.second / 2 + scoreTextSize.second / 2 }, font, std::string("Score: ").append(std::to_string(interface.screen.score)).c_str(), interface.graphics.black);
        interface.graphics.drawText({ (4 * interface.screen.pieceSize.first) / 2 + 12 * interface.screen.pieceSize.first - levelTextSize.first / 2, 7 * interface.screen.pieceSize.second + interface.screen.pieceSize.second / 2 + levelTextSize.second / 2 }, font, std::string("Level: ").append(std::to_string(interface.screen.level)).c_str(), interface.graphics.black);
        interface.graphics.drawText({ (4 * interface.screen.pieceSize.first) / 2 + 12 * interface.screen.pieceSize.first - linesTextSize.first / 2, 8 * interface.screen.pieceSize.second + interface.screen.pieceSize.second / 2 + linesTextSize.second / 2 }, font, std::string("Lines: ").append(std::to_string(interface.screen.lines)).c_str(), interface.graphics.black);
    }

    if (interface.screen.gameOver)
        restart.draw(interface);

    glutSwapBuffers();
}

void Resize(GLint newWidth, GLint newHeight) {
    if (newWidth >= 8 && newHeight >= 8) {
        interface.screen.automaticMove = false;

        for (auto &i : interface.pieces) {
            i.pos = { 
                (i.pos.first / interface.screen.pieceSize.first) * (newWidth / 17.f),
                (i.pos.second / interface.screen.pieceSize.second) * (newHeight / 16.f)
            };
            for (auto &j : i.hitbox) {
                j = { 
                    (j.first / interface.screen.pieceSize.first) * (newWidth / 17.f),
                    (j.second / interface.screen.pieceSize.second) * (newHeight / 16.f)
                };    
            }
        }
        interface.screen.size = { newWidth, newHeight };
        interface.screen.pieceSize = { newWidth / 17.f, newHeight / 16.f };
        restart.updatePos({ (17.f * interface.screen.pieceSize.first) / 2 - 4.f * interface.screen.pieceSize.first / 2, 9.f * interface.screen.pieceSize.second });
        restart.updateSize({ 4.f * interface.screen.pieceSize.first, interface.screen.pieceSize.second });
        glViewport( 0, 0, newWidth, newHeight );
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        glOrtho( 0, GLdouble (newWidth), GLdouble (newHeight), 0, 0, 100);
        interface.computeBorder();
        Render();
        glutPostRedisplay();
        interface.screen.automaticMove = true;
    }
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